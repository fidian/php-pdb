<?php
/* Class extender for Todo databases
 * Class donated by Eduardo Pascual Martinez
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */
 
// Attributes for the record
define('PDB_TODO_ATTRIB_EXPUNGED', 0x80);
define('PDB_TODO_ATTRIB_DELETED', 0x40);
define('PDB_TODO_ATTRIB_ARCHIVE', 0x08);  // Only set if expunged or deleted
define('PDB_TODO_ATTRIB_DIRTY', 0x20);
define('PDB_TODO_ATTRIB_PRIVATE', x010);

// Easier to use one of these if you need it
define('PDB_TODO_ATTRIB_DELE_EXP', 0xC0);  // Deleted and Expunged bits
define('PDB_TODO_ATTRIB_HIGH', 0xF0);  // All of above except Archive bit

/* A special note about the attributes:
 * The lower 4 bits (0x01 through 0x08) are usually reserved for the
 * category.  Apparently, if the record is expunged or deleted, then
 * the category is tossed aside and the archive bit can come into play.
 * This overlap is somewhat bad.  To try to help you out, the category
 * bits and attribute bits are separated in the class.  Please access
 * category bits with the record key of 'category' and the attribute
 * bits are available with the 'attributes' array key.
 *
 * Do not set a category and the archive bit at the same time.
 *
 * When checking to see if the record has the archive bit enabled, you
 * would be wise to use code similar to this:
 *
 *   if ($record['attributes'] & PDB_ADDR_ATTRIB_DELE_EXP) {
 *      if ($record['attributes'] & PDB_ADDR_ATTRIB_ARCHIVE) {
 *         echo "Record deleted/expunged and archive bit set.";
 *      } else {
 *         echo "Record deleted/expunged and archive bit not set.";
 *      }
 *   } else {
 *      echo "Record is not deleted/expunged.";
 *   }
 */


/* The data for SetRecordRaw and from GetRecordRaw should be/return a
 * special array, detailed below.  Optional values can be set to '' or not
 * defined.  If they are anything else (including zero), they are considered
 * to be 'set'.  Optional values are marked with a ^.
 *
 * Key            Example             Description
 * ----------------------------------------------
 * description    ToDo                This is the ToDo text
 * note           Note                ^ A note for the todo
 * due_date       2002-06-03          ^ Year-Month-Day of the todo
 * completed      1                   ^ 1/0 (default 0 not completed)
 * priority       3                   ^ 1-5 (default 1)
 * category       2                   ^ 0-15 (default 0)
 * attributes     ???                 ^ Bitmask of attributes (see above)
 */
 
class PalmTodoList extends PalmDB {
   var $CategoryList;
   var $FirstDay;

   // Constructor.
   function PalmTodoList() {
      PalmDB::PalmDB('DATA','todo','ToDoDB');
    
      //Set a default CategoryList array.
      $this->CategoryList[1]['Name'] = 'Bussines';
      $this->CategoryList[1]['Renamed'] = false;
      $this->CategoryList[2]['Name'] = 'Personal';
      $this->CategoryList[2]['Renamed'] = false;

      $this->FirstDay = 0;
   }

   // Returns a new array with default data for a new record.
   // This doesn't actually add the record.
   function NewRecord() {
      // No due date by default.
      $Item['due_date'] = '';

      $Item['completed'] = 0;
      $Item['prioity'] = 1;

      $Item['description'] = '';
      $Item['note'] = '';

      $Item['category'] = 0;

      return $Item;
   }

   // Converts a date string ( YYYY-MM-DD )( "2001-10-31" )
   // into bitwise ( YYYY YYYM MMMD DDDD )
   // Should only be used when saving
   function DateToInt16($date) {
      $YMD = explode('-', $date);
      return ($YMD[0] - 1904) * 512 + $YMD[1] * 32 + $YMD[2];
   }
   
   
   // Converts a bitwise date ( YYYY YYYM MMMD DDDD )
   // Into the human readable date string ( YYYY-MM-DD )( "2001-10-31" )
   // Should only be used when loading
   function Int16ToDate($number) {
      $year = $number / 512;
      settype($year, "integer");
      $year += 1904;
      $number = $number % 512;
      $month = $number / 32;
      settype($month, "integer");
      $day = $number % 32;
      return $year . '-' . $month . '-' . $day;
   }  

   // Overrides the GetRecordSize method.
   // Probably should only be used when saving
   function GetRecordSize($num = false) {
      if ($num === false)
         $num = $this->CurrentRecord;
     
      if (! isset($this->Records[$num]) || ! is_array($this->Records[$num]))
         return PalmDB::GetRecordSize($num);

      $data = $this->Records[$num];

      // Due_date (1).
      // Completed and Priority (2).
      // NULL for description
      // NULL for note
      $Bytes = 5;

      if ($data['description'] != '') {
         $Bytes += strlen($data['description']);
      }

      if ($data['note'] != '') {
         $Bytes += strlen($data['note']);
      }

      return $Bytes;
   }

   // Overrides the GetRecord method. We store data in associative arrays.
   // Just convert the data into the proper format and then return the
   // generated string.
   function GetRecord($num = false) {
      if ($num === false) {
         $num = $this->CurrentRecord;
      }

      if (! isset($this->Records[$num]) || ! is_array($this->Records[$num])) {
         return PalmDB::GetRecord($num);
      }

      $data = $this->Records[$num];
      $RecordString = '';

      if (isset($data['due_date'])) {
         list($Y,$M,$D) = split('-',$data['due_date']);
         $rawDate = ($D & 0x001f) |
                    (($M & 0x000f) << 5) |
                    ((($Y - 1904) & 0x007f) << 9);
      } else {
         $rawDate = 0xffff;
      }
      $RecordString .= $this->Int16($rawDate);

      $priority = $data['priority'] & 0x7f;
      if ($data['completed']) {
         $priority |= 0x80;
      }
      $RecordString .= $this->Int8($priority);

      $RecordString .= $this->String($data['description']);
      $RecordString .= $this->Int8(0);
      $RecordString .= $this->String($data['note']);
      $RecordString .= $this->Int8(0);

      return $RecordString;
   }

   // Sets the data for the current record
   function SetRecordRaw($A, $B = false) {
      if ($B === false) {
         $B = $A;
         $A = $this->CurrentRecord;
      }
      if (!isset($B['priority'])) {
         $B['priority'] = 1;
      }
      $this->Records[$A] = $B;

      if (isset($B['attributes'])) {
         if ($B['attributes']['expunged'] || $B['attributes']['deleted']) {
            $atts = $B['attributes'] & PDB_TODO_ATTRIB_ARCHIVE;
         } else {
            $atts = $B['category'] & PDB_CATEGORY_MASK;
         }
       
         $atts |= $B['attributes'] & PDB_TODO_ATTRIB_HIGH;

         $this->RecordAttrs[$A] = $atts;
      } else {
         $this->RecordAttrs[$A] = $B['category'] & PDB_CATEGORY_MASK;
      }
   }

   // Returns the size of the AppInfo block.  It is the size of the
   // category list plus four bytes.
   function GetAppInfoSize() {
      return PDB_CATEGORY_SIZE + 6;
   }

   // Returns the AppInfo block. It is composed of the category list.
   function GetAppInfo() {
      // Category List.
      $this->AppInfo = $this->CreateCategoryData($this->CategoryList);

      // Unknown thing (first_day_in_week)
      // 00 00 00 00 FD 00 == where FD is the first day in week.
      // I'm using 0 as the default value since I don't know what it should be
      $this->AppInfo .= $this->Int16(0);
      $this->AppInfo .= $this->Int16(0);
      $this->AppInfo .= $this->Int8(count($this->CategoryList) - 1);
      $this->AppInfo .= $this->Int8(0);
      return $this->AppInfo;
   }

   // Sets the category list.
   function SetCategoryList($list) {
      if (!is_array($list)) {
         return false;
      }

      // Validates if the first element is empty.
      $i = $list[0] != '' ? 1 : 0;

      $keys = array_keys($list);
      sort($keys);
      foreach ($keys as $id) {
         $this->CategoryList[$id+$i]['Name'] = $list[$id];
         $this->CategoryList[$id+$i]['Renamed'] = false;
      }
      return true;
   }

   // Returns the category list.
   function GetCategoryList() {
      return $this->CategoryList;
   }

   // Parse $fileData for the information we need when loading a Todo
   // file.
   function LoadAppInfo($fileData) {
      $this->AppInfo = bin2hex($fileData);
      $this->CategoryList = $this->LoadCategoryData($fileData);
      $fileData = substr($fileData, PDB_CATEGORY_SIZE + 4);
      $this->FirstDay = $this->LoadInt8($fileData);
   }

   // Converts the todo record data loaded from a file into the internal
   // storage method that is useed for the rest of the class and for ease of
   // use.
   // Return false to signal no error.
   function LoadRecord($fileData, $RecordInfo) {
      $this->RecordAttrs[$RecordInfo['UID']] = $RecordInfo['Attrs'];

      $NewRec = $this->NewRecord();

      //Load date
      $date = $this->LoadInt16(substr($fileData,0,2));
      if ($date != 0xffff) {
         $day = $date & 0x001f;
	 $month = ($date >> 5) & 0x000f;
	 $year = ($date >> 9) & 0x007f;
	 $year += 1904;
	 $NewRec['due_date'] = "$year-$month-$day";
      }

      $priority = $this->LoadInt8(substr($fileData,2,1));
      $completed = $priority & 0x80;
      $priority &= 0x7f;
      $NewRec['completed'] = $completed ? 1 : 0;
      $NewRec['priority'] = $priority;

      $i = 3;
      $description = '';
      while ($fileData[$i] != "\0") {
         $description .= $fileData[$i];
         $i++;
      }
      $i++;
      $note = '';
      while ($fileData[$i] != "\0") {
         $note .= $fileData[$i];
         $i++;
      }
      $NewRec['description'] = $description;
      $NewRec['note'] = $note;
      $NewRec['attributes'] = $RecordInfo['Attrs'] & PDB_TODO_ATTRIB_HIGH;
     
      if ($RecordInfo['Attrs'] & PDB_ADDR_ATTRIB_DELE_EXP) {
         $NewRec['attributes'] |=
	    $RecordInfo['Attrs'] & PDB_ADDR_ATTRIB_ARCHIVE;
      } else {
         $NewRec['category'] = $RecordInfo['Attrs'] & PDB_CATEGORY_MASK;
      }
      $this->Records[$RecordInfo['UID']] = $NewRec;
   }
}
