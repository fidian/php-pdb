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
    * Code and ideas from p5-Palm (Thanks!)
    * http://www.ooblick.com/software/coldsync/
    *
    * Maybe some code will be palmlib
    * http://mmmm.free.fr/palm/
    *
    * Maybe some code will be from PalmImage
    * http://palmimage.sourceforge.net/
    */

   /*
    * Define constants
    */

   // Sizes
   define('PDB_HEADER_SIZE', 72); // Size of the database header
   define('PDB_INDEX_HEADER_SIZE', 6); // Size of the record index header
   define('PDB_RECORD_HEADER_SIZE', 8); // Size of the record index entry
   define('PDB_RESOURCE_SIZE', 10);  // Size of the resource index entry
   define('PDB_EPOCH_1904', 2082844800); // Difference between Palm's time and Unix
   
   // Attribute Flags
   define('PDB_ATTRIB_RESOURCE', 1);
   define('PDB_ATTRIB_READ_ONLY', 2);
   define('PDB_ATTRIB_APPINFO_DIRTY', 4);
   define('PDB_ATTRIB_BACKUP', 8);
   define('PDB_ATTRIB_OK_NEWER', 16);
   define('PDB_ATTRIB_RESET', 32);
   define('PDB_ATTRIB_OPEN', 64);
   
   // Record Flags
   define('PDB_RECORD_ATTRIB_PRIVATE', 8);
   define('PDB_RECORD_ATTRIB_DELETED', 16);
   define('PDB_RECORD_ATTRIB_DIRTY', 32);
   define('PDB_RECORD_ATTRIB_EXPUNGED', 64);
   
   // Category support
   define('PDB_CATEGORY_NUM', 16);  // Number of categories
   define('PDB_CATEGORY_NAME_LENGTH', 16);  // Bytes allocated for name
   define('PDB_CATEGORY_SIZE', 276); // 2 + (num * length) + num + 1 + 1
   
   
   /*
    * PalmDB Class
    *
    * Contains all of the required methods and variables to write a pdb file.
    * Extend this class to provide functionality for memos, addresses, etc.
    */
   class PalmDB {
      var $Records = array();     // All of the data from the records is here
                                  // Key = record ID
      var $RecordAttrs = array(); // And their attributes are here
      var $CurrentRecord = 0;     // Which record we are currently editing
      var $Name = '';             // Name of the PDB file
      var $TypeID = '';           // The 'Type' of the file (4 chars)
      var $CreatorID = '';        // The 'Creator' of the file (4 chars)
      var $Attributes = 0;        // Attributes (bitmask)
      var $Version = 0;           // Version of the file
      var $CreationTime = 0;      // Stored in unix time (Jan 1, 1970)
      var $ModificationTime = 0;  // Stored in unix time (Jan 1, 1970)
      var $BackupTime = 0;        // Stored in unix time (Jan 1, 1970)
      var $AppInfo = '';          // Basic AppInfo block
      var $SortInfo = '';         // Basic SortInfo block


      // Creates a new database class
      function PalmDB($Type, $Creator, $Name = '')
      {
         $this->TypeID = $Type;
	 $this->CreatorID = $Creator;
	 $this->Name = $Name;
	 $this->CreationTime = time();
	 $this->ModificationTime = time();
      }


      /*
       * Data manipulation functions
       *
       * These convert various numbers and strings into the hexadecimal
       * format that is used internally to construct the file.  We use hex
       * encoded strings since that is a lot easier to work with than binary
       * data in strings, and we can easily tell how big the true value is.
       * B64 encoding does some odd stuff, so we just make the memory
       * consumption grow tremendously and the complexity level drops
       * considerably.
       */
       
      // Converts a byte and returns the value
      function Int8($value) {
         $value &= 0xFF;
         return sprintf("%02x", $value);
      }
      

      // Converts an integer (two bytes) and returns the value
      function Int16($value) {
         $value &= 0xFFFF;
         return sprintf("%02x%02x", $value / 256, $value % 256);
      }


      // Converts an integer (four bytes) and returns the value
      // Uncertain if this works
      function Int32($value) {
         $value = bin2hex(pack('N', $value));
	 return $value;
      }
      
      
      // Converts the number into a double and returns the encoded value
      // Not sure if this will work on all platforms.
      // 10.53 as a double should be 4025 0f5c 28f5 c28f
      function Double($value) {
	 $value = bin2hex(pack('d', $value));
	 $value = substr($value, 14, 2) . substr($value, 12, 2) . 
	    substr($value, 10, 2) . substr($value, 8, 2) . 
	    substr($value, 6, 2) . substr($value, 4, 2) . 
	    substr($value, 2, 2) . substr($value, 0, 2);
	 return $value;
      }


      // Converts a string into hexadecimal.
      // If $maxLen is specified and is greater than zero, the string is 
      // trimmed and will contain up to $maxLen characters.
      // String("abcd", 2) will return "ab" hex encoded (a total of 4
      // resulting bytes, but 2 encoded characters).
      // Returned string is *not* NULL-terminated.
      function String($value, $maxLen = false) {
         $value = bin2hex($value);
	 if ($maxLen !== false && $maxLen > 0)
	    $value = substr($value, 0, $maxLen * 2);
         return $value;
      }
      
      
      // Pads a hex-encoded value (typically a string) to a fixed size.
      // May grow too long if $value starts too long
      // $value = hex encoded value
      // $minLen = Append nulls to $value until it reaches $minLen
      // $minLen is the desired size of the string, unencoded.
      // PadString('6162', 3) results in '616200' (remember the hex encoding)
      function PadString($value, $minLen) {
         $PadBytes = '00000000000000000000000000000000';
	 $PadMe = $minLen - (strlen($value) / 2);
	 while ($PadMe > 0) {
	    if ($PadMe > 16)
	       $value .= $PadBytes;
	    else
	       return $value . substr($PadBytes, 0, $PadMe * 2);
	       
	    $PadMe = $minLen - (strlen($value) / 2);
	 }
	 
	 return $value;
      }


      /*
       * Record manipulation functions
       */
      
      // Returns the current record number
      // Sets the current record pointer to the new record number if an
      // argument is passed in.
      // Does basic record initialization if we are going to a new record.
      function GoToRecord($num = false) {
         if ($num === false)
	    return $this->CurrentRecord;
         $this->CurrentRecord = $num;
	 if (! isset($this->Records[$this->CurrentRecord])) {
	    $this->Records[$this->CurrentRecord] = '';
	    $this->RecordAttrs[$this->CurrentRecord] = 0;
	 }
	 return $num;
      }
      

      // Returns the size of the current record if no arguments.
      // Returns the size of the specified record if arguments.
      // Do not override this function -- override GetRecordSize_Class
      function GetRecordSize($num = false) {
         if ($num === false)
	    $num = $this->CurrentRecord;
	 return $this->GetRecordSize_Class($num);
      }
      
      
      // Returns the size of the record index passed in
      // Override this function instead of GetRecordSize
      function GetRecordSize_Class($num) {
         if (! isset($this->Records[$num]))
	    return 0;
         return strlen($this->Records[$num]) / 2;
      }
      
      
      // Adds to the current record.  The input data must be already
      // hex encoded.
      function AppendCurrent($value) {
         if (! isset($this->Records[$this->CurrentRecord]))
	    $this->Records[$this->CurrentRecord] = '';
	 $this->Records[$this->CurrentRecord] .= $value;
      }
      
      
      // Adds a byte to the current record
      function AppendInt8($value) {
         $this->AppendCurrent($this->Int8($value));
      }
      
      
      // Adds an integer (2 bytes) to the current record
      function AppendInt16($value) {
         $this->AppendCurrent($this->Int16($value));
      }
      
      
      // Adds an integer (4 bytes) to the current record
      function AppendInt32($value) {
         $this->AppendCurrent($this->Int32($value));
      }
      
      
      // Adds a double to the current record
      function AppendDouble($value) {
         $this->AppendCurrent($this->Double($value));
      }

 
      // Adds a string (not NULL-terminated)
      function AppendString($value, $maxLen = false) {
         $this->AppendCurrent($this->String($value));
      }
      

      // Returns the hex-encoded data for the specified record
      function GetRecord($Rec) {
         if (isset($this->Records[$Rec]))
            return $this->Records[$Rec];
	 return '';
      }
      
      
      // Erases all data for the current record
      function ClearCurrentRecord() {
         $this->Records[$this->CurrentRecord] = '';
      }
      
      
      // Deletes the current record
      // You are urged to use GoToRecord() and jump to an existing record
      // after this function call so that the deleted record doesn't
      // get accidentally recreated/used -- all append functions will
      // create a new, empty record if the current record doesn't exist.
      function DeleteCurrentRecord() {
         if (isset($this->Records[$this->CurrentRecord]))
	    unset($this->Records[$this->CurrentRecord]);
      }


      // Returns an array of available record IDs in the order they should
      // be written.
      function GetRecordIDs() {
         $keys = array_keys($this->Records);
	 if (! is_array($keys) || count($keys) < 1)
	    return array();
	 sort($keys);
	 return $keys;
      }
      
      
      // Returns the number of records.  This should match the number of
      // keys returned by GetRecordIDs().
      function GetRecordCount() {
         return count($this->Records);
      }
      
      
      // Returns the size of the AppInfo block.
      function GetAppInfoSize() {
         if (! isset($this->AppInfo))
	    return 0;
         return strlen($this->AppInfo) / 2;
      }
      
      
      // Returns the AppInfo block (hex encoded)
      function GetAppInfo() {
         if (! isset($this->AppInfo))
	    return 0;
	 return $this->AppInfo;
      }

      
      // Returns the size of the SortInfo block
      function GetSortInfoSize() {
         if (! isset($this->SortInfo))
	    return 0;
         return strlen($this->SortInfo) / 2;
      }
      
      
      // Returns the SortInfo block (hex encoded)
      function GetSortInfo() {
         if (! isset($this->SortInfo))
	    return 0;
	 return $this->SortInfo;
      }


      /*
       * Category Support
       */
       
      // Creates the hex-encoded data to be stuck in the AppInfo
      // block if the database supports categories.
      //
      // Data format:
      //    $categoryArray[id#] = name
      // Or:
      //    $categoryArray[id#]['name'] = name
      //    $categoryArray[id#]['renamed'] = true / false
      //
      // Tips:
      //  * I'd suggest numbering your categories sequentially
      //  * Do not have a category 0.  It must always be 'Unfiled'.  This
      //    function will overwrite any category with the ID of 0.
      //  * There is a maximum of 16 categories, including 'Unfiled'.
      //
      // Category 0 is reserved for 'Unfiled'
      // Categories 1-127 are used for handheld ID numbers
      // Categories 128-255 are used for desktop ID numbers
      // Do not let category numbers be created larger than 255
      function CreateCategoryData($CategoryArray) {
         $CategoryArray[0] = array('name' => 'Unfiled', 'renamed' => 0)
	 $CatsWritten = 0;
	 $LastIdWritten = 0;
	 $RenamedFlags = 0;
	 $CategoryStr = '';
	 $IdStr = ''
	 $keys = array_keys($CategoryArray);
	 sort($keys);
	 foreach ($keys as $id) {
	    if ($CatsWritten < PDB_CATEGORY_NUM) {
	       $CatsWritten ++;
	       $LastIdWritten = $id;
	       $RenamedFlags *= 2;
	       if (is_array($CategoryArray[$id]) && 
	           isset($CategoryArray[$id]['renamed']) &&
		   $CategoryArray[$id]['renamed'])
		  $RenamedFlags += 1;
	       $name = '';
	       if (is_array($CategoryArray[$id])) {
	          if (isset($CategoryArray[$id]['name']))
		     $name = $CategoryArray[$id]['name'];
	       } else
	          $name = CategoryArray[$id];
	       $name = $this->String($name, PDB_CATEGORY_NAME_LENGTH);
	       $CategoryStr .= $this->PadString($name,
	                                        PDB_CATEGORY_NAME_LENGTH);
	       $IdStr .= $this->Int8($id);
	    }
	 }
	 
	 while ($CatsWritten < PDB_CATEGORY_NUM) {
	    $CatsWritten ++;
	    $LastIdWritten ++;
	    $RenamedFlags *= 2;
	    $CategoryStr .= $this->PadString('', PDB_CATEGORY_NAME_LENGTH);
	    $IdStr .= $this->Int8($LastIdWritten);
	 }
	 
	 $TrailingBytes = $this->Int8($LastIdWritten);
	 $TrailingBytes .= $this->Int8(0);
	 
	 // Error checking
	 if ($LastIdWritten >= 256)
	    return $this->PadString('', PDB_CATEGORY_SIZE);
	 
	 return $this->Int32($RenamedFlags) . $CategoryStr . $IdStr . 
	    $TrailingBytes;
      }
     
     
      /*
       * Database Writing Functions
       */
      
      // Returns the hex encoded header of the pdb file
      // Header = name, attributes, version, creation/modification/backup 
      //          dates, modification number, some offsets, record offsets,
      //          record attributes, appinfo block, sortinfo block
      function MakeHeader() {
         // 32 bytes = name, but only 31 available (one for null)
	 $header = $this->String($this->Name, 31);
	 $header = $this->PadString($header, 32);
	 
	 // Attributes & version fields
	 $header .= $this->Int16($this->Attributes);
	 $header .= $this->Int16($this->Version);
	 
	 // Creation date & modification date
	 if ($this->CreationTime != 0)
  	    $header .= $this->Int32($this->CreationTime + PDB_EPOCH_1904);
	 else
	    $header .= $this->Int32(time() + PDB_EPOCH_1904);
	 if ($this->ModificationTime != 0)
  	    $header .= $this->Int32($this->ModificationTime + PDB_EPOCH_1904);
	 else
	    $header .= $this->Int32(time() + PDB_EPOCH_1904);
	 if ($this->BackupTime != 0)
  	    $header .= $this->Int32($this->BackupTime + PDB_EPOCH_1904);
	 else
	    $header .= $this->Int32(0);

         // Calculate the initial offset
	 $Offset = PDB_HEADER_SIZE + PDB_INDEX_HEADER_SIZE;
	 $Offset += PDB_RECORD_HEADER_SIZE * count($this->GetRecordIDs());
	 
	 // Modification number, app information id, sort information id
	 $header .= $this->Int32(0);

         $AppInfo_Size = $this->GetAppInfoSize();
	 if ($AppInfo_Size > 0) {
	    $header .= $this->Int32($Offset);
	    $Offset += $AppInfo_Size;
	 } else
  	    $header .= $this->Int32(0);
	    
	 $SortInfo_Size = $this->GetSortInfoSize();
	 if ($SortInfo_Size > 0) {
	    $header .= $this->Int32($Offset);
	    $Offset += $SortInfo_Size;
	 } else
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

         // AppInfo and SortInfo blocks go here
	 if ($AppInfo_Size > 0)
	    $header .= $this->GetAppInfo();
	    
	 if ($SortInfo_Size > 0)
	    $header .= $this->GetSortInfo();
	 
	 return $header;
      }
      

      // Writes the database to the file handle specified.
      // Use this function like this:
      //   $file = fopen("output.pdb", "wb"); 
      //   // "wb" = write binary for non-Unix systems
      //   if (! $file) {
      //      echo "big problem -- can't open file";
      //      exit;
      //   }
      //   $pdb->WriteToFile($file);
      //   fclose($file);
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
      
      
      // Writes the database to the standard output (like echo).
      // Can be trapped with output buffering
      function WriteToStdout() {
         // You'd think these three lines would work.
	 // If someone can figure out why they don't, please tell me.
	 //
         // $fp = fopen('php://stdout', 'wb');
         // $this->WriteToFile($fp);
         // fclose($fp);
	 
         $header = $this->MakeHeader();
         echo pack("H*", $header);
	 $keys = $this->GetRecordIDs();
	 sort($keys, SORT_NUMERIC);
	 foreach ($keys as $index) {
	    $data = $this->GetRecord($index);
	    echo pack("H*", $data);
	 }
      }
      
      
      // Writes the database to the standard output (like echo) but also
      // writes some headers so that the browser should prompt to save the
      // file properly.
      //
      // Use this only if you didn't send any content and you only want the
      // PHP script to output the PDB file and nothing else.  An example
      // would be if you wanted to have 'download' link so the user can
      // stick the information they are currently viewing and transfer
      // it easily into their handheld.
      //
      // $filename is the desired filename to download the database as.
      // For example, DownloadPDB('memos.pdb');
      function DownloadPDB($filename) {
         global $HTTP_USER_AGENT;
	 
	 // Alter the filename to only allow certain characters.
	 // Some platforms and some browsers don't respond well if
	 // there are illegal characters (such as spaces) in the name of
	 // the file being downloaded.
	 $filename = ereg_replace('[^-a-zA-Z0-9\\.]', '_', $filename);
	 
	 if (strstr($HTTP_USER_AGENT, 'compatible; MSIE ') !== false &&
	     strstr($HTTP_USER_AGENT, 'Opera') === false) {
	    // IE doesn't properly download attachments.  This should work
	    // pretty well for IE 5.5 SP 1
            header("Content-Disposition: inline; filename=$filename");
            header("Content-Type: application/download; name=\"$filename\"");
	 } else {
	    // Use standard headers for Netscape, Opera, etc.
            header("Content-Disposition: attachment; filename=\"$filename\"");
            header("Content-Type: application/x-pilot; name=\"$filename\"");
	 }

	 $this->WriteToStdout();
      }
   }
?>
