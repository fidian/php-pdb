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

<p>If you install this database onto your handheld, it will <i>overwrite</i>
the one that already exists on your handheld.  This is obviously irritating.
Please keep that in mind.  An ideal use for this PHP class would be for
talking to a conduit, where the conduit would upload the current address book,
the server would parse it and add/modify/delete entries with this class, the
server would send the modified address book back to the conduit, and the 
conduit would replace the device's address book with the modified one.</p>

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

<h3>Other functions</h3>

<dl>

<dt><b>GetRecordRaw()<br>
SetRecordRaw()</b></dt>
<dd>Please see <a href="../example.php">Basic Use</a> for how to use these
functions.  You use both of these to get/set records in the database.</dd>

<dt><b>NewRecord()</b></dt>
<dd>Returns an array with some default data for a new Datebook record.  Does
not actually add the record.  Use SetRecordRaw() for that.</dd>

<dt><b>SetCategoryList($list)</b></dt>
<dd>Sets the category list.  $list is an array with the list of categories.
You can have up to 15 user-defined categories.  Category 0 is reserved for
'Unfiled' and should not be specified in $list.</dd>

<dt><b>GetCategoryList()</b></dt>
<dd>Returns the category list.</dd>

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
// lastName => Last name
// firstName => First name
// company => Company
// phone1 => Work
// phone2 => Home
// phone3 => Fax
// phone4 => Other
// phone5 => E-mail
// phone6 => Main
// phone7 => Pager
// phone8 => Mobile
// address => Address
// city => City
// state => State
// zipCode => Zip Code
// country => Country
// title => Title
// custom1 => Custom 1
// custom2 => Custom 2
// custom3 => Custom 3
// custom4 => Custom 4
// note => Note

// Let\'s change a few things around
$labels[\'lastName\'] = \'Last Name\';  // Capitalize the "Name" part
$labels[\'firstName\'] = \'First Name\';  // Capitalize the "Name" part
$labels[\'phone5\'] = \'Email\';  // Remove hyphen
$labels[\'phone8\'] = \'Cellular\';  // "Mobile" -> "Cellular"

// Save the changes
$pdb->SetFieldLabels($labels);
');

?></dd>

</dl>

<h3>Record Format</h3>

<p>The data for the GetRecordRaw and the data returned from SetRecordRaw is
a specially formatted array, as detailed below.  Optional values can be set
to '' or not defined.  If an optional value is anything else (including
zero), it is considered to be set.</p>

<table align=center cellpadding=4 cellspacing=0 border=1 bgcolor=#FFDFDF>
<tr>
<th>Key</th><th>Example</th><th>Description</th>
</tr>
<tr><td>fields</td><td><i>Special</i></td><td>Array with the fields of the record</td></tr>
<tr><td>phoneLabel</td><td><i>Special</i></td><td>[Optional] Array with 5
references to phone labels</td></tr>
<tr><td>category</td><td>1</td><td>[Optional] 0 to 15.  Default is 0.</td></tr>
<tr><td>attributes</td><td><i>Special</i></td><td>Bitmask of attributes.
Default is 0 (no attributes are set).</td></tr>
</table>

<p>The fields array is an associative array in the following format.  No
values are required, except at least one thing should be specified.</p>

<table align=center cellpadding=4 cellspacing=0 border=1 bgcolor=#FFDFDF>
<tr>
<th>Key</th><th>Example</th><th>Description</th>
</tr>
<tr><td>lastName</td><td>Duck</td><td>The contact's last name</td></tr>
<tr><td>firstName</td><td>Daffy</td><td>The contact's first name</td></tr>
<tr><td>company</td><td>PHP-PDB Inc.</td><td>The name of the company</td></tr>
<tr><td>phone1</td><td>867-5309</td><td>What goes in the first
phone/email/etc field.</td></tr>
<tr><td>...</td><td>...</td><td>Same goes for phone2, phone3, phone4
...</td></tr>
<tr><td>phone5</td><td>dduck@toon.com</td><td>What goes in the last
phone/email/etc field.</td></tr>
<tr><td>address</td><td>Duck street 25</td><td>The address</td></tr>
<tr><td>city</td><td>Toon City</td><td>The city</td></tr>
<tr><td>state</td><td>FL</td><td>The state of the contact</td></tr>
<tr><td>zipCode</td><td>12345</td><td>The post office zip code for the contact</td></tr>
<tr><td>country</td><td>Toon Land</td><td>The contact's country</td></tr>
<tr><td>title</td><td>Sir</td><td>The title of the contact</td></tr>
<tr><td>custom1</td><td>Birth Date</td><td>Extra information</td></tr>
<tr><td>...</td><td>...</td><td>Same goes for custom2, custom3</td></tr>
<tr><td>custom4</td><td>Whatever</td><td>The last extra information field</td></tr>
<tr><td>note</td><td>Quack.</td><td>Notes for the contact</td></tr>
</table>

<p>The phoneLabel array in the class associates the five phoneX strings with
the proper labels.  There are eight possible strings that the phoneX can be
associated with.  Below is a table showing the default values for the eight
phone labels.  See the SetFieldLabels() and GetFieldLabels() for information
on how to change them.</p>

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
phoneLabel array is 'display', which is the index of the array to display.
The example will assume you want the email address displayed.  Lastly is a
'reserved' field, which contains unknown data.</p>

<p>So, the format of the phoneLabel array with the above assumptions in
place would look like this:</p>

<table align=center cellpadding=4 cellspacing=0 border=1 bgcolor=#FFDFDF>
<tr>
<th>Key</th><th>Example</th><th>Description</th>
</tr>
<tr><td>phone1</td><td>PDB_ADDR_LABEL_WORK</td><td>'phone1' and 'phone2' in
the fields array refer to work numbers</td></tr>
<tr><td>phone2</td><td>PDB_ADDR_LABEL_WORK</td><td>Note:  these are not set
to the string "PDB_ADDR_LABEL_WORK" -- they are set to the defined value
PDB_ADDR_LABEL_WORK</td></tr>
<tr><td>phone3</td><td>PDB_ADDR_LABEL_FAX</td><td>The third is set to the
fax number</td></tr>
<tr><td>phone4</td><td>PDB_ADDR_LABEL_EMAIL</td><td>The phone4 key in the
fields array refers to an email address.</td></tr>
<tr><td>phone5</td><td>PDB_ADDR_LABEL_MOBILE</td><td>The last data entry
is a cell phone number</td>
<tr><td>display</td><td>3</td><td>Array index 3 = phone4 = the email
address.  Display the chosen tidbit of information on the list
screen.</td></tr>
<tr><td>reserved</td><td><i>Special</i></td><td>I'd suggest not setting this
and just leave it as '' if you must set it.</td></tr>
</table>

<p>Lastly, the attributes are a bitmask.  You "bitwise and" (&) and "bitwise
or" (|) them together.  An interesting tidbit of information:  Even if the 
record is marked 'private', the data in the record is unencrypted.</p>

<?PHP

ShowExample('
// How to check the attributes
$record = $pdb->GetRecordRaw();
$attrib = $record[\'attributes\'];

// Show which attributes are there
if ($attrib & PDB_ADDR_ATTRIB_EXPUNGED) echo "Record is expunged.<br>\n";
if ($attrib & PDB_ADDR_ATTRIB_DELETED) echo "Record is deleted.<br>\n";
if ($attrib & PDB_ADDR_ATTRIB_DIRTY) echo "Record is dirty.<br>\n";
if ($attrib & PDB_ADDR_ATTRIB_PRIVATE) echo "Record is private.<br>\n";
if ($attrib & PDB_ADDR_ATTRIB_DEL_EXP) {
   // The archive bit is only set if the record is deleted or expunged.
   // Otherwise, the lower bits specify the category.
   if ($attrib & PDB_ADDR_ATTRIB_ARCHIVE) 
      echo "Record is marked to be archived.<br>\n";
}
');

?>

<h3>Example</h3>

<?PHP

ShowExample('
// How to write a database
$addr = new PalmAddress();
$categorias = array(\'VIP\',\'AAA\',\'Inicial\');
$addr->SetCategoryList($categorias);
$fields = array(\'name\' => \'Pascual\',
                \'firstName\' => \'Eduardo\',
                \'phone1\' => \'21221552\',
                \'phone2\' => \'58808912\',
                \'phone5\' => \'epascual@cie.com.mx\',
                \'address\' => \'Hda. la Florida 10A\',
                \'city\' => \'Izcalli\');
$record[\'fields\'] = $fields;
$addr->SetRecordRaw($record);
$addr->GoToRecord(\'+1\');
$fields = array(\'name\' => \'de tal\',
                \'firstName\' => \'fulanito\',
                \'address\' => \'Direccion\',
                \'phone1\' => \'21232425\',
                \'phone2\' => \'fulanito@dondesea.com\');
$phones = array(\'phone1\' => PDB_ADDR_LABEL_HOME,
                \'phone2\' => PDB_ADDR_LABEL_EMAIL,
                \'phone3\' => PDB_ADDR_LABEL_WORK,
                \'phone4\' => PDB_ADDR_LABEL_FAX,
                \'phone5\' => PDB_ADDR_LABEL_OTHER,
                \'display\' => 1,
                \'reserved\' => \'\');
$record[\'fields\'] = $fields;
$record[\'phoneLabel\'] = $phones;
$record[\'category\'] = 1;
$record[\'attributes\'] = PDB_ADDR_ATTRIB_PRIVATE;
$addr->SetRecordRaw($record);
$fp = fopen(\'./pdbs/addr.pdb\',\'wb\');
$addr->WriteToFile($fp);
fclose($fp);



// How to read a database
$addr = new PalmAddress();
$fp = fopen(\'./address.pdb\',\'r\');
$addr->ReadFile($fp);
fclose($fp);
echo "Name: $addr->Name<br>\n";
echo "Type: $addr->TypeID<br>";
echo "Creator: $addr->CreatorID<br>\n";
echo "Atttributes: $addr->Atributes<br>\n";
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
  echo "- Category: ".$record[\'category\']." = " .
    $addr->CategoryList[$record[\'category\']][\'Name\']. "<br>\n";
  echo "- Private: ". (($record[\'attributes\'] & PDB_ADDR_ATTRIB_PRIVATE)?
     "Yes" : "No") ."<br>\n";
  foreach ($record[\'phoneLabel\'] as $plk => $pl) {
    echo "-- $plk => $pl<br>\n";
  }
  echo " Fields:<br>\n";
  foreach($record[\'fields\'] as $reck => $rec) {
    echo "-- $reck => $rec<br>\n";
  }
}
echo "Field Labels<br>";
$labels = $addr->GetFieldLabels();
foreach($labels as $k => $v) {
  echo "$k = $v<br>";
}
');

StandardFooter();

?>
