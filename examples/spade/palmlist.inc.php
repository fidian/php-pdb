<?

//
// sorts record array alphabetically
//
function cmp($r1, $r2)
{
  $a = strtolower($r1['LastName']);
  $b = strtolower($r2['LastName']);

  if ($a == $b)
    return 0;
    
  return ($a > $b ? 1 : -1);
}


$addr = new PalmAddress();
$fp = fopen($pdbfile,'r');
$addr->ReadFile($fp);
fclose($fp);

//
// records do not refer to real category ids, but to their
// sequence number
//
//foreach ($addr->GetCategoryList() as $cat)
//  $catlist[] = $cat;
  
//
// clean labels from \0
//
foreach ($addr->GetFieldLabels() as $key => $val)
  $labels[$key] = rtrim($val, "\0");

//
// read all records in a custom array (for sorting)
// and add ID and attributes to each record
//
foreach($addr->GetRecordIDs() as $ID)
{
  $recs[] = $addr->GetRecordRaw($ID);
  $recs[sizeof($recs) - 1]['ID'] = $ID;
  $recs[sizeof($recs) - 1]['attr'] = $addr->GetRecordAttrib($ID);
}


usort($recs, "cmp");

//
// html output starts here
//

//
// small table for overhead stuff
//

echo "<table width='100%' cellpadding=0 cellspacing=0>\n";

echo "<tr><td class='listheader' align=left>\n";
echo "<a href=\"$PHP_SELF?action=add\">add record</a></td>\n";

//
// it is possible to display only one category
//

echo "<td class='listheader' align=right>filter:&nbsp";

//
// the category list also contains empty fields, skip them
//
foreach ($addr->GetCategoryList() as $catkey => $cat)
  if ($cat['Name'] != "")
    echo "[<a href=\"$PHP_SELF?action=list&filter=$catkey\">".$cat['Name']."</a>]&nbsp;\n";
    
echo "<a href=\"$PHP_SELF?action=list\">[filter off]</a></td></tr>\n";

echo "</table><p>\n";

//
// outer table for cell frame
//
echo "<table border=0 cellspacing=0 cellpadding=0><tr><td class='frametable'>\n";


echo "<table cellpadding=1 cellspacing=1>\n";

foreach ($recs as $record)
{
  //
  // get record attributes from array
  //
  $attributes = $addr->RecordAttrs[$record['ID']];

  //
  // category is encoded by some bits within the attribute value
  //
  $catnum = $attributes & PDB_CATEGORY_MASK;
  
  //
  // categories are arranged in the array $addr->CategoryList
  // each field has a name and an ID. an address record does not
  // refer to the ID, but to the index of the category within
  // that array! so $catnum is just the array key and not the 
  // attribute's ID!
  //
  $cat = $addr->CategoryList[$catnum];
 
  if ((isset($filter)) && ($filter != "") && ($filter != $catnum))
    continue;
    
  $name = $record['LastName']." ".$record['FirstName'];
  if ($name == " ")
    $name = $record['Company'];
  if ($name == "")
    $name = "<i>Unknown</i>";
  
  $address = $record['Address'];
  
  if (strlen($record['ZipCode']) > 0)
  {
    if (strlen($address) > 0)
      $address .= ", ";
    $address .= $record['ZipCode'];
  }
  
  if (strlen($record['City']) > 0)
  {
    if (strlen($address) > 0)
      $address .= ", ";
    $address .= $record['City'];
  }
  
  if (strlen($record['Country']) > 0)
  {
    if (strlen($address) > 0)
      $address .= ", ";
    $address .= $record['Country'];
  }

  if ($address == "")
    $address = "&nbsp;";

  $phone = "";
  $email = "";
  
  //
  // decide about row style and what to put in last column
  //
  if ($attributes & PDB_RECORD_ATTRIB_DELETED)
  {
    $style = "delfield";
    $edited = "deleted";
  }
  else if ($attributes & PDB_RECORD_ATTRIB_DIRTY)
  {
    $edited = "edited";
    $style = "editedfield";
  }
  else
  {
    $edited = "&nbsp;";
    $style = "field";
  }

  //
  // we got five phone fields. put all phonenumbers in one field and
  // all emails in a separate one
  //
  for ($i = 1; $i < 6; $i++)
  {
    $phonekey = "Phone".$i;
    $phonelabelkey = "Phone".$i."Type";
    
    //
    // ignore empty fields
    //
    if (strlen($record[$phonekey]) > 0)
    {
      $rec = $record[$phonekey];
      //
      // we have to find out the real label identifier
      // first, we take the label number of this phone entry
      //
      $lkey = $record[$phonelabelkey] + 1;
      //
      // each number (+1) is an
      // index to the general label array, where the label names are stored
      // the keys to the phone strings of this general label array
      // are 'Phone1' to 'Phone8', we have to construct them!
      //
      $labelkey = "Phone".$lkey;

      //
      // email addresses go in a separate field
      //
      if ($lkey - 1 == PDB_ADDR_LABEL_EMAIL)
        $email .= "<a href=\"mailto:$rec\">$rec</a><br>";
      else
        $phone .= "$labels[$labelkey]: $rec<br>";
    }
  }

  if ($phone == "")
    $phone = "&nbsp;";

  if ($email == "")
    $email = "&nbsp;";
  
  //
  // we got our row, so print it!
  //
  echo "<tr>";
  if (! isset($filter))
      $filter = "";
  echo "<td class='$style'><a class='name' href=\"$PHP_SELF?action=edit&filter=$filter&id=".$record['ID']."\">$name</a></td>";
  echo "<td class='$style'>$address</td>";
  echo "<td class='$style'>$phone</td>";
  echo "<td class='$style'>$email</td>";
  echo "<td class='$style' align=center>".$cat['Name']."</td>";
  echo "<td class='$style' align=center><i>$edited</i></td>";
  echo "</tr>\n";
}

echo "</table>\n";

//
// outer table end:
//
echo "</td></tr></table>";

?>
