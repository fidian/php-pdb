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
   include('../../modules/doc.inc');
} elseif (file_exists('./php-pdb.inc')) {
   include('./php-pdb.inc');
   include('./modules/doc.inc');
}

include('../functions.inc');
// START HERE

if (! isset($Stored)) {
?>
<html><head><title>Bookmark Test Generator</title>
</head><body bgcolor=#FFFFFF>
<p align=center><b><font size="+3">Bookmark Test Generator</font><br>
<font size="-1">(<a href="viewSource.php?file=bookmark_test.php">View
Source</a> -- <a href="../">Back to PHP-PDB Documentation</a>)</font></p>
<h3 align=center>What kind of bookmarks do you want generated?</h2>
<h3 align=center><a href="bookmark_test.php?Stored=1">Stored</a> - 
<a href="bookmark_test.php?Stored=0">Embedded</a></h3>
<?PHP TinyFooter(); ?>
</body></html>
<?PHP
   exit();
}

if ($Stored)
   $pdb = new PalmDoc("Stored Bookmark Test");
else
   $pdb = new PalmDoc("Embedded Bookmark Test");

if ($Stored) {
   $ThisTest = "stored";
   $OtherTest = "embedded";
   $UcFirst = "Stored";
} else {
   $ThisTest = "embedded";
   $OtherTest = "stored";
   $UcFirst = "Embedded";
}

$pdb->AddDocText("This is a test file to see if $ThisTest " .
"bookmarks are recognized by a document reader.  To determine if " .
"$OtherTest bookmarks are supported, you'll need to use the " .
"other bookmark test that you got from $PHP_SELF\n\n" .
"To figure out if it worked, look for the bookmark list in this " .
"document reader.  If it has a bookmark named \"$UcFirst " .
"Bookmark\", then it worked.  If not, then this document reader " .
"doesn't support $ThisTest style bookmarks.\n\n" .
"This document was automatically generated using the PHP-PDB " .
"library -- a free, GPL'd PHP library that manipulates PDB " .
"files.  Feel free to give it a look at\n" .
"http://php-pdb.sourceforge.net\n\n");

// Add the bookmark
if ($Stored)
   $pdb->AddBookmark("Stored Bookmark");
else
   $pdb->AddDocText("*Embedded Bookmark\n");
   
$pdb->AddDocText("This is where \"$UcFirst Bookmark\" should " .
"take you, if this document reader properly supports $ThisTest " .
"bookmarks.\n\n" .
"Don't forget to see if your document reader supports $OtherTest " .
"style bookmarks!\n\n");

$pdb->AddDocText("Have a GREAT day!\n\n");

if (! $Stored)
   $pdb->AddDocText("This next part is needed by the document " .
   "reader to detect where embedded bookmarks are located.  If " .
   "you plan on generating documents with embedded bookmarks, " .
   "you just need to add the special character (I'm using an " .
   "asterisk -- the star thing) at the beginning of the line and " .
   "the rest of the line becomes the bookmark.  Then, you just " .
   "include \"<X>\" at the end of your document (replacing the X " .
   "with the character you picked) to tell the doc reader which " .
   "character you used to define bookmarks, like what I have at " .
   "the end of this one.\n\n" .
   "Please note:  Aportis will use the special character if it " .
   "appears anywhere inside the file, so try to pick a character " .
   "that is not in the text you are converting into a doc " .
   "file:\n\n" .
   "<*>");
else
   $pdb->AddDocText("This is just some garbage that should fill " .
   "the screen so that when you jump to the stored bookmark, the " .
   "message saying that you are in the right spot should be " .
   "immediately at the top.  If this text is not there, and if " .
   "you use a small font, you may see it in the middle or closer " .
   "to the bottom.  This is annoying, and to keep things simple, " .
   "I just add this filler material at the bottom.");

$pdb->DownloadPDB("bookmark_" . $ThisTest . ".pdb");

// END HERE
