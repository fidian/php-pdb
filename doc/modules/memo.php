<?PHP
/* Documentation for PHP-PDB library -- DOC module
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */

include("../functions.inc");

StandardHeader('Memo Pad Files', 'modules');

?>

<p>Memo Pad files are extremely easy to modify, as you will see below.</p>

<?PHP ShowOverwriteWarning(); ?>

<h3>Including into your program</h3>

<?PHP

ShowExample('
include \'php-pdb.inc\';
include \'modules/memo.inc\';
');

?>

<h3>Creating a new database</h3>

<p>Memo pad files already have a specified name, type, and creator ID.  You
don't need to worry about anything.</p>

<?PHP

ShowExample('
$DB = new PalmMemo();
');

?>

<h3>Writing the database</h3>

<p>This is the same as the base class.  See <a href="../example.php">Basic
Use</a> for more information.</p>

<h3>Loading the database</h3>

<p>This works just like loading files with the base class.  Please see <a
href="../example.php">Basic Use</a> for further information.</p>

<h3>Category Support and Record Attributes</h3>

<p>Memos support categories.  This is the same as the base class.  Please
see <a href="../example.php">Basic Use</a> for more information and
examples.</p>

<h3>Other functions</h3>

<dl>

<dt><b>SetText($text, $record = false)</b></dt>
<dd>If the $record number is specified, it sets the record to $text.</dd>
<dd>If $record number is not specified, it just sets the current memo 
record to $text.</dd>
<dd>$text is limited to 4095 characters and will be automatically trimmed
if it is too long.</dd>

<dt><b>GetText($record = false)</b></dt>
<dd>Returns the text of the memo record specified, if $record was set.</dd>
<dd>Otherwise, it returns the text of the current record.</dd>

</dl>

<h3>Example</h3>

<?PHP

ShowExample('
$pdb = new PalmMemo();
$pdb->GoToRecord(0);
$pdb->SetText("This is the first memo");
$pdb->SetText("This is the second memo", 1);
$pdb->GoToRecord(2);
$pdb->SetText("This is the third memo");
$pdb->DownloadPDB("MemoDB.pdb");
');


StandardFooter();

?>
