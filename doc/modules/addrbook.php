<?PHP
/* Documentation for PHP-PDB library -- Addrbook module
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */

include("../functions.inc");

StandardHeader('Address Book', 'modules');

?>

<p>Address entries have a lot of data associated with them.  Because of
this, there is a somewhat complex way of getting to that data.  However,
I'll try to explain it thoroughly and show examples so that you don't get
lost.</p>

<?PHP ShowOverwriteWarning() ?>

<h3>Including into your program</h3>

<?PHP

ShowExample('
include \'php-pdb.inc\';
include \'modules/addrbook.inc\';
');

?>

<h3>Creating a new database</h3>

<P>Since an address book has a specified type, creator, and name, the class 
takes care of knowing what they are.  Creation of a new contacts database is
trivial.

<?PHP

ShowExample('
$DB = new PalmDatebook();
// or
$DB = new PalmDatebook($Country);
');

?>

<p>The country can be passed in order to facilitate i18n.  It is optional
and will default to PDB_ADDR_COUNTRY_DEFAULT (which is set to
PDB_ADDR_COUNTRY_UNITED_STATES in addrbook.inc -- but you can change it to
affect your entire site if you wish).  A list of all countries is in
addrbook.inc.</p>

<h3>Writing the database</h3>

<p>This is the same as the base class.  See <a href="../example.php">Basic
Use</a> for more information.</p>

<h3>Loading the database</h3>

<p>This is the same as the base class.  See <a href="../example.php">Basic
Use</a> for more information.</p>

<h3>Category Support and Record Attributes</h3>

<p>This supports categories and attributes.  See <a
href="../example.php">Basic Use</a> for more information.</p>

<h3>Other functions</h3>

<dl>

<dt><b>GetRecordRaw()<br>
SetRecordRaw()</b></dt>
<dd>Please see <a href="../example.php">Basic Use</a> for how to use these
functions.  You use both of these to get/set records in the database.</dd>

<dt><b>NewRecord()</b></dt>
<dd>Returns an array with some default data for a new Datebook record.  Does
not actually add the record.  Use SetRecordRaw() for that.</dd>

<dt><b>SetFieldLabels($list)</b><br>
<b>GetFieldLabels()</b></dt>
<dd>Get/set the field labels as an associative array.  The field labels
define what are shown when you view and edit an entry in your address book.
If you want "Last name:" to show up as "Surname:" and "Company:" as
"Business:", you change them in this array.</dd>
<dd><?PHP

ShowExample('
// Load the current set of field labels
$labels = GetFieldLabels();

// If you were to run this code ...
//     foreach ($labels as $key => $val) {
//        echo "$key => $val<br>\n";
//     }
// the results would look like this:
//
// LastName => Last name
// FirstName => First name
// Company => Company
// Phone1 => Work
// Phone2 => Home
// Phone3 => Fax
// Phone4 => Other
// Phone5 => E-mail
// Phone6 => Main
// Phone7 => Pager
// Phone8 => Mobile
// Address => Address
// City => City
// State => State
// ZipCode => Zip Code
// Country => Country
// Title => Title
// Custom1 => Custom 1
// Custom2 => Custom 2
// Custom3 => Custom 3
// Custom4 => Custom 4
// Note => Note

// Let\'s change a few things around
$labels[\'LastName\'] = \'Last Name\';  // Capitalize the "Name" part
$labels[\'FirstName\'] = \'First Name\';  // Capitalize the "Name" part
$labels[\'Phone5\'] = \'Email\';  // Remove hyphen
$labels[\'Phone8\'] = \'Cellular\';  // "Mobile" -> "Cellular"

// Save the changes
$pdb->SetFieldLabels($labels);
');

?></dd>

</dl>

<h3>Record Format</h3>

<p>The data for the GetRecordRaw and the data returned from SetRecordRaw is
a specially formatted array, as detailed below.  Optional values can be set
to '' or not defined.  If an optional value is anything else (including
zero), it is considered to be set.  At least one optional value should be
specified.  If no optional values are specified, then 'Empty Record' will be
displayed as the last name.</p>

<table align=center cellpadding=4 cellspacing=0 border=1 bgcolor=#FFDFDF>
<tr>
<th>Key</th><th>Example</th><th>Description</th>
</tr>
<tr><td>LastName</td><td>Duck</td><td>The contact's last name</td></tr>
<tr><td>FirstName</td><td>Daffy</td><td>The contact's first name</td></tr>
<tr><td>Company</td><td>PHP-PDB Inc.</td><td>The name of the company</td></tr>
<tr><td>Phone1</td><td>867-5309</td><td>What goes in the first
phone/email/etc field.</td></tr>
<tr><td>...</td><td>...</td><td>Same goes for Phone2, Phone3, Phone4
...</td></tr>
<tr><td>Phone5</td><td>dduck@toon.com</td><td>What goes in the last
phone/email/etc field.</td></tr>
<tr><td>Phone1Type</td><td>PDB_ADDR_LABEL_WORK</td><td>The type of the first 
phone record</td></tr>
<tr><td>...</td><td>...</td><td>Same goes for Phone2Type, Phone3Type,
Phone4Type ... (see below)</td></tr>
<tr><td>Phone5Type</td><td>PDB_ADDR_LABEL_EMAIL</td><td>What type of data is
in Phone5</td></tr>
<tr><td>Address</td><td>Duck street 25</td><td>The address</td></tr>
<tr><td>City</td><td>Toon City</td><td>The city</td></tr>
<tr><td>State</td><td>FL</td><td>The state of the contact</td></tr>
<tr><td>ZipCode</td><td>12345</td><td>The post office zip code for the contact</td></tr>
<tr><td>Country</td><td>Toon Land</td><td>The contact's country</td></tr>
<tr><td>Title</td><td>Sir</td><td>The title of the contact</td></tr>
<tr><td>Custom1</td><td>Birth Date</td><td>Extra information</td></tr>
<tr><td>...</td><td>...</td><td>Same goes for Custom2, Custom3</td></tr>
<tr><td>Custom4</td><td>Whatever</td><td>The last extra information field</td></tr>
<tr><td>Note</td><td>Quack.</td><td>Notes for the contact</td></tr>
<tr><td>Display</td><td>1</td><td>Which phone# entry to display on the list
screen</td></tr>
<tr><td>Reserved</td><td>'' (empty string)</td><td>Unknown</td></tr>
</table>

<p>The Phone#Type keys assocaite the five Phone# strings with the proper
labels that are defined with GetFieldLabels and SetFieldLabels.  There are
eight possible strings that Phone# can be associated with. Below is a table
showing the default values for the eight phone labels.  See the 
SetFieldLabels() and GetFieldLabels() for information on how to change the
text that shows up for the label.</p>

<table align=center cellpadding=4 cellspacing=0 border=1 bgcolor=#FFDFDF>
<tr><th>Array Index</th><th>Defined Variable</th><th>Default Value</th></tr>
<tr><td>0</td><td>PDB_ADDR_LABEL_WORK</td><td>Work</td></tr>
<tr><td>1</td><td>PDB_ADDR_LABEL_HOME</td><td>Home</td></tr>
<tr><td>2</td><td>PDB_ADDR_LABEL_FAX</td><td>Fax</td></tr>
<tr><td>3</td><td>PDB_ADDR_LABEL_OTHER</td><td>Other</td></tr>
<tr><td>4</td><td>PDB_ADDR_LABEL_EMAIL</td><td>E-Mail</td></tr>
<tr><td>5</td><td>PDB_ADDR_LABEL_MAIN</td><td>Main</td></tr>
<tr><td>6</td><td>PDB_ADDR_LABEL_PAGER</td><td>Pager</td></tr>
<tr><td>7</td><td>PDB_ADDR_LABEL_MOBILE</td><td>Mobile</td></tr>
</table>

<p>Let's assume that for your contact, you have two work numbers, a fax
number, an email address, and a cell phone number (in that order).  You'd
want to associate them with the correct labels for the numbers, and the
below table uses this information for its example.  Additionally, in the
record array is 'Display', which is the index of the array to display.
The example will assume you want the email address displayed.</p>

<?PHP

ShowExample('
// Set the other aspects of $record before this ...
// For example,   $record[\'FirstName\'] = \'Donald\';
// Also set the Phone# entries before here

// Associate Phone1 and Phone2 with the Work label
$record[\'Phone1\'] = PDB_ADDR_LABEL_WORK;
$record[\'Phone2\'] = PDB_ADDR_LABEL_WORK;

// Phone3 is a fax number, Phone4 is an email, Phone5 is a cell phone number
$record[\'Phone3\'] = PDB_ADDR_LABEL_FAX;
$record[\'Phone4\'] = PDB_ADDR_LABEL_EMAIL;
$record[\'Phone5\'] = PDB_ADDR_LABEL_MOBILE;

// Display the email address on the list screen of the address book app
$record[\'Display\'] = 3;
');

?>

<p>For the attributes and categories, see <a href="../example.php">Basic
Use</a> for the class.</p>

<h3>Example</h3>

<?PHP

ShowExample('
// How to write a database
$addr = new PalmAddress();

// Remember -- category 0 is reserved.
$categorias = array(1 => \'VIP\', \'AAA\', \'Inicial\');
$addr->SetCategoryList($categorias);
$fields = array(\'LastName\' => \'Pascual\',
                \'FirstName\' => \'Eduardo\',
                \'Phone1\' => \'21221552\',
                \'Phone2\' => \'58808912\',
                \'Phone5\' => \'epascual@cie.com.mx\',
                \'Address\' => \'Hda. la Florida 10A\',
                \'City\' => \'Izcalli\');
$addr->SetRecordRaw($fields);
$addr->GoToRecord(\'+1\');
$fields = array(\'LastName\' => \'de tal\',
                \'FirstName\' => \'fulanito\',
                \'Address\' => \'Direccion\',
                \'Phone1\' => \'21232425\',
                \'Phone1Type\' => PDB_ADDR_LABEL_HOME,
                \'Phone2\' => \'fulanito@dondesea.com\',
                \'Phone2Type\' => PDB_ADDR_LABEL_EMAIL,
                \'Phone3Type\' => PDB_ADDR_LABEL_WORK,
                \'Phone4Type\' => PDB_ADDR_LABEL_FAX,
                \'Phone5Type\' => PDB_ADDR_LABEL_OTHER,
                \'Display\' => 1);
$addr->SetRecordRaw($fields);
$addr->SetRecordAttrib(PDB_RECORD_ATTRIB_PRIVATE |
                       1);  // Category 1, private record
$fp = fopen(\'./pdbs/addr.pdb\',\'wb\');
$addr->WriteToFile($fp);
fclose($fp);
');

ShowExample('
// How to read a database
$addr = new PalmAddress();
$fp = fopen(\'./address.pdb\',\'r\');
$addr->ReadFile($fp);
fclose($fp);
echo "Name: $addr->Name<br>\n";
echo "Type: $addr->TypeID<br>";
echo "Creator: $addr->CreatorID<br>\n";
echo "Attributes: $addr->Attributes<br>\n";
echo "Version: $addr->Version<br>\n";
echo "ModNum: $addr->ModNumber<br>\n";
echo "CreationTime: $addr->CreationTime<br>\n";
echo "ModTime: $addr->ModificationTime<br>\n";
echo "BackTime: $addr->BackupTime<br>\n";
echo "NumRec: ".$addr->GetRecordCount()."<br>\n";
$recids = $addr->GetRecordIDs();
foreach ($recids as $ID) {
  $record = $addr->GetRecordRaw($ID);
  echo "Record $ID:<BR>";
  $attrib = $addr->GetRecordAttrib();
  echo "- Category: " . ($attrib & PDB_CATEGORY_MASK) . " = " .
    $addr->CategoryList[$attrib & PDB_CATEGORY_MASK][\'Name\'] . "<br>\n";
  echo "- Private: " . 
    (($attrib & PDB_RECORD_ATTRIB_PRIVATE) ? "Yes" : "No") ."<br>\n";
  echo " Fields:<br>\n";
  foreach ($record as $reck => $rec) {
    echo "-- $reck => $rec<br>\n";
  }
}
echo "Field Labels<br>";
$labels = $addr->GetFieldLabels();
foreach ($labels as $k => $v) {
  echo "$k = $v<br>";
}
');

StandardFooter();

?>
