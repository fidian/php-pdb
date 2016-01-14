<?php
/* Class extender for Address Book databases
 * Class donated by Eduardo Pascual Martinez
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL
 * See the doc/LEGAL file for more information
 * See https://github.com/fidian/php-pdb for more information about the library
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


/* The data for SetRecordRaw and from GetRecordRaw should be/return a
 * special array, detailed below.  All values are optional.  Also, the labels
 * that are displayed for each field can be changed -- see SetFieldLabels().
 * The phone# fields are special.  See below for more information.
 *
 * Note:  You must set at least one of the data fields (any field except the
 * phone#Type fields) for the record to be saved properly.  If no data is
 * in the record, the record will have 'Empty Record' as the last name.
 * Avoid this.
 *
 * Key           Example         Description
 * ------------------------------------------------
 * LastName      Duck                   The contact's last name
 * FirstName     Daffy                  The contact's first name
 * Company       PHP-PDB Inc.           Name of the company
 * Phone1        556-6778               A phone number, email, or any string
 * Phone1Type    PDB_ADDR_LABEL_WORK    What phone label to use
 * Phone2        867-5309               A phone number, email, or any string
 * Phone2Type    PDB_ADDR_LABEL_HOME    What phone label to use
 * Phone3        dduck@toon.com  A phone number, email, or any string
 * Phone3Type    PDB_ADDR_LABEL_EMAIL   What phone label to use
 * Phone4        55667788        A phone number, email, or any string
 * Phone4Type    PDB_ADDR_LABEL_OTHER   What phone label to use
 * Phone5        55667788        A phone number, email, or any string
 * Phone5Type    PDB_ADDR_LABEL_MOBILE  What phone label to use
 * Address       Duck street 25  A string with the address
 * City          Toon City       A string with the city
 * State         Toon State      A string with the state
 * ZipCode       78550           A string with the zip code
 * Country       Toon Land       A string with the country
 * Title         Sir             A string with the title
 * Custom1       Birth date      Any string with extra info
 * Custom2       Nick name       Any string with extra info
 * Custom3       Boss name       Any string with extra info
 * Custom4       Whatever        Any string with extra info
 * Note          He is green     Notes for the contact
 * Display       2               Which phone# entry to display [1-5]
 * Reserved      (empty string)  Some reserved data in the record.  Unknown.
 *
 * phone#Type could be one of this values:
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
	/* List of field keys that contribute to the record length
	 * Only the ones that have string data go in here
	 * Key = name of key in $record array, Value = bit in field map */
	public $DataLabels = array(
		'LastName' => 0x00001,
		'FirstName' => 0x00002,
		'Company' => 0x00004,
		'Phone1' => 0x00008,
		'Phone2' => 0x00010,
		'Phone3' => 0x00020,
		'Phone4' => 0x00040,
		'Phone5' => 0x00080,
		'Address' => 0x00100,
		'City' => 0x00200,
		'State' => 0x00400,
		'ZipCode' => 0x00800,
		'Country' => 0x01000,
		'Title' => 0x02000,
		'Custom1' => 0x04000,
		'Custom2' => 0x08000,
		'Custom3' => 0x10000,
		'Custom4' => 0x20000,
		'Note' => 0x40000
	);


	/* Default field labels.
	 * Can be changed for I18N -- see SetFieldLabels() and GetFieldLabels() */
	public $Labels = array(
		'LastName' => 'Last name',
		'FirstName' => 'First name',
		'Company' => 'Company',
		'Phone1' => 'Work',
		'Phone2' => 'Home',
		'Phone3' => 'Fax',
		'Phone4' => 'Other',
		'Phone5' => 'E-mail',
		'Phone6' => 'Main',
		'Phone7' => 'Pager',
		'Phone8' => 'Mobile',
		'Address' => 'Address',
		'City' => 'City',
		'State' => 'State',
		'ZipCode' => 'Zip Code',
		'Country' => 'Country',
		'Title' => 'Title',
		'Custom1' => 'Custom 1',
		'Custom2' => 'Custom 2',
		'Custom3' => 'Custom 3',
		'Custom4' => 'Custom 4',
		'Note' => 'Note'
	);
	public $country = PDB_ADDR_COUNTRY_DEFAULT;  // Default country.
	public $dirtyFields = 0;  // Unknown appinfo 4 bytes
	public $misc = 0;  // Unknown appinfo byte


	/* Constructor.
	 * $country is the Country for which the labels where designed. */
	public function PalmAddress($country = PDB_ADDR_COUNTRY_DEFAULT) {
		PalmDB::PalmDB('DATA', 'addr', 'AddressDB');
		$this->country = $country;
		$this->SetCategoryList(array());
	}


	/* Returns a new array with default data for a new record.
	 * This doesn't actually add the record. */
	public function NewRecord() {
		// Initialize the fields. Empty by default.
		$Record = array(
			'LastName' => '',
			'FirstName' => '',
			'Company' => '',
			'Phone1' => '',
			'Phone1Type' => PDB_ADDR_LABEL_WORK,
			'Phone2' => '',
			'Phone2Type' => PDB_ADDR_LABEL_HOME,
			'Phone3' => '',
			'Phone3Type' => PDB_ADDR_LABEL_FAX,
			'Phone4' => '',
			'Phone4Type' => PDB_ADDR_LABEL_OTHER,
			'Phone5' => '',
			'Phone5Type' => PDB_ADDR_LABEL_EMAIL,
			'Address' => '',
			'City' => '',
			'State' => '',
			'ZipCode' => '',
			'Country' => '',
			'Title' => '',
			'Custom1' => '',
			'Custom2' => '',
			'Custom3' => '',
			'Custom4' => '',
			'Note' => '',
			'Display' => 1,
			'Reserved' => ''
		);
		return $Record;
	}


	/* Overrides the GetRecordSize method.
	 * Probably should only be used when saving */
	public function GetRecordSize($num = false) {
		if ($num === false)
            $num = $this->CurrentRecord;

		if (!isset($this->Records[$num]) || !is_array($this->Records[$num]))
            return PalmDB::GetRecordSize($num);
		$data = $this->Records[$num];

		// Phone Flags (4) + Field Map (4) + Company field offset (1)
		$Bytes = 9;
		$keys = array_keys($this->DataLabels);
		$numSet = 0;

		foreach ($keys as $k) {
			if (isset($data[$k]) && $data[$k] != '') {
				$Bytes += strlen($data[$k]) + 1;  // NULL termination
				$numSet ++;
			}
		}

		if ($numSet == 0) {
			// We'll add "Empty Record" to the LastName field.
			$Bytes += 13;
		}

		return $Bytes;
	}

	/* Overrides the GetRecord method. We store data in associative arrays.
	 * Just convert the data into the proper format and then return the
	 * generated string.
	 * The record is packed in 4 steps.
	 *   1.  4 bytes for the phoneFlags which determines the phone mapping.
	 *   2.  4 bytes that specifies which fields the record contains.
	 *   3.  1 byte for the company field offset -- might be used for the
	 *       address book aplication to quickly display by company name.
	 *   4.  Series of NULL-terminated strings with the values of the record
	 *       fields.
	 * If a record doesn't have a given field, there is no string
	 * corresponding to it. */
	function GetRecord($num = false) {
		if ($num === false) {
			$num = $this->CurrentRecord;
		}

		if (!isset($this->Records[$num]) || !is_array($this->Records[$num])) {
			return PalmDB::GetRecord($num);
		}

		$data = $this->Records[$num];
		$RecordString = '';
		$phoneFlags = ($data['Phone1Type'] & 0x0f) | (($data['Phone2Type'] & 0x0f) << 4) | (($data['Phone3Type'] & 0x0f) << 8) | (($data['Phone4Type'] & 0x0f) << 12) | (($data['Phone5Type'] & 0x0f) << 16) | (($data['Display'] & 0x0f) << 20) | (($data['Reserved'] & 0x0f) << 24);
		$RecordString .= $this->Int32($phoneFlags);
		$fieldMap = 0;
		$fields = '';
		$companyFieldOff = 0;

		foreach ($this->DataLabels as $k => $v) {
			if (isset($data[$k]) && $data[$k] != '') {
				$fieldMap |= $v;
				$fields .= $this->String($data[$k]);
				$fields .= $this->Int8(0);

				if ($k == 'Company') {
					/* Company field offset is the size of the fields string
					 * before the company, plus one.
					 * Careful -- we are dealing with hex encoded data
					 * so I must divide the size of $fields by 2. */
					$companyFieldOff = (sizeof($fields) / 2) + 1;
				}
			}
		}

		if ($fieldMap == 0) {
			// Uh-ho -- no data!  Add some so that this is a valid record
			$fieldMap = $this->DataLabels['LastName'];
			$fields = $this->String('Empty Record');
			$fields .= $this->Int8(0);
		}

		$RecordString .= $this->Int32($fieldMap);
		$RecordString .= $this->Int8($companyFieldOff);
		$RecordString .= $fields;
		return $RecordString;
	}

	/* Sets the data for the current record
	 * SetRecordRaw($RecordArray)  -- Sets the current record
	 * SetRecordRaw($RecNo, $RecordArray)  -- Sets the specified record */
	function SetRecordRaw($A, $B = false) {
		if ($B === false) {
			$B = $A;
			$A = $this->CurrentRecord;
		}

		foreach (array(
				'Priority' => 1,
				'Phone1Type' => PDB_ADDR_LABEL_WORK,
				'Phone2Type' => PDB_ADDR_LABEL_HOME,
				'Phone3Type' => PDB_ADDR_LABEL_FAX,
				'Phone4Type' => PDB_ADDR_LABEL_OTHER,
				'Phone5Type' => PDB_ADDR_LABEL_EMAIL,
				'Display' => 1,
				'Reserved' => ''
			) as $k => $v) {
			if (!isset($B[$k])) {
				$B[$k] = $v;
			}
		}

		$this->Records[$A] = $B;
	}

	// Returns the size of the AppInfo block.
	function GetAppInfoSize() {
		// Standard category size + reserved (2) + dirty fields (4)
		$AppInfoSize = PDB_CATEGORY_SIZE + 6;

		// Field labels
		$AppInfoSize += PDB_ADDR_LABEL_LENGTH * PDB_ADDR_NUM_LABELS;

		/* Country code (1)
		 * misc (1)
		 * dirty fields (2) */
		$AppInfoSize += 4;
		return $AppInfoSize;
	}

	/* Returns the AppInfo block. It is composed of the category list plus
	 * 6 extra bytes, the field labels, the country code, the misc byte
	 * (used for display ordered by company). */
	function GetAppInfo() {
		// Category List
		$AppInfo = $this->CreateCategoryData();
		$AppInfo .= $this->Int16(0);  // Reserved ?

		// "Dirty fields"?  Don't know how this is used
		$AppInfo .= $this->Int32($this->dirtyFields);

		// Field labels
		$keys = array_keys($this->DataLabels);
		$keys[] = 'Phone6';
		$keys[] = 'Phone7';
		$keys[] = 'Phone8';

		foreach ($keys as $k) {
			$field = $this->String($this->Labels[$k], PDB_ADDR_LABEL_LENGTH - 1);
			$AppInfo .= $this->PadString($field, PDB_ADDR_LABEL_LENGTH);
		}

		// Country code
		$AppInfo .= $this->Int8($this->country);

		// Misc
		$AppInfo .= $this->Int8($this->misc);
		$AppInfo .= $this->Int16(0);
		return $AppInfo;
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

	/* Parse $fileData for the information we need when loading an AddressBook
	 * file. */
	function LoadAppInfo($fileData) {
		$this->LoadCategoryData($fileData);

		// The first 2 bytes after the categories are "reserved" (nulls)
		$fileData = substr($fileData, PDB_CATEGORY_SIZE + 2);
		$this->dirtyFields = $this->LoadInt32($fileData);
		$fileData = substr($fileData, 4);

		// Field labels
		$keys = array_keys($this->DataLabels);
		$keys[] = 'Phone6';
		$keys[] = 'Phone7';
		$keys[] = 'Phone8';

		foreach ($keys as $k) {
			$this->Labels[$k] = substr($fileData, 0, PDB_ADDR_LABEL_LENGTH);
			$this->Labels[$k] = rtrim($this->Labels[$k]);
			$fileData = substr($fileData, PDB_ADDR_LABEL_LENGTH);
		}

		$this->country = $this->LoadInt8($fileData);
		$fileData = substr($fileData, 1);
		$this->misc = $this->LoadInt8($fileData);
	}

	/* Converts the address record data loaded from a file into the internal
	 * storage method that is useed for the rest of the class and for ease of
	 * use.
	 * Return false to signal no error. */
	function LoadRecord($fileData, $RecordInfo) {
		$this->RecordAttrs[$RecordInfo['UID']] = $RecordInfo['Attrs'];
		$NewRec = $this->NewRecord();

		// Load phone flags
		$phoneFlags = $this->LoadInt32($fileData);
		$phoneLabels = array(
			'Phone1Type' => $phoneFlags & 0x0f,
			'Phone2Type' => ($phoneFlags >> 4) & 0x0f,
			'Phone3Type' => ($phoneFlags >> 8) & 0x0f,
			'Phone4Type' => ($phoneFlags >> 12) & 0x0f,
			'Phone5Type' => ($phoneFlags >> 16) & 0x0f,
			'Display' => ($phoneFlags >> 20) & 0x0f,
			'Reserved' => ($phoneFlags >> 24) & 0x0f
		);
		$NewRec = $phoneLabels;

		// Load the fieldMap
		$fieldMap = $this->LoadInt32(substr($fileData, 4));

		// Skip past the company field offset and the fieldMap
		$fileData = substr($fileData, 9);

		// Load the fields.
		$i = 0;
		$max = strlen($fileData);

		foreach ($this->DataLabels as $key => $mask) {
			$NewRec[$key] = '';

			if ($fieldMap &$mask) {
				while ($fileData[$i] != "\0" && $i < $max) {
					$NewRec[$key] .= $fileData[$i];
					$i ++;
				}

				$i ++;
			}
		}

		$this->Records[$RecordInfo['UID']] = $NewRec;
	}
}

