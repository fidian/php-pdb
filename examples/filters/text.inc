<?PHP
/* Examples for PHP-PDB library - Twister - Text to common format filters
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See https://github.com/fidian/php-pdb for more information about the library
 */

function FilterText($fileData, $RewrapParagraphs) {
   // Convert newlines
   $fileData = str_replace("\r\n", "\n", $fileData);
   $fileData = str_replace("\r", "\n", $fileData);

   if ($RewrapParagraphs) {
      $fileData = FilterTextRewrap($fileData);
   }

   return $fileData;
}

function FilterTextRewrap($text) {
   $Lines = explode("\n", $text);
   $text = '';

   $AddNewline = false;
   $WrappedLastLine = true;
   while (count($Lines)) {
      $Line = array_shift($Lines);

      // Three rules to determine if it shouldn't be wrapped
      // 1)  If the line starts with 4 or more spaces
      // 2)  If the last line was not wrapped
      // 3)  If the next line starts with a space

      if ($Line[0] == ' ' &&
          (substr($Line, 0, 4) == '    ' ||
	   $WrappedLastLine == false ||
	   (isset($Lines[0]) && $Lines[0] != '' && $Lines[0][0] == ' '))) {
         // Special line
         $WrappedLastLine = false;
	 $text .= $Line . "\n";
	 $AddNewline = false;
      } elseif ($Line == '') {
         $WrappedLastLine = true;
         $text .= "\n";
	 if ($AddNewline)
  	    $text .= "\n";
	 $AddNewline = false;
      } else {
         $WrappedLastLine = true;
         $text .= ' ' . trim($Line);
	 $AddNewline = true;
      }
   }

   return $text;
}
