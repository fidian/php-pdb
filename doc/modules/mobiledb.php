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

<p>This example is of poor quality since MobileDB support has just barely
been added.  It is better to load a MobileDB and then work on it instead of
trying to make one yourself.</p>

<?PHP

ShowExample('
// Create an instance of the class
$pdb = new PalmMobileDB();

// Create a new record
$Record = array("one", true, 35);

// Add the record to the database
$pdb->SetRecordRaw($Record);

// Advance to the next record just in case we want to add more
$pdb->GoToRecord("+1");
');


StandardFooter();

?>
