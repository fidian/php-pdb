<?php
/* Class extender for PalmOS zTXT files
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL
 * See the doc/LEGAL file for more information
 * See https://github.com/fidian/php-pdb for more information about the library
 *
 * For the record, this creates "Mode 1" documents.  They will be a bit
 * larger, but they are searchable.
 */
define('PDB_ZTXT_RECORD_SIZE', 8192);
define('PDB_ZTXT_ANNOTATION_RECORD_LENGTH', 4096);
define('PDB_ZTXT_BOOKMARK_NAME_LENGTH', 20);
define('PDB_ZTXT_BOOKMARK_SIZE', 24);
define('PDB_ZTXT_ANNOTATION_NAME_LENGTH', 20);
define('PDB_ZTXT_ANNOTATION_SIZE', 24);
define('PDB_ZTXT_VERSION', 0x012A);
define('PDB_ZTXT_FLAG_RANDOM_ACCESS', 0x01);
define('PDB_ZTXT_FLAG_NONUNIFORM', 0x02);


class PalmzTXT extends PalmDB {
	public $Bookmarks = array();  // Bookmarks stored in the zTXT file


	// $Bookmarks[position] = "name"
	public $Annotations = array();  // Annotations go here


	// $Annotations[position] = array('name' => '...', 'text' => '.....');
	public $CompressedData = array();  // Filled when saving zTXT file
	public function PalmzTXT($Title = '') {
		PalmDB::PalmDB('xTXT', 'GPlm', $Title);
		$this->EraseText();
	}


	// Gets all of the document's text and returns it as a string
	public function GetText() {
		$String = '';
		$i = 1;

		while (isset($this->Records[$i])) {
			$String .= pack('H*', $this->Records[$i]);
			$i ++;
		}

		return $String;
	}


	// Erases all text in the document
	public function EraseText() {
		$this->Records = array();
		$this->CompressedData = array();
		$this->Bookmarks = array();
		$this->Annotations = array();

		// Record 0 is reserved for header information
		$this->GoToRecord(1);
	}


	// Appends $String to the end of the document
	public function AddText($String) {
		$SpaceLeft = PDB_ZTXT_RECORD_SIZE - (strlen($this->Records[$this->CurrentRecord]) / 2);

		while ($String) {
			if ($SpaceLeft > 0) {
				$this->AppendString($String, $SpaceLeft);
				$String = substr($String, $SpaceLeft);
				$SpaceLeft = PDB_ZTXT_RECORD_SIZE - (strlen($this->Records[$this->CurrentRecord]) / 2);
			} else {
				$this->GoToRecord('+1');
				$SpaceLeft = PDB_ZTXT_RECORD_SIZE;
			}
		}
	}


	/* Creates the informational record (record 0)
	 * Used only for writing the file */
	public function MakeRecordZero() {
		$oldRec = $this->GoToRecord(0);
		$this->DeleteRecord();
		$MaxIndex = count(array_keys($this->Records));
		$Content = pack('H*', join('', $this->Records));
		$this->AppendInt16(PDB_ZTXT_VERSION);  // Version
		$this->AppendInt16($MaxIndex);  // Number of Records
		$this->AppendInt32(strlen($Content));  // Uncompressed text size
		$this->AppendInt16(PDB_ZTXT_RECORD_SIZE);  // Uncompressed record size
		if (count($this->Bookmarks)) {
			$MaxIndex ++;
			$this->AppendInt16(count($this->Bookmarks));  // # of bookmarks
			$this->AppendInt16($MaxIndex);  // Bookmark record
		} else {
			$this->AppendInt32(0);  // 0 for both values
		}

		if (count($this->Annotations)) {
			$this->AppendInt16(count($this->Annotations));  // # of annotations
			$this->AppendInt16($MaxIndex + 1);  // Annotation record
		} else {
			$this->AppendInt32(0);
		}

		$this->AppendInt8(PDB_ZTXT_FLAG_RANDOM_ACCESS);  // Flags
		$this->AppendInt8(0);  // Reserved
		$this->AppendInt32(crc32($Content));  // CRC
		$this->AppendInt32(0);  // Padding
		$this->AppendInt32(0);  // More padding.
		$this->GoToRecord($oldRec);
	}


	// Overrides the output function
	public function WriteToStdout() {
		$this->CompressData();
		PalmDB::WriteToStdout();
	}


	// Overrides the save function
	public function WriteToFile($file) {
		$this->CompressData();
		PalmDB::WriteToFile($file);
	}


	// Overrides the get record attrib function
	public function GetRecordAttrib($Rec = false) {
		// Make all dirty + private
		return 0x40;
	}


	/* Returns the size of the record specified, or the current record if
	 * no record is specified */
	public function GetRecordSize($num = false) {
		if ($num === false)
            $num = $this->CurrentRecord;

		if ($num == 0)
            return 32;

		if (isset($this->Records[$num])) {
			if (!isset($this->CompressedData[$num]))
                $this->CompressData();
			return strlen($this->CompressedData[$num]) / 2;
		}

		$numTemp = $num - 1 - count($this->Records);

		if (count($this->Bookmarks)) {
			if ($numTemp == 0)
                return PDB_ZTXT_BOOKMARK_SIZE * count($this->Bookmarks);
			$numTemp --;
		}

		if (count($this->Annotations)) {
			if ($numTemp == 0)
                return PDB_ZTXT_ANNOTATION_SIZE * count($this->Annotations);
			$numTemp --;

			if ($numTemp < count($this->Annotations))
                return strlen($this->Annotations[$numTemp]);
		}

		return PalmDB::GetRecordSize($num);
	}

	/* Returns the data of the specified record, or the current record if no
	 * record is specified.  If the record doesn't exist, returns ''. */
	function GetRecord($num = false) {
		if ($num === false)
            $num = $this->CurrentRecord;

		if ($num == 0) {
			$this->MakeRecordZero();
			return $this->Records[0];
		}

		if (isset($this->Records[$num])) {
			if (!isset($this->CompressedData[$num]))
                $this->CompressData();
			return $this->CompressedData[$num];
		}

		$numTemp = $num - 1 - count($this->Records);

		if (count($this->Bookmarks)) {
			if ($numTemp == 0) {
				// Sort bookmarks in order of appearance
				$keys = array_keys($this->Bookmarks);
				sort($keys);
				$str = '';

				// Create the bookmark record
				foreach ($keys as $offset) {
					$str .= $this->Int32($offset);
					$name = $this->String($this->Bookmarks[$offset], PDB_ZTXT_BOOKMARK_NAME_LENGTH);
					$str .= $this->PadString($name, PDB_ZTXT_BOOKMARK_NAME_LENGTH);
				}

				return $str;
			}

			$numTemp --;
		}

		if (count($this->Annotations)) {
			if ($numTemp == 0) {
				// Sort annotations in order of appearance
				$keys = array_keys($this->Annotations);
				sort($keys);
				$str = '';

				// Create the annotations index record
				foreach ($keys as $offset) {
					$str .= $this->Int32($offset);
					$name = $this->String($this->Annotations[$offset]['name'], PDB_ZTXT_ANNOTATION_NAME_LENGTH);
					$str .= $this->PadString($name, PDB_ZTXT_ANNOTATION_NAME_LENGTH);
				}

				return $str;
			}

			$numTemp --;

			if ($numTemp < count($this->Annotations)) {
				$keys = array_keys($this->Annotations);
				sort($keys);
				$offset = $keys[$numTemp];
				$str = $this->String($this->Annotations[$offset]['text']);
				$str .= $this->Int8(0);
				return $str;
			}
		}

		return '';
	}

	/* Compresses the entire doc file
	 * The compressed information is cached for better performance with
	 * successive writes */
	function CompressData() {
		$this->CompressedData = array();

		foreach ($this->Records as $index => $str) {
			$m = pack('H*', $str);
			$n = gzdeflate($m, 9);
			$this->CompressedData[$index] = $this->String($n);
		}
	}

	// Returns a list of records to write to a file in the order specified.
	function GetRecordIDs() {
		$ids = PalmDB::GetRecordIDs();

		if (!isset($this->Records[0]))
            array_unshift($ids, 0);
		$Max = 0;

		foreach ($ids as $val) {
			if ($Max <= $val)
                $Max = $val + 1;
		}

		if (count($this->Bookmarks))
            $ids[] = $Max ++;

		if (count($this->Annotations)) {
			$ids[] = $Max ++;

			foreach ($this->Annotations as $a)
                $ids[] = $Max ++;
		}

		return $ids;
	}

	// Returns the number of records to write
	function GetRecordCount() {
		$c = count($this->Records);

		if (!isset($this->Records[0]) && $c)
            $c ++;

		if (count($this->Bookmarks))
            $c ++;

		if (count($this->Annotations))
            $c += 1 + count($this->Annotations);
		return $c;
	}

	/* Adds a bookmark.
	 * $Name must be 20 chars or less (automatically trimmed)
	 * $Pos is the position to add the bookmark at, or the current position if
	 * not specified
	 * Returns true on error
	 * If $Pos already has a bookmark defined, this will blindly overwrite that
	 * bookmark. */
	function AddBookmark($Name, $Pos = false) {
		if ($Name == '')
            return true;

		if ($Pos === false) {
			$Pos = 0;

			foreach ($this->Records as $id => $data) {
				if ($id != 0) {
					$Pos += strlen($data) / 2;
				}
			}
		}

		$this->Bookmarks[$Pos] = substr($Name, 0, PDB_ZTXT_BOOKMARK_NAME_LENGTH);
		return false;
	}

	/* Adds an annotation.
	 * $Name must be 20 chars or less (automatically trimmed)
	 * $Text must be 4096 chars or less (automatically trimmed)
	 * $Pos is the position to add the annotation at, or the current position if
	 * not specified
	 * Returns true on error
	 * If $Pos already has a annotation defined, this will blindly overwrite
	 * that annotation. */
	function AddAnnotation($Name, $Text, $Pos = false) {
		if ($Name == '' || $Text == '')
            return true;

		if ($Pos === false) {
			$Pos = 0;

			foreach ($this->Records as $id => $data) {
				if ($id != 0) {
					$Pos += strlen($data) / 2;
				}
			}
		}

		$this->Annotations[$Pos] = array(
			'name' => substr($Name, 0, PDB_ZTXT_ANNOTATION_NAME_LENGTH),
			'text' => substr($Text, 0, PDB_ZTXT_ANNOTATION_RECORD_LENGTH)
		);
		return false;
	}


	function ReadFile($file) {
		$Ret = PalmDB::ReadFile($file);

		if ($Ret != false)
            return $Ret;

		// Renumber the records
		$rk = array_keys($this->Records);
		$R = array();
		$RA = array();

		foreach ($rk as $from) {
			$R[] = $this->Records[$from];
			$RA[] = $this->RecordAttrs[$from];
		}

		$this->Records = $R;
		$this->RecordAttrs = $RA;

		if (!isset($this->Records[0]))
            return true;

		if ($this->ParseRecordZero())
            return true;
	}

	/* Doesn't work
	 * gzinflate() says that the data is wrong.
	 * gzuncompress() will kill the program.
	 * compressed data is 'deflate'd */
	function ParseRecordZero() {
		$NumRecords = substr($this->Records[0], 4, 4);
		$NumRecords = hexdec($NumRecords);
		$FullText = '';
		$BM = array();
		$ANN = array();
		echo 'Flags: ' . substr($this->Records[0], 36, 2) . "\n";

		// Try something
		$m = pack('H*', $this->Records[1]);
		echo gzinflate($m);

		for ($i = 1; $i <= $NumRecords; $i ++) {
			$FullText .= pack('H*', $this->Records[$i]);
		}

		echo "right here\n";
		echo gzinflate($FullText);
		echo "bozo\n";
		$FullText = gzinflate($FullText);
		echo "goobley\n";
		echo 'FT->' . $FullText . "<-FT\n";

		// Bookmarks
		$bmnum = substr($this->Records[0], 20, 4);
		$bmnum = hexdec($bmnum);

		if ($bmnum) {
			$bmrec = substr($this->Records[0], 24, 4);
			$bmrec = hexdec($bmnum);
			$bmdata = $this->Records[$bmrec];

			while ($bmnum --) {
				$offset = substr($bmdata, 0, 8);
				$offset = hexdec($offset);
				$name = substr($bmdata, 8, 40);
				$name = pack('H*', $name);
				$BM[$offset] = $name;
				$bmdata = substr($bmdata, 48);
			}
		}

		// Annotations
		$annnum = substr($this->Records[0], 28, 4);
		$annnum = hexdec($annnum);

		if ($annnum) {
			$annrec = substr($this->Records[0], 32, 4);
			$annrec = hexdec($annnum);
			$anndata = $this->Records[$annrec];

			while ($annnum --) {
				$offset = substr($anndata, 0, 8);
				$offset = hexdec($offset);
				$name = substr($anndata, 8, 40);
				$name = pack('H*', $name);
				$annrec ++;
				$ANN[$offset] = array(
					'name' => $name,
					'text' => $this->Records[$annrec]
				);
				$anndata = substr($anndata, 48);
			}
		}

		$this->EraseText();
		$this->AddText($FullText);
		$this->Annotations = $ANN;
		$this->Bookmarks = $BM;
		return false;
	}
}

