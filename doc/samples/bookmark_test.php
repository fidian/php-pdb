<?PHP
/* Examples for PHP-PDB library - Bookmark test
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */

if (file_exists('../../php-pdb.inc')) {
   include('../../php-pdb.inc');
   include('../../php-pdb-doc.php');
} elseif (file_exists('./php-pdb.inc')) {
   include('./php-pdb.inc');
   include('./php-pdb-doc.php');
}


// START HERE

// This copies a string 16 times.  It puts newlines between the
// strings and adds another newline at the end for spacing.
function RepeatString($str) {
   $str .= "\n" . $str . "\n" . $str . "\n" . $str . "\n";
   return $str . $str . $str . $str . "\n";
}

$pdb = new PalmDoc("Bookmark Test");

// Add some text on top
$pdb->AddDocText(RepeatString("Top of Document"));

// Add a stored bookmark
$pdb->AddBookmark("Just Stored");

// Add some text to make it stand apart from the next bookmark
$pdb->AddDocText(RepeatString("Stored Bookmark"));

// Add a stored bookmark
$pdb->AddBookmark("Both - Stored");

// Add the embedded bookmark.  I am using the * character
// as my bookmark character.  Note:  You just add * at the
// beginning of the line and the rest of the line becomes
// your bookmark.
$pdb->AddDocText("* Both - Embedded\n");

// Add some text to make it stand apart from the next bookmark
$pdb->AddDocText(RepeatString("Both types of bookmarks"));

// Add just an embedded bookmark.
$pdb->AddDocText("* Just Embedded\n");

// Add some text to make it stand apart from the next bookmark
$pdb->AddDocText(RepeatString("Embedded bookmark"));

// This is how you specify the special character that becomes
// your bookmark character.  RepeatString() adds two newlines,
// so this is on its own line
$pdb->AddDocText("<*>");

$pdb->DownloadPDB("bookmark_test.pdb");

// END HERE
