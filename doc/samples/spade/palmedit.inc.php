<?

//
// prints a drop-down box containig all possible phone labels
// $num is the phone field number (1-5) and $selected is the
// currently chosen label for this field
//
function printphonemenu($num, $selected)
{
  global $labels;
  
  echo "<select name='new[Phone".$num."Type]' size=1>\n";
  for ($j = 0; $j < 8; $j++)
  {
    if ($j == $selected)
      $opt = "option selected";
    else
      $opt = "option";

    $key = $j + 1;
    $keystr = "Phone".$key;
    
    echo "<$opt value=$j>".$labels[$keystr]."</option>\n";
  }
  echo "</select>\n";
}

//
// all edit table rows look similar, so we use a function
//
function printeditline($field)
{
  global $labels, $record, $gapwidth;

  echo "<tr>";
  echo "<td class='edit' align=right>".$labels[$field].":</td>";
  echo "<td class='edit'><img src='images/0.gif' height=1 width=$gapwidth></td>\n";
  echo "<td class='edit'>";
  echo "<input type='text' name='new[$field]' size='20' value='".$record[$field]."'></td>\n";
  echo "</tr>\n";

}

$addr = new PalmAddress();
$fp = fopen($pdbfile, 'r');
$addr->ReadFile($fp);
fclose($fp);

  
//
// clean labels from \0
//
foreach ($addr->GetFieldLabels() as $key => $val)
  $labels[$key] = rtrim($val, "\0");

//
// do we add a new record (= we didn't get an $id value)?
//
if (!isset($id))
{
  //
  // find out next id number
  //
  $allids = $addr->GetRecordIDs();
  
  sort($allids);
  
  $id = $allids[sizeof($allids) - 1] + 1;
  
  //
  // get empty record
  //
  $record = $addr->NewRecord();

  //
  // default category is 'unfiled'
  //
  $cat = 0;

  $caption = "add record";
}
else
{
  //
  // get record of $id
  //
  $record = $addr->GetRecordRaw($id);
  
  $attr = $addr->GetRecordAttrib($id);
  
  $cat = $attr & PDB_CATEGORY_MASK;

  $caption = "edit record";
}


//
// width between labels and edit text fields
// also used as gap between buttons
//
$gapwidth = 5;

//
// html output starts here
//

echo "<form name='editform' action='$PHP_SELF?action=update' method='post'>\n";

echo "<input type=\"hidden\" name=\"id\" value=\"$id\">\n";
echo "<input type=\"hidden\" name=\"filter\" value=\"$filter\">\n";

//
// outer table for cell frame
//
echo "<table border=0 cellspacing=0 cellpadding=0 align=center><tr><td class='frametable'>\n";

//
// middle table for edit field sections (header, fields, footer with button)
//
echo "<table border=0 cellpadding=3 cellspacing=1>\n";
echo "<tr>\n";

echo "<td class=editcaption align=center>$caption</td></tr>\n";

echo "<tr><td class=edit>\n";

//
// inner table for all edit fields
//
echo "<table border=0 cellpadding=2 cellspacing=0>\n";

  printeditline('LastName');
  printeditline('FirstName');
  printeditline('Title');
  printeditline('Company');

  //
  // print all five phone fields
  //
  for ($i = 1; $i < 6; $i++)
  {
    $keystr = "Phone".$i;
    
    echo "<tr>";
    echo "<td class='edit' align=right>\n";
    
    //
    // each of it has a drop-down box with all phone labels
    //
    printphonemenu($i, $record[$keystr."Type"]);
    
    echo "</td>";
    echo "<td class='edit'><img src='images/0.gif' height=1 width=$gapwidth></td>\n";
    echo "<td class='edit'>";
    echo "<input type='text' name='new[$keystr]' size='20' value='".$record[$keystr]."'>\n";
    echo "<img src='images/0.gif' height=1 width=$gapwidth>\n";

    if ($record['Display'] == $i - 1)
      $checked = " checked";
    else
      $checked = "";
    
    //
    // radio buttons to choose which phone entry is to be displayed
    //
    echo "<input type='radio' name='new[Display]' value=".($i - 1).$checked."></input></td>\n";
    echo "</tr>\n";
  }

  printeditline('Address');
  printeditline('City');
  printeditline('State');
  printeditline('ZipCode');
  printeditline('Country');
  printeditline('Custom1');
  printeditline('Custom2');
  printeditline('Custom3');
  printeditline('Custom4');

  echo "<tr>";
  echo "<td class='edit' align=right>Category:</td>";
  echo "<td class='edit'><img src='images/0.gif' height=1 width=$gapwidth></td>\n";
  echo "<td class='edit'>";

  //
  // print drop-down box for categories
  //
  echo "<select name='category' size=1>\n";
  
  $catlist = $addr->CategoryList;
  
  for ($i = 0; $i < sizeof($catlist); $i++)
  {
    if ($catlist[$i]['Name'] == "")
      continue;
      
    if ($i == $cat)
      $opt = "option selected";
    else
      $opt = "option";

    echo "<$opt value=$i>".$catlist[$i]['Name']."</option>\n";
  }
  
  echo "</select>\n";
  
  echo "</td></tr>\n";
  
echo "</table>";

//
// end of inner table, now the button row of middle table follows
//
echo "</td></tr>\n";

echo "<tr><td class=edit align=center>\n";

//
// if we add, we don't need a delete/undelete button
//
if ($action != "add")
{
  if ($attr & PDB_RECORD_ATTRIB_DELETED)
    echo "<input class=button type=\"submit\" name=\"action\" value=\"undelete\">\n";
  else
    echo "<input class=button type=\"submit\" name=\"action\" value=\"delete\">\n";
}
  
echo "<img src='images/0.gif' height=1 width=$gapwidth>\n";

echo "<input class=button type=\"submit\" name=\"action\" value=\"update\">\n";


echo "</td></tr></table>";

echo "</td></tr></table>";


echo "</form>\n";

?>
