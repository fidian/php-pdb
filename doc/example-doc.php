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

<h3>Other functions</h3>

<dl>

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
