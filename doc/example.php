<?PHP

include("./functions.inc");

StandardHeader('Basic Use');

?>

<P>PHP-PDB can create nearly any type of <a
href="http://www.palmos.com/">PalmOS</a> database through the most basic
functions, as illustrated here.

<h3>Including into your program</h3>

<?PHP

ShowExample('
include \'php-pdb.inc\';
');

?>

<h3>Creating a new database</h3>

<P>To create a new database, you need to find out the correct type and
creator ID to make the database with.  You should also specify the name of
the database.</p>

<?PHP

ShowExample('
$DB = new PalmDB("Type", "Creator", "Name Of Database");
  // Typical usage
  
$pdb = new PalmDB();
  // Special:  If you want to create an instance of the class
  // and then use ReadFile() to load the database information
');

?>

<h3>Writing the database</h3>

<p><b>Writing to a file:</b>  It might be important to open the file with
<tt>wb</tt> as the mode.  Unix, Linux, and others don't care about the
'<tt>b</tt>', but all versions of Windows do care.</p>

<?PHP

ShowExample('
// Writing to a file
$fp = fopen("output_file.pdb", "wb");
if (! $fp) exit;  // There was an error opening the file
$pdb->WriteToFile($fp);
fclose($fp);
');

?>

<p><b>Writing to stdout:</b>  If you want the file to just be piped to the
standard output, as though you were just using <tt>echo</tt> to dump the
file, it is a bit simpler.</p>

<?PHP

ShowExample('
// Writing to standard output
$pdb->WriteToStdout();
');

?>

<p><b>Download through browser:</b>  If your PHP script just generates a
.pdb file to be downloaded through the browser, you can just call this
function and it will generate the correct file download headers and send the
file to the browser.  Only call this function if you didn't already send any
content.  There are a few known issues with IE and file downloads, so the
headers that are generated should work with the newest versions of IE and
might not work with older ones.  It's not my fault -- see Microsoft
technical support.</p>

<?PHP

ShowExample('
// Download through browser
// Specify filename you want the .pdb saved as on the remote computer
$pdb->DownloadPDB("filename.pdb");
');

?>

<h3>Loading the database</h3>

<p>This function reads data from the opened file and tries to load it
properly.  Returns false if there is no error.</p>

<?PHP

ShowExample('
// Create a dummy instance of the class
$pdb = new PalmDB("test", "test");
// Load a file
$fp = fopen("your_file.pdb", "r");
$pdb->ReadFile($fp);
fclose($fp);
');

?>

<h3>Other functions</h3>

<dl>

<dt><b>GoToRecord($num = false)</b></dt>
<dd>If $num is not specified, returns the current record number.</dd>
<dd>If $num is specified and is a string with '+' or '-' as its first
character, jumps ahead or back the specified number of records.  Returns the
old record number.</dd>
<dd>If $num is a number, jumps directly to the specified record.  Returns
the old record number.</dd>

<dt><b>GetRecordSize($num = false)</b></dt>
<dd>If $num is not specified, returns the size of the current record.</dd>
<dd>If $num is specified, returns the size of the specified record.</dd>

<dt><b>AppendInt8($value)</b></dt>
<dd>Appends a single byte to the current record.</dd>

<dt><b>AppendInt16($value)</b></dt>
<dd>Appends an integer (two bytes) to the current record.</dd>

<dt><b>AppendInt32($value)</b></dt>
<dd>Appends a long (four bytes) to the current record.</dd>

<dt><b>AppendDouble($value)</b></dt>
<dd>Appends a double (eight bytes, floating point) to the current record.</dd>

<dt><b>AppendString($string, $maxLen = false)</b></dt>
<dd>Appends a string to the current record.  The string is not null
terminated.  If $maxLen is specified and is greater than zero, the string is
trimmed and will contain up to $maxLen characters.</dd>

<dt><b>RecordExists($record = false)</b></dt>
<dd>If $record is not specified, returns true if the current record exists
and is set.</dd>
<dd>If $record is specified, returns true if the specified record exists and
is set.</dd>

<dt><b>GetRecord($record = false)</b></dt>
<dd>Returns the data for the record, or the current record if
$record is not specified.  If the desired record doesn't contain data,
returns ''.  GetRecordRaw() might be more useful, since this may be
overridden to return compressed and/or encoded data.</dd>

<dt><b>GetRecordRaw($record = false)</b></dt>
<dd>If $record is not specified, returns the raw data of the current record,
or false if the current record doesn't yet exist.</dd>
<dd>If $record is specified, returns the raw data of the record, or false if
the record doesn't exist.</dd>

<dt><b>SetRecordRaw($data)</b></dt>
<dd>Sets the raw data for the current record to be $data.

<dt><b>SetRecordRaw($record, $data)</b></dt>
<dd>Sets the raw data for $record to be $data.</dd>

<dt><b>DeleteCurrentRecord()</b></dt>
<dd>Erases and unsets the current record</dd>

<dt><b>GetRecordIDs()</b></dt>
<dd>Returns a list of the set record IDs in the order that they should be
written.</dd>

<dt><b>GetRecordCount()</b></dt>
<dd>Returns the number of records.  This should match the number of keys
returned by GetRecordIDs().</dd>

</dl>

<h3>More information</h3>

<p>The source code is pretty clean with lots of comments strewn about.
There are other functions that are not detailed here and are designed to be
overridden by classes that extend the base class.  Read the source to get to
know them.</p>

<?PHP

StandardFooter();

?>
