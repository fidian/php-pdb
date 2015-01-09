PHP-PDB API
===========

PHP-PDB can create nearly any type of [Palm OS](http://www.palmos.com) database through the most basic functions, as illustrated here.  If you want to write a format of database that isn't directly supported by a module, I would suggest you check out the [custom formats](custom.md) page.


Including into your program
---------------------------

    include 'php-pdb.inc';


Creating a new database
-----------------------

To create a new database, you need to find out the correct type and creator ID to make the database with.  You should also specify the name of the database.

    $DB = new PalmDB("Type", "Creator", "Name Of Database");
      // Typical usage

    $pdb = new PalmDB();
      // Special:  If you want to create an instance of the class
      // and then use ReadFile() to load the database information


Writing the database
--------------------

**Writing to a file:**  It might be important to open the file with `wb` as the mode.  Unix, Linux, and others don't care about the `b`, but all versions of Windows do care.

    // Writing to a file
    $fp = fopen("output_file.pdb", "wb");
    if (! $fp) exit;  // There was an error opening the file
    $pdb->WriteToFile($fp);
    fclose($fp);

**Writing to stdout:**  If you want the file to just be piped to the standard output, as though you were just using `echo` to dump the file, it is a bit simpler.

    // Writing to standard output
    $pdb->WriteToStdout();

**Download through browser:**  If your PHP script just generates a `.pdb` file to be downloaded through the browser, you can just call this function and it will generate the correct file download headers and send the file to the browser.  Only call this function if you didn't already send any content.  There are a few known issues with IE and file downloads, so the headers that are generated should work with the newest versions of IE and might not work with older ones.  It's not my fault -- see Microsoft technical support.

    // Download through browser
    // Specify filename you want the .pdb saved as on the remote computer
    $pdb->DownloadPDB("filename.pdb");


Loading the database
--------------------

This function reads data from the opened file and tries to load it properly.  Returns false if there is no error.

    // Create a dummy instance of the class
$addr = new PalmDB("test", "test");
// Load a file
$fp = fopen("your_file.pdb", "r");
$addr->ReadFile($fp);
fclose($fp);

// Show information about the database
echo "Name: $addr->Name<br>\n";
echo "Type: $addr->TypeID<br>";
echo "Creator: $addr->CreatorID<br>\n";
echo "Attributes: $addr->Attributes<br>\n";
echo "Version: $addr->Version<br>\n";
echo "ModNum: $addr->ModNumber<br>\n";
echo "CreationTime: $addr->CreationTime<br>\n";
echo "ModTime: $addr->ModificationTime<br>\n";
echo "BackTime: $addr->BackupTime<br>\n";
echo "NumRec: ".$addr->GetRecordCount()."<br>\n";
$recids = $addr->GetRecordIDs();
foreach ($recids as $ID) {
  $record = $addr->GetRecordRaw($ID);
  echo "Record $ID:<BR>";
  // Depending on what the record stored, you might be able to do this:
  // echo "- Data: " . $record
}


Category Support and Record Attributes
--------------------------------------

If you plan on using categories, you will need to understand a bit of tricky information.  The attributes byte also has the category information in it.  The lower four bits (0x0F) usually are the category number (0 - 15), but if the record is deleted or expunged, then it is no longer category information and the fourth bit (0x08) is a flag for if the record should be archived or not.  This may be a bit confusing, so here's some code to help you out:

// Get the record attributes
$attr = $pdb->GetRecordAttrib();

// If the record is deleted or expunged ...
if ($attr & PDB_RECORD_ATTRIB_DEL_EXP) {
   // Check if the record should be archived
   if ($attr & PDB_RECORD_ATTRIB_ARCHIVE)
      echo "Record is deleted/expunged and should be archived.\n";
   else
      echo "Record is deleted/expunged and should not be archived.\n";
} else {
   // If the record is not deleted/expunged, then the lower four
   // bits are the category number.
   echo "Record is not deleted/expunged.\n";
   echo "Record category number = " . ($attr & PDB_CATEGORY_MASK) . "\n";
}

This little bit of code might help you figure out more information about the attributes.

$attrib = $pdb->GetRecordAttrib();

// Show which attributes are there
if ($attrib & PDB_RECORD_ATTRIB_EXPUNGED) echo "Record is expunged.<br>\n";
if ($attrib & PDB_RECORD_ATTRIB_DELETED) echo "Record is deleted.<br>\n";
if ($attrib & PDB_RECORD_ATTRIB_DIRTY) echo "Record is dirty.<br>\n";
if ($attrib & PDB_RECORD_ATTRIB_PRIVATE) echo "Record is private.<br>\n";
if ($attrib & PDB_RECORD_ATTRIB_DEL_EXP) {
   // The archive bit is only set if the record is deleted or expunged.
   // Otherwise, the lower bits specify the category.
   if ($attrib & PDB_ADDR_ATTRIB_ARCHIVE)
      echo "Record is marked to be archived.<br>\n";
}

In order to define/see what the 16 categories are, you use the `SetCategoryList()` and `GetCategoryList()` functions.  See `SetCategoryList()` for special rules that apply to the array of data.  If you wish to include category support in your module/application, you will need to create an extender class and add `LoadAppInfo()` and `GetAppInfo()` functions which load/save the category data correctly:

// LoadAppInfo() example
function LoadAppInfo($fileData) {
   // Load category data
   $this->LoadCategoryData($fileData);
   // Skip past category data
   $fileData = substr($fileData, PDB_CATEGORY_SIZE);

   // .... rest of your code goes here
}


// GetAppInfo() example
function GetAppInfo() {
   $AppInfo = $this->CreateCategoryData();

   // .... rest of your code goes here
   // .... append data to the $AppInfo string

   return $AppInfo;
}

Here is a more thorough example that illustrates how the category numbers, category IDs, records, attributes and various other things all work together.  For this to work properly, make sure that there is a file called `AddressDB.pdb` and make sure that it is an address book database.

include "php-pdb.inc";            // Load PHP-PDB class
include "modules/addrbook.inc";   // Load Addressbook module

// Load address book
$addr = new PalmAddress();
$fp = fopen("AddressDB.pdb","r");
$addr->ReadFile($fp);
fclose($fp);

// Fill the data array with entries from the address book.
// $data["Category String"] = array("Person/Entry", "Person/Entry", ...)

$data = array();

// Preload the categories
$Categories = $addr->GetCategoryList();

// Record keys
$RecordNumbers = $addr->GetRecordIDs();

foreach ($RecordNumbers as $Rec)
{
    // Go to the record
    $addr->GoToRecord($Rec);

    $attrs = $addr->GetRecordAttrib();

    // If this record is not deleted or expunged, show it
    if (! ($attrs & PDB_RECORD_ATTRIB_DEL_EXP))
    {
       // Build the name
       $record = $addr->GetRecordRaw();
       $Index = "";
       if (isset($record["FirstName"]) && $record["FirstName"] != "")
          $Index = $record["FirstName"];
       if (isset($record["LastName"]) && $record["LastName"] != "")
       {
          if ($Index != "")
             $Index = $record["LastName"] . ", " . $Index;
          else
             $Index = $record["LastName"];
       }
       if (isset($record["Company"]) && $record["Company"] != "")
       {
          if ($Index != "")
             $Index .= " @ " . $record["Company"];
          else
             $Index = $record["Company"];
       }

       // Build the value for the $data array
       $cat = $Categories[$attrs & PDB_CATEGORY_MASK];
       $Cate = $cat["Name"] . " (ID # " . $cat["ID"] . ")";

       // Add entry
       if (! isset($data[$Cate]))
          $data[$Cate] = array();
       $data[$Cate][] = $Index;
    }
}

// Dump the data
ksort($data);
foreach ($data as $name => $arr)
{
   echo "<b>$name</b><br>\n";

   // Sort alphabetically
   sort($arr);

   foreach ($arr as $entry)
      echo " &nbsp; $entry<br>\n";
}

Check out the explanations for the `SetRecordAttrib()`, `GetRecordAttrib()`, `GetCategoryList()`, `SetCategoryList()`, `CreateCategoryData()`, and `LoadCategoryData()` functions below.


Other functions
---------------

This is not a comprehensive list.  There are other functions in the main class, but they are primarily used for converting data and would be best used in modules.  Read the source if you want to see what the rest of the functions are.


### `GoToRecord($num = false)`

If `$num` is not specified, returns the current record number.

If `$num` is specified and is a string with '+' or '-' as its first character, jumps ahead or back the specified number of records.  Returns the old record number.

If `$num` is a number, jumps directly to the specified record.  Returns the old record number.


### `GetRecordSize($num = false)`

If `$num` is not specified, returns the size of the current record.

If `$num` is specified, returns the size of the specified record.


### `AppendInt8($value)`

Appends a single byte to the current record.


### `AppendInt16($value)`

Appends an integer (two bytes) to the current record.


### `AppendInt32($value)`

Appends a long (four bytes) to the current record.


### `AppendDouble($value)`

Appends a double (eight bytes, floating point) to the current record.


### `AppendString($string, $maxLen = false)`

Appends a string to the current record.  The string is not null terminated.  If `$maxLen` is specified and is greater than zero, the string is trimmed and will contain up to `$maxLen` characters.


### `RecordExists($record = false)`

If `$record` is not specified, returns true if the current record exists and is set.

If `$record` is specified, returns true if the specified record exists and is set.



### `GetRecord($record = false)`

Returns the data for the record, or the current record if `$record` is not specified.  If the desired record doesn't contain data, returns ''.  `GetRecordRaw()` might be more useful, since this may be overridden to return compressed and/or encoded data.


### `GetRecordRaw($record = false)`

If `$record` is not specified, returns the raw data of the current record, or false if the current record doesn't yet exist.

If `$record` is specified, returns the raw data of the record, or false if the record doesn't exist.


### `SetRecordRaw($data)`

Sets the raw data for the current record to be `$data.


###` `SetRecordRaw($record, $data)`

Sets the raw data for `$record` to be `$data.


###` `DeleteCurrentRecord()`

Erases and unsets the current record .


### `GetRecordIDs()`

Returns a list of the set record IDs in the order that they should be written.


### `GetRecordCount()`

Returns the number of records.  This should match the number of keys returned by `GetRecordIDs()`.


### `GetRecordAttrib($record = false)`

If `$record` is specified, this returns the attributes bitmask for the specified record if it exists.  Otherwise, it returns 0.

If `$record` is not specified, this returns the attributes bitmask for the current record.


### `SetRecordAttrib($attr)`

Sets the current record's attributes to `$attr.


###` `SetRecordAttrib($record, $attr)`

Sets the specified record's attributes to `$attr.


###` `GetCategoryList()`

Returns an array containing the different categories.  See `SetCategoryList()` for the format of the array.


### `SetCategoryList($data)`

Sets the categories to what you specified.  `$data` is in one of two different formats:

    // Easy way:
    $data[$id] = "Name";

    // Harder, but this is how GetCategoryList() returns data
    $data[$index] = array('Name' => "Name",
                          'Renamed' => false,
                          'ID' => 27);

    /* $index is a number from 0 to 15.
     * "Name" is the category's name
     * The Renamed flag is true/false.  Not sure what it does.
     * ID is the unique ID given to that category.
     */

Tips/Rules:

* The first category (`$index = 0`, `'ID' => 0`) is reserved for the 'Unfiled' category.  If you specify anything otherwise, it will be overwritten.
* There's a maximum of 16 categories, including 'Unfiled'.  This means that you only have 15 to play with.
* Category IDs 1-127 are typically reserved for handheld ID numbers
* Category IDs 128-255 are used for desktop ID numbers
* Do not let categories be created with ID numbers larger than 255 -- they will be ignored/erased or arbitrarily reassigned a different ID number.
* Don't let two categories have the same ID number.  One will be arbitrarily assigned a new number.
* If you just want to set the categories quickly, you can use code like this: `$pdb->SetCategoryList(array(1 => 'Category1', 'Category2', 'Etc.'))`


### `CreateCategoryData()`

Looks at the category information stored in the class and creates the proper hex-encoded data for the AppInfo block.


### `LoadCategoryData($fileData)`

Used when loading data from a file.  Reads in a chunk of the AppInfo block into the class's data structures.


More information
----------------

The source code is pretty clean with lots of comments strewn about.  There are other functions that are not detailed here and are designed to be overridden by classes that extend the base class.  Read the source to get to know them.
