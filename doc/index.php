<?PHP
/* Documentation for PHP-PDB library
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */

include("./functions.inc");

StandardHeader('Main');

?>

<P>PHP-PDB is a set of <a href="http://www.php.net">PHP</a> classes that 
manipulate <a href="http://www.palmos.com">Palm OS</a> databases.  It lets
you read, write, alter, and easily use data that is meant to be sent to or 
retrieved from a handheld.  It requires PHP 4.0.1 or later, but due to
security problems with PHP 4, I'd suggest you have version 4.1.2 or patch your
current version..</p>

<h3>Directly supported databases</h3>

<p>These types of databases have had separate classes written to extend the
base class and make life easier for the developer.  PHP-PDB can be used with
your own <a href="custom.php">custom</a> database type as well.</p>

<ul>
<li><b><a href="modules/addrbook.php">Address Book</a></b> - Reading and
writing fully supported.</li>
<li><b><a href="modules/datebook.php">Datebook</a></b> - Reading and writing 
fully supported.</li>
<li><b><a href="modules/doc.php">DOC</a></b> - Can read and write 
compressed and uncompressed doc files.  Can use embedded and stored 
bookmarks.</li>
<li><B><a href="modules/mobiledb.php">MobileDB</a></b> - Can read and write
databases.  No utility functions yet, so this class is difficult to use.</li>
<li><B><a href="modules/smallbasic.php">SmallBASIC</a></b> - Can read and 
write databases.  Easily convert from/to this format.</li>
</ul>

<h3>Features</h3>

<ul>
<li>The base class can be extended to provide easy support for specific
types of databases.</li>
<li>The class supports reading databases as well as writing databases.</li>
<li>Easily be able to pipe a Palm OS database to the browser or out to a
file.</li>
<li>Category support, but it is currently somewhat limited.</li>
<li>Limited appinfo and sortinfo support exists.</li>
</ul>

<h3>As Seen In ...</h3>

<p>This PHP class works quite well and it is being used in the following
manners.  If you want your site listed here, I'd love to add it -- tell me
about it!</p>

<ul>
<li><a href="http://www.ampoliros.com/">Ampoliros</a> - Used as a package
for a web-based system that will be used by other various modules to
import/export data.</li>
<li><a href="http://rumkin.com/projects/marco/">Marco</a> - Data point
conversion from different formats into the internal format used by Marco.</li>
should eventually be released from this site.</li>
<li><a href="http://www.shiblon.com/trivia/">LDS Trivia</a> - Real-time trivia
database creation from questions submitted on the web site.</li>	
<li><a href="samples/twister.php">Twister!</a> - Web page, text, and <a
href="http://promo.net/pg/">Project Gutenberg</a> file conversion to DOC or
SmallBASIC format.</li>
	
<!-- Removed because the patch author did not get back to me.
<li><a href="http://www.moregroupware.com/">Moregroupware</a> - A custom
"export to palm datebook" patch was written.  It is being improved and
-->
<!--  Is this still there?
<li><a href="http://palmvoc.sourceforge.net/">palmvoc</a> - Free vocabulary
trainer for Palm OS.</li>
-->
</ul>

<?PHP

StandardFooter();

?>
