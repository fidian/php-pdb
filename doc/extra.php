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

<h3>What do you need for PHP-PDB</h3>

<ul>
<li>A server that runs PHP 4.0.1 or later.</li>
<li>It would be nice to have it fast and for it to have lots of memory.</li>
<li>Extra memory is good, since PHP doesn't manipulate 8-bit strings very
well.  Because of this, I use hex encoded data, doubling the size of the
.pdb file in memory when loaded.  So, if you want to work with a 500k DOC
file, you should have 1 meg of memory free or your server will do a lot of
swapping.</li>
</ul>

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

<p><b>Modifications:</b></p>

<ul>
<li>Add better <a
href="http://www.handmark.com/products/mobiledb/dbstructure.htm">MobileDB</a>
support.</li>
<li>Regex bookmark scanning for Twister!  (Idea from <a
href="http://gutenpalm.sourceforge.net/makeztxt.php">makeztxt</a>)</li>
<li>Instead of breaking on chapter, you could break on # of records or
approximate size.</li>
<Li>Would backwards size searching for compressing DOC records be faster?
Test it out.  DOC compression is too slow.</li>
<li>Alter todo and addrbook to use the attributes from php-pdb's main
class.</li>
<li>Add more category functions from the todo and addrbook into the php-pdb
main class.</li>
</ul>

<p><b>New Ideas:</b></p>

<ul>
<li>Add <a href="http://pilot-db.sourceforge.net/">Pilot-DB</a>
support</a></li>
<li>How about adding <a
href="http://gutenpalm.sourceforge.net/ztxt_format.php">zTXT</a> format
ebooks for GutenPalm?  It's quite similar to DOC.</li>
<li>Add memo support</li>
<li>Add PDAToolbox support (waiting for information about structure -- if
you have info, please mail me)</li>
<li>Wouldn't image support from <a
href="http://palmimage.sourceforge.net/">PalmImage</a> be grand?  Especially
if it merged with GD.</li>
<li>Take an in-depth look <a
href="http://zurk.sourceforge.net/zdoc.html">here</a> for PeanutPress 
files.</li>
</ul>

<?PHP

StandardFooter();

?>
