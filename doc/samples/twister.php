<?PHP
/* Examples for PHP-PDB library - HTML to DOC converter
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */

if (file_exists('../../php-pdb.inc')) {
   include('../../php-pdb.inc');
   include('../../modules/doc.inc');
} elseif (file_exists('./php-pdb.inc')) {
   include('./php-pdb.inc');
   include('./modules/doc.inc');
}


$MyFilename = $PHP_SELF;
$pos = strrpos($MyFilename, '/');
$MyFilename = substr($MyFilename, $pos + 1);

if (isset($action) && $action == "download") {
   DownloadPDB();
} elseif (isset($action) && $action == "convert") {
   ConvertFile();
} else {
   session_start();
   ShowInitialHelp();
   ShowInitialForm();
}
exit();



function StartHTML($title = '') {
   if ($title == '')
      $title = 'Twister!';
      
?><html><head><title><?PHP echo $title ?></title></head>
<body bgcolor="#FFFFFF">
<h1 align=center><?PHP echo $title ?></h1>
<?PHP
}


function ShowInitialHelp() {
   StartHTML();
?><p>Twister is a conversion tool to convert web pages, and text files into
Palm DOC format.  It can be later extended to write to zTXT and other
formats, once PHP-PDB has a class for accessing them.</p>

<p>To start the conversion process, just fill in this form:</p>
<?PHP
}


function ShowInitialForm() {
   global $MyFilename;
   
?><form action="<?PHP echo $MyFilename ?>" method="post">
<table border=1 align=center cellpadding=5 cellspacing=0>
  <tr>
    <td align=right><b>Source:</b></td>
    <td><input type=radio name="Source" value="File" checked>
      File:  <input type=file name=filedata size=45><br>
      <input type=radio name="Source" value="URL"> 
      URL:  <input type=input name=urldata size=60></td>
  </tr>
  <tr>
    <td align=right><b>Source Type:</b></td>
    <td><input type=radio name="SourceType" value="HTML" checked>
      HTML<br>
      <br>
      
      <input type=radio name="SourceType" value="Text">
      Text<br>
      &nbsp; &nbsp; &nbsp;<input type=checkbox name="RewrapParagraphs">
      Rewrap paragraphs<br>
      <br>
      
      <input type=radio name="SourceType" value="Gutenberg">
      Project Gutenberg Text<br>
      &nbsp; &nbsp; &nbsp;<input type=checkbox name="BreakOnChapter">
      Create a separate file for each chapter
      
      </td>
  </tr>
  <tr>
    <td align=right><b>Convert Into:</b></td>
    <td><input type=radio name="TargetType" value="DOC" checked>
      DOC<br>
      &nbsp; &nbsp; &nbsp;<input type=checkbox name="UncompressedDoc">
      Don't compress DOC file
      </td>
  </tr>
  <tr>
    <td colspan=2 align=center><input type=submit value="Convert File!"></td>
  </tr>
</table>
</form>
<?PHP
}
