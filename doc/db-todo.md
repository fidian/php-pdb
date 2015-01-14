ToDo List Module
================

The todo module is mostly straightforward, and should be fairly easy to use.

This module's output will overwrite existing databases.  Please read the [overwrite warning](overwrite.md) for more information.


Including into your program
---------------------------

    include 'php-pdb.inc';
    include 'modules/todo.inc';


Creating a new database
-----------------------

Since a todo list has a specified type, creator, and name, the class takes care of knowing what they are.  Creation of a new database is a snap.

$DB = new PalmTodoList();


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


### `GetRecordRaw()`, `SetRecordRaw()`

Please see the [API] for how to use these functions.  You use both of these to get/set records in the database.


### `NewRecord()`

Returns an array with some default data for a new Todo record.  Does not actually add the record.  Use `SetRecordRaw()` to add it to the database.


Record Format
-------------

The data for `GetRecordRaw` and the data returned from `SetRecordRaw` is a specially formatted array, as detailed below.  Optional values can be set to '' or not defined.  If an optional value is anything else (including zero), it is considered to be set.

| Key         | Example              | Description                                                           |
|-------------|----------------------|-----------------------------------------------------------------------|
| Description | This is my todo      | Short description of thing to do                                      |
| Note        | Extended information | (Optional) A note attached to the record                              |
| DueDate     | 2001-01-23           | (Optional) Year, month and day of when the item should be done        |
| Priority    | 1                    | (Optional) Priority of the event, 1 through 5                         |
| Completed   | false                | (Optional) Boolean indicating whether the action was completed or not |

If description is not specified, then the string 'No description' will be used instead.


Example
-------

This shows how to write records.

    // Write Example
    $todo = new PalmTodoList();
    $categories = array(1 => 'Visita', 'Fax', 'Correo');
    $todo->SetCategoryList($categories);
    $record = array('Description' => 'Enviar Fax',
                    'Note' => "25\nProbar palm",
                    'Priority' => 2);
    $todo->SetRecordRaw($record);
    $todo->SetRecordAttrib(2); // Category 2

    $todo->GoToRecord('+1');
    $record = array('Description' => 'Llamar a juan',
                    'Note' => '35',
                    'Completed' => true,
                    'DueDate' => '2002-5-31');
    $todo->SetRecordRaw($record);
    $todo->SetRecordAttrib(PDB_RECORD_ATTRIB_DIRTY |
                           PDB_RECORD_ATTRIB_PRIVATE | 0);
                           // Category 0, dirty, private

    $fp = fopen('./pdbs/todo.pdb','wb');
    $todo->WriteToFile($fp);
    fclose($fp);

Here is how you read the file.

    // Read Example
    $pdb = new PalmTodoList();
    $fp = fopen('./tdread.pdb', 'rb');
    $pdb->ReadFile($fp);
    fclose($fp);

    echo "Name: $pdb->Name<BR>\n";
    echo "Type ID: $pdb->TypeID<br>\n";
    echo "Creator: $pdb->CreatorID<br>\n";
    echo "Attributes: $pdb->Attributes<br>\n";
    echo "Version: $pdb->Version<br>\n";
    echo "ModNum: $pdb->ModNumber<br>\n";
    echo "CreationTime: $pdb->CreationTime<br>\n";
    echo "ModTime: $pdb->ModificationTime<br>\n";
    echo "BackTime: $pdb->BackupTime<br>\n";
    echo 'NumRec: '.$pdb->GetRecordCount()."<br>\n";
    $recids = $pdb->GetRecordIDs();
    $record1 = $pdb->GetRecordRaw($recids[0]);
    $attrib = $pdb->GetRecordAttrib($recids[0]);
    echo "record1 = $record1<br>\n";
    echo "Desc: " . $record1['Description'] . "<br>\n";
    echo "Note: " . $record1['Note'] . "<br>\n";
    echo 'Due Date: ' . $record1['DueDate'] . "<br>\n";
    echo 'Cat: ' . $record1['Category'] . "<br>\n";
    $categories = $pdb->GetCategoryList();
    echo "NumCat = " . count($categories) . "<br>\n";
    foreach ($categories as $k => $v) {
        echo "categories[$k] = $v<br>\n";
        foreach ($categories[$k] as $key => $val) {
            echo "  categories[$k][$key] = $val<br>";
        }
    }


[API]: api.md
