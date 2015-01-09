<?PHP
/* Documentation for PHP-PDB library -- MobileDB module
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */

include("../functions.inc");

StandardHeader('List Databases', 'modules');

?>

<p>List databases are supported fully.  You have to use the SetRecordRaw and
GetRecordRaw functions currently, but that can change in the future to use a
tailored function instead.</p>

<h3>Including into your program</h3>

<?PHP

ShowExample('
include \'php-pdb.inc\';
include \'modules/list.inc\';
');

?>

<h3>Creating a new database</h3>

<P>Because List databases already have a specific type and creator, you
only need to specify the name of the new database.</p>

<?PHP

ShowExample('
$DB = new PalmListDB("Name of DB");
');

?>

<h3>Writing the database</h3>

<p>This is the same as the base class.  See <a href="../example.php">Basic
Use</a> for more information.</p>

<h3>Loading the database</h3>

<p>This is the same as the base class.  See <a href="../example.php">Basic
Use</a> for more information.</p>

<h3>Category Support and Record Attributes</h3>

<p>List supports categories.  This is the same as the base class.  See <a
href="../example.php">Basic Use</a> for more information.</p>

<h3>Other functions</h3>

<p>There are no utility functions defined yet.  Sorry.</p>

<h3>Record Format</h3>

<p>All records in a List database are arrays.  The first two elements can be
a maximum of 63 characters.  The third (and last) element is a note field
and can be up to 1023 characters.</p>

<h3>Example</h3>

<?PHP

ShowExample('
include_once ("php-pdb.inc");
include_once ("modules/list.inc"); 

$pdb = new PalmListDB("New Database");

// Name the two fields
$pdb->Field1 = "Name"
$pdb->Field2 = "Job"

// Add a few people
$pdb->SetRecordRaw(array("Tyler Akins", "IT", "A really great guy."));
$pdb->GoToRecord("+1");
$pdb->SetRecordRaw(array("Madonna", "Singer"));
$pdb->GoToRecord("+1");
$pdb->SetRecordRaw(array("Adam Sandler", "Entertainer",
   "Did a few movies, sang a few songs, got mentioned here."));

// Write to a file
$fp = fopen("$ouput_file", "wb");
$pdb->WriteToFile($fp);
fclose($fp);
');


StandardFooter();

?>
