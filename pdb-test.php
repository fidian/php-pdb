<?PHP

include "php-pdb.inc";
include "php-pdb-datebook.inc";
include "php-pdb-doc.inc";

?>
<html><head><title>PHP-PDB Testing</title>
<body bgcolor="#FFFFFF">
<h1>Data conversion testing</h1>
<ul>
<li>Int8 = <?PHP PassFail(PalmDB::Int8(40), '28') ?></li>
<li>Int16 = <?PHP PassFail(PalmDB::Int16(1796), '0704') ?></li>
<li>Int32 = <?PHP PassFail(PalmDB::Int32(40195090), '02655412') ?></li>
<li>Double = <?PHP PassFail(PalmDB::Double(10.53), '40250f5c28f5c28f') ?></li>
<li>String = <?PHP PassFail(PalmDB::String('abcd', '3'), '616263') ?></li>
</ul>
<h1>Modules</h1>
<ul>
<li>Datebook = <?PHP PassFail(DatebookTest()) ?></li>
<li>Doc = <?PHP PassFail(DocTest()) ?></li>
</ul>
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
   
   // Change the dates so the header looks the same no matter when we
   // generate the file
   $d->CreationTime = 0;
   $d->ModificationTime = 0;
   $d->BackupTime = 0;
   
   ob_start();
   $d->WriteToStdout();
   $file = ob_get_contents();
   ob_end_clean();

   if (md5($file) == 'b2b9ff7287dfc1e2a15a86a41a0291f3')
      return true;
   return false;
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
   
   // Change the dates so the header looks the same no matter when we
   // generate the file
   $d->CreationTime = 0;
   $d->ModificationTime = 0;
   $d->BackupTime = 0;
   
   ob_start();
   $d->WriteToStdout();
   $file = ob_get_contents();
   ob_end_clean();

   if (md5($file) == '0896109489fff87ad468cd4f32b8eb0b')
      return true;
   return false;
}


function PassFail($test, $want = false) {
   if (($want === false && $test) ||
       ($want !== false && $test == $want))
      echo "<FONT color=\"green\">pass</font>";
   else {
      echo "<FONT color=\"red\">fail</font>";
      if ($want !== false)
         echo " - Got \"$test\", wanted \"$want\".";
   }
}

?>
