<?PHP
/* Class extender for PalmOS DOC files
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */
 
define('PDB_DOC_RECORD_SIZE', 4096);
 
class PalmDoc extends PalmDB {
   var $Bookmarks = array();  // Bookmarks stored in the doc file
                              // $Bookmarks[position] = "name"
   
   
   function PalmDoc ($Title = '') {
      PalmDB::PalmDB('TEXt', 'REAd', $Title);
      
      // Record 0 is reserved for header information
      $this->GoToRecord(1);
   }


   // Appends $String to the end of the document
   function AddDocText ($String) {
      $SpaceLeft = PDB_DOC_RECORD_SIZE - $this->GetRecordSize();
      while ($String) {
         if ($SpaceLeft > 0) {
	    $this->AppendString($String, $SpaceLeft);
	    $String = substr($String, $SpaceLeft);
	    $SpaceLeft = PDB_DOC_RECORD_SIZE - $this->GetRecordSize();
	 } else {
	    $this->GoToRecord('+1');
	    $SpaceLeft = PDB_DOC_RECORD_SIZE;
	 }
      }
   }
   
   
   // Creates the informational record (record 0)
   // Used only for writing the file
   function MakeDocRecordZero () {
      $oldRec = $this->GoToRecord(0);
      $this->DeleteCurrentRecord();
      $this->AppendInt16(1);  // Version
        // 1 = uncompressed, 2 = compressed
      $this->AppendInt16(0);  // Reserved
      
      $Content_Length = 0;
      $MaxIndex = 0;
      ksort($this->Records, SORT_NUMERIC);
      $keys = array_keys($this->Records);
      array_shift($keys);
      $MaxIndex = array_pop($keys);
      $keys[] = $MaxIndex;
      foreach ($keys as $index) {
         $Content_Length += $this->GetRecordSize($index);
	 $this->RecordAttrs[$index] = 0x40;  // dirty + private
      }
      $this->AppendInt32($Content_Length);       // Doc Size
      $this->AppendInt16($MaxIndex);             // Number of Records
      $this->AppendInt16(PDB_DOC_RECORD_SIZE);   // Record size
      $this->AppendInt32(0);                     // Reserved

      $this->GoToRecord($oldRec);
   }


   // Returns the size of the record specified, or the current record if
   // no record is specified
   function GetRecordSize($num = false) {
      if ($num === false)
         $num = $this->CurrentRecord;
      if ($num == 0)
         return 16;
      if (! isset($this->Records[$num])) {
         $bookmark = -1;
	 while (! isset($this->Records[$num]) && $num > 0) {
	    $bookmark ++;
	    $num --;
	 }
	 if ($bookmark < count($this->Bookmarks))
	    return 20;
      }
      return PalmDB::GetRecordSize($num);
   }
   
   
   // Returns the data of the specified record, or the current record if no
   // record is specified.  If the record doesn't exist, returns ''.
   function GetRecord($num = false) {
      if ($num === false)
         $num = $this->CurrentRecord;
      if ($num == 0)
         $this->MakeDocRecordZero();
      if (! isset($this->Records[$num])) {
         $bookmark = -1;
	 while (! isset($this->Records[$num]) && $num > 0) {
	    $bookmark ++;
	    $num --;
	 }
	 // Sort bookmarks in order of appearance
	 ksort($this->Bookmarks);
	 if ($bookmark < count($this->Bookmarks)) {
  	    $Positions = array_keys($this->Bookmarks);
	    $Desired = $this->Bookmarks[$Positions[$bookmark]];
	    $str = $this->String($Desired, 15);
	    $str = $this->PadString($str, 16);
	    $str .= $this->Int32($Positions[$bookmark]);
	    return $str;
	 }
         return '';
      }
      return $this->Records[$num];
   }
   
   
   // Returns a list of records to write to a file in the order specified.
   function GetRecordIDs() {
      $ids = PalmDB::GetRecordIDs();
      if (! isset($this->Records[0]))
         array_unshift($ids, 0);
      $Max = 0;
      foreach ($ids as $val) {
         if ($Max <= $val)
	    $Max = $val + 1;
      }
      foreach ($this->Bookmarks as $val) {
         $ids[] = $Max ++;
      }
      return $ids;
   }
   
   
   // Returns the number of records to write
   function GetRecordCount() {
      $c = count($this->Records);
      if (! isset($this->Records[0]) && $c)
         $c ++;
      $c += count($this->Bookmarks);
      return $c;
   }
   
   
   // Adds a bookmark.
   // $Name must be 15 chars or less (automatically trimmed)
   // $Pos is the position to add the bookmark at, or the current position if
   // not specified
   // Returns true on error
   // If $Pos already has a bookmark defined, this will blindly overwrite that
   // bookmark.
   function AddBookmark($Name, $Pos = false) {
      if ($Name == '')
         return true;
      if ($Pos === false) {
         $Pos = 0;
         foreach ($this->Records as $id => $data) {
	    if ($id != 0) {
	       $Pos += $this->GetRecordSize($id);
	    }
	 }
      }
      $this->Bookmarks[$Pos] = $Name;
      return false;
   }
}

?>
