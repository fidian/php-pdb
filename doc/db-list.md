List Database Module
====================

List databases are supported fully.  You have to use the `SetRecordRaw` and `GetRecordRaw` functions.


Including into your program
---------------------------

    include 'php-pdb.inc';
    include 'modules/list.inc';


Creating a new database
-----------------------

Because List databases already have a specific type and creator, you only need to specify the name of the new database.

    $DB = new PalmListDB("Name of DB");


Writing the database
--------------------

This is the same as the base class.  See the [API] for more information.


Loading the database
--------------------

This is the same as the base class.  See the [API] for more information.


Category Support and Record Attributes
--------------------------------------

List supports categories.  This is the same as the base class.  See the [API] for more information.


Other functions
---------------

There are no utility functions defined yet.


Record Format
-------------

All records in a List database are arrays.  The first two elements can be a maximum of 63 characters.  The third (and last) element is a note field and can be up to 1023 characters.


Example
-------

    include_once ("php-pdb.inc");
    include_once ("modules/list.inc");

    $pdb = new PalmListDB("New Database");

    // Name the two fields
    $pdb->Field1 = "Name"
    $pdb->Field2 = "Job"

    // Add a few people
    $pdb->SetRecordRaw(array("Tyler Akins", "IT", "A really great guy."));
    $pdb->GoToRecord("+1");
    $pdb->SetRecordRaw(array("Madonna", "Singer"));
    $pdb->GoToRecord("+1");
    $pdb->SetRecordRaw(array("Adam Sandler", "Entertainer",
       "Did a few movies, sang a few songs, got mentioned here."));

    // Write to a file
    $fp = fopen("$ouput_file", "wb");
    $pdb->WriteToFile($fp);
    fclose($fp);


[API]: api.md
