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

$Tests = array();
$TestType = 'Unknown';

?>
<html><head><title>PHP-PDB Testing</title>
<body bgcolor="#FFFFFF">
<h1>Data conversion testing</h1><?PHP $TestType = 'Data' ?>
<ul>
<li>Int8 = <?PHP PassFail(PalmDB::Int8(40), '28') ?></li>
<li>Int16 = <?PHP PassFail(PalmDB::Int16(1796), '0704') ?></li>
<li>Int32 = <?PHP PassFail(PalmDB::Int32(40195090), '02655412') ?></li>
<li>Double = <?PHP PassFail(PalmDB::Double(10.53), '40250f5c28f5c28f') ?></li>
<li>String = <?PHP PassFail(PalmDB::String('abcd', '3'), '616263') ?></li>
</ul>
<h1>Modules</h1><?PHP $TestType = 'Modules' ?>
<ul>
<li>Datebook = <?PHP PassFail(DatebookTest(), 'be8223a89be8face9c4734df74cdec32') ?></li>
<li>Doc = <?PHP PassFail(DocTest(), '56fa283daa40aa6d547cb866b46ef368') ?></li>
</ul>
<h1>Summary</h1>
<table align=center bgcolor="#EFEFFF" border=1 cellpadding=10 cellspacing=0>
<tr><th>Test Type</th><th>Pass</th><th>Fail</th><th>% Working</th></tr>
<?PHP
   $TestTotalPass = 0;
   $TestTotalFail = 0;
   
   foreach ($Tests as $Type => $Test) {
      $Percent = $Test['Pass'] / ($Test['Pass'] + $Test['Fail']);
      $Percent *= 100;
      $Percent = intval($Percent);
      echo "<tr><td>" . $Type . "</td><td align=center>" . 
         $Test['Pass'] . "</td><td align=center>" .
         $Test['Fail'] . "</td><td align=center>" . $Percent . 
	 " %</td></tr>\n";
      $TestTotalPass += $Test['Pass'];
      $TestTotalFail += $Test['Fail'];
   }
?><tr><td><b>Total</b></td><td align=center><b><?PHP echo $TestTotalPass
?></b></td><td align=center><b><?PHP echo $TestTotalFail
?></b></td><td align=center><b><?PHP
   $Percent = $TestTotalPass / ($TestTotalPass + $TestTotalFail);
   $Percent *= 100;
   $Percent = intval($Percent);
   echo $Percent;
?> %</b></td></tr>
</table>
</body></html>
<?PHP


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


function DocTest() {
   $d = new PalmDoc("Title Goes Here");
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


function GenerateMd5(&$PalmDB) {
   // Change the dates so the header looks the same no matter when we
   // generate the file
   $PalmDB->CreationTime = 0;
   $PalmDB->ModificationTime = 0;
   $PalmDB->BackupTime = 0;
   
   ob_start();
   $PalmDB->WriteToStdout();
   $file = ob_get_contents();
   ob_end_clean();
   
   return md5($file);
}


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
