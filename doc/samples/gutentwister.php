<?PHP
/* Examples for PHP-PDB library - Project Gutenberg text to DOC converter
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */


if (file_exists('../../php-pdb.inc')) {
   include('../../php-pdb.inc');
   include('../../php-pdb-doc.php');
} elseif (file_exists('./php-pdb.inc')) {
   include('./php-pdb.inc');
   include('./php-pdb-doc.php');
}


session_start();

if ($action == 'process') {
   ProcessFile();
} elseif ($action == 'download') {
   DownloadFile();
} else {
   ShowBeginning();
}

exit();


function ShowBeginning() {
   global $SessionOK;
   
   $SessionOK = true;
   session_register('SessionOK');
}


function ProcessFile() {
   SessionOKCheck();
}


function DownloadFile() {
   SessionOKCheck();
}


function SessionOKCheck() {
   if (! session_is_registered('SessionOK')) {
      $Error = <<< EOS
Session information could not be found.
Please make sure that you have cookies enabled in your browser.
EOS;
      ErrorOut($Error);
   }
}


function StartHTML($Title) {
?><html><head><title><?PHP echo $Title ?></title></head>
<body bgcolor=#FFFFFF>
<?PHP
}


function EndHTML() {
?><hr>
<font size="-2">GutenTwister is part of the <a
href="http://php-pdb.sourceforge.net">PHP-PDB</a> library and is free to use
by anyone.</font></body></html><?PHP
}


function ErrorOut($Message) {
   StartHTML("Error!");
?><table align=center border=2 cellpadding=5 cellspacing=0 bgcolor="#FFDFDF">
<tr><th>ERROR!</th></tr>
<tr><td><?PHP echo $Message ?></td></tr>
</table>
?>
   EndHTML();
   exit();
}
