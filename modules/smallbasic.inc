<?php
/* Class extender for SmallBASIC source files
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL
 * See the doc/LEGAL file for more information
 * See https://github.com/fidian/php-pdb for more information about the library
 */
define('PDB_SMALLBASIC_MAIN_SECTION', 'Main');
define('PDB_SMALLBASIC_MAX_SECTION', 32766);
define('PDB_SMALLBASIC_FILE_VERSION', 4);


class PalmSmallBASIC extends PalmDB {
	public function PalmSmallBASIC($Title = '') {
		PalmDB::PalmDB('TEXT', 'SmBa', $Title);
		$this->Records = array();
	}


	/* Returns a giant string that can be saved to a file and loaded by
	 * other SmallBASIC interpreters */
	public function ConvertToText() {
		$RecordNames = $this->GetOrderedSectionNames;
		$String = '';

		foreach ($RecordNames as $Name) {
			if ($String != '')
                $String .= "\n";

			if ($Name == PDB_SMALLBASIC_MAIN_SECTION) {
				$Data = explode("\n", $this->Records[$Name], 2);

				if ($Data[0][0] == '#') {
					$String .= $Data[0] . '#sec:' . $Name . "\n";
				} else {
					$String .= '#sec:' . $Name . "\n" . $Data[0];
				}

				if (isset($Data[1]))
                    $String .= "\n" . $Data[1];
			} else {
				$String .= '#sec:' . $Name . "\n" . $this->Records[$Name];
			}
		}

		return $String;
	}

	/* Sets all of the records properly to the SmallBASIC code passed
	 * in.
	 * Returns 0 on success
	 * Returns array(section num, section name, section bytes)
	 * if the text of a section is > 32k */
	function ConvertFromText($String) {
		$this->Records = array();

		// Convert newlines to \n
		$String = str_replace("\r\n", "\n", $String);
		$String = str_replace("\r", "\n", $String);
		$Lines = explode("\n", $String);
		$ThisName = PDB_SMALLBASIC_MAIN_SECTION;

		while (count($Lines)) {
			if (strncmp($Lines[0], '#sec:', 5) == 0) {
				$ThisName = array_shift($Lines);
				$ThisName = substr($ThisName, 5);
			} else {
				if (isset($this->Records[$ThisName]))
                    $this->Records[$ThisName] .= "\n";
				else
                    $this->Records[$ThisName] = '';
				$this->Records[$ThisName] .= array_shift($Lines);
			}
		}

		$SectionNames = $this->GetOrderedSectionNames();

		foreach ($SectionNames as $index => $Name) {
			if (strlen($this->Records[$Name]) > PDB_SMALLBASIC_MAX_SECTION)
                return array(
                    $index,
                    $this->Records[$Name],
                    strlen($this->Records[$Name])
                );
		}

		return false;
	}

	/* Returns the sorted list of section names
	 * Forces 'Main' to be first */
	function GetOrderedSectionNames() {
		$keys = array_keys($this->Records);
		sort($keys);

		if (!isset($this->Records[PDB_SMALLBASIC_MAIN_SECTION]) || $keys[0] == PDB_SMALLBASIC_MAIN_SECTION)
            return $keys;
		$SkipName = $keys[0];
		$index = 1;

		while ($keys[$index] != PDB_SMALLBASIC_MAIN_SECTION)
            $index ++;

		while ($index) {
			$keys[$index] = $keys[$index - 1];
		}

		$keys[0] = PDB_SMALLBASIC_MAIN_SECTION;
		return $keys;
	}

	// Returns the size of the specified record
	function GetRecordSize($num = false) {
		if ($num === false)
            $num = $this->CurrentRecord;

		if ($num == 0)
            return 68;
		$keys = $this->GetOrderedSectionNames();
		$num --;

		if (!isset($keys[$num]))
            return 0;
		return strlen($this->Records[$keys[$num]]) + 71;
	}

	/* Returns the data of the specified record, or the current record if no
	 * record is specified.  If the record doesn't exist, returns ''. */
	function GetRecord($num = false) {
		if ($num === false)
            $num = $this->CurrentRecord;

		if ($num == 0)
            return $this->CreateRecordZero();
		$num --;
		$keys = $this->GetOrderedSectionNames();

		if (!isset($keys[$num]))
            return '';
		$Str = $this->String('S');  // Sign
		$Str .= $this->Int8(0);  // Unused
		$Str .= $this->Int16(PDB_SMALLBASIC_FILE_VERSION);  // version

		// Not sure if this is always the same as the one in the

		// header.  Since it was the same when I was given the specs,

		// I just used the define()d value.
		if ($keys[$num] == PDB_SMALLBASIC_MAIN_SECTION)
            $Str .= $this->Int16(1);  // Flags (1 for main section)
		else
            $Str .= $this->Int16(0);  // Flags (0 for every other section)
		$name = $this->String($keys[$num]);
		$name = substr($name, 0, 63);  // Trim if it is too long of a name
		$Str .= $this->PadString($name, 64);
		$code = $this->Records[$keys[$num]];
		$code = substr($code, 0, PDB_SMALLBASIC_MAX_SECTION);
		$Str .= $this->String($code);
		return $Str . '00';
	}

	// Returns what should be record zero
	function CreateRecordZero() {
		$Str = $this->String('H');  // Sign
		$Str .= $this->Int8(PDB_SMALLBASIC_FILE_VERSION);
		$Str .= '0000';  // First byte = unused.

		// Second = category (unfiled)
		$Str .= $this->PadString('', 64);

		/* First four bytes = flags
		 *   0x00000001 = compressed (not supported yet)
		 *   0x00000002 = PalmOS script */
		return $Str;
	}

	// Returns a list of records to write to a file in the order specified
	function GetRecordIDs() {
		$keys = array_keys($this->Records);
		$keys[] = 'blah';
		return array_keys($keys);
	}

	// Returns the number of records to write
	function GetRecordCount() {
		return count($this->Records) + 1;
	}

	/* Converts the PalmOS record to our internal format
	 * Returns false to signal no error */
	function LoadRecord($fileData, $RecordInfo) {
		/* Throw away record 0 since we can just regenerate it easily enough
		 * (Note:  We're losing the category info.  If that is important
		 *         enough, it is easy enough to grab.) */
		if ($RecordInfo['UID'] == 0)
            return false;

		// Grab the name
		$i = 6;
		$Name = '';

		while ($fileData[$i] != "\0") {
			$Name .= $fileData[$i];
			$i ++;
		}

		/* Take code.
		 * Mmmmmmmm.  Code good. */
		$this->Records[$Name] = substr($fileData, 70, strlen($fileData) - 71);
		return false;
	}
}

