SmallBASIC Module
=================

[SmallBASIC](http://smallbasic.sourceforge.net) is a programming language that runs on the handheld.  With this class, you can convert to/from the .PDB format that the SmallBASIC interpreter uses.

**Warning:**  This class has not yet been tested extensively, so don't use this for mission-critical applications without further testing.  However, it does both read and write this format successfully for me, so I expect that it will work for you.


Including into your program
---------------------------

    include 'php-pdb.inc';
    include 'modules/smallbasic.inc';


Creating a new database
-----------------------

SmallBASIC files already have a defined type and creator ID.  The only
thing left for you to do is to specify the name.  Usually, this is the name
of the file that you are converting.

    $DB = new PalmSmallBASIC("MyTest.BAS");
      // Typical usage to create the SmallBASIC file

    $pdb = new PalmSmallBASIC();
      // Special:  If you want to create an instance of the class
      // and then use ReadFile() to load the database information


Writing the database
--------------------

This is the same as the base class.  See the [API] for more information.

>To let you know, the first record is the header, and the rest contain separate sections of code.  No single section can be bigger than 32k.  If, by some fluke, there is a section that is too large, it will be automatically trimmed when the database is written.


Loading the database
--------------------

This works just like loading files with the base class.  Please see the [API] for further information.


Category Support and Record Attributes
--------------------------------------

SmallBASIC files do not support categories nor record attributes.


Other functions
---------------


### `ConvertToText()`

Returns the code as a giant string, which can be saved to a regular file.  This also adds the `#sec:` tags to allow simple conversion back to Palm OS format.


### `ConvertFromText($String)`

Takes the .BAS file and converts it to the internal Palm OS format.  It also splits apart the `#sec:` sections into separate records.

`$String` = The entire contents of the file that you need converted.

Returns false on success.

If there is an error, returns an array of the [0] section number, [1] section name, and [2] the size of the section in bytes.


Example
-------

    $contents = file("your_file.bas");
    // You should check to make sure the file
    // was loaded properly before proceeding
    $contents = implode("", $contents);
    $pdb = new PalmSmallBASIC("your_file.bas");
    $pdb->ConvertFromText($contents);
    $pdb->DownloadPDB("doc_test.pdb");


[API]: api.md
