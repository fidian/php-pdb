<?PHP
/* Documentation for PHP-PDB library -- MobileDB module
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */

include("../functions.inc");

StandardHeader('MobileDB Files', 'modules');

?>

<p>MobileDB files are somewhat supported, but it isn't easy to add things or
to move data around yet.  Be forewarned.</p>

<h3>Including into your program</h3>

<?PHP

ShowExample('
include \'php-pdb.inc\';
include \'modules/mobiledb.inc\';
');

?>

<h3>Creating a new database</h3>

<P>Because MobileDB databases already have a specific type and creator, you
only need to specify the name of the new database.</p>

<?PHP

ShowExample('
$DB = new PalmMobileDB("Name of DB");
');

?>

<h3>Writing the database</h3>

<p>This is the same as the base class.  See <a href="../example.php">Basic
Use</a> for more information.</p>

<h3>Loading the database</h3>

<p>This is the same as the base class.  See <a href="../example.php">Basic
Use</a> for more information.</p>

<h3>Category Support and Record Attributes</h3>

<p>MobileDB databases do support categories, but they are used strictly for
keeping track of internal data.  Do not use them.</p>

<h3>Other functions</h3>

<p>There are no utility functions defined yet.  Sorry.</p>

<h3>Record Format</h3>

<p>All records in a MobileDB database are arrays.  Since there are no
utility functions for manipulating records yet, it is suggested that you
do not create new MobileDB databases.</p>


<h3>Example</h3>

<p>This great example was submitted by Cristiano Nuernberg 
<?PHP HideEmail('cnuernberg', 'whdh.com') ?>.  Just as a note, it is
better to load a MobileDB and then work on it instead of trying to make
one yourself from scratch.</p>

<?PHP

ShowExample('
#########################################################################
# Base files
#########################################################################

include_once ("php-pdb.inc");
include_once ("modules/mobiledb.inc"); 

#########################################################################
# Output file (generate a unique file name to avoid any type of
caching)
# Set appropriate permissions to "./output/"
#########################################################################

$rand = time().mt_rand(100,999);

$ouput_file = "./output/example-$rand.pdb";

#########################################################################
# Databse name / create instance of the class
#########################################################################

$pdb = new PalmMobileDB("New Database");

#########################################################################
# Headers
#########################################################################

// For some reason we need this (attrib=68)
$Record = array("","","");
$pdb->SetRecordRaw($Record);
$pdb->SetRecordAttrib(68);
$pdb->GoToRecord("+1");

// Field lengths (attrib=70)
$Record = array("80","40","40");
$pdb->SetRecordRaw($Record);
$pdb->SetRecordAttrib(70);
$pdb->GoToRecord("+1");

// Field titles (attrib=65)
$Record = array("Last","First","Time");
$pdb->SetRecordRaw($Record);
$pdb->SetRecordAttrib(65);
$pdb->GoToRecord("+1");

// Data type: T = text, d = time in seconds since midnight (there are
another 1/2-dozen types)
// (attrib=69)
$Record = array("T","T","d");
$pdb->SetRecordRaw($Record);
$pdb->SetRecordAttrib(69);
$pdb->GoToRecord("+1");

#########################################################################
# Actual data
#########################################################################

$Record = array("Nuernberg","Cristiano","28800");

$Record = $record_array;
$pdb->SetRecordRaw($Record);
$pdb->SetRecordAttrib(66);
$pdb->GoToRecord("+1");

$Record = array("Smith","Mark","25200");

$Record = $record_array;
$pdb->SetRecordRaw($Record);
$pdb->SetRecordAttrib(66);
$pdb->GoToRecord("+1");

// etc... 

#########################################################################
# Writing to a file
#########################################################################

$fp = fopen("$ouput_file", "wb");
$pdb->WriteToFile($fp);
fclose($fp);
');


StandardFooter();

?>
