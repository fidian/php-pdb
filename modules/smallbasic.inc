<?PHP
/* Class extender for SmallBASIC source files
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */
 
class PalmSmallBASIC extends PalmDB {
   function PalmSmallBASIC ($Title = '') {
      PalmDB::PalmDB('TEXT', 'SmBa', $Title);

      $this->EraseText();
   }


   // Gets all of the document's text and returns it as a string
   function GetText () {
      $String = '';
      $i = 1;
      while (isset($Records[$i])) {
         $String .= pack('H*', $Records[$i]);
	 $i ++;
      }
      return $String;
   }
   
   
   // Erases all text in the document
   function EraseText () {
      $this->Records = array();
      // Record 0 is reserved for header information
      $this->GoToRecord(1);
   }
   
   
   // Sets the text in the document to $String.
   // Returns 0 on success
   // Returns array(section num, section name, section bytes)
   // if the text of a section is > 32k
   function SetText ($String) {
      $this->EraseText();
      
      // Convert newlines to \n
      $String = str_replace("\r\n", "\n", $String);
      $String = str_replace("\r", "\n", $String);
      
      $Lines = explode("\n", $String);
      $lnx_head = '';
      if (strncmp($Lines[0], '#!', 2) == 0) {
         $lnx_head = array_shift($Lines);
	 $lnx_head = substr($lnx_head, 2) . "\n";
      }

      $RecordNames = array();
      $RecordCode = array();
      $ThisName = 'Main';
      $ThisCode = '';
      while (count($Lines)) {
         if (strncmp($Lines[0], '#sec:', 5) == 0) {
	    if ($ThisCode != '' || $ThisName != 'Main') {
	       $RecordNames[] = $ThisName;
	       $RecordCode[] = $ThisCode;
	    }
	    $ThisName = array_shift($Lines);
	    $ThisName = substr($ThisName, 5);
	    $ThisCode = '';
	 } else {
	    if ($ThisCode != '')
	       $ThisCode .= "\n";
	    $ThisCode .= array_shift($Lines);
	 }
      }
      $RecordNames[] = $ThisName;
      $RecordCode[] = $ThisCode;
      
      foreach ($RecordCode as $index => $Code) {
         $len = 0;
         if ($index == 0)
	    $len = strlen($lnx_head);
	 $len += strlen($Code) + 1;
	 if ($len > 32767)
	    return array($index, $RecordNames[$index], $len);
      }
   }
}

?>
