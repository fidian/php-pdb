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


/*
 * PalmDB Class
 *
 * Contains all of the required methods and variables to write a pdb file.
 * Extend this class to provide functionality for memos, addresses, etc.
 */
class PalmMobileDB  extends PalmDB {

   // Creates a new database class
   function PalmMobileDB($Name = '') {
      PalmDB::PalmDB('Mdb1', 'Mdb1', $Name);
   }


   // Generic function to load the AppInfo block into $this->AppInfo
   // Should only be called within this class
   function LoadAppInfo($fileData) {
      $this->AppInfo = bin2hex($fileData);
      return false;
   }
   
   
   // Generic function to load the SortInfo block into $this->SortInfo
   // Should only be called within this class
   function LoadSortInfo($fileData) {
      $this->SortInfo = bin2hex($fileData);
      return false;
   }
   
   
   // Generic function to load a record
   // Should only be called within this class
   function LoadRecord($fileData, $recordInfo) {
      $this->Record[$recordInfo['UID']] = bin2hex($fileData);
      $this->RecordAttrs[$recordInfo['UID']] = $recordInfo['Attrs'];
      return false;
   }
}
?>
