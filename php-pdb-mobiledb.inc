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

define('PDB_MOBILEDB_DBVERSION', 1);
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
   

   // Override the built-in ReadFile function to allow for post-processing
   function ReadFile($file) {
      $res = PalmDB::ReadFile($file);
      if ($res)
         return $res;
      // add post-processing here
      return false;
   }
   
   
   // Generic function to load the AppInfo block into $this->AppInfo
   // Should only be called within this class
   // Return false to signal no error
   function LoadAppInfo($fileData) {
      $this->CategoryData = $this->LoadCategoryData($fileData);
      $fileData = substr($fileData, PDB_CATEGORY_SIZE);
      $this->MobileDBVersion = $this->LoadInt16($fileData);
      // WARNING -- this is short-circuited
      if ($this->MobileDBVersion != PDB_MOBILEDB_DBVERSION || 1) {
         $this->InitializeMobileDB();
	 return false;
      } 
      $fileData = substr($fileData, 2);
      $this->MobileDBLock = $this->LoadInt32($fileData);
      $fileData = substr($fileData, 4);
      $this->AppInfo = bin2hex($fileData);
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
      $this->Record[$recordInfo['UID']] = bin2hex($fileData);
      $this->RecordAttrs[$recordInfo['UID']] = $recordInfo['Attrs'];
      return false;
   }
}
?>
