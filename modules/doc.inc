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
   var $IsCompressed = true;
   
   
   function PalmDoc ($Title = '') {
      PalmDB::PalmDB('TEXt', 'REAd', $Title);

      $this->EraseDocText();
      $this->IsCompressed = true;
      $this->IsCompressed = false;
   }


   // Gets all of the document's text and returns it as a string
   function GetDocText () {
      $String = '';
      $i = 1;
      while (isset($Records[$i])) {
         $String .= pack('H*', $Records[$i]);
	 $i ++;
      }
      return $String;
   }
   
   
   // Erases all text in the document
   function EraseDocText () {
      $this->Records = array();
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
      if ($this->IsCompressed)
         $this->AppendInt16(2);  // "Version"
      else
         $this->AppendInt16(1);  // "Version"
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
         // possibly used for position in doc?

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
      // If it is compressed, GetRecord() will compress the record before
      // returning the data.  Since the data is hex encoded, divide the
      // size of the resulting string by 2.
      if ($this->IsCompressed)
         return strlen($this->GetRecord($num)) / 2;
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
      
      if ($this->IsCompressed)
         return $this->CompressRecord($this->Records[$num]);
	 
      return $this->Records[$num];
   }
   
   
   // Compresses a single string.  Please note that the string passed in and
   // the string returned are both hex encoded!
   //
   // 0x00 = represents itself
   // 0x01 - 0x08 = Read next n bytes verbatim
   // 0x09 - 0x7F = Represents itself
   // 0x80 - 0xBF = Read next byte to make 16-bit number.  Remove top 2 bits.
   //               Next 11 bits = how far back to read.  Last 3 bits should
   //               be (# of bytes to copy - 3), with the last three bits
   //               never being zero.
   // 0xC0 - 0xFF = Space + 7-bit char
   //
   // If I use *1 or *2 for compress code bytes, I can illustrate a few
   // problems with this compression code.  I think that every byte counts
   // on such a limited device, so maybe this compression code could be 
   // optimized a bit more.  Anyone have a good plan?
   //
   //  abcdefghijgabcgabcdefghij
   // should compress to
   //  abcdefghijgabcg*1     *1 = abcdefghij
   // instead of
   //  abcdefghijgabc*1*2    *1 = gabc   *2 = defghij
   //
   //  aaaaaaaabaaaaaaaaabaababaaaaaaaaaaaaaaaaab
   // should compress to
   //  aaaa*1ba*2aba*3*4
   //  *1 = aaaa    *2 = aaaaaaaaba    *3 = baaaaaaaaa    *4 = aaaaaaaab
   //
   // I've tried thinking of a recusion technique and a looping technique, but
   // I can't think of anything that will provide the best compression in
   // every circumstance.  Until then, I'll just keep this semi-fast stuff
   // here.
   function CompressRecord($In) {
      // We are not ready yet
      return $In;
      
      
      $Out = '';
      $spots = array();
      
      $pos = 0;
      while ($pos < strlen($In)) {
         // Search for a string
	 $lastMatchPos = 0;
	 $lastMatchSize = 0;
	 $Key = substr($In, $pos, 2);
	 if (isset($spots[$Key]) && is_array($spots[$Key]) && 
	     count($spot[$Key]))
	    foreach ($spots[$Key] as $potential) {
	       // Remember -- $pos and $potential are pointing at hex-encoded
	       // strings!
	       if ($pos - $potential < 4094) {
  	          for ($size = 0; $size < 22 && $size + $potential < $pos &&
	               $In[$potential + $size] == $In[$pos + $size] &&
		       $In[$potential + $size + 1] == $In[$pos + $size + 1];
		       $size += 2) {
	             if ($size / 2 > $lastMatchSize) {
		        $lastMatchPos = ($pos - $potential) / 2;
			$lastMatchSize = $size / 2;
		     }
		  }
	       }
	 }
	 if ($lastMatchSize > 2) {
	    $size = $lastMatchSize;
	    $lastMatchSize -= 3;
	    $lastMatchSize = $lastMatchSize & 0x07;
	    $lastMatchPos = $lastMatchPos & 0x3FF8;
	    $lastMatchPos = $lastMatchPos << 3;
	    $Command = 0x8000 + $lastMatchPos + $lastMatchSize;
	    $Out .= $this->Int16($Command);
	    while ($size --) {
	       $Key = substr($In, $pos, 2);
	       if (! isset($spots[$Key]))
	          $spots[$Key] = array();
	       $spots[$Key][] = $pos;
	       $pos += 2;
	    }
	 } else {
	    // Check for space encoding
//	    if (substr($Out, -2) == '20' && ) {
//	       $Out = substr($Out...
	    // Do literal encoding
	 }
      }
      
      return $Out;
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
