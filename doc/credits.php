<?PHP
/* Documentation for PHP-PDB library
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */

include("./functions.inc");

StandardHeader('Credits and Thanks');

?>

<p>I did actually write all code from scratch, but credit should be given to
people who wrote other open-source programs that I learned from.  Also, this
list is to give thanks to people who have helped me out with PHP-PDB.  Lastly,
I also show appreciation to companies and anything else that helped me out
with writing this class.</p>

<ul>
<li>Arensburger, Andrew -- Thanks for making <a
   href="http://www.ooblick.com/software/coldsync/">P5-Palm</a>!  I used 
   several ideas when creating PHP-PDB, and a few comments really helped me
   out.</li>
<li>Dean, Wayne -- Helped me out with finding out why the Desktop 4.0 didn't
   like installing PHP-PDB generated .pdb files.  (Padding issue.)</li>
<li>Dittgen, Pierre -- His <a 
   href="http://mmmm.free.fr/palm/download/">palmlib</a> (aka <a 
   href="http://depot.free-system.com/index.php3?projectid=2&action=show"
   >ToPTIP</a>) was handy to have around when initially adding DOC support
   to compare output and make sure mine worked.</li>
<li><a href="http://www.handmark.com">Handmark</a> -- Provided detailed 
   database structure <a
   href="http://www.handmark.com/products/mobiledb/dbstructure.htm">information</a>
   for <a href="http://www.handmark.com/products/mobiledb/">MobileDB</a>.
   Cassidy Lackey answered many of my questions.  Thanks!</li>
<li><a href="http://www.palm.com">Palm Computing, Inc.</a> -- Without them,
   there would not be a PalmOS.  Also, they provided <a
   href="http://www.palmos.com/dev/tech/docs/">documentation<a> (especially
   detailing the <a
   href="http://www.palmos.com/dev/tech/docs/FileFormats/FileFormatsTOC.html">file
   formats</a>) that made my life significantly easier.</li>
<li><a href="http://www.pyrite.org/">Pyrite</a> -- They provided a great
   <a href="http://www.pyrite.org/doc_format.php">description</a> about the 
   compression format and stored bookmarks.</li>
</ul>

<?PHP

StandardFooter();

?>
