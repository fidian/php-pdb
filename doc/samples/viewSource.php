<?PHP
/* Examples for PHP-PDB library - View source program
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 *
 * Although not truly a file that helps illustrate how to use PHP-PDB, this
 * lets people on the web see source code that uses PHP-PDB and lets the
 * browser run the file to produce the sample output.
 */

include '../functions.inc';

StandardHeader('View Source of ' . $file, 'samples');

if (strpos($file, '/') !== false ||
    strpos($file, '..') !== false ||
    preg_match('/[^-_a-zA-Z\\.0-9]/', $file) ||
    ! file_exists($file)) {
    
?>
<P>Sorry, but <?PHP $file ?> does not exist or is not allowed.</p>
<?PHP

} else {

?>
<P>You can run the example here:  <a href="<?PHP echo $file ?>"><?PHP 
echo $file ?></a></p>
<?PHP
ShowExampleFile($file);

}

StandardFooter();
