<?PHP

include "php-pdb.inc";
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
<li>Doc = <?PHP PassFail(DocTest()) ?></li>
</body></html>
<?PHP

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
   ob_start();
   $d->WriteToStdout();
   $file = ob_get_contents();
   ob_end_clean();
   
   if (md5($file) == '9a4383eff4c8857f6577ccc7ee714a44')
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
