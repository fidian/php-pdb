<?php
/* Class extender for Address Book databases
 * Class donated by Eduardo Pascual Martinez
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */

define('PDB_ADDR_LABEL_LENGTH', 16);
define('PDB_ADDR_NUM_LABELS', 22);

// Countries
define('PDB_ADDR_COUNTRY_AUSTRALIA', 1);
define('PDB_ADDR_COUNTRY_AUSTRIA', 2);
define('PDB_ADDR_COUNTRY_BELGIUM', 3);
define('PDB_ADDR_COUNTRY_BRAZIL', 4);
define('PDB_ADDR_COUNTRY_CANADA', 5);
define('PDB_ADDR_COUNTRY_DENMARK', 6);
define('PDB_ADDR_COUNTRY_FINLAND', 7);
define('PDB_ADDR_COUNTRY_FRANCE', 8);
define('PDB_ADDR_COUNTRY_GERMANY', 9);
define('PDB_ADDR_COUNTRY_HONG_KONG', 10);
define('PDB_ADDR_COUNTRY_ICELAND', 11);
define('PDB_ADDR_COUNTRY_IRELAND', 12);
define('PDB_ADDR_COUNTRY_ITALY', 13);
define('PDB_ADDR_COUNTRY_JAPAN', 14);
define('PDB_ADDR_COUNTRY_LUXEMBOURG', 15);
define('PDB_ADDR_COUNTRY_MEXICO', 16);
define('PDB_ADDR_COUNTRY_NETHERLANDS', 17);
define('PDB_ADDR_COUNTRY_NEW_ZEALAND', 18);
define('PDB_ADDR_COUNTRY_NORWAY', 19);
define('PDB_ADDR_COUNTRY_SPAIN', 20);
define('PDB_ADDR_COUNTRY_SWEDEN', 21);
define('PDB_ADDR_COUNTRY_SWITZERLAND', 22);
define('PDB_ADDR_COUNTRY_UNITED_KINDOM', 23);
define('PDB_ADDR_COUNTRY_UNITED_STATES', 24);

// Change this to match your locale
define('PDB_ADDR_COUNTRY_DEFAULT', PDB_ADDR_COUNTRY_UNITED_STATES);

// Phone labels index
define('PDB_ADDR_LABEL_WORK', 0);
define('PDB_ADDR_LABEL_HOME', 1);
define('PDB_ADDR_LABEL_FAX', 2);
define('PDB_ADDR_LABEL_OTHER', 3);
define('PDB_ADDR_LABEL_EMAIL', 4);
define('PDB_ADDR_LABEL_MAIN', 5);
define('PDB_ADDR_LABEL_PAGER', 6);
define('PDB_ADDR_LABEL_MOBILE', 7);

// Attributes for the record
define('PDB_ADDR_ATTRIB_EXPUNGED', 0x80);
define('PDB_ADDR_ATTRIB_DELETED', 0x40);
define('PDB_ADDR_ATTRIB_ARCHIVE', 0x08);  // Only set if expunged or deleted
define('PDB_ADDR_ATTRIB_DIRTY', 0x20);
define('PDB_ADDR_ATTRIB_PRIVATE', 0x010);

// Easier to use one of these if you need it
define('PDB_ADDR_ATTRIB_DELE_EXP', 0xC0);  // Deleted and Expunged bits
define('PDB_ADDR_ATTRIB_HIGH', 0xF0);  // All of above except Archive bit

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
 * Key              Example     Description
 * -------------------------------------------------
 * fields           ???         Array with the fields of the record.
 * phoneLabel       ???         ^ Array with 5 references to phone labels
 * category         1           ^ 0-15 (default 0)
 * attributes       ???         ^ Bitmask of attributes (default 0)
 *                                See above for the flags
 *
 * This is the basic structure of the fields array.  It could have 
 * certain customization settings for the labels that will be used for 
 * each field.  See the SetFieldsLabel() method for more info.
 *
 * Key           Example         Description
 * ------------------------------------------------
 * lastName      Duck            The contact's last name
 * firstName     Daffy           The contact's first name
 * company       PHP-PDB Inc.    Name of the company
 * phone1        556-6778        A telephone number, email, or any string
 * phone2        867-5309        A telephone number, email, or any string
 * phone3        dduck@toon.com  A telephone number, email, or any string
 * phone4        55667788        A telephone number, email, or any string
 * phone5        55667788        A telephone number, email, or any string
 * address       Duck street 25  A string with the address
 * city          Toon City       A string with the city 
 * state         Toon State      A string with the state
 * zipCode       78550           A string with the zip code
 * country       Toon Land       A string with the country
 * title         Sir             A string with the title
 * custom1       Birth date      Any string with extra info
 * custom2       Nick name       Any string with extra info
 * custom3       Boss name       Any string with extra info
 * custom4       Whatever        Any string with extra info
 * note          He is green     Notes for the contact
 *
 * The phoneLabels array in the class provides up to 8 descriptions for the 
 * 5 phone labels in each record.  The phoneLabel array in the record ties
 * together the proper description for each phone* entry.  The 8
 * descriptions can be changed (so your address book might say "Cellular" 
 * instead of "Mobile").  See the SetPhoneLabels() method for more 
 * information.
 *
 * Key            Example               Description
 * ------------------------------------------------
 * phone1         PDB_ADDR_LABEL_WORK   Use the label referenced by 
 *                                      PDB_ADDR_LABEL_WORK
 * phone2         PDB_ADDR_LABEL_HOME   Use the label referenced by 
 *                                      PDB_ADDR_LABEL_HOME
 * phone3         PDB_ADDR_LABEL_FAX    Use the label referenced by 
 *                                      PDB_ADDR_LABEL_FAX
 * phone4         PDB_ADDR_LABEL_OTHER  Use the label referenced by 
 *                                      PDB_ADDR_LABEL_OTHER
 * phone5         PDB_ADDR_LABEL_EMAIL  Use the label referenced by 
 *                                      PDB_ADDR_LABEL_EMAIL
 * display        2                     [0-4] index of this array. 
 *                                      0 is phone1, 1 is phone2, etc. 
 *                                      Which of the 5 phone fields is 
 *                                      displayed in the list of contacts
 * reserved       ''                    ???
 *
 * phone* could be one of this values:
 *  PDB_ADDR_LABEL_WORK
 *  PDB_ADDR_LABEL_HOME
 *  PDB_ADDR_LABEL_FAX
 *  PDB_ADDR_LABEL_OTHER
 *  PDB_ADDR_LABEL_EMAIL
 *  PDB_ADDR_LABEL_MAIN
 *  PDB_ADDR_LABEL_PAGER
 *  PDB_ADDR_LABEL_MOBILE
 */
class PalmAddress extends PalmDB {
   // Default field labels.
   // Can be changed for I18N -- see SetFieldLabels() and GetFieldLabels()
   var $Labels = array('lastName'  => 'Last name',
 		       'firstName' => 'First name',
		       'company'   => 'Company',
		       'phone1'    => 'Work',
		       'phone2'    => 'Home',
		       'phone3'    => 'Fax',
		       'phone4'    => 'Other',
		       'phone5'    => 'E-mail',
		       'phone6'    => 'Main',
		       'phone7'    => 'Pager',
		       'phone8'    => 'Mobile',
		       'address'   => 'Address',
		       'city'      => 'City',
		       'state'     => 'State',
		       'zipCode'   => 'Zip Code',
		       'country'   => 'Country',
		       'title'     => 'Title',
		       'custom1'   => 'Custom 1',
		       'custom2'   => 'Custom 2',
		       'custom3'   => 'Custom 3',
		       'custom4'   => 'Custom 4',
		       'note'      => 'Note');
		      
   // Default phone labels -- The text description for the type of
   // information you entered for a phone* field.
   // If you wish to change "Mobile" to "Cellular", you can do this:
   //   $Labels = $pdb->GetPhoneLabels();
   //   $Labels[PDB_ADDR_LABEL_MOBILE] = 'Cellular';
   //   $pdb->SetPhoneLabels($Labels)
   var $phoneLabels = array(PDB_ADDR_LABEL_WORK   => 'Work',
			    PDB_ADDR_LABEL_HOME   => 'Home',
			    PDB_ADDR_LABEL_FAX    => 'Fax',
			    PDB_ADDR_LABEL_OTHER  => 'Other',
			    PDB_ADDR_LABEL_EMAIL  => 'E-mail',
			    PDB_ADDR_LABEL_MAIN   => 'Main',
			    PDB_ADDR_LABEL_PAGER  => 'Pager',
			    PDB_ADDR_LABEL_MOBILE => 'Mobile');

   var $country = PDB_ADDR_COUNTRY_DEFAULT;   // Default country.
   var $misc = 0;                             // ???


   // Constructor.
   // $country is the Country for which the labels where designed.
   function PalmAddress($country = PDB_ADDR_COUNTRY_DEFAULT) {
      PalmDB::PalmDB('DATA','addr','AddressDB');

      $this->country = $country;
   }


   // Returns a new array with default data for a new record.
   // This doesn't actually add the record.
   function NewRecord() {
      // Initialize the fields. Empty by default.
      $Record['fields'] = array('lastName'  => '',
			        'firstName' => '',
			        'company'   => '',
			        'phone1'    => '',
			        'phone2'    => '',
			        'phone3'    => '',
			        'phone4'    => '',
			        'phone5'    => '',
			        'addres'    => '',
			        'city'      => '',
			        'state'     => '',
			        'zipCode'   => '',
			        'country'   => '',
			        'title'     => '',
			        'custom1'   => '',
			        'custom2'   => '',
			        'custom3'   => '',
			        'custom4'   => '',
			        'note'      => '');
     
      // Initialize the phone labels;
      $Record['phoneLabel'] = array('phone1' => PDB_ADDR_LABEL_WORK,
				    'phone2' => PDB_ADDR_LABEL_HOME,
				    'phone3' => PDB_ADDR_LABEL_FAX,
				    'phone4' => PDB_ADDR_LABEL_OTHER,
				    'phone5' => PDB_ADDR_LABEL_EMAIL,
				    'display' => PDB_ADDR_LABEL_WORK,
				    'reserved' => '');

      // Initialize the category.
      $Record['category'] = 0;
      
      return $Record;
   }


   // Overrides the GetRecordSize method.
   // Probably should only be used when saving
   function GetRecordSize($num = false) {
      if ($num === false)
         $num = $this->CurrentRecord;
     
      if (! isset($this->Records[$num]) || ! is_array($this->Records[$num]))
         return PalmDB::GetRecordSize($num);

      $data = $this->Records[$num];

      // Phone Flags (4) + Field Map (4) + Company field offset (1)
      $Bytes = 9;

      $keys = array_keys($data['fields']);
      foreach($keys as $k) {
         if ($data['fields'][$k] != '') {
	    $Bytes += strlen($data['fields'][$k]);
	    $Bytes += 1; // NULL termination
         }
      }
      return $Bytes;
   }


   // Overrides the GetRecord method. We store data in associative arrays.
   // Just convert the data into the proper format and then return the
   // generated string.
   // The record is packed in 4 steps.
   //   1.  4 bytes for the phoneFlags which determines the phone mapping.
   //   2.  4 bytes that specifies which fields the record contains.
   //   3.  1 byte for the company field offset -- might be used for the
   //       address book aplication to quickly display by company name.
   //   4.  Series of NULL-terminated strings with the values of the record 
   //       fields.
   // If a record doesn't have a given field, there is no string 
   // corresponding to it. 
   function GetRecord($num = false) {
      if ($num === false) {
         $num = $this->CurrentRecord;
      }

      if (! isset($this->Records[$num]) || ! is_array($this->Records[$num])) {
         return PalmDB::GetRecord($num);
      }

      $data = $this->Records[$num];
      $RecordString = '';

      $phoneFlags = ($data['phoneLabel']['phone1']    & 0x0f) |
 	            (($data['phoneLabel']['phone2']   & 0x0f) <<  4) |
                    (($data['phoneLabel']['phone3']   & 0x0f) <<  8) |
   	            (($data['phoneLabel']['phone4']   & 0x0f) << 12) |
	            (($data['phoneLabel']['phone5']   & 0x0f) << 16) |
	            (($data['phoneLabel']['display']  & 0x0f) << 20) |
	            (($data['phoneLabel']['reserved'] & 0x0f) << 24);
      $RecordString .= $this->Int32($phoneFlags);

      $fieldMap = 0;
      if ($data['fields']['lastName'] != '')  { $fieldMap |= 0x0001; }
      if ($data['fields']['firstName'] != '') { $fieldMap |= 0x0002; }
      if ($data['fields']['company'] != '')   { $fieldMap |= 0x0004; }
      if ($data['fields']['phone1'] != '')    { $fieldMap |= 0x0008; }
      if ($data['fields']['phone2'] != '')    { $fieldMap |= 0x0010; }
      if ($data['fields']['phone3'] != '')    { $fieldMap |= 0x0020; }
      if ($data['fields']['phone4'] != '')    { $fieldMap |= 0x0040; }
      if ($data['fields']['phone5'] != '')    { $fieldMap |= 0x0080; }
      if ($data['fields']['address'] != '')   { $fieldMap |= 0x0100; }
      if ($data['fields']['city'] != '')      { $fieldMap |= 0x0200; }
      if ($data['fields']['state'] != '')     { $fieldMap |= 0x0400; }
      if ($data['fields']['zipCode'] != '')   { $fieldMap |= 0x0800; }
      if ($data['fields']['country'] != '')   { $fieldMap |= 0x1000; }
      if ($data['fields']['title'] != '')     { $fieldMap |= 0x2000; }
      if ($data['fields']['custom1'] != '')   { $fieldMap |= 0x4000; }
      if ($data['fields']['custom2'] != '')   { $fieldMap |= 0x8000; }
      if ($data['fields']['custom3'] != '')   { $fieldMap |= 0x10000; }
      if ($data['fields']['custom4'] != '')   { $fieldMap |= 0x20000; }
      if ($data['fields']['note'] != '')      { $fieldMap |= 0x40000; }
      
      $RecordString .= $this->Int32($fieldMap);

      $fields = '';
      $companyFieldOff = 0;

      if ($data['fields']['lastName'] != '') {
         $fields .= $this->String($data['fields']['lastName']);
         $fields .= $this->Int8(0);
      }
      if ($data['fields']['firstName'] != '') {
         $fields .= $this->String($data['fields']['firstName']);
         $fields .= $this->Int8(0);
      }
      if ($data['fields']['company'] != '') {
         $companyFieldOff = sizeof($data['fields']['company']) + 1;
         $fields .= $this->String($data['fields']['company']);
         $fields .= $this->Int8(0);
      }
      if ($data['fields']['phone1'] != '') {
         $fields .= $this->String($data['fields']['phone1']);
         $fields .= $this->Int8(0);
      }
      if ($data['fields']['phone2'] != '') {
         $fields .= $this->String($data['fields']['phone2']);
         $fields .= $this->Int8(0);
      }
      if ($data['fields']['phone3'] != '') {
         $fields .= $this->String($data['fields']['phone3']);
         $fields .= $this->Int8(0);
      }
      if ($data['fields']['phone4'] != '') {
         $fields .= $this->String($data['fields']['phone4']);
         $fields .= $this->Int8(0);
      }
      if ($data['fields']['phone5'] != '') {
         $fields .= $this->String($data['fields']['phone5']);
         $fields .= $this->Int8(0);
      }
      if ($data['fields']['address'] != '') {
         $fields .= $this->String($data['fields']['address']);
         $fields .= $this->Int8(0);
      }
      if ($data['fields']['city'] != '') {
         $fields .= $this->String($data['fields']['city']);
         $fields .= $this->Int8(0);
      }
      if ($data['fields']['state'] != '') {
         $fields .= $this->String($data['fields']['state']);
         $fields .= $this->Int8(0);
      }
      if ($data['fields']['zipCode'] != '') {
         $fields .= $this->String($data['fields']['zipCode']);
         $fields .= $this->Int8(0);
      }
      if ($data['fields']['country'] != '') {
         $fields .= $this->String($data['fields']['country']);
         $fields .= $this->Int8(0);
      }
      if ($data['fields']['title'] != '') {
         $fields .= $this->String($data['fields']['title']);
         $fields .= $this->Int8(0);
      }
      if ($data['fields']['custom1'] != '') {
         $fields .= $this->String($data['fields']['custom1']);
         $fields .= $this->Int8(0);
      }
      if ($data['fields']['custom2'] != '') {
         $fields .= $this->String($data['fields']['custom2']);
         $fields .= $this->Int8(0);
      }
      if ($data['fields']['custom3'] != '') {
         $fields .= $this->String($data['fields']['custom3']);
         $fields .= $this->Int8(0);
      }
      if ($data['fields']['custom4'] != '') {
         $fields .= $this->String($data['fields']['custom4']);
         $fields .= $this->Int8(0);
      }
      if ($data['fields']['note'] != '') {
         $fields .= $this->String($data['fields']['note']);
         $fields .= $this->Int8(0);
      }
     
      $RecordString .= $this->Int8($companyFieldOff);
      $RecordString .= $fields;
     
      return $RecordString;
   }


   // Sets the data for the current record
   // SetRecordRaw($RecordArray)  -- Sets the current record
   // SetRecordRaw($RecNo, $RecordArray)  -- Sets the specified record
   function SetRecordRaw($A, $B = false) {
      if ($B === false) {
         $B = $A;
         $A = $this->CurrentRecord;
      }
      if (!isset($B['priority'])) {
         $B['priority'] = 1;
      }
      if (!isset($B['category'])) {
         $B['category'] = 0;
      }
      if (!isset($B['phoneLabel'])) {
         $B['phoneLabel'] = array('phone1' => PDB_ADDR_LABEL_WORK,
				  'phone2' => PDB_ADDR_LABEL_HOME,
				  'phone3' => PDB_ADDR_LABEL_FAX,
				  'phone4' => PDB_ADDR_LABEL_OTHER,
				  'phone5' => PDB_ADDR_LABEL_EMAIL,
				  'display' => PDB_ADDR_LABEL_WORK,
				  'reserved' => '');
      }
      $this->Records[$A] = $B;
     
      if (isset($B['attributes'])) {
	 // Set the lower 4 bits accordingly
         if ($B['attributes'] & PDB_ADDR_ATTRIB_DELE_EXP) {
	    if ($B['attributes'] & PDB_ADDR_ATTRIB_ARCHIVE) {
	       $atts = PDB_ADDR_ATTRIB_ARCHIVE;
	    }
         } else {
	    $atts = $B['category'] & PDB_CATEGORY_MASK;
         }

         // Bits 0x10 through 0x80 are the expunged, dirty, deleted and
	 // private bits.
	 $atts |= $B['attributes'] & PDB_ADDR_ATTRIB_HIGH;
       
         $this->RecordAttrs[$A] = $atts;
      } else {
         $this->RecordAttrs[$A] = $B['category'] & PDB_CATEGORY_MASK;
      }
   }


   // Returns the size of the AppInfo block.
   function GetAppInfoSize() {
      // Standard category size
      $AppInfoSize = PDB_CATEGORY_SIZE + 6;

      // Field labels
      $AppInfoSize += PDB_ADDR_LABEL_LENGTH * PDB_ADDR_NUM_LABELS;

      // Country code (1)
      // misc (1)
      // dirty fields (2)
      $AppInfoSize += 4;

      return $AppInfoSize;
   }


   // Returns the AppInfo block. It is composed of the category list plus 
   // 6 extra bytes, the field labels, the country code, the misc byte 
   // (used for display ordered by company).
   function GetAppInfo() {
      // Category List
      $this->AppInfo = $this->CreateCategoryData($this->CategoryList);
      $this->AppInfo .= $this->Int32(0);
      $this->AppInfo .= $this->Int8(count($this->CategoryList) - 1);
      $this->AppInfo .= $this->Int8(0);

      // Field labels
      $field = $this->String($this->Labels['lastName'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['firstName'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['company'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['phone1'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['phone2'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['phone3'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['phone4'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['phone5'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['address'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['city'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['state'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['zipCode'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['country'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['title'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['custom1'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['custom2'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['custom3'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['custom4'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['note'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['phone6'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['phone7'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);
      $field = $this->String($this->Labels['phone8'],PDB_ADDR_LABEL_LENGTH -1);
      $this->AppInfo .= $this->PadString($field,PDB_ADDR_LABEL_LENGTH);

      // Country code
      $this->AppInfo .= $this->Int8($this->country);

      // Misc
      $this->AppInfo .= $this->Int8($this->misc);

      $this->AppInfo .= $this->Int16(0);

      return $this->AppInfo;
   }


   // Sets the category list.
   // $list is an array with the list of categories.  Skips the first 
   // element which is reserved for the 'Unfiled' category (index 0), and 
   // must be a maximum of 16 elements including the Unfiled.
   function SetCategoryList($list) {
      if (!is_array($list)) {
         return false;
      }
      
      // Validates if the first element is empty.
      $i = ($list[0] != '' ? 1 : 0);

      $keys = array_keys($list);
      sort($keys);
      foreach ($keys as $id) {
         $this->CategoryList[$id+$i]['Name'] = $list[$id];
         $this->CategoryList[$id+$i]['Renamed'] = false;
      }
      
      return true;
   }


   // Returns the category list array.
   function GetCategoryList() {
      return $this->CategoryList;
   }


   /* Set the phone labels.
    * $P is an array with 8 elements, each element is mapped with a 
    * constant to the record's phoneLabel array.
    *
    * Key                     Example       Description
    * --------------------------------------------------
    * PDB_ADDR_LABEL_WORK     'Company'     A string label.  Index 0.
    * PDB_ADDR_LABEL_HOME     'HomePhone'   A string label.  Index 1.
    * PDB_ADDR_LABEL_FAX      'FaxNumber'   A string label.  Index 2.
    * PDB_ADDR_LABEL_OTHER    'Celular'     A string label.  Index 3.
    * PDB_ADDR_LABEL_EMAIL    'E-mail'      A string label.  Index 4.
    * PDB_ADDR_LABEL_MAIN     'E-mail 2'    A string label.  Index 5.
    * PDB_ADDR_LABEL_PAGER    'Company 2'   A string label.  Index 6.
    * PDB_ADDR_LABEL_MOBILE   'BossPhone'   A string label.  Index 7.
    */
   function SetPhoneLabels($P) {
      if (is_array($P)) {
         $this->phoneLabels[PDB_ADDR_LABEL_WORK]   = $P[PDB_ADDR_LABEL_WORK];
         $this->phoneLabels[PDB_ADDR_LABEL_HOME]   = $P[PDB_ADDR_LABEL_HOME];
         $this->phoneLabels[PDB_ADDR_LABEL_FAX]    = $P[PDB_ADDR_LABEL_FAX];
         $this->phoneLabels[PDB_ADDR_LABEL_OTHER]  = $P[PDB_ADDR_LABEL_OTHER];
         $this->phoneLabels[PDB_ADDR_LABEL_EMAIL]  = $P[PDB_ADDR_LABEL_EMAIL];
         $this->phoneLabels[PDB_ADDR_LABEL_MAIN]   = $P[PDB_ADDR_LABEL_MAIN];
         $this->phoneLabels[PDB_ADDR_LABEL_PAGER]  = $P[PDB_ADDR_LABEL_PAGER];
         $this->phoneLabels[PDB_ADDR_LABEL_MOBILE] = $P[PDB_ADDR_LABEL_MOBILE];
      }
   }


   // Returns an array with the phone labels
   function GetPhoneLabels() {
      return $this->phoneLabels;
   }


   /* Set the field labels. It could be used for I18N.
    * $L is an array with some or all the field labels used by the address book 
    * application. The class has default field labels, only if the $L array has a value
    * for a specific field label it is overwritten.
    * The key names are the internal names of the fields, but the values are the names
    * that the application will display.
    * In example, the 'title' key could have as value 'Nick name'.
    */
   function SetFieldLabels($L) {
      if (is_array($L)) {
         foreach ($this->Labels as $Key => $Val) {
	    $this->Labels[$Key] = isset($L[$Key]) ? $L[$Key] : $Val;
	 }
      }
   }


   // Returns the field labels
   function GetFieldLabels() {
      return $this->Labels;
   }


   // Parse $fileData for the information we need when loading an AddressBook 
   // file.
   function LoadAppInfo($fileData) {
      $this->AppInfo = bin2hex($fileData);
      $this->CategoryList = $this->LoadCategoryData($fileData);
      $fileData = substr($fileData, PDB_CATEGORY_SIZE + 6);
      $this->Labels['lastName'] = substr($fileData, 0, PDB_ADDR_LABEL_LENGTH);
      $index = PDB_ADDR_LABEL_LENGTH;
      $this->Labels['firstName'] = 
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;
      $this->Labels['company'] = 
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;
      $this->Labels['phone1'] =
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;
      $this->Labels['phone2'] = 
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;
      $this->Labels['phone3'] = 
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;
      $this->Labels['phone4'] = 
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;
      $this->Labels['phone5'] =
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;
      $this->Labels['address'] =
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;
      $this->Labels['city'] =
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;
      $this->Labels['state'] =
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;
      $this->Labels['zipCode'] =
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;
      $this->Labels['country'] =
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;
      $this->Labels['title'] =
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;
      $this->Labels['custom1'] =
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;
      $this->Labels['custom2'] =
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;
      $this->Labels['custom3'] =
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;
      $this->Labels['custom4'] =
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;
      $this->Labels['note'] =
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;
      $this->Labels['phone6'] =
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;
      $this->Labels['phone7'] =
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;
      $this->Labels['phone8'] =
         substr($fileData, $index, PDB_ADDR_LABEL_LENGTH);
      $index += PDB_ADDR_LABEL_LENGTH;

     $this->country = $this->LoadInt8($fileData);
     $this->misc = $this->LoadInt8(substr($fileData, 1));
   }


   // Converts the address record data loaded from a file into the internal
   // storage method that is useed for the rest of the class and for ease of
   // use.
   // Return false to signal no error.
   function LoadRecord($fileData, $RecordInfo) {
      $this->RecordAttrs[$RecordInfo['UID']] = $RecordInfo['Attrs'];

      $NewRec = $this->NewRecord();

      // Load phone flags
      $phoneFlags = $this->LoadInt32($fileData);
      $phoneLabels = array('phone1'   => $phoneFlags         & 0x0f,
			   'phone2'   => ($phoneFlags >>  4) & 0x0f,
			   'phone3'   => ($phoneFlags >>  8) & 0x0f,
			   'phone4'   => ($phoneFlags >> 12) & 0x0f,
			   'phone5'   => ($phoneFlags >> 16) & 0x0f,
			   'display'  => ($phoneFlags >> 20) & 0x0f,
			   'reserved' => ($phoneFlags >> 24) & 0x0f);
      $NewRec['phoneLabel'] = $phoneLabels;

      // Load the fieldMap
      $fieldMap = $this->LoadInt32(substr($fileData,4));
      
      // Load the company field offset
      $companyFieldOff = $this->LoadInt8(substr($fileData,8));

      // Load the fields.
      $filters = array('lastName'  => 0x0001,
		       'firstName' => 0x0002,
		       'company'   => 0x0004,
		       'phone1'    => 0x0008,
		       'phone2'    => 0x0010,
		       'phone3'    => 0x0020,
		       'phone4'    => 0x0040,
		       'phone5'    => 0x0080,
		       'address'   => 0x0100,
		       'city'      => 0x0200,
		       'state'     => 0x0400,
		       'zipCode'   => 0x0800,
		       'country'   => 0x1000,
		       'title'     => 0x2000,
		       'custom1'   => 0x4000,
		       'custom2'   => 0x8000,
		       'custom3'   => 0x10000,
		       'custom4'   => 0x20000,
		       'note'      => 0x40000);
		       
      $i=9;
     
      foreach ($filters as $key => $mask) {
         if ($fieldMap & $mask) {
	    while ($fileData[$i] != "\0") {
	       $fields[$key] .= $fileData[$i];
	       $i++;
	    }
	    $i++;
         } else {
	    $fields[$key] = '';
         }
      }
      $NewRec['fields'] = $fields;
      $NewRec['attributes'] = $RecordInfo['Attrs'] & PDB_ADDR_ATTRIB_HIGH;

      if ($RecordInfo['Attrs'] & PDB_ADDR_ATTRIB_DELE_EXP) {
         $NewRec['attributes'] |=
	    $RecordInfo['Attrs'] & PDB_ADDR_ATTRIB_ARCHIVE;
      } else {
         $NewRec['category'] = $RecordInfo['Attrs'] & PDB_CATEGORY_MASK;
      }

      $this->Records[$RecordInfo['UID']] = $NewRec;
   }
}
?>