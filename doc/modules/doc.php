<?PHP
/* Documentation for PHP-PDB library -- DOC module
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */

include("../functions.inc");

StandardHeader('DOC Files', 'modules');

$BookmarkTest = array(
   'CSpotRun' => array(
      'Version' => 1.1,
      'URL' => 'http://32768.com/cspotrun/',
      'Stored' => true,
      'Embedded' => false,
      'Notes' => 'Freeware, open source' ),
   'MiniWrite' => array(
      'Version' => 1.4,
      'URL' => 'http://www.solutionsinhand.com/mw/miniwrite.htm',
      'Stored' => true,
      'Embedded' => false,
      'Notes' => 'DOC reader/writer')
);
?>

<p>Although the compressed form of DOC files and loading DOC files are both
being worked on, you can still create and write uncompressed doc files quite
easily.</p>

<h3>Including into your program</h3>

<?PHP

ShowExample('
include \'php-pdb.inc\';
include \'modules/doc.inc\';
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

<p>This is the same as the base class.  See <a href="../example.php">Basic
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

<p><a href="../samples/viewSource.php?file=bookmark_test.php">This sample</a>
will create a file with both types of bookmarks.  The
first bookmark will be a stored bookmark, the second will use both types,
and the third will be just an embedded bookmark.  Please note that the
maximum length for a stored bookmark name is 15 characters.  The maximum
length for embedded bookmarks could vary.</p>

<p>The example will create a test DOC (also available <a
href="../samples/bookmark_test.php">here</a>) that has both kinds of bookmarks.
Below is a table showing various doc readers and what type of bookmarks they
support.  Please help me expand this list so that the capabilities of more
DOC readers can be known.  Just mail me (link at bottom of
page) or the php-pdb-general 
<a href="http://sourceforge.net/mail/?group_id=29740">mailing list</a>.</p>

<table bgcolor="DFDFFF" align=center cellpadding=3 cellspacing=0 border=1>
<tr bgcolor="#CFCFFF"><th>Program and Version</th>
<th>Stored</th>
<th>Embedded</th>
<th>Notes</th></tr>

<?PHP

   $Total = 0;
   $Stored = 0;
   $Embedded = 0;
   
   ksort($BookmarkTest);
   foreach ($BookmarkTest as $Name => $Info) {
      $Total ++;
      if ($Info['Stored']) {
         $StoredColor = 'green';
	 $StoredString = 'Yes';
	 $Stored ++;
      } else {
         $StoredColor = 'red';
	 $StoredString = 'No';
      }
      if ($Info['Embedded']) {
         $EmbeddedColor = 'green';
	 $EmbeddedString = 'Yes';
	 $Embedded ++;
      } else {
         $EmbeddedColor = 'red';
	 $EmbeddedString = 'No';
      }
      
?><tr><td><b><a href="<?PHP echo $Info['URL'] ?>"><?PHP echo $Name
?></a>, <?PHP echo $Info['Version'] ?></b></td>
<td align=center><font color="<?PHP echo $StoredColor ?>"><B><?PHP
echo $StoredString ?></B></font></td>
<td align=center><font color="<?PHP echo $EmbeddedColor ?>"><B><?PHP
echo $EmbeddedString ?></B></font></td>
<td><font size="-1"><?PHP echo $Info['Notes'] ?></font></td></tr>
<?PHP

   }
   
   $StoredPercent = $Stored / $Total;
   $StoredPercent *= 100;
   settype($StoredPercent, 'integer');
   
   $EmbeddedPercent = $Embedded / $Total;
   $EmbeddedPercent *= 100;
   settype($EmbeddedPercent, 'integer');
   
?><tr bgcolor="#CFCFFF"><td>Total:</td>
<td align=center><?PHP echo $Stored . ' (' . $StoredPercent ?>%)</td>
<td align=center><?PHP echo $Embedded . ' (' . $EmbeddedPercent ?>%)</td>
<td>&nbsp;</td></tr>

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

<dt><b>EraseDocText()</b></dt>
<dd>Erases all text from the document being generated.</dd>

<dt><b>GetDocText()</b></dt>
<dd>Returns a single string (potentially very large) that contains the entire
document's text.</dd>

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
