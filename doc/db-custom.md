<?PHP
/* Documentation for PHP-PDB library
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */

include("./functions.inc");

StandardHeader('Custom Formats');

?>

<p>All files and programs on a <a href="http://www.palmos.com/">Palm OS</a>
device are databases.  Most have their own specific format.  Because there
isn't a class for every different type of database, and because new formats
and programs are being developed continuously, you may need information
about how to write a custom format database.</p>

<p>If you get around to writing a class that supports your database type, I
would love to have it included in PHP-PDB as part of the main distribution.
Just mail me the code with a note of what it does and a URL to your software
and I'll make sure it gets included in the next release.</p>

<h3>Before, during, and after</h3>

<p>Because there is no class to handle your custom format, you are limited
to creation of the database in memory and saving the database.  You can not
load the database unless you write a custom load procedure.  Please read the
code for other database types in order to see how to load data.  Since
writing a database is a lot easier than reading, this document will only
cover writing a file.</p>

<p>For help about the syntax and usage of functions listed here, see <a
href="example.php">Basic Use</a>.</p>

<?PHP

ShowExample('
include \'php-pdb.inc\';

// Your code goes here to get the data and whatnot.

$pdb = new PalmDB("type", "creator", "Name of Database");

// This sample loop will turn your data into records
// You will need to modify it a lot to convert your data
// into the format you desire.
foreach ($data_records as $array_of_data) {
    // Insert appropriate Append* functions here
    // See below for example lines to stick into here
    $pdb->GoToRecord(\'+1\');  // Go to the next record
}

$pdb->DownloadPDB(\'example.pdb\');  // Write the file to the browser
');

?>

<h3>Converting your information</h3>

<p>Now we have to get to the nitty gritty details about converting
$array_of_data into records.  For the sake of simplicity, I will always
assume that the data in the array is in the order you want it to appear in
the record.</p>

<p>If you want to have three NULL-terminated strings, you could use this
snippit of code:</p>

<?PHP

ShowExample('
// AppendString() adds a string to the record.
// The string is not automatically NULL terminated.
// AppendInt8() in this example is used to add the NULL
// characters at the end of each string.
$pdb->AppendString($array_of_data[0]);
$pdb->AppentInt8(0);
$pdb->AppendString($array_of_data[1]);
$pdb->AppentInt8(0);
$pdb->AppendString($array_of_data[2]);
$pdb->AppentInt8(0);
');

?>

<p>It is likely that you want to store some numbers with the record.
Assuming that the data that you want to store is a short int, int, long int,
and then a NULL-terminated string, your PHP code may look like this.</p>

<?PHP

ShowExample('
// Four types of data.  A \'short int\', \'int\', \'long int\',
// and a NULL-terminated string
$pdb->AppendInt8($array_of_data[0]);
$pdb->AppendInt16($array_of_data[1]);
$pdb->AppendInt32($array_of_data[2]);
$pdb->AppendString($array_of_data[3]);
$pdb->AppendInt8(0);  // the NULL
');

?>

<p>For setting the backup bit on the file itself, you can use this little 
snippet of code.  Just plop this line in anywhere after the creation of the 
PDB file in memory and before you write the file.</p>

<?PHP

ShowExample('
$pdb->Attributes |= PDB_ATTRIB_BACKUP;
');

?>

<p>You can use this with any of the other PDB_ATTRIB_* defined values.  They
are all listed at the top of php-pdb.inc.

<p>Just in case you were wondering about the backup flag for each record,
you can set it either when you create each record, or you can set them all
right before you write the record.  If you want to set the attribute while 
you are writing the record, add this somewhere in the sample loop, before
the GoToRecord() function.</p>

<?PHP

ShowExample('
$pdb->RecordAttrs[$pdb->GoToRecord()] |= PDB_RECORD_ATTRIB_DIRTY;
');

?>

<p>If you would rather just set all of the attributes at the end, these
three lines should complete the task:</p>

<?PHP

ShowExample('
foreach ($pdb->GetRecordIDs() as $id) {
   $pdb->RecordAttrs[$id] |= PDB_RECORD_ATTRIB_DIRTY;
}
');

?>

<p>Again, there are other attributes that you can set.  They are also at the
beginning of php-pdb.inc and all of them start with PDB_RECORD_ATTRIB_*.</p>
				
<?PHP

StandardFooter();

?>
