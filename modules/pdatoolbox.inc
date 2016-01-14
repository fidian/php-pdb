<?php
/* Class extender for PDA Toolbox databases
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL
 * See the doc/LEGAL file for more information
 * See https://github.com/fidian/php-pdb for more information about the library
 */


/* The data for the GetRecord and SetRecord functions should return/be fed
 * an array.  The data in the array should match the format specified by the
 * data format string.
 *
 * For instance, if your data format string is 'sdkljasfgljhg'
 * Then your array should have X elements and look like this:
 *   $array[0] =
 *   $array[1] =
 */
class PalmDatebook extends PalmDB {
	// Constructor -- initialize the default values
	public function PalmDatebook($CreatorID, $Name = '') {
		// Are all called TOUR ?
		PalmDB('TOUR', $CreatorID, $Name);
	}


	// Overrides the GetRecordSize method.
	public function GetRecordSize($num = false) {
		if ($num === false)
            $num = $this->CurrentRecord;

		if (!isset($this->Records[$num]) || !is_array($this->Records[$num]))
            return PalmDB::GetRecordSize($num);
		$data = $this->Records[$num];

		// Insert code here
		return $Bytes;
	}

	/* Overrides the GetRecord method.  We store data in associative arrays.
	 * Just convert the data into the proper format and then return the
	 * generated string. */
	function GetRecord($num) {
		if (!isset($this->Records[$num]) || !is_array($this->Records[$num]))
            return PalmDB::GetRecord($num);
		$data = $this->Records[$num];
		$RecordString = '';

		// Insert code here
		return $RecordString;
	}

	/* Converts the datebook record data loaded from a file into the internal
	 * storage method that is used for the rest of the class and for ease of
	 * use. */
	function LoadRecord($fileData, $RecordInfo) {
		$this->RecordAttrs[$RecordInfo['UID']] = $RecordInfo['Attrs'];
		$NewRec = array();

		/* Check for record 0
		 * Insert code here */
		$this->Records[$RecordInfo['UID']] = $NewRec;
	}
}

