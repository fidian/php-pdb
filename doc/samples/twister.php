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
 *      TitleOfDoc = Title
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

include('../functions.inc');

// START HERE

set_time_limit(0);

// The viewSource.php file will not let you view the filter
// files.  If you want to see them, you can use the web-based
// CVS viewer that SourceForge provides.  This awfully long URL
// is broken onto two lines -- make sure it is all on one in 
// your browser.
//
//     http://cvs.sourceforge.net/cgi-bin/viewcvs.cgi/php-pdb/
//     php-pdb/doc/samples/filters/

include('./filters/text.inc');
include('./filters/html.inc');
include('./filters/gutenberg.inc');

session_start();

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
<p align=center><b><font size="+3"><?PHP echo $title ?></font></b><br>
<font size="-1">(<a href="viewSource.php?file=twister.php">View
Source</a> -- <a href="../">Back to PHP-PDB Documentation</a>)</font></p>
<?PHP
}


function ShowInitialHelp() {
   StartHTML();
?><p>Twister is a conversion tool to convert web pages, and text files into
Palm DOC format.  It can be later extended to write to zTXT and other
formats, once PHP-PDB has a class for accessing them.</p>

<p><B><font size="+2" color="purple">Be Warned:</font></B> Twister is
quite new and may not work well for you.  However, if it doesn't work to your
expectations, just contact the <a
href="http://php-pdb.sourceforge.net/">PHP-PDB development team</a> and
hopefully the bugs will vanish quickly.</p>

<p>To start the conversion process, just fill in the form below.  The file
will be converted (or an error message will be displayed) and you should see
links for downloading the converted file.  This requires cookies to run
properly.</p>
<?PHP
}


function ShowInitialForm() {
   global $MyFilename, $Source, $SourceType, $RewrapParagraphs,
      $BreakOnChapter, $TargetType, $TitleOfDoc, $PHP_SELF, $filedata,
      $urldata;
   // $UncompressedDoc
   
   ShowDownloadLinks();
   
?><form action="<?PHP echo $MyFilename ?>" method="post">
<input type=hidden name=action value="convert">
<table border=1 align=center cellpadding=5 cellspacing=0>
  <tr>
    <td align=right><b>Source:</b></td>
    <td><input type=radio name="Source" value="File"<?PHP
      if (! isset($Source) || $Source != 'URL') echo ' checked'; ?>>
      File:  <input type=file name=filedata size=45<?PHP
      if (isset($filedata)) echo ' value="' . htmlspecialchars($filedata)
      . '"'; ?>><br>
      
      <?PHP if (stristr($PHP_SELF, ".sourceforge.net/") === false) { ?>
      <input type=radio name="Source" value="URL"<?PHP
      if (isset($Source) && $Source == 'URL') echo ' checked'; ?>> 
      URL:  <input type=input name=urldata size=60<?PHP
      if (isset($filedata)) echo ' value="' . htmlspecialchars($urldata)
      . '"'; ?>>
      <?PHP } else { ?><br>
      <i>(Twister! supports converting URLs, but Sourceforge can't download
      from URLs.  Download and install <a
      href="http://php-pdb.sourceforge.net/">PHP-PDB</a> on your own site
      and Twister! should work fine.  It'll be included in the "doc/samples"
      directory.)</i><?PHP
      } ?></td>
  </tr>
  <tr>
    <td align=right><b>Source Type:</b></td>
    <td><input type=radio name="SourceType" value="HTML"<?PHP
      if (! isset($SourceType) || ($SourceType != 'Text' && 
         $SourceType != 'Gutenberg')) echo ' checked'; ?>>
      HTML<br>
      <br>
      
      <input type=radio name="SourceType" value="Text"<?PHP
      if (isset($SourceType) && $SourceType == 'Text') echo ' checked'; ?>>
      Text<br>
      &nbsp; &nbsp; &nbsp;<input type=checkbox name="RewrapParagraphs"<?PHP
      if (isset($RewrapParagraphs) && $RewrapParagraphs) echo ' checked'; ?>>
      Rewrap paragraphs<br>
      <br>
      
      <input type=radio name="SourceType" value="Gutenberg"<?PHP
      if (isset($SourceType) && $SourceType == 'Gutenberg') 
         echo ' checked'; ?>>
      Project Gutenberg Text (<a href="http://promo.net/pg">What is 
      this?</a>)<br>
      &nbsp; &nbsp; &nbsp;<input type=checkbox name="BreakOnChapter"<?PHP
      if (isset($BreakOnChapter) && $BreakOnChapter) echo ' checked'; ?>>
      Create a separate file for each chapter
      
      </td>
  </tr>
  <tr>
    <td align=right><b>Convert Into:</b></td>
    <td><input type=radio name="TargetType" value="DOC" checked>
      DOC (Uncompressed)<br>
      &nbsp; &nbsp; &nbsp;DOC Title:
      <input type=text name="TitleOfDoc" value="<?PHP
      if (isset($TitleOfDoc)) echo htmlspecialchars($TitleOfDoc); ?>">
      <!--
      <br>
      &nbsp; &nbsp; &nbsp;<input type=checkbox name="UncompressedDoc">
      Don't compress DOC file
      -->
      <br><br>
      
      <i>(More will be added later)</i>
      </td>
  </tr>
  <tr>
    <td colspan=2 align=center><input type=submit value="Convert File!"></td>
  </tr>
</table>
</form>
<?PHP
   TinyFooter();
}


function ConvertFile() {
   global $TitleOfDoc;
   StartHTML('Converting File');
   if (ConversionSanityChecks()) {
      ShowInitialForm();
      return;
   }
   $filedata = GetTheFile();
   if ($filedata === false) {
      ShowInitialForm();
      return;
   }
   
   $rawData = ConvertFromFormat($filedata);
   if ($rawData === false) {
      ShowInitialForm();
      return;
   }
   
   if (is_array($rawData)) {
      foreach ($rawData as $index => $d) {
         StoreAsPRC($TitleOfDoc . ' [' . ($index + 1) . '/' .
	    count($rawData) . ']', $d);
      }
   } else {
      StoreAsPRC($TitleOfDoc, $rawData);
   }
   
   ShowStatus("Conversion complete!");
   
   ShowInitialForm();
}


// Returns true on error
function ConversionSanityChecks() {
   global $TargetType, $TitleOfDoc, $Source, $filedata, $urldata, $SourceType;
   
   if ($TargetType == 'DOC' && $TitleOfDoc == '') {
      ShowError('You must specify a title for the DOC file.');
      return true;
   }
   if ($TargetType != 'DOC') {
      ShowError('Invalid target type.');
      return true;
   }
   if ($Source != 'File' && $Source != 'URL') {
      ShowError('Invalid source.');
      return true;
   }
   if ($Source == 'File' && ! $filedata) {
      ShowError('Invalid file source.  Make sure to upload a file.');
      return true;
   }
   if ($Source == 'URL' && ! $urldata) {
      ShowError('Invalid URL source.  Make sure to specify a URL.');
      return true;
   }
   if ($SourceType != 'Text' && $SourceType != 'HTML' && 
       $SourceType != 'Gutenberg') {
      ShowError('Invalid source type.');
      return true;
   }
   return false;
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
      ShowStatus('File loaded.  Total size is ' . ShowSize(strlen($d)) . 
         ".\nStarting conversion of the file.");
      fclose($fp);
      return $d;
   }
   ShowError('Invalid source.');
   return false;
}


function ConvertFromFormat($filedata) {
   global $SourceType, $RewrapParagraphs, $BreakOnChapter;
   
   if ($SourceType == 'Text') {
      if (! isset($RewrapParagraphs))
         $RewrapParagraphs = false;
      return FilterText($filedata, $RewrapParagraphs);
   }
   if ($SourceType == 'HTML') {
      return FilterHtml($filedata);
   }
   if ($SourceType == 'Gutenberg') {
      if (! isset($BreakOnChapter))
         $BreakOnChapter = false;
      return FilterGutenberg($filedata, $BreakOnChapter);
   }
   ShowError('Invalid source type.');
   return false;
}


function StoreAsPRC($title, $rawData) {
   echo "<h1>$title</h1>\n<pre>$rawData\n</pre>\n"; return;
   global $SavedPDB;
   
   if (! isset($SavedPDB) || ! is_array($SavedPDB)) {
      $SavedPDB = array();
      session_register('SavedPDB');
   }
   
   $SavedInfo['Title'] = $title;
   $SavedInfo['Type'] = 'DOC';
   $SavedInfo['Time'] = time();
   $prc = new PalmDoc($title);
   $prc->AddDocText($rawData);
   ob_start();
   $prc->WriteToStdout();
   $prc = ob_get_contents();
   ob_end_clean();
   $SavedInfo['Data'] = $prc;
   
   $key = time();
   while (isset($SavedPDB[$key])) {
      $key ++;
   }
   $SavedPDB[$key] = $SavedInfo;
}


function ShowDownloadLinks() {
   global $SavedPDB, $MyFilename;
   
   if (! isset($SavedPDB) || ! is_array($SavedPDB)) {
      $SavedPDB = array();
      session_register('SavedPDB');
   }

   krsort($SavedPDB);
   if (count($SavedPDB)) {
?>
<table align=center border=1 cellpadding=5 cellspacing=0>
  <tr bgcolor="#FFD0D0">
    <th>Title</th>
    <th>Type</th>
    <th>Size</th>
    <th>When Created</th>
    <th>&nbsp;</th>
  </tr>
<?PHP
      foreach ($SavedPDB as $key => $SavedInfo) {
?>  <tr>
    <td><?PHP echo $SavedInfo['Title'] ?></td>
    <td><?PHP echo $SavedInfo['Type'] ?></td>
    <td><?PHP echo ShowSize(strlen($SavedInfo['Data'])) ?></td>
    <td><?PHP echo date("g:i:s a", $SavedInfo['Time']) ?></td>
    <td><a href="<?PHP echo $MyFilename
       ?>?action=download&file=<?PHP echo $key ?>">Download</a></td>
  </tr>
<?PHP
      }
   
      echo "</table>\n";
   }
}


function DownloadPDB() {
   global $file, $SavedPDB, $HTTP_USER_AGENT;
   
   if (! isset($SavedPDB) || ! is_array($SavedPDB) || 
       ! isset($SavedPDB[$file])) {
      StartHTML('Error Downloading File');
      ShowError('The specified file no longer is saved.  It most ' .
         'likely expired.');
      ShowInitialForm();
      return;
   }

   $filename = preg_replace('/[^-a-zA-Z0-9\\.]/', '_', $file . '.pdb');
      
   if (strstr($HTTP_USER_AGENT, 'compatible; MSIE ') !== false &&
       strstr($HTTP_USER_AGENT, 'Opera') === false) {
      // IE doesn't properly download attachments.  This should work
      // pretty well for IE 5.5 SP 1
      header("Content-Disposition: inline; filename=$filename");
      header("Content-Type: application/download; name=\"$filename\"");
   } else {
      // Use standard headers for Netscape, Opera, etc.
      header("Content-Disposition: attachment; filename=\"$filename\"");
      header("Content-Type: application/x-pilot; name=\"$filename\"");
   }

   echo $SavedPDB[$file]['Data'];
}