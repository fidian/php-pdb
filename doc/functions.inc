<?PHP

function StandardHeader($title = '', $curdir = '') {
   $newTitle = "PHP-PDB";
   
   if ($title != '')
      $newTitle .= ': ' . $title;
    
   if ($curdir == 'modules') {
      $modulePath = '';
      $rootPath = '../';
   } else {
      $modulePath = 'modules/';
      $rootPath = '';
   }

?><HTML><HEAD><TITLE><?PHP echo $newTitle ?></TITLE></HEAD>
<body bgcolor=#FFFFFF text=#000000>
<h1 align=center><?PHP echo $newTitle ?></h1>
<table align=center border=1 bgcolor=#DFDFAF cellpadding=2 cellspacing=0>
<tr><td>
  <table align=center border=0 cellpadding=0 cellspacing=0>
  <tr>
    <td>&nbsp;</td>
    <td><a href="<?PHP echo $rootPath ?>index.php">Main Page</a></td>
    <td>&nbsp;&nbsp;&nbsp;</td>
    <td><a href="<?PHP echo $rootPath ?>example.php">Basic Use</a></td>
    <td>&nbsp;&nbsp;&nbsp;</td>
    <td><a href="<?PHP echo $modulePath ?>datebook.php">Datebook</a></td>
    <td>&nbsp;&nbsp;&nbsp;</td>
    <td><a href="<?PHP echo $modulePath ?>doc.php">DOC</a></td>
    <td>&nbsp;&nbsp;&nbsp;</td>
    <td><a href="<?PHP echo $rootPath ?>extra.php">Extra Info</a></td>
    <td>&nbsp;</td>
  </tr>
  </table>
</td></tr>
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
All names, trademarks, etc. are property of their respective owners.</font></p>
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
