<?php

define('PDB_ADDR_LABEL_LENGTH', 16);
define('PDB_NUM_FIELD_LABELS', 22);

// Countries.
define('PDB_AUSTRALIA', 1);
define('PDB_AUSTRIA', 2);
define('PDB_BELGIUM', 3);
define('PDB_BRAZIL', 4);
define('PDB_CANADA', 5);
define('PDB_DENMARK', 6);
define('PDB_FINLAND', 7);
define('PDB_FRANCE', 8);
define('PDB_GERMANY', 9);
define('PDB_HONG_KONG', 10);
define('PDB_ICELAND', 11);
define('PDB_IRELAND', 12);
define('PDB_ITALY', 13);
define('PDB_JAPAN', 14);
define('PDB_LUXEMBOURG', 15);
define('PDB_MEXICO', 16);
define('PDB_NETHERLANDS', 17);
define('PDB_NEW_ZEALAND', 18);
define('PDB_NORWAY', 19);
define('PDB_SPAIN', 20);
define('PDB_SWEDEN', 21);
define('PDB_SWITZERLAND', 22);
define('PDB_UNITED_KINDOM', 23);
define('PDB_UNITED_STATES', 24);

// Phone labelse index.
define('PDB_WORK', 0);
define('PDB_HOME', 1);
define('PDB_FAX', 2);
define('PDB_OTHER', 3);
define('PDB_EMAIL', 4);
define('PDB_MAIN', 5);
define('PDB_PAGER', 6);
define('PDB_MOBILE', 7);

/* The data for SetRecordRaw and from GetRecordRaw should be/return a
 * special array, detailed below.  Optional values can be set to '' or not
 * defined.  If they are anything else (including zero), they are considered
 * to be 'set'.  Optional values are marked with a ^.
 * Key              Example              Description
 * -------------------------------------------------
 * fields           ???                  Array with the fields of the record.
 * phoneLabel       ???                  ^ Array with 5 references to phone labels
 * category         1                    ^ 0-15 (default 0)
 * attributes       ???                  ^ Array of attributes
 *
 * The attributes array coud be like this.
 * 
 * Key            Example              Description
 * -----------------------------------------------
 * expunged       1                    ^ 1/0/'' (default '' not set)
 * deleted        1                    ^ 1/0/'' (default '' not set)
 * arvhive        1                    ^ 1/0/'' (default '' not set)
 * dirty          1                    ^ 1/0/'' (default '' not set)
 * private        1                    ^ 1/0/'' (default '' not set)
 * 
 * This is the basic structur of the fields array. It could have certain customitation
 * seting the labels that will be used for each field. See the SetFieldsLabel()
 * method for more info.
 *
 * Key           Example                Description
 * ------------------------------------------------
 * name          Duck                   The contact's last name
 * firstName     Daffy                  The contact's first name
 * company       PHP-PDB Inc.           Name of the company
 * phone1        55667788               A thelephone number, email, or any string
 * phone2        55667788               A thelephone number, email, or any string
 * phone3        55667788               A thelephone number, email, or any string
 * phone4        55667788               A thelephone number, email, or any string
 * phone5        55667788               A thelephone number, email, or any string
 * address       Duck stret 25          A string with the address
 * city          Toon City              A string with the city 
 * state         Toon State             A string with the state
 * zipCode       78550                  A string with the zip code
 * country       Toon Land              A string with the country
 * title         Sir                    A string with the title
 * custom1       Birth date             Any string with extra info
 * custom2       Nick name              Any string with extra info
 * custom3       Boss name              Any string with extra info
 * custom4       Wathever               Any string with extra info
 * note          He is green            Notes for the contact
 *
 * The phoneLabel array is for relationate the 1-8 phone labels with the 1-5 phone
 * fields. The labels could have customization, see the SetPhoneLabels() function
 * for more info.
 *
 * Key            Example               Description
 * ------------------------------------------------
 * phone1         PDB_WORK              Use the label referenced by PDB_WORK
 * phone2         PDB_HOME              Use the label referenced by PDB_HOME
 * phone3         PDB_FAX               Use the label referenced by PDB_FAX
 * phone4         PDB_OTHER             Use the label referenced by PDB_OTHER
 * phone5         PDB_EMAIL             Use the label referenced by PDB_EMAIL
 * display        2                     [0-4] index of this array. 0 is phone1,
 *                                      1 is phone2, etc. Which of the 5 phone
 *                                      fields must be displayed in the list of
 *                                      contacts
 * reserved       ''                    ???
 *
 * phone* could be one of this values:
 *  PDB_WORK
 *  PDB_HOME
 *  PDB_FAX
 *  PDB_OTHER
 *  PDB_EMAIL
 *  PDB_MAIN
 *  PDB_PAGER
 *  PDB_MOBILE
 */
class PalmAddress extends PalmDB {
  // Default field labels.
  var $Labels = array('name'      => 'Name',
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
  // Default phone labels
  var $phoneLabels = array(PDB_WORK   => 'Work',
			   PDB_HOME   => 'Home',
			   PDB_FAX    => 'Fax',
			   PDB_OTHER  => 'Other',
			   PDB_EMAIL  => 'E-mail',
			   PDB_MAIN   => 'Main',
			   PDB_PAGER  => 'Pager',
			   PDB_MOBILE => 'Mobile');
  var $country = PDB_MEXICO;         // Default country.
  var $misc = 0;                     // ???

  // Constructor.
  // $country is the Country for which the labels where designed.
  function PalmAddress($country = PDB_MEXICO) {
    PalmDB::PalmDB('DATA','addr','AddressDB');

    $this->country = $country;
  }

  // Returns a new array with default data for a new record.
  // This doesn't actually add the record.
  function NewRecord() {
    // Initialize the fields. Empty by default.
    $Record['fields'] = array('name'      => '',
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
    $Record['phoneLabel'] = array('phone1' => PDB_WORK,
				  'phone2' => PDB_HOME,
				  'phone3' => PDB_FAX,
				  'phone4' => PDB_OTHER,
				  'phone5' => PDB_EMAIL,
				  'display' => PDB_WORK,
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

     // Phone Flags
     $Bytes = 4;

     // Field Map
     $Bytes += 4;

     // Company Field Offset.
     $Bytes += 1;

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
   // 1. 4 bytes for the phoneFlags which determines the phone mapping.
   // 2 .4 bytes that specifies which fields the record contains. 
   // 3. 1 byte for the company field offset maybe used for the  address book
   // aplication to quickly display by company name.
   // 4. Series of NULL-terminated string with the values of the record fields.
   // if a record doesn't have a given field, ther is no string corresponding to it. 
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
     if ($data['fields']['name'] != '')      { $fieldMap |= 0x0001; }
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

     if ($data['fields']['name'] != '') {
       $fields .= $this->String($data['fields']['name']);
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
   function SetRecordRaw($A, $B = false) {
     if ($B === false) {
       $B = $A;
       $A = $this->CurrentRecord;
     }
     if (!isset($B['priority'])) {
       $B['priority'] = 1;
     }
     if (!isset($B['phoneLabel'])) {
       $B['phoneLabel'] = array('phone1' => PDB_WORK,
				'phone2' => PDB_HOME,
				'phone3' => PDB_FAX,
				'phone4' => PDB_OTHER,
				'phone5' => PDB_EMAIL,
				'display' => PDB_WORK,
				'reserved' => '');
     }
     $this->Records[$A] = $B;
     
     if (isset($B['attributes'])) {
       $atts = 0;
       if ($B['attributes']['expunged'] || $B['attributes']['deleted']) {
	 if ($B['attributes']['archive']) {
	   $atts |= 0x08;
	 }
       } else {
	 $atts = $B['category'] & 0x0f;
       }
       if ($B['attributes']['expunged']) {
	 $atts |= 0x80;
       }
       if ($B['attributes']['dirty']) {
	 $atts |= 0x40;
       }
       if ($B['attributes']['deleted']) {
	 $atts |= 0x20;
       }
       if ($B['attributes']['private']) {
	 $atts |= 0x10;
       }
       
       $this->RecordAttrs[$A] = $atts;
     } else {
       $this->RecordAttrs[$A] = $B['category'] & 0x0f;
     }
   }

   // Returns the size of the AppInfo block.  It is the size of the
   // category list plus four bytes.
   function GetAppInfoSize() {
     // Standar category size
     $AppInfoSize = PDB_CATEGORY_SIZE + 6;

     // Field labels
     $AppInfoSize += PDB_ADDR_LABEL_LENGTH * PDB_NUM_FIELD_LABELS;

     // Country code (1)
     // misc (1)
     // dirty fields (2)
     $AppInfoSize += 4;

     return $AppInfoSize;
   }

   // Returns the AppInfo block. It is composed of the category list plus 6 extra
   // byetes, the field labels, the country code, the misc byte (used for display
   // ordered by company.
   function GetAppInfo() {
     // Category List.
     $this->AppInfo = $this->CreateCategoryData($this->CategoryList);
     $this->AppInfo .= $this->Int32(0);
     $this->AppInfo .= $this->Int8(count($this->CategoryList) - 1);
     $this->AppInfo .= $this->Int8(0);

     // Field labels
     $field = $this->String($this->Labels['name'],PDB_ADDR_LABEL_LENGTH -1);
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
   // $list is an array with the list of categoris. Skips the first element which is
   // reserved for the 'Unfiled' category (index 0), and must be a maximum of 16
   // elements including the Unfiled.
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

   // Returns the category list array.
   function GetCategoryList() {
     return $this->CategoryList;
   }

   /* Set the phone labels.
    * $P is an array with 8 elements, each element is maped with a constant to the
    * record's phoneLabel array.
    *
    * Key              Example               Description
    * --------------------------------------------------
    * PDB_WORK         Company               A string label. Index 0.
    * PDB_HOME         HomePhone             A string label. Index 1.
    * PDB_FAX          FaxNumber             A string label. Index 2.
    * PDB_OTHER        Celular               A string label. Index 3.
    * PDB_EMAIL        E-mail                A string label. Index 4.
    * PDB_MAIN         E-mail 2              A string label. Index 5.
    * PDB_PAGER        Company 2             A string label. Index 6.
    * PDB_MOBILE       BossPhone             A string label. Index 7.
    */
   function SetPhoneLabels($P) {
     if (is_array($P)) {
       $this->phoneLabels[PDB_WORK]   = $P[PDB_WORK];
       $this->phoneLabels[PDB_HOME]   = $P[PDB_HOME];
       $this->phoneLabels[PDB_FAX]    = $P[PDB_FAX];
       $this->phoneLabels[PDB_OTHER]  = $P[PDB_OTHER];
       $this->phoneLabels[PDB_EMAIL]  = $P[PDB_EMAIL];
       $this->phoneLabels[PDB_MAIN]   = $P[PDB_MAIN];
       $this->phoneLabels[PDB_PAGER]  = $P[PDB_PAGER];
       $this->phoneLabels[PDB_MOBILE] = $P[PDB_MOBILE];
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
       $A['firstName'] = isset($L['firstName']) ? $L['firstName'] : $this->Labels['firstName'];
       $A['name']    = isset($L['name'])    ? $L['name']    : $this->Labels['name'];
       $A['company'] = isset($L['company']) ? $L['company'] : $this->Labels['company'];
       $A['phone1']  = isset($L['phone1'])  ? $L['phone1']  : $this->Labels['phone1'];
       $A['phone2']  = isset($L['phone2'])  ? $L['phone2']  : $this->Labels['phone2'];
       $A['phone3']  = isset($L['phone3'])  ? $L['phone3']  : $this->Labels['phone3'];
       $A['phone4']  = isset($L['phone4'])  ? $L['phone4']  : $this->Labels['phone4'];
       $A['phone5']  = isset($L['phone5'])  ? $L['phone5']  : $this->Labels['phone5'];
       $A['phone6']  = isset($L['phone6'])  ? $L['phone6']  : $this->Labels['phone6'];
       $A['phone7']  = isset($L['phone7'])  ? $L['phone7']  : $this->Labels['phone7'];
       $A['phone8']  = isset($L['phone8'])  ? $L['phone8']  : $this->Labels['phone8'];
       $A['address'] = isset($L['address']) ? $L['address'] : $this->Labels['address'];
       $A['city']    = isset($L['city'])    ? $L['city']    : $this->Labels['city'];
       $A['state']   = isset($L['state'])   ? $L['state']   : $this->Labels['state'];
       $A['zipCode'] = isset($L['zipCode']) ? $L['zipCode'] : $this->Labels['zipCode'];
       $A['country'] = isset($L['country']) ? $L['country'] : $this->Labels['country'];
       $A['title']   = isset($L['title'])   ? $L['title']   : $this->Labels['title'];
       $A['custom1'] = isset($L['custom1']) ? $L['custom1'] : $this->Labels['custom1'];
       $A['custom2'] = isset($L['custom2']) ? $L['custom2'] : $this->Labels['custom2'];
       $A['custom3'] = isset($L['custom3']) ? $L['custom3'] : $this->Labels['custom3'];
       $A['custom4'] = isset($L['custom4']) ? $L['custom4'] : $this->Labels['custom4'];
       $A['note']    = isset($L['note'])    ? $L['note']    : $this->Labels['note'];
       
       $this->Labels = $A;
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
     $this->Labels['name'] = substr($fileData,0,PDB_ADDR_LABEL_LENGTH);
     $index = PDB_ADDR_LABEL_LENGTH;
     $this->Labels['firstName'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;
     $this->Labels['company'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;
     $this->Labels['phone1'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;
     $this->Labels['phone2'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;
     $this->Labels['phone3'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;
     $this->Labels['phone4'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;
     $this->Labels['phone5'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;
     $this->Labels['address'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;
     $this->Labels['city'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;
     $this->Labels['state'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;
     $this->Labels['zipCode'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;
     $this->Labels['country'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;
     $this->Labels['title'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;
     $this->Labels['custom1'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;
     $this->Labels['custom2'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;
     $this->Labels['custom3'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;
     $this->Labels['custom4'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;
     $this->Labels['note'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;
     $this->Labels['phone6'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;
     $this->Labels['phone7'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;
     $this->Labels['phone8'] = substr($fileData,$index,PDB_ADDR_LABEL_LENGTH);
     $index += PDB_ADDR_LABEL_LENGTH;

     $this->country = $this->LoadInt8($fileData);
     $this->misc = $this->LoadInt8(substr($fileData,1));
   }

   // Converts the address record data loaded from a file into the internal
   // storage method that is useed for the rest of the class and for ease of
   // use.
   // Return false to signal no error.
   function LoadRecord($fileData, $RecordInfo) {
     $this->RecordAttrs[$RecordInfo['UID']] = $RecordInfo['Attrs'];

     $NewRec = $this->NewRecord();

     //Load phone flags
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

     //Load the fields.
     $filters = array('name'      => 0x0001,
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

     $NewRec['attributes']['expunged'] = $RecordInfo['Attrs'] & 0x80 ? 1 : 0;
     $NewRec['attributes']['dirty'] = $RecordInfo['Attrs'] & 0x40 ? 1 : 0;
     $NewRec['attributes']['deleted'] = $RecordInfo['Attrs'] & 0x20 ? 1 : 0;
     $NewRec['attributes']['private'] = $RecordInfo['Attrs'] & 0x10 ? 1 : 0;
     if (($RecordInfo['Attrs'] & 0xa0) == 0) {
       $NewRec['category'] = $RecordInfo['Attrs'] & 0x0f;
     } else {
       $NewRec['attributes']['archive'] = $RecordInfo['Attrs'] & 0x08 ? 1 : 0;
     }

     $this->Records[$RecordInfo['UID']] = $NewRec;
   }
}


/* WRITE EXAMPLE
 *
$addr = new PalmAddress();
$categorias = array('VIP','AAA','Inicial');
$addr->SetCategoryList($categorias);
$fields = array('name' => 'Pascual',
		'firstName' => 'Eduardo',
		'phone1' => '21221552',
		'phone2' => '58808912',
		'phone5' => 'epascual@cie.com.mx',
		'address' => 'Hda. la Florida 10A',
		'city' => 'Izcalli');
$record['fields'] = $fields;
$addr->SetRecordRaw($record);
$addr->GoToRecord('+1');
$fields = array('name' => 'de tal',
		'firstName' => 'fulanito',
		'address' => 'Direccion',
		'phone1' => '21232425',
		'phone2' => 'fulanito@dondesea.com');
$phones = array('phone1' => PDB_HOME,
		'phone2' => PDB_EMAIL,
		'phone3' => PDB_WORK,
		'phone4' => PDB_FAX,
		'phone5' => PDB_OTHER,
		'display' => 1,
		'reserved' => '');

$record['fields'] = $fields;
$record['phoneLabel'] = $phones;
$record['category'] = 1;
$record['attributes']['private'] = 1;
$addr->SetRecordRaw($record);
$fp = fopen('./pdbs/addr.pdb','wb');
$addr->WriteToFile($fp);
fclose($fp);
 *
 * READ EXAMPLE
 *
$addr = new PalmAddress();
$fp = fopen('./address.pdb','r');
$addr->ReadFile($fp);
fclose($fp);
echo "Nombre: $addr->Name<br>\n";
echo "Tipo: $addr->TypeID<br>";
echo "Creator: $addr->CreatorID<br>\n";
echo "Atributos: $addr->Atributes<br>\n";
echo "Version: $addr->Version<br>\n";
echo "ModNum: $addr->ModNumber<br>\n";
echo "CreationTime: $addr->CreationTime<br>\n";
echo "ModTime: $addr->ModificationTime<br>\n";
echo "BackTime: $addr->BackupTime<br>\n";
echo 'NumRec: '.$addr->GetRecordCount()."<br>\n";
$recids = $addr->GetRecordIDs();
foreach ($recids as $ID) {
  $record = $addr->GetRecordRaw($ID);
  echo "Record $ID:<BR>";
  echo "- Category: ".$record['category']."<br>\n";
  echo "- Private: ". $record['attributes']['private']."<br>\n";
  foreach ($record['phoneLabel'] as $plk => $pl) {
    echo "-- $plk => $pl<br>\n";
  }
  echo " Fields:<br>\n";
  foreach($record['fields'] as $reck => $rec) {
    echo "-- $reck => $rec<br>\n";
  }
}
echo "Field Labels<br>";
$labels = $addr->GetFieldLabels();
foreach($labels as $k => $v) {
  echo "$k = $v<br>";
}
echo "Phone Labels<br>";
$pl = $addr->GetPhoneLabels();
foreach($pl as $k => $v) {
  echo "$k = $v<br>";
}
 *
 */
?>
