<?PHP

include("functions.inc");

StandardHeader('DOC Files');

?>

<p>Although the compressed form of DOC files and loading doc files is being
worked on, you can still create and write uncompressed doc files.</p>

<h3>Including into your program</h3>

<?PHP

ShowExample('
include \'php-pdb.inc\';
include \'php-pdb-doc.inc\';
');

?>

<h3>Creating a new database</h3>

<P>DOC files already have a specified type and creator ID.  The only thing
that is left for you to specify is the name of the document you wish to
create.</p>

<?PHP

ShowExample('
$DB = new PalmDoc("Name Of Document");
  // Typical usage
  
$pdb = new PalmDoc();
  // Special:  If you want to create an instance of the class
  // and then use ReadFile() to load the database information
  // Note:  ReadFile() is not supported yet!
');

?>

<h3>Writing the database</h3>

<p>This is the same as the base class.  See <a href="example.php">Basic
Use</a> for more information.</p>

<h3>Loading the database</h3>

<p>Not yet supported.</p>

<h3>Adding Bookmarks</h3>

<p>In DOC files, there are two ways to make bookmarks -- stored and
embedded.  Stored bookmarks are where additional records are added to the
output file.  These are nicer to use, and most DOC readers support them.
Embedded bookmarks are embedded in the text and require the DOC reader to
scan the entire DOC file the first time you read it.  They use a unique
character at the beginning of a line to mark that line as a bookmark.  Then,
to signify which character is the "bookmark" character, you include it at
the end of the DOC file in angle brackets.</p>

<p>The below sample will create a file with both types of bookmarks.  The
first bookmark will be a stored bookmark, the second will use both types,
and the third will be just an embedded bookmark.  Please note that the
maximum length for a stored bookmark name is 15 character.  The maximum
length for embedded bookmarks could vary.</p>

<?PHP

ShowExample('
// This copies a string 16 times.  It puts newlines between the
// strings and adds another newline at the end for spacing.
function RepeatString($str) {
   $str .= "\n" . $str . "\n" . $str . "\n" . $str . "\n";
   return $str . $str . $str . $str . "\n";
}

$pdb = new PalmDoc("Bookmark Test");

// Add some text on top
$pdb->AddDocText(RepeatString("Top of Document"));

// Add a stored bookmark
$pdb->AddBookmark("Just Stored");

// Add some text to make it stand apart from the next bookmark
$pdb->AddDocText(RepeatString("Stored Bookmark"));

// Add a stored bookmark
$pdb->AddBookmark("Both - Stored");

// Add the embedded bookmark.  I am using the * character
// as my bookmark character.  Note:  You just add * at the
// beginning of the line and the rest of the line becomes
// your bookmark.
$pdb->AddDocText("* Both - Embedded\n");

// Add some text to make it stand apart from the next bookmark
$pdb->AddDocText(RepeatString("Both types of bookmarks"));

// Add just an embedded bookmark.
$pdb->AddDocText("* Just Embedded\n");

// Add some text to make it stand apart from the next bookmark
$pdb->AddDocText(RepeatString("Embedded bookmark"));

// This is how you specify the special character that becomes
// your bookmark character.  RepeatString() adds two newlines,
// so this is on its own line
$pdb->AddDocText("<*>");

$pdb->DownloadPDB("bookmark_test.pdb");
');

?>

<p>The example will create a test DOC (also availale <a
href="samples/bookmark_test.pdb">here</a>) that has both kinds of bookmarks.
Below is a table showing various doc readers and what type of bookmarks they
support.  If you wish to add to this file, just mail me (link at bottom of
page) or the php-pdb-general mailing list.</p>

<table bgcolor="DFDFFF" align=center cellpadding=3 cellspacing=0 border=1>
<tr><th>Program and Version</th><th>Stored</th><th>Embedded</th>
<th>Notes</th></tr>

<tr><td><b><a href="http://32768.com/cspotrun/">CSpotRun</a>, 1.1</b></td>
<td align=center><font color="green"><B>Yes</B></font></td>
<td align=center><font color="red"><B>No</B></font></td>
<td><font size="-1">Freeware, open source</font></td></tr>

</table>

<h3>Other functions</h3>

<dl>

<dt><b>AddBookmark($name, $position = false)</b></dt>
<dd>If $position is specified, adds a bookmark at $position bytes into the
file.</dd>
<dd>If $position is not specified, adds a bookmark at the current position.
I strongly recommend using this form.</dd>
<dd>$name is limited to 15 characters and will automatically be trimmed if
it is too long.</dd>

<dt><b>AddDocText($string)</b></dt>
<dd>Adds the specified text to the end of the doc file.  To get newlines,
use "\n".  This function can add single words, entire lines, paragraphs, or
all of the text at once.  There is no size nor line limit.</dd>

</dl>

<h3>Example</h3>

<?PHP

ShowExample('
$pdb = new PalmDoc("Doc Test");
$pdb->AddDocText("This is a test.
This is a test of the PHP-PDB DOC class.\\n");
$pdb->AddDocText("This is only a test.");
$pdb->DownloadPDB("doc_test.pdb");
');


StandardFooter();

?>
