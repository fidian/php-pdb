<?

if (file_exists('../../../php-pdb.inc')) {
   include('../../../php-pdb.inc');
   include('../../../modules/addrbook.inc');
} elseif (file_exists('../php-pdb.inc')) {
   include('../php-pdb.inc');
   include('../modules/addrbook.inc');
}

include('../../functions.inc');

$pdbfile = "AddressDB.pdb";

function print_header()
{
  global $PHP_AUTH_USER, $PHP_AUTH_PW;

  include_once("auth.php");
  
  echo "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\">\n";
  echo "<html>\n";
  echo "<head>\n";

  echo "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=ISO-8859-1\">\n";
  echo "<meta http-equiv=\"pragma\" content=\"no-cache\">\n";
  echo "<title>palm data</title>\n";

  echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"styles.css\">\n";

  echo "</head>\n";

  echo "<body>\n";
  echo "<a href=\"../../index.php\">&lt;-- Back to PHP-PDB Homepage</a>\n";
}

function print_footer()
{
  echo "</body>\n";

  echo "</html>\n";
}


//
// default action is list 
//
if (!isset($action))
  $action = "list";
  

switch($action)
{
case "list":

  print_header();

  include("palmlist.inc.php");
  
  break;

case "add":

  print_header();

  //
  // include edit form, since no $id post variable is set here, it will know that no
  // record is to edit, but an empty one to display!
  //
  include("palmedit.inc.php");

  break;
  
case "edit":

  print_header();

  include("palmedit.inc.php");
  
  break;

case "update":

  $new = $_POST['new'];
  
  $addr = new PalmAddress();
  $fp = fopen($pdbfile,'r');
  $addr->ReadFile($fp);
  fclose($fp);

  $addr->GoToRecord($id);
  
  //
  // did we add a new one or edited existing?
  //
  if (in_array($id, $addr->GetRecordIDs()))
  {
    //
    // yes, it does exist. take record and update all
    // edited fields.
    //
    $edited = $addr->GetRecordRaw();

    foreach($edited as $key => $val)
      if (array_key_exists($key, $new))
        $edited[$key] = $new[$key];
  }
  else
  {
    //
    // new record - take it entirely
    //
    $edited = $new;
  }
  
  //
  // set modification timestamp?
  //

  // $addr->ModificationTime = time();

  $addr->SetRecordRaw($edited);
  
  //
  // reset the category bits of attributes
  //
  $attr = $addr->GetRecordAttrib() & ~PDB_CATEGORY_MASK;

  //
  // and set the edited ones
  //
  $attr = $attr | $_POST['category'];

  //
  // mark it dirty to indicate sync needed
  // (at least this works for coldsync)
  //
  $attr = $attr | PDB_RECORD_ATTRIB_DIRTY;

  $addr->SetRecordAttrib($attr);

  $fp = fopen($pdbfile,'w');
  $addr->WriteToFile($fp);
  fclose($fp);

  //
  // display list
  //
  header("Location: $PHP_SELF?filter=$filter");
  
  break;

case "delete":

  $addr = new PalmAddress();
  $fp = fopen($pdbfile,'r');
  $addr->ReadFile($fp);
  fclose($fp);

  //
  // do we have a valid and existing id?
  //
  if ((isset($id)) && (in_array($id, $addr->GetRecordIDs())))
  {
    $addr->GoToRecord($id);

    //
    // mark record deleted - at next syncing, it will be purged
    //
    $addr->SetRecordAttrib($addr->GetRecordAttrib() | PDB_RECORD_ATTRIB_DELETED);
  
    $fp = fopen($pdbfile,'w');
    $addr->WriteToFile($fp);
    fclose($fp);
  }
  
  header("Location: $PHP_SELF?filter=$filter");
  
  break;
  
case "undelete":

  //
  // undelete means to unmark delete flag, works of course only BEFORE
  // syncing!
  //

  $addr = new PalmAddress();
  $fp = fopen($pdbfile,'r');
  $addr->ReadFile($fp);
  fclose($fp);

  //
  // do we have a valid id?
  //
  if ((isset($id)) && (in_array($id, $addr->GetRecordIDs())))
  {
    $addr->GoToRecord($id);

    $addr->SetRecordAttrib($addr->GetRecordAttrib() & ~PDB_RECORD_ATTRIB_DELETED);
  
    $fp = fopen($pdbfile,'w');
    $addr->WriteToFile($fp);
    fclose($fp);
  }
  
  header("Location: $PHP_SELF?filter=$filter");
  
  break;
  
default:
  break;
}


?>
