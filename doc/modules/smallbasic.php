<?PHP
/* Documentation for PHP-PDB library -- DOC module
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */

include("../functions.inc");

StandardHeader('SmallBASIC', 'modules');

?>

<p><a href="http://smallbasic.sourceforge.net">SmallBASIC</a> is a
programming language that runs on the handheld.  With this class, you can
convert to/from the .PDB format that the SmallBASIC interpreter uses.</p>

<p><b>Warning:</b>  This class has not yet been tested extensively, so don't
use this for mission-critical applications without further testing.
However, it does both read and write this format successfully for me, so I
expect that it will work for you.</p>

<h3>Including into your program</h3>

<?PHP

ShowExample('
include \'php-pdb.inc\';
include \'modules/smallbasic.inc\';
');

?>

<h3>Creating a new database</h3>

<P>SmallBASIC files already have a defined type and creator ID.  The only
thing left for you to do is to specify the name.  Usually, this is the name
of the file that you are converting.</p>

<?PHP

ShowExample('
$DB = new PalmSmallBASIC("MyTest.BAS");
  // Typical usage to create the SmallBASIC file

$pdb = new PalmDoc();
  // Special:  If you want to create an instance of the class
  // and then use ReadFile() to load the database information
');

?>

<h3>Writing the database</h3>

<p>This is the same as the base class.  See <a href="../example.php">Basic
Use</a> for more information.</p>

<p>To let you know, the first record is the header, and the rest contain
separate sections of code.  No single section can be bigger than 32k.  If,
by some fluke, there is a section that is too large, it will be
automatically trimmed when the database is written.</p>

<h3>Loading the database</h3>

<p>This works just like loading files with the base class.  Please see <a
href="../example.php">Basic Use</a> for further information.</p>

<h3>Category Support and Record Attributes</h3>

<p>SmallBASIC files do not support categories nor record attributes.</p>

<h3>Other functions</h3>

<dl>

<dt><b>ConvertToText()</b></dt>
<dd>Returns the code as a giant string, which can be saved to a regular
file.  This also adds the <tt>#sec:</tt> tags to allow simple conversion
back to Palm OS format.</dd>

<dt><b>ConvertFromText($String)</b></dt>
<dd>Takes the .BAS file and converts it to the internal Palm OS format.  It
also splits apart the <tt>#sec:</tt> sections into separate records.</dd>
<dd>$String = The entire contents of the file that you need converted.</dd>
<dd>Returns false on success.</dd>
<dd>If there is an error, returns an array of the [0] section number, [1] 
section name, and [2] the size of the section in bytes.</dd>

</dl>

<h3>Example</h3>

<?PHP

ShowExample('
$contents = file("your_file.bas");
// You should check to make sure the file
// was loaded properly before proceeding
$contents = implode("", $contents);
$pdb = new PalmSmallBASIC("your_file.bas");
$pdb->ConvertFromText($contents);
$pdb->DownloadPDB("doc_test.pdb");
');

StandardFooter();

?>
