Address Book Module
===================

Address entries have a lot of data associated with them.  Because of this, there is a somewhat complex way of getting to that data.  However, I'll try to explain it thoroughly and show examples so that you don't get lost.

This module's output will overwrite existing databases.  Please read the [overwrite warning](overwrite.md) for more information.


Including into your program
---------------------------

    include 'php-pdb.inc';
    include 'modules/addrbook.inc';


Creating a new database
-----------------------

Since an address book has a specified type, creator, and name, the class takes care of knowing what they are.  Creation of a new contacts database is trivial.

    $DB = new PalmDatebook();
    // or
    $DB = new PalmDatebook($Country);

The country can be passed in order to facilitate i18n.  It is optional and will default to `PDB_ADDR_COUNTRY_DEFAULT` (which is set to `PDB_ADDR_COUNTRY_UNITED_STATES` in `addrbook.inc` -- but you can change it to affect your entire site if you wish).  A list of all countries is in `addrbook.inc`.


Writing the database
--------------------

This is the same as the base class.  See the [API] for more information.


Loading the database
--------------------

This is the same as the base class.  See the [API] for more information.


Category Support and Record Attributes
--------------------------------------

This supports categories and attributes.  See the [API] for more information.


Other functions
---------------


### `GetRecordRaw()` and `SetRecordRaw()`

Please see the [API] for how to use these functions.  You use both of these to get/set records in the database.


### `NewRecord()`

Returns an array with some default data for a new Datebook record.  Does not actually add the record.  Use `SetRecordRaw()` for that.


### `SetFieldLabels($list)` and `GetFieldLabels()`

Get/set the field labels as an associative array.  The field labels define what are shown when you view and edit an entry in your address book.  If you want "Last name:" to show up as "Surname:" and "Company:" as "Business:", you change them in this array.

    // Load the current set of field labels
    $labels = GetFieldLabels();

    // If you were to run this code ...
    //     foreach ($labels as $key => $val) {
    //        echo "$key => $val<br>\n";
    //     }
    // the results would look like this:
    //
    // LastName => Last name
    // FirstName => First name
    // Company => Company
    // Phone1 => Work
    // Phone2 => Home
    // Phone3 => Fax
    // Phone4 => Other
    // Phone5 => E-mail
    // Phone6 => Main
    // Phone7 => Pager
    // Phone8 => Mobile
    // Address => Address
    // City => City
    // State => State
    // ZipCode => Zip Code
    // Country => Country
    // Title => Title
    // Custom1 => Custom 1
    // Custom2 => Custom 2
    // Custom3 => Custom 3
    // Custom4 => Custom 4
    // Note => Note

    // Let's change a few things around
    $labels['LastName'] = 'Last Name';  // Capitalize the "Name" part
    $labels['FirstName'] = 'First Name';  // Capitalize the "Name" part
    $labels['Phone5'] = 'Email';  // Remove hyphen
    $labels['Phone8'] = 'Cellular';  // "Mobile" -> "Cellular"

    // Save the changes
    $pdb->SetFieldLabels($labels);


Record Format
-------------

The data for `GetRecordRaw` and the data returned from `SetRecordRaw` is a specially formatted array, as detailed below.  Optional values can be set to '' or not defined.  If an optional value is anything else (including zero), it is considered to be set.  At least one optional value should be specified.  If no optional values are specified, then 'Empty Record' will be displayed as the last name.

| Key          | Example              | Description                                            |
|--------------|----------------------|--------------------------------------------------------|
| LastName     | Duck                 | The contact's last name                                |
| FirstName    | Daffy                | The contact's first name                               |
| Company      | PHP-PDB Inc.         | The name of the company                                |
| Phone1       | 870-5309             | What goes in the first phone/email/etc field           |
| Phone...     | Same as above        | Same goes for Phone2, Phone3, Phone4                   |
| Phone5       | dduck@example.com    | This is the last phone/email/etc field                 |
| Phone1Type   | PDB_ADDR_LABEL_WORK  | The type of the first phone record                     |
| Phone...Type | Same as above        | Same goes for Phone2Type, Phone3Type, Phone4Type,      |
| Phone5Type   | PDB_ADDR_LABEL_EMAIL | The type of the first phone record                     |
| Address      | Mallard St. 25       | The address                                            |
| City         | Toon City            | The city                                               |
| State        | CA                   | The state of the contact                               |
| ZipCode      | 12345                | The postal code for the contact                        |
| Country      | Toon Land            | The contact's country                                  |
| Title        | Sir                  | The title of the contact                               |
| Custom1      | Birth Date           | Extra information                                      |
| Custom...    | Same as above        | Same goes for Custom2, Custom3                         |
| Custom4      | Whatever             | The last extra information field                       |
| Note         | Quack.               | Notes for the contact                                  |
| Display      | 1                    | Which phone number entry to display on the list screen |
| Reserved     | '' (empty string)    | Unknown                                                |

The Phone#Type keys associate the five Phone# strings with the proper labels that are defined with `GetFieldLabels` and `SetFieldLabels`.  There are eight possible strings that Phone# can be associated with. Below is a table showing the default values for the eight phone labels.  See `SetFieldLabels()` and `GetFieldLabels()` for information on how to change the text that shows up for the label.

| Array Index | Defined Variable      | Default Value |
|:-----------:|-----------------------|---------------|
|      0      | PDB_ADDR_LABEL_WORK   | Work          |
|      1      | PDB_ADDR_LABEL_HOME   | Home          |
|      2      | PDB_ADDR_LABEL_FAX    | Fax           |
|      3      | PDB_ADDR_LABEL_OTHER  | Other         |
|      4      | PDB_ADDR_LABEL_EMAIL  | E-Mail        |
|      5      | PDB_ADDR_LABEL_MAIN   | Main          |
|      6      | PDB_ADDR_LABEL_PAGER  | Pager         |
|      7      | PDB_ADDR_LABEL_MOBILE | Mobile        |

Let's assume that for your contact, you have two work numbers, a fax number, an email address, and a cell phone number (in that order).  You'd want to associate them with the correct labels for the numbers.  Additionally, in the record array is 'Display', which is the index of the array to display.  The example will assume you want the email address displayed.

    // Set the other aspects of $record before this ...
    // For example,   $record['FirstName'] = 'Donald';
    // Also set the Phone# entries before here

    // Associate Phone1 and Phone2 with the Work label
    $record['Phone1'] = PDB_ADDR_LABEL_WORK;
    $record['Phone2'] = PDB_ADDR_LABEL_WORK;

    // Phone3 is a fax number, Phone4 is an email, Phone5 is a cell phone number
    $record['Phone3'] = PDB_ADDR_LABEL_FAX;
    $record['Phone4'] = PDB_ADDR_LABEL_EMAIL;
    $record['Phone5'] = PDB_ADDR_LABEL_MOBILE;

    // Display the email address on the list screen of the address book app
    $record['Display'] = 3;

For the attributes and categories, see the [API] for the class.

<h3>Example</h3>

We are fortunate enough to have two examples.  The first writes a database.

    // How to write a database
    $addr = new PalmAddress();

    // Remember -- category 0 is reserved.
    $categories = array(1 => 'VIP', 'AAA', 'Inicial');
    $addr->SetCategoryList($categories);
    $fields = array('LastName' => 'Pascual',
                    'FirstName' => 'Eduardo',
                    'Phone1' => '21221552',
                    'Phone2' => '58808912',
                    'Phone5' => 'epascual at cie.com.mx',
                    'Address' => 'Hda. la Florida 10A',
                    'City' => 'Izcalli');
    $addr->SetRecordRaw($fields);
    $addr->GoToRecord('+1');
    $fields = array('LastName' => 'de tal',
                    'FirstName' => 'fulanito',
                    'Address' => 'Direccion',
                    'Phone1' => '21232425',
                    'Phone1Type' => PDB_ADDR_LABEL_HOME,
                    'Phone2' => 'fulanito at dondesea.com',
                    'Phone2Type' => PDB_ADDR_LABEL_EMAIL,
                    'Phone3Type' => PDB_ADDR_LABEL_WORK,
                    'Phone4Type' => PDB_ADDR_LABEL_FAX,
                    'Phone5Type' => PDB_ADDR_LABEL_OTHER,
                    'Display' => 1);
    $addr->SetRecordRaw($fields);
    $addr->SetRecordAttrib(PDB_RECORD_ATTRIB_PRIVATE |
                           1);  // Category 1, private record
    $fp = fopen('./pdbs/addr.pdb','wb');
    $addr->WriteToFile($fp);
    fclose($fp);

This second example shows how to read a database.

    // How to read a database
    $addr = new PalmAddress();
    $fp = fopen('./address.pdb','r');
    $addr->ReadFile($fp);
    fclose($fp);
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
    $cats = $addr->GetCategoryList();
    foreach ($recids as $ID) {
      $record = $addr->GetRecordRaw($ID);
      echo "Record $ID:<BR>";
      $attrib = $addr->GetRecordAttrib($ID);
      echo "- Category number: " . ($attrib & PDB_CATEGORY_MASK) . " = " .
        $cats[$attrib & PDB_CATEGORY_MASK]['Name'] . "<br>\n";
      echo "- Private: " .
        (($attrib & PDB_RECORD_ATTRIB_PRIVATE) ? "Yes" : "No") ."<br>\n";
      echo " Fields:<br>\n";
      foreach ($record as $reck => $rec) {
        echo "-- $reck => $rec<br>\n";
      }
    }
    echo "Field Labels<br>";
    $labels = $addr->GetFieldLabels();
    foreach ($labels as $k => $v) {
      echo "$k = $v<br>";
    }

API: api.md
