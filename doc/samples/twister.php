<?PHP
/* Examples for PHP-PDB library - HTML to DOC converter
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */

/* Variables possibly passed in:
 *   action = 'convert' to convert, 'download' to get a file
 *
 *   If action == 'convert'
 *      Source = 'File' or 'URL' for designating where the source is
 *      if Source == 'File'
 *         filedata = the file ??
 *      if Source == 'URL'
 *         urldata = the URL
 *      SourceType = 'HTML', 'Text', 'Gutenberg'
 *      If SourceType == 'Text'
 *         RewrapParagraphs = checkbox for rewrapping paragraphs
 *      If SourceType == 'Gutenberg'
 *         BreakOnChapter = checkbox for breaking the file on chapters
 *      TargetType = 'DOC'
 *
 *   If action == 'download'
 *      file = name of the file in $SavedFiles
 */

if (file_exists('../../php-pdb.inc')) {
   include('../../php-pdb.inc');
   include('../../modules/doc.inc');
} elseif (file_exists('./php-pdb.inc')) {
   include('./php-pdb.inc');
   include('./modules/doc.inc');
}

// START HERE

include('./filters/text.inc');
include('./filters/html.inc');
include('./filters/gutenberg.inc');

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
<input type=hidden name=action value="convert">
<table border=1 align=center cellpadding=5 cellspacing=0>
  <tr>
    <td align=right><b>Source:</b></td>
    <td><input type=radio name="Source" value="File">
      File:  <input type=file name=filedata size=45><br>
      <input type=radio name="Source" value="URL" checked> 
      URL:  <input type=input name=urldata size=60 value="http://rumkin.com/alice30.txt"></td>
  </tr>
  <tr>
    <td align=right><b>Source Type:</b></td>
    <td><input type=radio name="SourceType" value="HTML">
      HTML<br>
      <br>
      
      <input type=radio name="SourceType" value="Text">
      Text<br>
      &nbsp; &nbsp; &nbsp;<input type=checkbox name="RewrapParagraphs">
      Rewrap paragraphs<br>
      <br>
      
      <input type=radio name="SourceType" value="Gutenberg" checked>
      Project Gutenberg Text<br>
      &nbsp; &nbsp; &nbsp;<input type=checkbox name="BreakOnChapter" checked>
      Create a separate file for each chapter
      
      </td>
  </tr>
  <tr>
    <td align=right><b>Convert Into:</b></td>
    <td><input type=radio name="TargetType" value="DOC" checked>
      DOC<!--
      <br>
      &nbsp; &nbsp; &nbsp;<input type=checkbox name="UncompressedDoc">
      Don't compress DOC file
      -->
      </td>
  </tr>
  <tr>
    <td colspan=2 align=center><input type=submit value="Convert File!"></td>
  </tr>
</table>
</form>
<?PHP
}


function ConvertFile() {
   StartHTML('Converting File');
   $filedata = GetTheFile();
   if ($filedata !== false) {
      $rawData = ConvertFromFormat($filedata);
      if ($rawData !== false) {
         StoreAsPRC($rawData);
      }
   }
   ShowDownloadLinks();
}


function ShowInTable($Msg, $BGColor) {
   echo "<table align=center width=60% bgcolor=\"" . $BGColor .
      "\" border=1 cellpadding=10 cellspacing=2>";
   echo "<tr><td align=center>";
   echo "<B>" . nl2br(htmlspecialchars($Msg)) . "</B>";
   echo "</td></tr></table><br>\n";
}


function ShowError($Err) {
   ShowInTable($Err, '#FFA0A0');
}


function ShowStatus($Str) {
   ShowInTable($Str, '#A0FFA0');
   flush();
}


// We care about three digits or so.
// 1 byte
// 135 bytes
// 1.23 k
// 12.3 k
// 123 k
// and so on
function ShowSize($bytes) {
    $tags = array('b', 'k', 'meg', 'gig', 'terra');
    $index = 0;
    while ($bytes > 999 && isset($tags[$index + 1])) {
       $bytes /= 1024;
       $index ++;
    }
    $rounder = 1;
    if ($bytes < 10)
       $rounder *= 10;
    if ($bytes < 100)
       $rounder *= 10;
    $bytes *= $rounder;
    settype($bytes, 'integer');
    $bytes /= $rounder;
    
    return $bytes . ' ' . $tags[$index];
}


function GetTheFile() {
   global $Source, $urldata;
   
   if ($Source == 'File') {
      ShowError('I am unable to handle uploaded files at the moment.');
      return false;
   }
   if ($Source == 'URL') {
      $fp = @fopen($urldata, "r");
      if (! $fp) {
         ShowError("Unable to read from the URL specified:\n" . $urldata);
	 return false;
      }
      ShowStatus('Loading file.  Please be patient.');
      $d = '';
      while (! feof($fp)) {
         $d .= fread($fp, 8192);
      }
      ShowStatus('File loaded.  Total size is ' . ShowSize(strlen($d)) . '.');
      fclose($fp);
      return $d;
   }
   ShowError('Invalid source.');
   return false;
}


function ConvertFromFormat($filedata) {
   global $SourceType, $RewrapParagraphs, $BreakOnChapter;
   
   if ($SourceType == 'Text') {
      return FilterText($filedata, $RewrapParagraphs);
   }
   if ($SourceType == 'HTML') {
      return FilterHtml($filedata);
   }
   if ($SourceType == 'Gutenberg' {
      return FilterGutenberg($filedata, $BreakOnChapter);
   }
   ShowError('Invalid source type.');
   return false;
}


function StoreAsPRC($rawData) {
}


function ShowDownloadLinks() {
}