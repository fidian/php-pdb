<?PHP
   /* PHP-PDB -- PHP class to write PalmOS databases.
    *            (maybe read in the future)
    *
    * Copyright (C) 2001 - Tyler Akins
    * Licensed under the GNU LGPL software license.
    * See the LEGAL file for more information
    *
    *
    * As a note, storing all of the information as hexadecimal kinda sucks,
    * but it is tough to store a binary string in PHP.  We double the size
    * of the data but decrease the difficulty level immensely.
    *
    *
    * Major thanks to the PalmOS documentation.  It made my life easier.
    *
    * I read the code from ToPTIP and used some ideas from it initially.
    * My code was written from scratch, but I compared output to ToPTIP.
    * http://depot.free-system.com/index.php3?projectid=2&action=show
    *
    * Code from p5-Palm
    * http://www.ooblick.com/software/coldsync/
    *
    * Maybe some code will be palmlib
    * http://mmmm.free.fr/palm/
    *
    * Maybe some code will be from PalmImage
    * http://palmimage.sourceforge.net/
    */

   define('PDB_HEADER_SIZE', 72); // Size of the database header
   define('PDB_INDEX_HEADER_SIZE', 6); // Size of the record index header
   define('PDB_RECORD_HEADER_SIZE', 8); // Size of the record index entry
   define('PDB_RESOURCE_SIZE', 10);  // Size of the resource index entry
   define('PDB_EPOCH_1904', 2082844800); // Difference between Palm's time and Unix
   
   define('PDB_ATTRIB_RESOURCE', 1);
   define('PDB_ATTRIB_READ_ONLY', 2);
   define('PDB_ATTRIB_APPINFO_DIRTY', 4);
   define('PDB_ATTRIB_BACKUP', 8);
   define('PDB_ATTRIB_OK_NEWER', 16);
   define('PDB_ATTRIB_RESET', 32);
   define('PDB_ATTRIB_OPEN', 64);
   
   define('PDB_RECORD_ATTRIB_PRIVATE', 8);
   define('PDB_RECORD_ATTRIB_DELETED', 16);
   define('PDB_RECORD_ATTRIB_DIRTY', 32);
   define('PDB_RECORD_ATTRIB_EXPUNGED', 64);
   
   class PalmDB {
      var $Records = array(); // All of the data from the records is here
      var $RecordAttrs = array(); // And their attributes are here
      var $CurrentRecord = 0; // Which record we are currently editing
      var $Name = ''; // Title of the document / name of the PDB file
      var $TypeID = '';
      var $CreatorID = '';
      var $Attributes = 0;
      var $Version = 0;
      var $CreationTime = 0; // Stored in unix time (Jan 1, 1970)
      var $ModificationTime = 0; // Stored in unix time (Jan 1, 1970)
      var $BackupTime = 0; // Stored in unix time (Jan 1, 1970)
      

      // Creates a new database class
      function PalmDB($Type, $Creator, $Name = '')
      {
         $this->TypeID = $Type;
	 $this->CreatorID = $Creator;
	 $this->Name = $Name;
	 $this->CreationTime = time();
	 $this->ModificationTime = time();
      }


      // Converts a byte and returns the value
      function Int8($value) {
         $value &= 0xFF;
         //echo "Int8(" . sprintf("%02x", $value) . ")<br>\n";
         return sprintf("%02x", $value);
      }
      
      
      // Converts an integer (two bytes) and returns the value
      function Int16($value) {
         $value &= 0xFFFF;
         //echo "Int16(" . sprintf("%02x%02x", $value / 256, $value % 256) . ")<br>\n";
         return sprintf("%02x%02x", $value / 256, $value % 256);
      }


      // Converts an integer (four bytes) and returns the value
      // Uncertain if this works
      function Int32($value) {
         $value = bin2hex(pack('N', $value));
         //echo "Int32(" . $value . ")<br>\n";
	 return $value;
      }
      
      // Converts the number into a double and returns the encoded value
      // 10.53 as a double should be 4025 0f5c 28f5 c28f
      function Double($value) {
         // May not work on every machine (or at least may require tweaks)
	 $value = bin2hex(pack('d', $value));
	 $value = substr($value, 14, 2) . substr($value, 12, 2) . 
	    substr($value, 10, 2) . substr($value, 8, 2) . 
	    substr($value, 6, 2) . substr($value, 4, 2) . 
	    substr($value, 2, 2) . substr($value, 0, 2);
	 return $value;
      }


      function String($value, $maxLen = false) {
         $value = bin2hex($value);
	 if ($maxLen !== false)
	    $value = substr($value, 0, $maxLen * 2);
	    
	 //echo "String(" . $value . ")<br>\n";
         return $value;
      }


      function GoToRecord($num = false) {
         if ($num === false)
	    return $this->CurrentRecord;
         $this->CurrentRecord = $num;
	 if (! isset($this->Records[$this->CurrentRecord]))
	    $this->Records[$this->CurrentRecord] = '';
	 return $num;
      }
      
      
      function GetRecordSize_Class($num) {
         return strlen($this->Records[$num]) / 2;
      }
      
      
      function GetRecordSize($num = false) {
         if ($num === false)
	    $num = $this->CurrentRecord;
	 return $this->GetRecordSize_Class($num);
      }
      
      
      // Adds to the current record.  The input data must be hex.
      function AppendCurrent($value) {
         if (! isset($this->Records[$this->CurrentRecord]))
	    $this->Records[$this->CurrentRecord] = '';
	 $this->Records[$this->CurrentRecord] .= $value;
      }
      
      
      function AppendInt8($value) {
         $this->AppendCurrent($this->Int8($value));
      }
      
      
      function AppendInt16($value) {
         $this->AppendCurrent($this->Int16($value));
      }
      
      
      function AppendInt32($value) {
         $this->AppendCurrent($this->Int32($value));
      }
      
      
      function AppendDouble($value) {
         $this->AppendCurrent($this->Double($value));
      }


      function AppendString($value, $maxLen = false) {
         $this->AppendCurrent($this->String($value));
      }
      

      function GetRecord($Rec) {
         if (isset($this->Records[$Rec]))
            return $this->Records[$Rec];
	 return '';
      }
      
      
      function ClearCurrentRecord() {
         $this->Records[$this->CurrentRecord] = '';
      }


      function GetRecordIDs() {
	 return array_keys($this->Records);
      }
      
      function GetRecordCount() {
         return count($this->Records);
      }
      
      
      // Returns the hexadecimal header of the pdb file
      function MakeHeader() {
         // 32 bytes = name, but only 31 available (one for null)
	 $header = $this->String($this->Name, 31);
	 $header .= 
	    '0000000000000000000000000000000000000000000000000000000000000000';
	 $header = substr($header, 0, 64);
	 
	 // Attributes & version fields
	 $header .= $this->Int16($this->Attributes);
	 $header .= $this->Int16($this->Version);
	 
	 // Creation date & modification date
	 if ($this->CreationTime != 0)
  	    $header .= $this->Int32($this->CreationTime + PDB_EPOCH_1904);
	 else
	    $header .= $this->Int32(0);
	 if ($this->ModificationTime != 0)
  	    $header .= $this->Int32($this->ModificationTime + PDB_EPOCH_1904);
	 else
	    $header .= $this->Int32(0);
	 if ($this->BackupTime != 0)
  	    $header .= $this->Int32($this->BackupTime + PDB_EPOCH_1904);
	 else
	    $header .= $this->Int32(0);
	 
	 // Modification number, app information id, sort information id
	 $header .= $this->Int32(0);
	 $header .= $this->Int32(0);
	 $header .= $this->Int32(0);
	 
	 // Type, creator
	 $header .= $this->String($this->TypeID, 4);
	 $header .= $this->String($this->CreatorID, 4);
	 
	 // Unique ID seed
	 $header .= $this->Int32(0);
	 
	 // next record list
	 $header .= $this->Int32(0);

         // Number of records
	 $header .= $this->Int16($this->GetRecordCount());
	 
	 // Dump each record
	 if ($this->GetRecordCount() != 0) {
	    $Offset = PDB_HEADER_SIZE + PDB_INDEX_HEADER_SIZE;
	    $Offset += PDB_RECORD_HEADER_SIZE * count($this->Records);
	    $keys = $this->GetRecordIDs();
	    sort($keys, SORT_NUMERIC);
  	    foreach ($keys as $index) {
	       $header .= $this->Int32($Offset);
	       if (isset($this->RecordAttrs[$index]))
	          $header .= $this->Int8($this->RecordAttrs[$index]);
	       else
	          $header .= $this->Int8(0);
		  
	       // The unique id is just going to be the record number
	       $UID = $this->Int32($index);
	       $UID = substr($UID, 2);
	       $header .= $UID;

               $Offset += $this->GetRecordSize($index);
	    }
	 }
	 $header .= '0000'; // placeholder bytes

         // AppInfo and SortInfo blocks go here
	 // It's a shame that we don't support them yet
	 
	 return $header;
      }
      

      function WriteToFile($file) {
         $header = $this->MakeHeader();
         fwrite($file, pack("H*", $header), 
	        strlen($header) / 2);
	 $keys = $this->GetRecordIDs();
	 sort($keys, SORT_NUMERIC);
	 foreach ($keys as $index) {
	    $data = $this->GetRecord($index);
	    //echo "Record $index -> $data<br>\n";
	    fwrite($file, pack("H*", $data), 
	           strlen($data) / 2);
	 }
	 fflush($file);
      }
      
      function WriteToStdout() {
         // You'd think these three lines would work!
//         $fp = fopen('php://stdout', 'wb');
//	 $this->WriteToFile($fp);
//	 fclose($fp);
	 
         $header = $this->MakeHeader();
         echo pack("H*", $header);
	 $keys = $this->GetRecordIDs();
	 sort($keys, SORT_NUMERIC);
	 foreach ($keys as $index) {
	    $data = $this->GetRecord($index);
	    //echo "Stdout Record $index -> $data<br>\n";
	    echo pack("H*", $data);
	 }
      }
      
      function DownloadPDB($filename) {
         global $HTTP_USER_AGENT;
	 
	 $isIE = 0;
	 if (strstr($HTTP_USER_AGENT, 'compatible; MSIE ') !== false &&
	     strstr($HTTP_USER_AGENT, 'Opera') === false) {
	    $isIE = 1;
	 }
	 
	 $filename = ereg_replace('[^-a-zA-Z0-9\.]', '_', $filename);
	 
	 if ($isIE) {
            header("Content-Disposition: inline; filename=$filename");
            header("Content-Type: application/download; name=\"$filename\"");
	 } else {
            header("Content-Disposition: attachment; filename=\"$filename\"");
            header("Content-Type: application/x-pilot; name=\"$filename\"");
	 }

	 $this->WriteToStdout();
      }
   }
?>
