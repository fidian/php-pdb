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
   'AportisDoc' => array(
      'Version' => '2.2.3',
      'URL' => 'http://aportis.com/',
      'Stored' => true,
      'Embedded' => true,
      'Notes' => 'The one that started it all'),
   'CSpotRun' => array(
      'Version' => 1.1,
      'URL' => 'http://32768.com/cspotrun/',
      'Stored' => true,
      'Embedded' => false,
      'Notes' => 'Freeware, open source'),
   'iSilo' => array(
      'Version' => 3.05,
      'URL' => 'http://isilo.com/',
      'Stored' => true,
      'Embedded' => false,
      'Notes' => 'Free version of iSilo.'),
   'iSilo Free' => array(
      'Version' => 1.5,
      'URL' => 'http://isilo.com/old/index.htm',
      'Stored' => false,
      'Embedded' => false,
      'Notes' => 'Free version of iSilo.'),
   'MiniWrite' => array(
      'Version' => 1.4,
      'URL' => 'http://www.solutionsinhand.com/mw/miniwrite.htm',
      'Stored' => true,
      'Embedded' => false,
      'Notes' => 'DOC reader/writer'),
   'QED' => array(
      'Version' => 2.62,
      'URL' => 'http://qland.de/qed/',
      'Stored' => true,
      'Embedded' => true,
      'Notes' => 'DOC reader/writer'),
   'ReadThemAll' => array(
      'Version' => 1.65,
      'URL' => 'http://palmgear.com/software/showsoftware.cfm?prodID=14149',
      'Stored' => false,
      'Embedded' => false,
      'Notes' => ''),
   'RichReader' => array(
      'Version' => '1.62 Freeware',
      'URL' => 'http://users.rcn.com/arenamk/',
      'Stored' => true,
      'Embedded' => false,
      'Notes' => 'Free version of RichReader'),
   'Smoothy' => array(
      'Version' => '1.5.0',
      'URL' => 'http://www.handwave.com/handwavesmoothybenefits.html',
      'Stored' => true,
      'Embedded' => false,
      'Notes' => ''),
   'SuperDoc' => array(
      'Version' => 1.4,
      'URL' => 'http://www.codemill.net/products/superdoc/index.html',
      'Stored' => true,
      'Embedded' => true,
      'Notes' => ''),
   'TealDoc' => array(
      'Version' => '4.51D',
      'URL' => 'http://www.tealpoint.com/softdoc.htm',
      'Stored' => true,
      'Embedded' => true,
      'Notes' => '')
);
?>

<p>DOC files is the standard way of storing text files on a handheld.  You
will need a DOC reader in order to see any DOC file you have loaded onto
your device.  There are several ones listed <a 
href="#DocReaders">below</a>.  To create DOC files, there are many programs
already out there that will work for you.  Additionally, you can use
<a href="../samples/twister.php">Twister</a>, which just uses PHP and this
PHP-PDB library to create DOC files from plain text files, web pages, and
files from <a href="http://promo.net/pg/">Project Gutenberg</a>.</p>

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
  // Typical usage to create a compressed DOC file

$DB = new PalmDoc("Uncompressed DOC", false);
  // This is how you create an uncompressed DOC file
  
$pdb = new PalmDoc();
  // Special:  If you want to create an instance of the class
  // and then use ReadFile() to load the database information
');

?>

<h3>Writing the database</h3>

<p>This is the same as the base class.  See <a href="../example.php">Basic
Use</a> for more information.</p>

<p>If the $pdb was set to be a compressed file, the contents will be
transparently compressed.  Also, you can further manipulate the text
normally after writing -- it will just be recompressed every time you write 
the database if the contents were changed.</p>

<h3>Loading the database</h3>

<p>This works just like loading files with the base class.  Please see <a
href="../example.php">Basic Use</a> for further information.</p>

<h3>Category Support and Record Attributes</h3>

<p>DOC files do not support categories nor record attributes.</p>

<h3>Adding Bookmarks</h3>

<p>In DOC files, there are two ways to make bookmarks -- stored and
embedded.  Stored bookmarks are where additional records are added to the
output file.  These are nicer to use, and most DOC readers support them.
Embedded bookmarks are embedded in the text and require the DOC reader to
scan the entire DOC file the first time you read it.  They use a unique
character at the beginning of a line to mark that line as a bookmark.  Then,
to signify which character is the "bookmark" character, you include it at
the end of the DOC file in angle brackets.</p>

<p>Make sure to just pick one type of bookmarks to use!  In the <a
href="../samples/bookmark_test.php">tests</a> that I have performed, it
appears that if the bookmark reader can handle both types of bookmarks, it
will only search for embedded bookmarks if there are no stored bookmarks.</p>

<p>If you decide to use embedded bookmarks, be very careful of the bookmark
character that you pick, because some doc readers don't check to see if that
character is at the beginning of a line before blindly adding it to the
bookmark list.  If you pick a character like an apostrophe or a period, then
you are potentially in for a huge surprise.</p>

<p>Stored bookmark names are limited to 15 characters.  The maximum length
for embedded bookmark names could vary.</p>

<h4><a name="DocReaders">Doc Readers and Results of Bookmark Test</a></h4>

<p>This list is not a comprehensive list of DOC readers.  If you know of a
DOC reader that is not yet on this list, just email me the program name and
URL, and I'll test it to see what kinds of bookmarks are supported.  (You
can see my test <a href="../samples/bookmark_test.php">here</a>.)</p>

<table bgcolor="DFDFFF" align=center cellpadding=3 cellspacing=0 border=1>
<tr bgcolor="#CFCFFF"><th>Program and Version</th>
<th>Stored</th>
<th>Embedded</th>
<th>Notes</th></tr>

<?PHP

   $Total = 0;
   $Stored = 0;
   $Embedded = 0;
   
   $keys = array_keys($BookmarkTest);
   natcasesort($keys);
   foreach ($keys as $Name) {
      $Info = $BookmarkTest[$Name];
      
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
<td><font size="-1"><?PHP if ($Info['Notes'] != '') 
   echo $Info['Notes'];
else
   echo '&nbsp;'; ?></font></td></tr>
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
file.  This is the uncompressed count of bytes, not the number of bytes of
compressed text.</dd>
<dd>If $position is not specified, adds a bookmark at the current position.
I strongly recommend using this form.</dd>
<dd>$name is limited to 15 characters and will automatically be trimmed if
it is too long.</dd>

<dt><b>AddDocText($string)</b></dt>
<dd>Adds the specified text to the end of the doc file.  To get newlines,
use "\n".  This function can add single words, entire lines, paragraphs, or
all of the text at once.  There is no size nor line limit.  It would be wise
to have less than a few hundred kilobytes of total text, but that's at your
discretion.</dd>

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
$pdb->AddDocText("This is only a test.\\n");
$pdb->AddDocText("This DOC will be automatically compressed.");
$pdb->DownloadPDB("doc_test.pdb");
');

?>

<p>Another example is the <a
href="../samples/bookmark_test.php">Bookmark Test</a> that I use in order to
find out what bookmark types are supported with each DOC reader I have 
listed.</p>

<?PHP

StandardFooter();

?>
