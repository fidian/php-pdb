<?PHP
/* Documentation for PHP-PDB library
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */

function StandardHeader($title = '', $curdir = '') {
   $newTitle = "PHP-PDB";
   
   if ($title != '')
      $newTitle .= ': ' . $title;
    
   if ($curdir == 'modules') {
      $modulePath = '';
      $rootPath = '../';
      $samplePath = '../samples/';
   } elseif ($curdir == 'samples') {
      $modulePath = '../modules/';
      $rootPath = '../';
      $samplePath = '';
   } else {
      $modulePath = 'modules/';
      $rootPath = '';
      $samplePath = 'samples/';
   }
   
   $TH_Color = '#AFAFDF';

?><HTML><HEAD><TITLE><?PHP echo $newTitle ?></TITLE></HEAD>
<body bgcolor=#FFFFFF text=#000000>
<h1 align=center><?PHP echo $newTitle ?></h1>
  <table border=0 cellpadding=5 cellspacing=0 bgcolor="#BFBFEF"
align=right>
    <tr><td><a href="<?PHP echo $rootPath 
      ?>index.php">Main&nbsp;Page</a></td></tr>
    <tr><th bgcolor="<?PHP echo $TH_Color ?>">General</th></tr>
    <tr><td><a href="<?PHP echo $rootPath 
      ?>example.php">Basic&nbsp;Use</a><br>
      <a href="<?PHP echo $rootPath 
      ?>extra.php">Extra&nbsp;Info</a></td></tr>
    <tr><th bgcolor="<?PHP echo $TH_Color ?>">Modules</th></tr>
    <tr><td><a href="<?PHP echo $modulePath 
       ?>datebook.php">Datebook</a><br>
       <a href="<?PHP echo $modulePath 
       ?>doc.php">DOC</a><br>
       <a href="<?PHP echo $modulePath 
       ?>mobiledb.php">MobileDB</a></td></tr>
    <tr><th bgcolor="<?PHP echo $TH_Color ?>">Examples</th></tr>
    <tr><td><a href="<?PHP echo $samplePath
       ?>viewSource.php?file=bookmark_test.php">Bookmark Test</a></td></tr>
    <tr><th bgcolor="<?PHP echo $TH_Color ?>">SourceForge</th></tr>
    <tr><td><a href="http://sourceforge.net/project/showfiles.php?group_id=29740">
       Download</a><br>
       <a href="http://sourceforge.net/projects/php-pdb/">
       Project&nbsp;Page</a><br>
       <a href="http://sourceforge.net/cvs/?group_id=29740">
       CVS</a><br>
       <a href="http://sourceforge.net/mail/?group_id=29740">
       Mailing&nbsp;Lists</a><br>
       <a href="http://sourceforge.net/tracker/?atid=397207&group_id=29740&func=browse">
       Bug&nbsp;Reporting</a><br>
  </table>
<?PHP
}


function StandardFooter() {
?>
<hr>
<p><a href="http://sourceforge.net"><img
src="http://sourceforge.net/sflogo.php?group_id=29740" width="88"
height="31" border="0" alt="SourceForge Logo" align=right></a>
<font size=-2>&copy;opyright 2001, Tyler Akins (<?PHP
HideEmail("fidian@tiny.net") ?>)<br><br>
All names, trademarks, etc. are property of their respective
owners.</font></p>
</td></tr>
</table>
</body>
</html>
<?PHP
}


function HideEmail($email) {
   $MailLink = "<a href=\"mailto:$email\">$email</a>";
   
   $MailLinkNew = '';
   for ($i = 0; $i < strlen($MailLink); $i ++) {
      $MailLinkNew .= '%' . bin2hex($MailLink[$i]);
   }
   $MailLink = $MailLinkNew;
   
   $MailLinkNew = array();
   while (strlen($MailLink)) {
      $MailLinkNew[] = substr($MailLink, 0, 25) . '" + "' .
         substr($MailLink, 25, 25);
      $MailLink = substr($MailLink, 50);
   }
   
?><SCRIPT LANGUAGE="javascript">
MaIlMe=new Array();
<?PHP foreach ($MailLinkNew as $index =>$data) {
?>MaIlMe[<?PHP echo $index ?>]="<?PHP echo $data ?>";
<?PHP } ?>
OutString="";
for (j=0;j<MaIlMe.length;j++){
OutString+=MaIlMe[j];
}document.write(unescape(OutString));
</SCRIPT><NOSCRIPT>Sorry, you need javascript to see my email address</noscript><?PHP
}


function ShowExample($text) {
?><table align=center width=80% border=1 cellpadding=5 cellspacing=0
bgcolor=#DFFFDF>
<tr>
<td><font face='courier'><?PHP

   $text = trim($text);
   $text = htmlspecialchars($text);
   $text = nl2br($text);
   $text = explode("\n", $text);
   foreach ($text as $t) {
      if ($t[0] == ' ') {
         echo '&nbsp;';
         $t = substr($t, 1);
      }
      $t = str_replace('  ', ' &nbsp;', $t);
      echo $t;
   }

?></font></td></tr>
</table>
<?PHP
}


function ShowExampleFile($filename) {
   if (! file_exists($filename)) {
      ShowExample("$filename doesn't exist!");
      return;
   }
   $fileData = file($filename);
   $write = 0;
   $output = '';
   while (count($fileData)) {
      $line = array_shift($fileData);
      if (strpos($line, "END HERE") !== false)
         $write = 0;
      if ($write)
         $output .= $line;
      if (strpos($line, "START HERE") !== false)
         $write = 1;
   }
   ShowExample($output);
}
