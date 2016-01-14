<?PHP
/* Examples for PHP-PDB library - HTML to DOC converter
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See https://github.com/fidian/php-pdb for more information about the library
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
 *      TargetType = 'DOC', 'SmallBASIC'
 *      if TargetType == 'DOC'
 *         TitleOfDoc = Title
 *         UncompressedDOC = true/false to generate an uncompressed doc
 *      if TargetType == 'SmallBASIC'
 *         TitleOfBasicFile = Title of basic file to generate
 *
 *   If action == 'download'
 *      file = name of the file in $SavedFiles
 */

if (file_exists('../../php-pdb.inc')) {
   include('../../php-pdb.inc');
   include('../../modules/doc.inc');
   include('../../modules/smallbasic.inc');
} elseif (file_exists('./php-pdb.inc')) {
   include('./php-pdb.inc');
   include('./modules/doc.inc');
   include('./modules/smallbasic.inc');
}

include('../functions.inc');

// START HERE

// Two minutes should be more than enough.
// If nothing else, it should stop the server from being
// brought to its knees by a snippet of bad code.
// (Speaking from experience.)
set_time_limit(1200);

include('./filters/text.inc');
include('./filters/html.inc');
include('./filters/gutenberg.inc');

session_start();

$tiny_self = strtolower($HTTP_HOST);

$MyFilename = $PHP_SELF;
$pos = strrpos($MyFilename, '/');
$MyFilename = substr($MyFilename, $pos + 1);

if (isset($action) && $action == "download") {
   DownloadPDB();
} elseif (isset($action) && $action == 'erase') {
   StartHTML('File Erased');
   ErasePDB();
   ShowInitialForm();
} elseif (isset($action) && $action == "convert") {
   ConvertFile();
} else {
   ShowInitialHelp();
   ShowInitialForm();
}
exit();



function StartHTML($title = '') {
   if ($title == '')
      $title = 'Twister';

?><html><head><title><?PHP echo $title ?></title></head>
<body bgcolor="#FFFFFF">
<p align=center><b><font size="+3"><?PHP echo $title ?></font></b></p>
<?PHP
}


function ShowInitialHelp() {
   StartHTML();
?><p>Twister is a conversion tool to convert web pages, and text files into
Palm DOC format.  SmallBASIC .BAS files can be converted to SmallBASIC for
Palm OS format.  It can be later be easily extended to write to zTXT and other
formats, once PHP-PDB has classes for accessing them.  To start the
conversion process, just fill out the form below.  The file will be
converted and you will see links to download the converted file.</p>

<p>Twister is quite new and may not work perfectly.  If you experience
problems or if something is not converted quite right (like the HTML looks
goofy when converted to text), the just contact the <a
href="https://github.com/fidian/php-pdb">PHP-PDB development team</a> and
we will see what we can do.</p>

<?PHP if (! ini_get('allow_url_fopen')) { ?>
<P><b>The URL converter doesn't work on sites with "allow_url_fopen" turned
off, but it should work for you if you download
<a href="https://github.com/fidian/php-pdb">PHP-PDB</a>
and have it running on your own site.  <font size="-1">(Sorry.)</font></p>

<?PHP
   }
}


function ShowInitialForm() {
   global $MyFilename, $Source, $SourceType, $RewrapParagraphs,
      $BreakOnChapter, $TargetType, $TitleOfDoc, $filedata,
      $urldata, $UncompressedDoc, $TitleOfBasicFile;

   echo "<br>\n";

   ShowDownloadLinks();

?><form action="<?PHP echo $MyFilename
?>" method="post" enctype="multipart/form-data">
<input type=hidden name=action value="convert">
<table border=1 align=center cellpadding=5 cellspacing=0>
  <tr>
    <td align=right><b>Source:</b></td>
    <td><?PHP if (ini_get('allow_url_fopen')) { ?>
      <input type=radio name="Source" value="URL"<?PHP
      if (! isset($Source) || $Source != 'File') echo ' checked'; ?>>
      URL:  <input type=input name=urldata size=60<?PHP
      if (isset($urldata)) echo ' value="' . htmlspecialchars($urldata)
      . '"'; ?>><br>
      <?PHP } ?>

      <input type=radio name="Source" value="File"<?PHP
      if ((isset($Source) && $Source == 'File') ||
	  ! ini_get('allow_url_fopen'))
         echo ' checked'; ?>>
      File:  <input type=file name=filedata size=45<?PHP
      if (isset($filedata)) echo ' value="' . htmlspecialchars($filedata)
      . '"'; ?>>
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
      &nbsp; &nbsp; &nbsp;Create a new DOC file every
      <input type=text name="BreakOnChapter" size=4 value="<?PHP
      if (isset($BreakOnChapter)) {
         settype($BreakOnChapter, 'integer');
         echo htmlspecialchars($BreakOnChapter);
      } else
         echo '0'; ?>"> chapters<br>
      &nbsp; &nbsp; &nbsp;<i>(Use 0 to disable)

      </td>
  </tr>
  <tr>
    <td align=right><b>Convert Into:</b></td>
    <td><input type=radio name="TargetType" value="DOC"<?PHP
      if (! isset($TargetType) ||
	  ($TargetType != 'SmallBASIC' && $TargetType != 'zTXT'))
         echo ' checked'; ?>> DOC<br>
      &nbsp; &nbsp; &nbsp;DOC Title:
      <input type=text name="TitleOfDoc" value="<?PHP
      if (isset($TitleOfDoc)) echo htmlspecialchars($TitleOfDoc); ?>">
      <br>
      &nbsp; &nbsp; &nbsp;<input type=checkbox name="UncompressedDoc">
      Don't compress DOC file (faster to convert, larger file)
      <br><br>


      <input type=radio name="TargetType" value="zTXT"<?PHP
	if (isset($TargetType) && $TargetType == 'zTXT')
	   echo ' checked'; ?>> zTXT (Weasel Reader)<br>
      &nbsp; &nbsp; &nbsp;zTXT Title:
      <input type=text name="TitleOfzTXT" value="<?PHP
      if (isset($TitleOfzTXT)) echo htmlspecialchars($TitleOfzTXT); ?>">
      <br><br>


      <input type=radio name="TargetType" value="SmallBASIC"<?PHP
      if (isset($TargetType) && $TargetType == 'SmallBASIC')
         echo ' checked'; ?>> SmallBASIC<br>
      &nbsp; &nbsp; &nbsp;Name of File:
      <input type=text name="TitleOfBasicFile" value="<?PHP
      if (isset($TitleOfBasicFile))
         echo htmlspecialchars($TitleOfBasicFile); ?>">
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
   global $TitleOfDoc, $TitleOfBasicFile, $TargetType;

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

   if ($TargetType == 'SmallBASIC')
      $DaTitle = $TitleOfBasicFile;
   if ($TargetType == 'DOC')
      $DaTitle = $TitleOfDoc;
   if ($TargetType == 'zTXT')
      $DaTitle = $TitleOfzTXT;

   if (is_array($rawData)) {
      foreach ($rawData as $index => $d) {
         StoreAsPRC($DaTitle . ' [' . ($index + 1) . '/' .
	    count($rawData) . ']', $d);
      }
   } else {
      StoreAsPRC($DaTitle, $rawData);
   }

   ShowStatus("Conversion complete!");

   ShowInitialForm();
}


// Returns true on error
function ConversionSanityChecks() {
   global $TargetType, $TitleOfDoc, $Source, $filedata, $urldata,
      $SourceType, $TitleOfBasicFile;

   if (! isset($TargetType) || ($TargetType != 'DOC' && $TargetType !=
       'SmallBASIC' && $TargetType != 'zTXT')) {
       ShowError('Invalid target type.');
       return true;
   }

   if ($TargetType == 'DOC' && (! isset($TitleOfDoc) || $TitleOfDoc == '')) {
      ShowError('You must specify a title for the DOC file.');
      return true;
   }

   if ($TargetType == 'zTXT' && (! isset($TitleOfzTXT) || $TitleOfzTXT == '')) {
      ShowError('You must specify a title for the zTXT file.');
      return true;
   }

   if ($TargetType == 'SmallBASIC' && (! isset($TitleOfBasicFile) ||
       $TitleOfBasicFile == '')) {
      ShowError('You must specify a file name for the SmallBASIC file.');
      return true;
   }

   if (! isset($Source) || ($Source != 'File' && $Source != 'URL')) {
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


function ShowInTable($Msg, $BGColor, $border) {
   echo "<table align=center width=60% bgcolor=\"" . $BGColor .
      "\" border=1 cellpadding=$border cellspacing=2>";
   echo "<tr><td align=center>";
   echo "<B>" . nl2br(htmlspecialchars($Msg)) . "</B>";
   echo "</td></tr></table>\n";
}


function ShowError($Err) {
   echo "<br>\n";
   ShowInTable($Err, '#FFA0A0', 10);
   echo "<br>\n";
}


function ShowStatus($Str) {
   ShowInTable($Str, '#A0FFA0', 4);
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
   global $Source, $urldata, $HTTP_POST_FILES, $filedata;

   if ($Source == 'File') {
      $fp = @fopen($HTTP_POST_FILES['filedata']['tmp_name'], 'r');
      if (! $fp) {
         ShowError("I am unable to open the uploaded file.  Weird.");
         return false;
      }
      $d = '';
      while (! feof($fp)) {
         $d .= fread($fp, 8192);
      }
      ShowStatus('File loaded.  Total size is ' . ShowSize(strlen($d)) .
         ".\nStarting conversion of the file.");
      fclose($fp);
      return $d;
   }
   if ($Source == 'URL') {
      $IsGood = false;
      foreach (array("http://", "ftp://") as $match) {
         if (strcasecmp(substr($urldata, 0, strlen($match)), $match) == 0)
	    $IsGood = true;
      }
      if (! $IsGood) {
         ShowError("Unacceptable URL.  It must begin with http:// or " .
	    "ftp://");
	 return false;
      }
      $fp = @fopen($urldata, "r");
      if (! $fp) {
         ShowError("Unable to read from the URL specified:\n" . $urldata);
	 return false;
      }
      ShowStatus('Downloading file.  Please be patient.');
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
   // echo "<h1>$title</h1>\n<pre>$rawData\n</pre>\n"; return;
   global $SavedPDB, $UncompressedDoc, $TargetType, $CompressWarningDisplayed;

   $fileName = preg_replace('/[^-a-zA-Z_0-9]/', '_', $title);

   if (! isset($SavedPDB) || ! is_array($SavedPDB)) {
      $SavedPDB = array();
      session_register('SavedPDB');
   }

   $SavedInfo['Title'] = $title;
   $SavedInfo['Type'] = $TargetType;
   $SavedInfo['Time'] = time();
   if ($TargetType == 'DOC') {
      if (isset($UncompressedDoc) && $UncompressedDoc)
         $prc = new PalmDoc($title, false);
      else
         $prc = new PalmDoc($title);
      $prc->AddText($rawData);
      if (!isset($CompressWarningDisplayed)) {
         $WarningTime = '';
	 if (count($prc->Records) > 5)
	    $WarningTime = 'a bit';
	 if (count($prc->Records) > 10)
	    $WarningTime = 'a while';
	 if (count($prc->Records) > 15)
	    $WarningTime = 'a long time';
	 if (count($prc->Records) > 25)
	    $WarningTime = 'a very long time';
	 if (count($prc->Records) > 40)
	    $WarningTime = 'enough time to write a poem';
	 if (count($prc->Records) > 60)
	    $WarningTime = 'enough time for you learn a new hobby';
	 if (count($prc->Records) > 100)
	    $WarningTime = 'so long that PHP will likely time out and ' .
	       'kill this conversion';
	 if ($WarningTime != '')
            ShowStatus("Compressing the DOC.\nThis could take " .
	       $WarningTime . '.');
	 $CompressWarningDisplayed = true;
      }
   } elseif ($TargetType == 'zTXT') {
      $prc = new PalmzTXT($title);
      $prc->AddText($rawData);
   } elseif ($TargetType == 'SmallBASIC') {
      $prc = new PalmSmallBASIC($title);
      $result = $prc->ConvertFromText($rawData);
      if (is_array($result)) {
         ShowError('Error converting the file.  The section "' .
	    $result[1] . '" is too big.  It must be broken into ' .
	    'multiple sections for the conversion to work.');
	 return;
      }
   }
   ob_start();
   $prc->WriteToStdout();
   $prc = ob_get_contents();
   ob_end_clean();
   $SavedInfo['Data'] = $prc;

   $key = $fileName;
   $num = 1;
   while (isset($SavedPDB[$key])) {
      $num ++;
      $key = $fileName . '-' . $num;
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
    <td><a href="<?PHP echo $MyFilename
       ?>?action=erase&file=<?PHP echo $key ?>">Erase</a></td>
  </tr>
<?PHP
      }

      echo "</table>\n";
   }
}


function ErasePDB() {
   global $file, $SavedPDB;

   if (isset($SavedPDB) && is_array($SavedPDB) && isset($SavedPDB[$file]))
      unset($SavedPDB[$file]);
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

   $filename = $file . '.pdb';

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
