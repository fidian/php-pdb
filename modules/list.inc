<?php
/* Class extender for List databases.
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See https://github.com/fidian/php-pdb for more information about the library
 *
 *
 * Database format is not detailed on the web, but the List homepage is at
 *   http://www.magma.ca/~roo/list/list.html
 * Perl database conversion code is at
 *   http://home.pacbell.net/dunhamd/
 *
 * Format is for List, presumably all versions
 */


/*
 * Define constants
 */
define('PDB_LIST_FIELD_MAXCHARS', 63);
define('PDB_LIST_NOTE_MAXCHARS', 1023);
define('PDB_LIST_FIELD_NAME_MAXCHARS', 16);


/*
 * PalmDB Class
 *
 * Contains all of the required methods and variables to write a pdb file.
 * Extend this class to provide functionality for memos, addresses, etc.
 */
class PalmListDB extends PalmDB {
	public $Field1, $Field2;  // Names for the two fields
	public $DisplayReverse;  // Show Field2, then Field1?
	public $WriteProtect;  // Write protect the database?


	// Creates a new database class
	public function PalmListDB($Name = '') {
		PalmDB::PalmDB('DATA', 'LSdb', $Name);
		$this->InitializeListDB();
	}


	// Sets all of the variables to a good default value
	public function InitializeListDB() {
		$this->SetCategoryList(array(
				'Unfiled'
			));
		$this->Field1 = 'Field1';
		$this->Field2 = 'Field2';
		$this->Records = array();
		$this->DisplayReverse = false;
		$this->WriteProtect = false;
	}


	/* Returns the size of the current record if no arguments.
	 * Returns the size of the specified record if arguments. */
	public function GetRecordSize($num = false) {
		if ($num === false)
            $num = $this->CurrentRecord;

		if (!isset($this->Records[$num]) || !is_array($this->Records[$num]))
            return 0;
		$this->CleanRecord($num);
		$size = 5 + strlen($this->Records[$num][0]) + strlen($this->Records[$num][1]) + strlen($this->Records[$num][2]);

		/* 5 = 3 NULL bytes and 3 position indicators
		 * Add another NULL byte if there is anything for the note field */
		if (strlen($this->Records[$num][2]))
            $size ++;
		return $size;
	}

	// Cleans a record -- trims data to the correct size
	function CleanRecord($num) {
		if (!isset($this->Records[$num][0]))
            $this->Records[$num][0] = '';

		if (!isset($this->Records[$num][1]))
            $this->Records[$num][1] = '';

		if (!isset($this->Records[$num][2]))
            $this->Records[$num][2] = '';

		if (strlen($this->Records[$num][0]) > PDB_LIST_FIELD_MAXCHARS)
            $this->Records[$num][0] = substr($this->Records[$num][0], 0, PDB_LIST_FIELD_MAXCHARS);

		if (strlen($this->Records[$num][1]) > PDB_LIST_FIELD_MAXCHARS)
            $this->Records[$num][1] = substr($this->Records[$num][1], 0, PDB_LIST_FIELD_MAXCHARS);

		if (strlen($this->Records[$num][2]) > PDB_LIST_NOTE_MAXCHARS)
            $this->Records[$num][2] = substr($this->Records[$num][2], 0, PDB_LIST_NOTE_MAXCHARS);
	}

	/* Returns the hex-encoded data for the specified record or the current
	 * record if not specified */
	function GetRecord($Rec = false) {
		if ($Rec === false)
            $Rec = $this->CurrentRecord;

		if (!isset($this->Records[$Rec]))
            return '';
		$this->CleanRecord($Rec);
		$size = 3;
		$RecStr = $this->Int8($size);
		$size += 1 + strlen($this->Records[$Rec][0]);
		$RecStr .= $this->Int8($size);

		/* The third field's reference is NULL if there is nothing in the
		 * third field. */
		if (strlen($this->Records[$Rec][2]))
            $size += 1 + strlen($this->Records[$Rec][1]);
		else $size = 0;
		$RecStr .= $this->Int8($size);
		$RecStr .= $this->String($this->Records[$Rec][0]) . $this->Int8(0);
		$RecStr .= $this->String($this->Records[$Rec][1]) . $this->Int8(0);

		if ($size > 0)
            $RecStr .= $this->String($this->Records[$Rec][2]) . $this->Int8(0);
		return $RecStr;
	}

	// Returns the size of the AppInfo block
	function GetAppInfoSize() {
		// This should be 512 bytes all the time.
		return 512;
	}

	// Returns the AppInfo block
	function GetAppInfo() {
		$AppInfo = $this->CreateCategoryData(false);

		if ($this->DisplayReverse)
            $AppInfo .= $this->Int8(1);
		else
            $AppInfo .= $this->Int8(0);

		if ($this->WriteProtect)
            $AppInfo .= $this->Int8(1);
		else
            $AppInfo .= $this->Int8(0);
		$AppInfo .= $this->Int8(0xff);
		$s = $this->String(substr($this->Field1, 0, PDB_LIST_FIELD_NAME_MAXCHARS));
		$AppInfo .= $this->PadString($s, PDB_LIST_FIELD_NAME_MAXCHARS);
		$s = $this->String(substr($this->Field2, 0, PDB_LIST_FIELD_NAME_MAXCHARS));
		$AppInfo .= $this->PadString($s, PDB_LIST_FIELD_NAME_MAXCHARS);
		$AppInfo .= $this->PadString('', 202);
		return $AppInfo;
	}

	/* Generic function to load the AppInfo block into $this->AppInfo
	 * Should only be called within this class
	 * Return false to signal no error */
	function LoadAppInfo($fileData) {
		$this->LoadCategoryData($fileData);
		$fileData = substr($fileData, PDB_CATEGORY_SIZE - 1);

		/* Compensate for accidentally having the high bit on for some odd
		 * reason. */
		$this->DisplayReverse = $this->LoadInt8($fileData) & 0x7F;
		$fileData = substr($fileData, 1);
		$this->WriteProtect = $this->LoadInt8($fileData);
		$fileData = substr($fileData, 2);

		/* We just skipped the unused byte that is supposed to be for
		 * the last category or last record or something. */
		$this->Field1 = $this->LoadString($fileData, PDB_LIST_FIELD_NAME_MAXCHARS);
		$fileData = substr($fileData, PDB_LIST_FIELD_NAME_MAXCHARS);
		$this->Field2 = $this->LoadString($fileData, PDB_LIST_FIELD_NAME_MAXCHARS);
		return false;
	}

	/* Generic function to load a record
	 * Should only be called within this class
	 * Return false to signal no error */
	function LoadRecord($fileData, $recordInfo) {
		// Skip the first three bytes, then extract three strings.
		$fileData = substr($fileData, 3);
		$data = array();
		$data[0] = $this->LoadString($fileData, PDB_LIST_FIELD_MAXCHARS);
		$fileData = substr($fileData, strlen($data[0]) + 1);
		$data[1] = $this->LoadString($fileData, PDB_LIST_FIELD_MAXCHARS);
		$fileData = substr($fileData, strlen($data[1]) + 1);
		$data[2] = $this->LoadString($fileData, PDB_LIST_NOTE_MAXCHARS);
		$this->Records[$recordInfo['UID']] = $data;
		$this->RecordAttrs[$recordInfo['UID']] = $recordInfo['Attrs'];
		return false;
	}
}

