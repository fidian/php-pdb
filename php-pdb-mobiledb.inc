<?PHP
/* Class extender for Handmark MobileDB databases.
 *
 * Copyright (C) 2001 - Tyler Akins
 * Licensed under the GNU LGPL software license.
 * See the LEGAL file for more information
 *
 *
 * Database format detailed at
 *   http://www.handmark.com/products/mobiledb/dbstructure.htm
 *
 * Format is for MobileDB database version 3.0
 */

/*
 * Define constants
 */

define('PDB_MOBILEDB_DBVERSION', 3);  // As of Nov 22, 2001, the docs lie.
define('PDB_MOBILEDB_FILTER_TEXT_LENGTH', 40);
define('PDB_MOBILEDB_FILTER_LENGTH', 44);
define('PDB_MOBILEDB_SORT_LENGTH', 4);


/*
 * PalmDB Class
 *
 * Contains all of the required methods and variables to write a pdb file.
 * Extend this class to provide functionality for memos, addresses, etc.
 */
class PalmMobileDB  extends PalmDB {
   var $CategoryData;  // Category information
   var $MobileDBVersion;  // Version number
   var $MobileDBLock;  // Hash of password
   var $MobileDBDontSearch;  // True = DB is invisible to Find
   var $MobileDBEditOnSelect;  // True = Record should be edited by default
   var $MobileDBLongDates;  // True = Display dates in long format
   var $MobileDBReserved = array();  // Reserved bytes
   var $MobileDBFilters = array();  // The user's filters
   var $MobileDBSort = array();  // The user's sort criteria
   var $FieldLabels = -1;  // Labels for the fields
   var $Preferences = -1;  // "Dynamic info"
   var $DataType = -1;  // Data types for the fields
   var $FieldLengths = -1;  // Visible field lengths for fields

   // Creates a new database class
   function PalmMobileDB($Name = '') {
      PalmDB::PalmDB('Mdb1', 'Mdb1', $Name);
      $this->InitializeMobileDB();
   }


   // Sets all of the variables to a good default value
   function InitializeMobileDB() {
      $this->CategoryData = array(
         0 => array('Name' => 'Unfiled', 'Renamed' => false),
	 1 => array('Name' => 'FieldLabels', 'Renamed' => false),
	 2 => array('Name' => 'DataRecords', 'Renamed' => false),
	 3 => array('Name' => 'DataRecordsFout', 'Renamed' => false),
	 4 => array('Name' => 'Preferences', 'Renamed' => false),
	 5 => array('Name' => 'DataType', 'Renamed' => false),
	 6 => array('Name' => 'FieldLengths', 'Renamed' => false)
      );
      $this->MobileDBVersion = PDB_MOBILEDB_DBVERSION;
      $this->MobileDBLock = 0;
      $this->MobileDBDontSearch = 0;
      $this->MobileDBEditOnSelect = 0;
      $this->MobileDBLongDates = 0;
      $this->MobileDBReserved = array(0, 0);
      $this->MobileDBFilters = array(
         array('', 0, 0),
	 array('', 0, 0),
	 array('', 0, 0)
      );
      $this->MobileDBSort = array(
         array(0, 0, 0),
	 array(0, 0, 0),
	 array(0, 0, 0)
      );
   }
   

   // Returns the size of the current record if no arguments.
   // Returns the size of the specified record if arguments.
   function GetRecordSize($num = false) {
      if ($num === false)
         $num = $this->CurrentRecord;
      if (! isset($this->Records[$num]))
         return 0;
      return strlen($this->GetRecord($num)) / 2;
   }
   
   
   // Returns the hex-encoded data for the specified record or the current
   // record if not specified
   function GetRecord($Rec = false) {
      if ($Rec === false)
         $Rec = $this->CurrentRecord;
      if (! isset($this->Records[$Rec]))
         return '';
      $RecStr = $this->Int32(4294967041) . $this->Int16(65280) .
         $this->Int8(0);

      ksort($this->Records[$Rec]);
      foreach ($this->Records[$Rec] as $id => $data) {
         settype($id, 'integer');
	 $RecStr .= $this->Int8($id);
	 if ($data != 0 && $data != '')
	    $RecStr .= bin2hex($data);
	 $RecStr .= $this->Int8(0);
      }
      $RecStr .= $this->Int8(255);

      return $RecStr;
   }   
   
   
   // Generic function to load the AppInfo block into $this->AppInfo
   // Should only be called within this class
   // Return false to signal no error
   function LoadAppInfo($fileData) {
      $this->CategoryData = $this->LoadCategoryData($fileData);
      $fileData = substr($fileData, PDB_CATEGORY_SIZE);
      $this->MobileDBVersion = $this->LoadInt16($fileData);
      // WARNING -- this is short-circuited
      if ($this->MobileDBVersion != PDB_MOBILEDB_DBVERSION) {
         $this->InitializeMobileDB();
	 return false;
      } 
      $fileData = substr($fileData, 2);
      $this->MobileDBLock = $this->LoadInt32($fileData);
      $fileData = substr($fileData, 4);
      $this->MobileDBFilters = array();
      $this->MobileDBFilters[] = $this->LoadAppInfo_Filter($fileData);
      $fileData = substr($fileData, PDB_MOBILEDB_FILTER_LENGTH);
      $this->MobileDBFilters[] = $this->LoadAppInfo_Filter($fileData);
      $fileData = substr($fileData, PDB_MOBILEDB_FILTER_LENGTH);
      $this->MobileDBFilters[] = $this->LoadAppInfo_Filter($fileData);
      $fileData = substr($fileData, PDB_MOBILEDB_FILTER_LENGTH);
      $this->MobileDBSort = array();
      $this->MobileDBSort[] = $this->LoadAppInfo_Sort($fileData);
      $fileData = substr($fileData, PDB_MOBILEDB_SORT_LENGTH);
      $this->MobileDBSort[] = $this->LoadAppInfo_Sort($fileData);
      $fileData = substr($fileData, PDB_MOBILEDB_SORT_LENGTH);
      $this->MobileDBSort[] = $this->LoadAppInfo_Sort($fileData);
      return false;
   }
   
   
   // Loads a single filter from the string passed in
   function LoadAppInfo_Filter($data) {
      $text = false;
      for ($i = 0; $i < PDB_MOBILEDB_FILTER_TEXT_LENGTH; $i ++) {
         if (bin2hex($data[$i]) == '00') {
	    $text = substr($data, 0, $i);
	    $i = PDB_MOBILEDB_FILTER_TEXT_LENGTH;
	 }
      }
      if ($text === false)
         $text = substr($data, 0, PDB_MOBILEDB_FILTER_TEXT_LENGTH);
      $data = substr($data, PDB_MOBILEDB_FILTER_TEXT_LENGTH);
      $fieldNo = $this->LoadInt8($data);
      $data = substr($data, 1);
      $flags = $this->LoadInt8($data);
      return array($text, $fieldNo, $flags);
   }
   
   
   // Loads a single sort criteria from the string passed in
   function LoadAppInfo_Sort($data) {
      $fieldNo = $this->LoadInt8($data);
      $data = substr($data, 1);
      $descending = $this->LoadInt8($data);
      $data = substr($data, 1);
      $type = $this->LoadInt8($data);
      return array($fieldNo, $descending, $type);
   }
   

   // Generic function to load a record
   // Should only be called within this class
   // Return false to signal no error
   function LoadRecord($fileData, $recordInfo) {
      // There should be no 'Unfiled' records
      if ($recordInfo['Attrs'] & PDB_CATEGORY_MASK == 0)
         return true;
    
      // Check the first 6 bytes.  They must always be correct or else the
      // record is invalid
      if ($this->LoadInt32($fileData) != 4294967041)
         return true;
      $fileData = substr($fileData, 4);
      if ($this->LoadInt16($fileData) != 65280)
         return true;
      // 3 bytes removed -- it appears that an extra NULL is inserted here
      $fileData = substr($fileData, 3);
      
      // Every record is an array.  Data is separated by NULL characters
      // Format = [# of field] [field data with no NULLs] [NULL]
      // End of data = [0xFF]
      $SaveInfo = array();
      $Fieldnum = $this->LoadInt8($fileData);
      $fileData = substr($fileData, 1);
      while ($Fieldnum != 255 && strlen($fileData) > 1) {
         $fileData = explode("\x00", $fileData, 2);
	 $SaveInfo[$Fieldnum] = $fileData[0];
	 $fileData = $fileData[1];
	 $Fieldnum = $this->LoadInt8($fileData);
	 $fileData = substr($fileData, 1);
      }
      
      // Now that we have the data, just put it in the right spot.
      if (($recordInfo['Attrs'] & PDB_CATEGORY_MASK) == 1)
         $this->FieldLabels = $recordInfo['UID'];
      elseif (($recordInfo['Attrs'] & PDB_CATEGORY_MASK) == 4)
         $this->Preferences = $recordInfo['UID'];
      elseif (($recordInfo['Attrs'] & PDB_CATEGORY_MASK) == 5)
         $this->DataType = $recordInfo['UID'];
      elseif (($recordInfo['Attrs'] & PDB_CATEGORY_MASK) == 6)
         $this->FieldLengths = $recordInfo['UID'];

      $this->Records[$recordInfo['UID']] = $SaveInfo;
      $this->RecordAttrs[$recordInfo['UID']] = $recordInfo['Attrs'];

      return false;
   }
}
?>
