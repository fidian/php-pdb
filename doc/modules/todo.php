<?PHP
/* Documentation for PHP-PDB library -- Todo module
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */

include("../functions.inc");

StandardHeader('ToDo List', 'modules');

?>

<p>The todo module is mostly straightforward, and should be fairly easy to
use.</p>

<?PHP ShowOverwriteWarning() ?>

<h3>Including into your program</h3>

<?PHP

ShowExample('
include \'php-pdb.inc\';
include \'modules/todo.inc\';
');

?>

<h3>Creating a new database</h3>

<P>Since a todo list has a specified type, creator, and name, the class takes
care of knowing what they are.  Creation of a new database is a snap.

<?PHP

ShowExample('
$DB = new PalmTodoList();
');

?>

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
<dd>Returns an array with some default data for a new Todo record.  Does
not actually add the record.  Use SetRecordRaw() for that.</dd>

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
<tr><td>Description</td><td>This is my todo</td><td>Short description of
thing to do</td></tr>
<tr><td>Note</td><td>Extended information</td><td>[Optional] A note attached to the
record</td></tr>
<tr><td>DueDate</td><td>2001-01-23</td><td>[Optional] Year, month, and day of when the
item should be done</td></tr>
<tr><td>Priority</td><td>1</td><td>[Optional]Priority of the event ([1-5])</td></tr>
<tr><td>Completed</td><td>false</td><td>[Optional] True/false indicating
whether the action was completed or not.</td></tr>
</table>

<p>If description is not specified, then the string 'No description' will be
used instead.</p>

<h3>Example</h3>

<?PHP

ShowExample('
// Write Example
$todo = new PalmTodoList();
$categories = array(1 => \'Visita\', \'Fax\', \'Correo\');
$todo->SetCategoryList($categories);
$record = array(\'Description\' => \'Enviar Fax\',
                \'Note\' => "25\nProbar palm",
                \'Priority\' => 2);
$todo->SetRecordRaw($record);
$todo->SetRecordAttrib(2); // Category 2

$todo->GoToRecord(\'+1\');
$record = array(\'Description\' => \'Llamar a juan\',
                \'Note\' => \'35\',
		\'Completed\' => true,
                \'DueDate\' => \'2002-5-31\');
$todo->SetRecordRaw($record);
$todo->SetRecordAttrib(PDB_RECORD_ATTRIB_DIRTY |
                       PDB_RECORD_ATTRIB_PRIVATE | 0);
                       // Category 0, dirty, private

$fp = fopen(\'./pdbs/todo.pdb\',\'wb\');
$todo->WriteToFile($fp);
fclose($fp);
');

ShowExample('
// Read Example
$pdb = new PalmTodoList();
$fp = fopen(\'./tdread.pdb\', \'r\');
$pdb->ReadFile($fp);
fclose($fp);

echo "Name: $pdb->Name<BR>\n";
echo "Type ID: $pdb->TypeID<br>\n";
echo "Creator: $pdb->CreatorID<br>\n";
echo "Attributes: $pdb->Attributes<br>\n";
echo "Version: $pdb->Version<br>\n";
echo "ModNum: $pdb->ModNumber<br>\n";
echo "CreationTime: $pdb->CreationTime<br>\n";
echo "ModTime: $pdb->ModificationTime<br>\n";
echo "BackTime: $pdb->BackupTime<br>\n";
echo \'NumRec: \'.$pdb->GetRecordCount()."<br>\n";
$recids = $pdb->GetRecordIDs();
$record1 = $pdb->GetRecordRaw($recids[0]);
$attrib = $pdb->GetRecordAttrib($recids[0]);
echo "record1 = $record1<br>\n";
echo "Desc: " . $record1[\'Description\'] . "<br>\n";
echo "Note: " . $record1[\'Note\'] . "<br>\n";
echo \'Due Date: \' . $record1[\'DueDate\'] . "<br>\n";
echo \'Cat: \' . $record1[\'Category\'] . "<br>\n";
$categories = $pdb->GetCategoryList();
echo "NumCat = " . count($categories) . "<br>\n";
foreach ($categories as $k => $v) {
  echo "categories[$k] = $v<br>\n";
  foreach ($categories[$k] as $key => $val) {
    echo "  categories[$k][$key] = $val<br>";
  }
}
');

StandardFooter();

?>