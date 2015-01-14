MobileDB Module
===============

MobileDB files are somewhat supported, but it isn't easy to add things or to move data around yet.  Be forewarned.


Including into your program
---------------------------

    include 'php-pdb.inc';
    include 'modules/mobiledb.inc';


Creating a new database
-----------------------

Because MobileDB databases already have a specific type and creator, you only need to specify the name of the new database.

    $DB = new PalmMobileDB("Name of DB");


Writing the database
--------------------

This is the same as the base class.  See the [API] for more information.


Loading the database
--------------------

This is the same as the base class.  See the [API] for more information.


Category Support and Record Attributes
--------------------------------------

MobileDB databases do support categories, but they are used strictly for keeping track of internal data.  Do not use them.


Other functions
---------------

There are no utility functions defined yet.  Sorry.


Record Format
-------------

All records in a MobileDB database are arrays.  Since there are no utility functions for manipulating records yet, it is suggested that you do not create new MobileDB databases.


Example
-------

This great example was submitted by Cristiano Nuernberg.  Just as a note, it is better to load a MobileDB and then work on it instead of trying to make one yourself from scratch.

    #########################################################################
    # Base files
    #########################################################################

    include_once ("php-pdb.inc");
    include_once ("modules/mobiledb.inc");

    #########################################################################
    # Output file (generate a unique file name to avoid any type of caching)
    # Set appropriate permissions to "./output/"
    #########################################################################

    $rand = time().mt_rand(100,999);

    $ouput_file = "./output/example-$rand.pdb";

    #########################################################################
    # Databse name / create instance of the class
    #########################################################################

    $pdb = new PalmMobileDB("New Database");

    #########################################################################
    # Headers
    #########################################################################

    // For some reason we need this (attrib=68)
    $Record = array("","","");
    $pdb->SetRecordRaw($Record);
    $pdb->SetRecordAttrib(68);
    $pdb->GoToRecord("+1");

    // Field lengths (attrib=70)
    $Record = array("80","40","40");
    $pdb->SetRecordRaw($Record);
    $pdb->SetRecordAttrib(70);
    $pdb->GoToRecord("+1");

    // Field titles (attrib=65)
    $Record = array("Last","First","Time");
    $pdb->SetRecordRaw($Record);
    $pdb->SetRecordAttrib(65);
    $pdb->GoToRecord("+1");

    // Data type: T = text, d = time in seconds since midnight (there are
    another 1/2-dozen types)
    // (attrib=69)
    $Record = array("T","T","d");
    $pdb->SetRecordRaw($Record);
    $pdb->SetRecordAttrib(69);
    $pdb->GoToRecord("+1");

    #########################################################################
    # Actual data
    #########################################################################

    $Record = array("Nuernberg","Cristiano","28800");

    $Record = $record_array;
    $pdb->SetRecordRaw($Record);
    $pdb->SetRecordAttrib(66);
    $pdb->GoToRecord("+1");

    $Record = array("Smith","Mark","25200");

    $Record = $record_array;
    $pdb->SetRecordRaw($Record);
    $pdb->SetRecordAttrib(66);
    $pdb->GoToRecord("+1");

    // etc...

    #########################################################################
    # Writing to a file
    #########################################################################

    $fp = fopen("$ouput_file", "wb");
    $pdb->WriteToFile($fp);
    fclose($fp);

[API]: api.md
