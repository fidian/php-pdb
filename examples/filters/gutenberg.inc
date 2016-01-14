<?PHP
/* Examples for PHP-PDB library - Twister - Project Gutenberg text format
 * to common format filters
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See https://github.com/fidian/php-pdb for more information about the library
 */

function FilterGutenberg($fileData, $BreakOnChapter) {
   // Convert newlines
   $fileData = str_replace("\r\n", "\n", $fileData);
   $fileData = str_replace("\r", "\n", $fileData);

   // Look for the "Small Print!"
   $fileData = explode("*END*\n", $fileData, 2);
   if (isset($fileData[1])) {
      $SmallPrint = $fileData[0];
      $fileData = $fileData[1];

      $SmallPrint .= '*END*';

      // Reformat the "Small Print!"
      $SmallPrint = FilterGutenbergSmallPrint($SmallPrint);
   } else {
      $SmallPrint = '';
      $fileData = $fileData[0];
   }

   // Reformat the story
   $fileData = FilterGutenbergText($fileData, $BreakOnChapter);

   // Remove extra newlines from the end only
   $fileData = rtrim($fileData);

   // Check for an extra 0x1a (^Z) at the end of the text
   if (bin2hex(substr($fileData, -1)) == "1a")
      $fileData = substr($fileData, 0, strlen($fileData) - 1);

   // Remove more extra newlines from the end only
   $fileData = rtrim($fileData);

   // Remove extra blank lines from beginning only
   // Add back just one newline at the end
   $pos = 0;
   while ($fileData[$pos] == "\n" || $fileData[$pos] == ' ') {
      $pos ++;
   }
   $pos --;
   while ($pos >= 0 && $fileData[$pos] == ' ') {
      $pos --;
   }
   $fileData = substr($fileData, $pos + 1) . "\n";

   $fileData = $SmallPrint . "\n\n" . $fileData;

   if (! $BreakOnChapter) {
      return $fileData;
   }

   $Return = array();
   $NewlineCount = 0;
   $Lines = explode("\n", $fileData);
   $Section = array_shift($Lines);
   $Chapters = 0;

   while (count($Lines)) {
      $Line = array_shift($Lines);
      if ($Line == '') {
         $NewlineCount ++;
	 $Section .= "\n";
      } else {
         $NewlineCount = 0;
	 $pos = 0;
	 while ($pos < strlen($Line) && $Line[$pos] == ' ') {
	    $pos ++;
	 }
	 if (strtoupper(substr($Line, $pos, 8)) == 'CHAPTER ') {
	    // Chapter Break
	    $Chapters ++;
	    if ($Chapters > $BreakOnChapter) {
	       $Chapters = 1;
	       $Return[] = rtrim($Section) . "\n";
	       $Section = $Line;
	    } else {
	       $Section .= "\n" . $Line;
	    }
	 } else {
	    $Section .= "\n" . $Line;
	 }
      }
   }

   $Return[] = $Section;

   if (count($Return) == 1)
      return $Return[0];

   return $Return;
}


function FilterGutenbergSmallPrint($SmallPrint) {
   $Lines = explode("\n", $SmallPrint);
   $SmallPrint = array_shift($Lines);
   $prevSize = strlen($SmallPrint);

   while (count($Lines)) {
      $Line = array_shift($Lines);

      if ($prevSize < 50 && $prevSize > 0) {
         $SmallPrint .= "\n" . $Line;
	 $prevSize = strlen($Line);
      } elseif ($Line == '') {
         $SmallPrint .= "\n";
	 $prevSize = 0;
      } else {
         $pos = 0;
         while ($Line[$pos] == ' ')
            $pos ++;

         if ($Line[$pos] == '*')
            $SmallPrint .= "\n" . $Line;
         else
            $SmallPrint .= ' ' . substr($Line, $pos);

	 $prevSize = strlen($Line);
      }
   }

   return $SmallPrint . "\n";
}


function FilterGutenbergText($text, $BreakOnChapter) {
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

      if (strlen($Line) && $Line[0] == ' ' &&
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
