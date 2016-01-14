<?php
/* Testing program to make sure PHP-PDB will work on this system
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more legal information
 * See https://github.com/fidian/php-pdb for more information about the library
 */


// Turn on all error reporting
ini_set('error_reporting', E_ALL);
include'./php-pdb.inc';
include'./modules/addrbook.inc';
include'./modules/datebook.inc';
include'./modules/doc.inc';
include'./modules/list.inc';
include'./modules/memo.inc';
include'./modules/smallbasic.inc';
include'./modules/todo.inc';
$Tests = array();
$TestType = 'Unknown';


/*
 * Perform the tests
 * Cache the results
 */

?>
<html><head><title>PHP-PDB Testing</title>
<body bgcolor="#FFFFFF">
<?php

ob_start();

?>
<h1><a name="Data"></a>Data conversion testing</h1><?php $TestType = 'Data' ?>
<ul>
<li>Int8 (Write) = <?php

PassFail('return PalmDB::Int8(40);', '28') ?></li>
<li>Int8 (Read) = <?php

PassFail('return PalmDB::LoadInt8(pack(\'H*\', \'28\'));', 40) ?></li>
<li>Int16 (Write) = <?php

PassFail('return PalmDB::Int16(1796);', '0704') ?></li>
<li>Int16 (Read) = <?php

PassFail('return PalmDB::LoadInt16(pack(\'H*\', \'0704\'));', 1796) ?></li>
<li>Int32 (Write) = <?php

PassFail('return PalmDB::Int32(40195090);', '02655412') ?></li>
<li>Int32 (Read) = <?php

PassFail('return PalmDB::LoadInt32(pack(\'H*\', \'02655412\'));', 40195090) ?></li>
<?php


// Double() refers to $this, so it must be in an instantiated class
$dbl = new PalmDB();
$Fail = 0;

?>
<li>Double (Write) = <?php

$Fail += PassFail('$dbl = new PalmDB();
                     return $dbl->Double(10.53);', '40250f5c28f5c28f') ?></li>
<li>Double (Read) = <?php

$Fail += PassFail('$dbl = new PalmDB();
                     return $dbl->LoadDouble(pack(\'H*\', \'40250f5c28f5c28f\'));', 10.53);

if ($Fail) {
    echo '<br>Don\'t worry -- this method is not used by anything in ' . 'the PHP-PDB library yet.  It is a utility function that is ' . 'added for completeness.  If you absolutely do need this ' . 'function, take a look at the PHP-PDB source or just submit ' . "a bug report.\n";
}

?></li>
<li>String (Write) = <?php

PassFail('return PalmDB::String(\'abcd\', \'3\');', '616263') ?></li>
<li>String (Read) = <?php

PassFail('$p = new PalmDB();
        return $p->LoadString(pack(\'H*\', \'6162630000\'), 4);', 'abc'); ?></li>
</ul>
<h1><a name="Modules"></a>Modules</h1><?php $TestType = 'Modules' ?>
<ul>
<li>Addresses = <?php PassFail('return AddressbookTest();', '3ce11d66da0a869632623f000460374a') ?></li>
<li>Datebook = <?php PassFail('return DatebookTest();', 'acb80f080d5d8161fb6651e0fc0310df') ?></li>
<li>Doc = <?php PassFail('return DocTest(false);', '91fa7442b46075d8ff451c58457d7246') ?></li>
<li>Doc (compressed) = <?php PassFail('return DocTest(true);', 'c5d02609dcfd8e565318da246226cd64') ?></li>
<li>List = <?php PassFail('return ListTest();', 'dfcb278a4508f33a3e6a0ba288d5d49e') ?></li>
<li>Memo = <?php PassFail('return MemoTest();', '3066a78037a2c7394605d9a6b2af4e48') ?></li>
<li>SmallBASIC = <?php PassFail('return SmallBASICTest();', 'e680fa5719b5ca1d7408148e2d8c7b43') ?></li>
<li>Todo = <?php PassFail('return TodoTest();', 'd009e145a7633a33f1376712e3a6bc12') ?></li>
</ul>
<?php

$results = ob_get_contents();
ob_end_clean();


/*
 * Create the summary table
 */

?>
<h1>Summary</h1>
<table align=center bgcolor="#EFEFFF" border=1 cellpadding=10 cellspacing=0>
<tr bgcolor="#C0C0FF"><th>Test Type</th>
<th bgcolor="#C0FFC0">Pass</th><th bgcolor="#FFC0C0">Fail</th><th>% Working</th></tr>
<?php

$TestTotalPass = 0;
$TestTotalFail = 0;

foreach ($Tests as $Type => $Test) {
    $Percent = $Test['Pass'] / ($Test['Pass'] + $Test['Fail']);
    $Percent *= 100;
    $Percent = intval($Percent);
    echo '<tr';

    if ($Test['Fail'])echo ' bgcolor="#FFDFDF"';
    echo '><td><a href="#' . $Type . '">' . $Type . '</a></td><td align=center bgcolor="#EFFFEF">' . $Test['Pass'] . '</td><td align=center bgcolor="#FFEFEF">' . $Test['Fail'] . '</td><td align=center>' . $Percent . " %</td></tr>\n";
    $TestTotalPass += $Test['Pass'];
    $TestTotalFail += $Test['Fail'];
}

?><tr bgcolor="#C0C0FF"><td><b>Total</b></td>
<td align=center bgcolor="#C0FFC0"><b><?php echo $TestTotalPass

?></b></td><td align=center bgcolor="#FFC0C0"><b><?php echo $TestTotalFail

?></b></td><td align=center><b><?php

$Percent = $TestTotalPass / ($TestTotalPass + $TestTotalFail);
$Percent *= 100;
$Percent = intval($Percent);
echo $Percent;

?> %</b></td></tr>
</table>
<?php

/*
 * If there were failures,
 * Then print out some cool information
 * Also print out cached test results here
 */
if ($TestTotalFail) {

    ?>
<p align=center><a href="#SolveProblems">Click Here</a> for information
about test failures and what should be done about them.</p>
<?php
}

echo $results;

if ($TestTotalFail) {

    ?>
<h1><a name="SolveProblems"></a>Test failure information</h1>

<p>Please perform these steps, in order, to resolve the problem.</p>

<ol>
<li>Make sure that you are running the latest version of <a
href="https://github.com/fidian/php-pdb">PHP-PDB</a>.  If you are not,
upgrade.</li>
<li>Determine which tests failed.  Just look above this section for anything
that says "<FONT color="red">fail</font>".</li>
  <ul>
  <li>If it was in the Data section and it was not the Double test, then
  something is <b>seriously</b> wrong.  Please send us a bug report
  immediately.</li>
  <li>If it was the Double test in the Data section, then you do not need
  to worry.  Nothing in the PHP-PDB library uses this function (yet), but
  a class might be written to use the Double function.  It is a good idea
  to get the problem fixed, just in case you wish to use double-precision
  numbers saved in your generated databases.  If you do report the problem,
  please include the entire error message that was generated (the <i>Wanted
  "abcdefg" but got "1234567"</i> message).</li>
  <li>If it was in the Modules section, and if you don't need that module,
  then you can safely ignore the warning, but it would be really nice if you
  reported the error to us.  (Just the fact that module <i>X</i> failed --
  we don't need the long string of letters and numbers.)</li>
  <li>If you can't see the error, then something is wacky.  Weird.  Try
  looking at the Summary table (at the top) to see which section the problem
  is in.</li>
  </ul>
<li>Report the problem to us in the <a
href="https://github.com/fidian/php-pdb/issues">Issue Tracker</a></li>
</ol>

<?php
}

?>
</body></html>
<?php

/*////////////////////////////////////
 * Thus began the testing functions //
 * ////////////////////////////////////
 *
 * Address book
 */
function AddressbookTest() {
    $addr = new PalmAddress();

    // Create some categories
    $categorias = array(
        1 => 'VIP',
        'AAA',
        'Inicial'
    );
    $addr->SetCategoryList($categorias);

    // Add one entry
    $fields = array(
        'LastName' => 'Pascual',
        'FirstName' => 'Eduardo',
        'Phone1' => '21221552',
        'Phone2' => '58808912',
        'Phone5' => 'epascual@cie.com.mx',
        'Address' => 'Hda. la Florida 10A',
        'City' => 'Izcalli'
    );
    $addr->SetRecordRaw($fields);
    $addr->GoToRecord('+1');

    // Add another
    $fields = array(
        'LastName' => 'de tal',
        'FirstName' => 'fulanito',
        'Address' => 'Direccion',
        'Phone1' => '21232425',
        'Phone2' => 'fulanito@dondesea.com',
        'Phone1Type' => PDB_ADDR_LABEL_HOME,
        'Phone2Type' => PDB_ADDR_LABEL_EMAIL,
        'Phone3Type' => PDB_ADDR_LABEL_WORK,
        'Phone4Type' => PDB_ADDR_LABEL_FAX,
        'Phone5Type' => PDB_ADDR_LABEL_OTHER,
        'Display' => 1,
        'Reserved' => ''
    );
    $addr->SetRecordRaw($fields);
    $addr->SetRecordAttrib(PDB_RECORD_ATTRIB_PRIVATE | 1);  // Private record, category 1
    return GenerateMd5($addr);
}

/*
 * Datebook
 */
function DatebookTest() {
    $d = new PalmDatebook();

    /* Create a repeating event that happens every other Friday.
     * I want it to be an all-day event that says "Pay Day!" */
    $Repeat = array(
        'Type' => PDB_DATEBOOK_REPEAT_WEEKLY,
        'Frequency' => 2,
        'Days' => '5',
        'StartOfWeek' => 0
    );
    $Record = array(
        'Date' => '2001-11-2',
        'Description' => 'Pay Day!',
        'Repeat' => $Repeat
    );

    // Add the record to the datebook
    $d->SetRecordRaw($Record);
    return GenerateMd5($d);
}

/*
 * DOC
 */
function DocTest($IsCompressed) {
    $d = new PalmDoc('Title Goes Here', $IsCompressed);
    $text = "Mary had a little lamb,\n" .
        "little lamb,\n" .
        "little lamb.\n" .
        "Mary had a little lamb,\n" .
        "its fleece as white as snow.\n" .
        "\n" .
        "It followed her to school one day,\n" .
        "school one day,\n" .
        "school one day.\n" .
        "It followed her to school one day.\n" .
        "and I hope this doc text test works well.\n" .
        "\n" .
        "(Yeah, I know.  It does not rhyme.)\n";;

    // Just in case the file is edited and the newlines are changed a bit.
    $text = str_replace("\r\n", "\n", $text);
    $text = str_replace("\r", "\n", $text);
    $text = explode("\n", $text);
    $newText = '';

    foreach ($text as $t) {
        trim($t);
        $newText .= $t . "\n";
    }

    $d->AddText($newText);
    return GenerateMd5($d);
}

/*
 * List
 */
function ListTest() {
    $d = new PalmListDB('List Test');
    $d->SetRecordRaw(array(
            'abc',
            '123',
            'Have Lots Of Fun!'
        ));
    return GenerateMd5($d);
}

/*
 * Memo
 */
function MemoTest() {
    $d = new PalmMemo();
    $d->SetText('Rolling along with the wind is no place to be.');
    return GenerateMd5($d);
}

/*
 * SmallBASIC
 */
function SmallBASICTest() {
    $d = new PalmSmallBASIC('pen.bas');
    $text = "! pen                     \n" .
        "\n" .
        "print \"Use /B (Graffiti) to exit\"\n" .
        "\n" .
        "pen on\n" .
        "while 1\n" .
        " if pen(0)\n" .
        "  line pen(4),pen(5)\n" .
        " fi\n" .
        "wend\n" .
        "pen off\n";;
    $text = str_replace('!', '\'', $text);
    $text = str_replace("\r\n", "\n", $text);
    $text = str_replace("\r", "\n", $text);
    $d->ConvertFromText($text);
    return GenerateMd5($d);
}

/*
 * Todo
 */
function TodoTest() {
    $todo = new PalmTodoList();

    // Add some categories
    $categorias = array(
        1 => 'Visita',
        'Fax',
        'Correo'
    );
    $todo->SetCategoryList($categorias);

    // Add a record
    $record = array(
        'Description' => 'Enviar Fax',
        'Note' => "25\nProbar palm",
        'Priority' => 2
    );
    $todo->SetRecordRaw($record);
    $todo->SetRecordAttrib(2);  // Category #2
    $todo->GoToRecord('+1');

    // Add another record
    $record = array(
        'Description' => 'Llamar a juan',
        'Note' => '35',
        'DueDate' => '2002-5-31'
    );
    $todo->SetRecordRaw($record);
    $todo->SetRecordAttrib(PDB_RECORD_ATTRIB_PRIVATE | PDB_RECORD_ATTRIB_DIRTY | 0);  // Two flags and category 0
    return GenerateMd5($todo);
}

/*////////////////////////////////////
 * Thus began the utility functions //
 * ////////////////////////////////////
 *
 * GenerateMd5
 */
function GenerateMd5(&$PalmDB, $DumpToScreen = false) {
    /* Change the dates so the header looks the same no matter when we
     * generate the file */
    $PalmDB->CreationTime = 1;
    $PalmDB->ModificationTime = 1;
    $PalmDB->BackupTime = 1;

    if (! $DumpToScreen)ob_start();
    $PalmDB->WriteToStdout();

    if (! $DumpToScreen) {
        $file = ob_get_contents();
        ob_end_clean();
        return md5($file);
    }

    return 'MD5 not generated';
}

/*
 * PassFail
 */
function PassFail($test, $want = false) {
    global $TestType, $Tests;

    // Run the test, and try to detect errors
    error_reporting(0);
    $test_1 = eval($test);
    error_reporting(E_ALL);
    $test_2 = eval($test);

    if (!isset($Tests[$TestType]))$Tests[$TestType] = array(
        'Fail' => 0,
        'Pass' => 0
    );
    $FailMsg = '';
    $Failure = false;

    if ($test_1 != $test_2) {
        $FailMsg = 'Warning or error encountered during test.';
        $Failure = true;
    } elseif ($want === false) {
        if ($test_1) {
            $FailMsg = "Data:  \"$test_1\"";
            $Failure = true;
        }
    } else {
        if ($test_1 != $want) {
            $FailMsg = "Wanted \"$want\" but got \"$test_1\"";
            $Failure = true;
        }
    }

    if ($Failure) {
        echo "<FONT color=\"red\">fail</font><br>$FailMsg";
        $Tests[$TestType]['Fail']++;
        return 1;
    }

    echo '<FONT color="green">pass</font>';
    $Tests[$TestType]['Pass']++;
    return 0;
}
