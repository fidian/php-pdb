<?PHP
/* Documentation for PHP-PDB library
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */

include("./functions.inc");

StandardHeader('Extra Information');

if (file_exists('../pdb-test.php'))
   $PDB_TEST = '../pdb-test.php';
else
   $PDB_TEST = 'samples/pdb-test.php';
?>

<h3>Making sure PHP-PDB works</h3>

<p>After you installed PHP-PDB somewhere, just use your web browser and view
<tt>pdb-test.php</tt>.  It will perform a few tests and
tell you the results of the tests.  They should all say 
<font color=green>pass</font>.  If they don't, contact me for help.
You can see <a href="<?PHP echo $PDB_TEST ?>">how the test looks</a> for
this installation, but you will want to also test it on your machine.</p>

<h3>Contact information</h3>

<p>There are <a href="http://sourceforge.net/mail/?group_id=29740">mailing
lists</a> set up so that you can get announcements of new versions and
participate in a general discussion about PHP-PDB.  If you are asking
for help, please subscribe to the general mailing list and ask your
question.  Your answer should come to you pretty fast.  If you abhore
mailing lists and would greatly prefer to not subscribe to one, just email
me directly.  I'll simply forward your request to the mailing list and then
ask that the list members reply to you and to the list.  My email address 
is at the bottom of the page.</p>

<h3>Legalese</h3>

<P>PHP-PDB is released under the <a
href="http://www.gnu.org/copyleft/lesser.html">GNU LGPL</a> and is basically
free to use for everyone.  Please see the LGPL for specific details.</p>

<h3>Still to do</h3>

<ul>
<li>Add better <a
href="http://www.handmark.com/products/mobiledb/dbstructure.htm">MobileDB</a>
support.</li>
<li>Add <a href="http://pilot-db.sourceforge.net/">Pilot-DB</a>
support</a></li>
<li>Add todo support</li>
<li>Add compression, decompression, and loading to DOC files.
[ <a
href="http://www.linuxmafia.com/pub/palmos/development/pilotdoc-compression.html">AportisDoc</a>
<a href="http://www.pyrite.org/doc_format.php">Pyrite</a> ]
</li>
<li>Add phonebook support</li>
<li>Add memo support</li>
<li>Add PDAToolbox support (waiting for information about structure -- if
you have info, please mail me)</li>
<li>Maybe look closer at <a href="http://mmmm.free.fr/palm/">palmlib</a></li>
<li>Wouldn't image support from <a
href="http://palmimage.sourceforge.net/">PalmImage</a> be grand?  Especially
if it merged with GD.</li>
<li>Take an in-depth look <a
href="http://zurk.sourceforge.net/zdoc.html">here</a> for DOC files and for
PeanutPress files.</li>
<li>How about adding <a
href="http://gutenpalm.sourceforge.net/ztxt_format.php">zTXT</a> format
ebooks for GutenPalm?  It's quite similar to DOC.</li>
<li>Regex bookmark scanning for Twister!  (Idea from <a
href="http://gutenpalm.sourceforge.net/makeztxt.php">makeztxt</a>)</li>
<li>Finish adding <a
href="http://smallbasic.sourceforge.net/">SmallBASIC</a> module to convert
code to/from PDB format.</li>
</ul>

<?PHP

StandardFooter();

?>
