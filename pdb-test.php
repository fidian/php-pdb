<?PHP
/* Testing program to make sure PHP-PDB will work on this system
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more legal information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */

// Turn on all error reporting
ini_set('error_reporting', E_ALL);

include "./php-pdb.inc";
include "./modules/datebook.inc";
include "./modules/doc.inc";
include "./modules/smallbasic.inc";

$Tests = array();
$TestType = 'Unknown';



//
// Perform the tests
// Cache the results
//

?>
<html><head><title>PHP-PDB Testing</title>
<body bgcolor="#FFFFFF">
<?PHP

ob_start();

?>
<h1><a name="Data"></a>Data conversion testing</h1><?PHP $TestType = 'Data' ?>
<ul>
<li>Int8 = <?PHP PassFail(PalmDB::Int8(40), '28') ?></li>
<li>Int16 = <?PHP PassFail(PalmDB::Int16(1796), '0704') ?></li>
<li>Int32 = <?PHP PassFail(PalmDB::Int32(40195090), '02655412') ?></li>
<li>Double = <?PHP PassFail(PalmDB::Double(10.53), '40250f5c28f5c28f') ?></li>
<li>String = <?PHP PassFail(PalmDB::String('abcd', '3'), '616263') ?></li>
</ul>
<h1><a name="Modules"></a>Modules</h1><?PHP $TestType = 'Modules' ?>
<ul>
<li>Datebook = <?PHP PassFail(DatebookTest(), 
                              'acb80f080d5d8161fb6651e0fc0310df') ?></li>
<li>Doc = <?PHP PassFail(DocTest(false),
                         'ed869c7a31e720537f759fcc88d8c447') ?></li>
<li>Doc (compressed) = <?PHP PassFail(DocTest(true),
                         '26664289f16ba7e0ebbfeb3663babd86') ?></li>
<li>SmallBASIC = <?PHP PassFail(SmallBASICTest(),
                         '8d96040782a8fc085bc7cdddd4bb52ed') ?></li>
</ul>
<?PHP

$results = ob_get_contents();
ob_end_clean();



//
// Create the summary table
//

?>
<h1>Summary</h1>
<table align=center bgcolor="#EFEFFF" border=1 cellpadding=10 cellspacing=0>
<tr bgcolor="#C0C0FF"><th>Test Type</th>
<th bgcolor="#C0FFC0">Pass</th><th bgcolor="#FFC0C0">Fail</th><th>% Working</th></tr>
<?PHP
   $TestTotalPass = 0;
   $TestTotalFail = 0;
   
   foreach ($Tests as $Type => $Test) {
      $Percent = $Test['Pass'] / ($Test['Pass'] + $Test['Fail']);
      $Percent *= 100;
      $Percent = intval($Percent);
      echo "<tr";
      if ($Test['Fail'])
         echo " bgcolor=\"#FFDFDF\"";
      echo "><td><a href=\"#" . $Type . "\">" . $Type . 
         "</a></td><td align=center bgcolor=\"#EFFFEF\">" .
         $Test['Pass'] . "</td><td align=center bgcolor=\"#FFEFEF\">" .
         $Test['Fail'] . "</td><td align=center>" . $Percent . 
	 " %</td></tr>\n";
      $TestTotalPass += $Test['Pass'];
      $TestTotalFail += $Test['Fail'];
   }
?><tr bgcolor="#C0C0FF"><td><b>Total</b></td>
<td align=center bgcolor="#C0FFC0"><b><?PHP echo $TestTotalPass
?></b></td><td align=center bgcolor="#FFC0C0"><b><?PHP echo $TestTotalFail
?></b></td><td align=center><b><?PHP
   $Percent = $TestTotalPass / ($TestTotalPass + $TestTotalFail);
   $Percent *= 100;
   $Percent = intval($Percent);
   echo $Percent;
?> %</b></td></tr>
</table>
<?PHP 



//
// If there were failures,
// Then print out some cool information
// Also print out cached test results here
//

if ($TestTotalFail) {

?>
<p align=center><a href="#SolveProblems">Click Here</a> for information
about test failures and what should be done about them.</p>
<?PHP

}

echo $results;

if ($TestTotalFail) {

?>
<h1><a name="SolveProblems"></a>Test failure information</h1>

<p>Please perform these steps, in order, to resolve the problem.</p>

<ol>
<li>Make sure that you are running the latest version of <a
href="http://php-pdb.sourceforge.net">PHP-PDB</a>.  If you are not,
upgrade.</li>
<li>If the error still persists, and if you can, try the CVS version of
PHP-PDB.  See the <a href="http://php-pdb.sourceforge.net/download.php">
download page</a> for more information.</li>
<li>Determine which tests failed.  Just look above this section for anything
that says "<FONT color="red">fail</font>".</li>
  <ul>
  <li>If it was in the Data section, then something is <b>seriously</b> 
  wrong.  Please send us a bug report immediately.</li>
  <li>If it was in the Modules section, and if you don't need that module, 
  then you can safely ignore the warning, but it would be really nice if you
  reported the error to us.  (Just the fact that module <i>X</i> failed --
  we don't need the long string of letters and numbers.)</li>
  <li>If you can't see the error, then something is wacky.  Weird.  Try
  looking at the Summary table (at the top) to see which section the problem
  is in.</li>
  </ul>
<li>Report the problem to us in the <a
href="http://sourceforge.net/tracker/?atid=397207&group_id=29740&func=browse">Bug
Tracker</a></li>
</ol>

<?PHP
}

?>
</body></html>
<?PHP




//////////////////////////////////////
// Thus began the testing functions //
//////////////////////////////////////


//
// Datebook 
//

function DatebookTest() {
   $d = new PalmDatebook();

   // Create a repeating event that happens every other Friday.
   // I want it to be an all-day event that says "Pay Day!"
   $Repeat = array(
      "Type" => PDB_DATEBOOK_REPEAT_WEEKLY,
      "Frequency" => 2,
      "Days" => "5",
      "StartOfWeek" => 0
   );
   $Record = array(
      "Date" => "2001-11-2",
      "Description" => "Pay Day!",
      "Repeat" => $Repeat
   );
		     
   // Add the record to the datebook
   $d->SetRecordRaw($Record);
   
   return GenerateMd5($d);
}



//
// DOC
//

function DocTest($IsCompressed) {
   $d = new PalmDoc("Title Goes Here", $IsCompressed);
   $text = <<< EOS
Mary had a little lamb,
little lamb,
little lamb.
Mary had a little lamb,
its fleece as white as snow.

It followed her to school one day,
school one day,
school one day.
It followed her to school one day.
and I hope this doc text test works well.

(Yeah, I know.  It doesn't rhyme.)
EOS;
   $text = str_replace("\r\n", "\n", $text);
   $text = str_replace("\r", "\n", $text);
   $text = explode("\n", $text);
   $newText = '';
   foreach ($text as $t) {
      trim($t);
      $newText .= $t . "\n";
   }
   $d->AddDocText($newText);
 
   return GenerateMd5($d);
}



//
// SmallBASIC
//

function SmallBASICTest() {
   $d = new PalmSmallBASIC("pen.bas");
   $text = <<< EOS
' pen

print "Use /B (Graffiti) to exit"

pen on
while 1
 if pen(0)
  line pen(4),pen(5)
 fi
wend
pen off
EOS;
   $text = str_replace("\r\n", "\n", $text);
   $text = str_replace("\r", "\n", $text);
   $d->ConvertFromText($text);
 
   return GenerateMd5($d);
}



//////////////////////////////////////
// Thus began the utility functions //
//////////////////////////////////////

//
// GenerateMd5
//

function GenerateMd5(&$PalmDB) {
   // Change the dates so the header looks the same no matter when we
   // generate the file
   $PalmDB->CreationTime = 1;
   $PalmDB->ModificationTime = 1;
   $PalmDB->BackupTime = 1;
   
   ob_start();
   $PalmDB->WriteToStdout();
   $file = ob_get_contents();
   ob_end_clean();
   
   return md5($file);
}



//
// PassFail
//

function PassFail($test, $want = false) {
   global $TestType, $Tests;
   
   if (! isset($Tests[$TestType]))
      $Tests[$TestType] = array('Fail' => 0, 'Pass' => 0);
      
   $FailMsg = '';
   $Failure = false;
   
   if ($want === false) {
      if ($test) {
         $FailMsg = "Data:  \"$test\"";
	 $Failure = true;
      }
   } else {
      if ($test != $want) {
         $FailMsg = "Wanted \"$want\" but got \"$test\"";
         $Failure = true;
      }
   }
   
   if ($Failure) {
      echo "<FONT color=\"red\">fail</font><br>$FailMsg";
      $Tests[$TestType]['Fail'] ++;
   } else {
      echo "<FONT color=\"green\">pass</font>";
      $Tests[$TestType]['Pass'] ++;
   }
}

?>
