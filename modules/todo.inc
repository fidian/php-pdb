<?php
/* Class extender for Todo databases
 * Class donated by Eduardo Pascual Martinez
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL
 * See the doc/LEGAL file for more information
 * See https://github.com/fidian/php-pdb for more information about the library
 */


/* The data for SetRecordRaw and from GetRecordRaw should be/return a
 * special array, detailed below.  Optional values can be set to '' or not
 * defined.  If they are anything else (including zero), they are considered
 * to be 'set'.  Optional values are marked with a ^.
 *
 * Key            Example             Description
 * ----------------------------------------------
 * Description    ToDo                This is the ToDo text
 * Note           Note                ^ A note for the todo
 * DueDate        2002-06-03          ^ Year-Month-Day of the todo
 * Completed      1                   ^ 1/0 (default 0 not completed)
 * Priority       3                   ^ 1-5 (default 1)
 *
 * If description is not specified, then the string 'No description' will be
 * used.
 */
class PalmTodoList extends PalmDB {
	public $dirty = 0;  // Unknown
	public $sortOrder = 0;  // Unknown


	// Constructor.
	public function PalmTodoList() {
		PalmDB::PalmDB('DATA', 'todo', 'ToDoDB');

		// Set a default CategoryList array.
		$this->SetCategoryList(array(
				1 => 'Business',
				'Personal'
			));
		$this->FirstDay = 0;
	}


	/* Returns a new array with default data for a new record.
	 * This doesn't actually add the record. */
	public function NewRecord() {
		// No due date by default.
		$Item['DueDate'] = '';
		$Item['Completed'] = 0;
		$Item['Priority'] = 1;
		$Item['Description'] = '';
		$Item['Note'] = '';
		return $Item;
	}


	/* Overrides the GetRecordSize method.
	 * Probably should only be used when saving */
	public function GetRecordSize($num = false) {
		if ($num === false)
            $num = $this->CurrentRecord;

		if (!isset($this->Records[$num]) || !is_array($this->Records[$num]))
            return PalmDB::GetRecordSize($num);
		$data = $this->Records[$num];

		/* Due_date (1).
		 * Completed and Priority (2).
		 * NULL for description
		 * NULL for note */
		$Bytes = 5;

		if (isset($data['Description'])) {
			$Bytes += strlen($data['Description']);
		}

		if (isset($data['Note'])) {
			$Bytes += strlen($data['Note']);
		}

		return $Bytes;
	}

	/* Overrides the GetRecord method. We store data in associative arrays.
	 * Just convert the data into the proper format and then return the
	 * generated string. */
	function GetRecord($num = false) {
		if ($num === false) {
			$num = $this->CurrentRecord;
		}

		if (!isset($this->Records[$num]) || !is_array($this->Records[$num])) {
			return PalmDB::GetRecord($num);
		}

		$data = $this->Records[$num];
		$RecordString = '';

		if (isset($data['DueDate'])) {
			$rawDate = $this->DateToInt16($data['DueDate']);
		} else {
			$rawDate = 0xffff;
		}

		$RecordString .= $this->Int16($rawDate);

		if (isset($data['Priority']))
            $priority = $data['Priority'];
		else
            $priority = 1;

		if ($priority < 1)
            $priority = 1;

		if ($priority > 5)
            $priority = 5;

		if (isset($data['Completed']) && $data['Completed']) {
			$priority |= 0x80;
		}

		$RecordString .= $this->Int8($priority);

		if (isset($data['Description']) && $data['Description'] != '')
            $RecordString .= $this->String($data['Description']);
		else
            $RecordString .= $this->String('No description');
		$RecordString .= $this->Int8(0);

		if (isset($data['Note']))
            $RecordString .= $this->String($data['Note']);
		$RecordString .= $this->Int8(0);
		return $RecordString;
	}

	// Sets the data for the current record
	function SetRecordRaw($A, $B = false) {
		if ($B === false) {
			$B = $A;
			$A = $this->CurrentRecord;
		}

		if (!isset($B['Priority'])) {
			$B['Priority'] = 1;
		}

		$this->Records[$A] = $B;
	}

	/* Returns the size of the AppInfo block.  It is the size of the
	 * category list plus six bytes. */
	function GetAppInfoSize() {
		return PDB_CATEGORY_SIZE + 6;
	}

	// Returns the AppInfo block. It is composed of the category list.
	function GetAppInfo() {
		// Category List.
		$AppInfo = $this->CreateCategoryData();

		/* Two unknown (reserved?) bytes
		 * I'm using 0 as the default value since I don't know what it should be */
		$AppInfo .= $this->Int16(0);

		// Two bytes for "dirty" information?
		$AppInfo .= $this->Int16($this->dirty);

		// One byte for sort order?
		$AppInfo .= $this->Int8($this->sortOrder);

		// One null byte to land on an even boundary
		$AppInfo .= $this->Int8(0);
		return $AppInfo;
	}

	/* Parse $fileData for the information we need when loading a Todo
	 * file. */
	function LoadAppInfo($fileData) {
		$this->LoadCategoryData($fileData);

		// Skip past the category information and some unknown data
		$fileData = substr($fileData, PDB_CATEGORY_SIZE + 2);
		$this->dirty = $this->LoadInt16($fileData);
		$fileData = substr($fileData, 2);
		$this->sortOrder = $this->LoadInt8($fileData);
	}

	/* Converts the todo record data loaded from a file into the internal
	 * storage method that is used for the rest of the class and for ease of
	 * use.
	 * Return false to signal no error. */
	function LoadRecord($fileData, $RecordInfo) {
		$this->RecordAttrs[$RecordInfo['UID']] = $RecordInfo['Attrs'];
		$NewRec = $this->NewRecord();

		// Load date
		$date = $this->LoadInt16(substr($fileData, 0, 2));

		if ($date != 0xffff) {
			$NewRec['DueDate'] = $this->Int16ToDate($date);
		}

		$priority = $this->LoadInt8(substr($fileData, 2, 1));
		$completed = $priority & 0x80;
		$priority &= 0x7f;
		$NewRec['Completed'] = $completed ? 1 : 0;
		$NewRec['Priority'] = $priority;
		$i = 3;
		$max = strlen($fileData);
		$description = '';

		while ($fileData[$i] != "\0") {
			echo "Looping\n";
			$description .= $fileData[$i];
			$i ++;

			if ($i > $max)
                return true;
		}

		$i ++;
		$note = '';

		while ($fileData[$i] != "\0") {
			$note .= $fileData[$i];
			$i ++;

			if ($i > $max)
                return true;
		}

		$NewRec['Description'] = $description;
		$NewRec['Note'] = $note;
		$this->Records[$RecordInfo['UID']] = $NewRec;
	}
}
