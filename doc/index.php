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
manipulate <a href="http://www.palmos.com">PalmOS</a> databases.  It lets
you read, write, alter, and easily use data that is meant to be sent to or 
retrieved from a handheld.  It requires PHP 4.0.1 or later.</p>

<P>For downloads, CVS access, bug reporting, patch submitting, requesting
features, and tons of other information, please see our
<a href="http://sourceforge.net/projects/php-pdb/">SourceForge Project
Page</a>.</p>

<h3>Directly supported databases</h3>

<p>These types of databases have had separate classes written to extend the
base class and make life easier for the developer.

<ul>
<li><b>Datebook</b> - Reading and writing fully supported.</li>
<li><b>DOC</b> - Can write uncompressed doc files but can't yet read or do
compression. Can use embedded and stored bookmarks.</li>
<li><B>MobileDB</b> - Can read and write databases.  No utility functions
yet, so using this class is extremely difficult.</li>
</ul>

<h3>Features</h3>

<ul>
<li>The base class can be extended to provide easy support for specific
types of databases.</li>
<li>The class supports reading databases as well as writing databases.</li>
<li>Easily be able to pipe a PalmOS database to the browser or out to a
file.</li>
<li>Limited category support.</li>
<li>Very limited appinfo and sortinfo support.</li>
</ul>

<?PHP

StandardFooter();

?>
