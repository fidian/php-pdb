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
  <table border=0 cellpadding=5 cellspacing=0 bgcolor="#BFBFEF" align=right>
    <tr><td>
      <a href="<?PHP echo $rootPath ?>index.php">Main&nbsp;Page</a>
    </td></tr>
    <tr><th bgcolor="<?PHP echo $TH_Color ?>">General</th></tr>
    <tr><td>
      <a href="<?PHP echo $rootPath ?>example.php">Basic&nbsp;Use</a><br>
      <a href="<?PHP echo $rootPath ?>custom.php">Custom&nbsp;Formats</a><br>
      <a href="<?PHP echo $rootPath ?>extra.php">Extra&nbsp;Info</a><br>
      <a href="<?PHP echo $rootPath ?>credits.php">Credits</a>
    </td></tr>
    <tr><th bgcolor="<?PHP echo $TH_Color ?>">Modules</th></tr>
    <tr><td>
       <a href="<?PHP echo $modulePath ?>addrbook.php">Address&nbsp;Book</a><br>
       <a href="<?PHP echo $modulePath ?>datebook.php">Datebook</a><br>
       <a href="<?PHP echo $modulePath ?>doc.php">DOC</a><br>
       <a href="<?PHP echo $modulePath ?>mobiledb.php">MobileDB</a><br>
       <a href="<?PHP echo $modulePath ?>smallbasic.php">SmallBASIC</a><br>
       <a href="<?PHP echo $modulePath ?>todo.php">Todo</a>
    </td></tr>
    <tr><th bgcolor="<?PHP echo $TH_Color ?>">Examples</th></tr>
    <tr><td>
       <a href="<?PHP echo $samplePath 
          ?>bookmark_test.php">Bookmark&nbsp;Test</a><br>
       <a href="<?PHP echo $samplePath ?>twister.php">Twister</a>
    </td></tr>
    <tr><th bgcolor="<?PHP echo $TH_Color ?>">SourceForge</th></tr>
    <tr><td>
       <a href="<?PHP echo $rootPath ?>download.php">Download</a><br>
       <a href="http://sourceforge.net/projects/php-pdb/">
          Project&nbsp;Page</a><br>
       <a href="http://sourceforge.net/mail/?group_id=29740">
          Mailing&nbsp;Lists</a><br>
       <a href="http://sourceforge.net/tracker/?atid=397207&group_id=29740&func=browse">
          Bug&nbsp;Reporting</a><br>
    </td></tr>
  </table>
<?PHP
}


function TinyFooter() {
   global $HTTP_HOST;
   $small_self = strtolower($HTTP_HOST);
?>
<p><hr>
<font size=-2><?PHP
if (strpos($small_self, '.sourceforge.net') !== false) {
?><a href="http://sourceforge.net"><img
src="http://sourceforge.net/sflogo.php?group_id=29740" width="88"
height="31" border="0" alt="SourceForge Logo" align=right></a>
<?PHP } ?>
&copy;opyright 2001, Tyler Akins (<?PHP
HideEmail('fidian', 'tiny.net') ?>)<br>
All names, trademarks, etc. are property of their respective
owners.</font>&nbsp;</p>
<?PHP
}


function StandardFooter() {
   TinyFooter();
?>
</td></tr>
</table>
</body>
</html>
<?PHP
}


function HideEmail($user, $pass) {
   $MailLink = '<a href="mailto:' . $user . '@' . $pass . '">' . $user .
      '@' . $pass . '</a>';
   
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
</SCRIPT><NOSCRIPT>Sorry, you need javascript to this email address</noscript><?PHP
}


function ShowExample($text) {
?><table align=center width=80% border=1 cellpadding=5 cellspacing=0
bgcolor=#DFFFDF>
<tr>
<td><font face='courier' size='-1'><?PHP

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


function ShowOverwriteWarning() {
?>
<p>If you install a database created with this class onto your handheld,
it will <i>overwrite</i> the one that already exists on your handheld.
This is because a new database will have the same name as the current
database on your handheld and you can't change the name of the database
otherwise the application won't work. This is obviously irritating --
please keep this in mind.  An ideal use
for this PHP class would be for talking to a conduit, where the conduit
would upload the current data, the server would parse it and 
add/modify/delete entries with this class, the server would send the
modified file back to the conduit, and the conduit would replace the 
device's database with the modified one.</p>
<?PHP
}