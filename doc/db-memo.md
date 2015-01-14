Memo Pad Module
---------------

Memo Pad files are extremely easy to modify, as you will see below.

This module's output will overwrite existing databases.  Please read the [overwrite warning](overwrite.md) for more information.


Including into your program
---------------------------

    include 'php-pdb.inc';
    include 'modules/memo.inc';


Creating a new database
-----------------------

Memo pad files already have a specified name, type, and creator ID.  You don't need to worry about anything.

    $DB = new PalmMemo();


Writing the database
--------------------

This is the same as the base class.  See the [API] for more information.


Loading the database
--------------------

This works just like loading files with the base class.  Please see the [API] for further information.


Category Support and Record Attributes
--------------------------------------

Memos support categories.  This is the same as the base class.  Please see the [API] for more information and examples.


Other functions
---------------


### `SetText($text, $record = false)`

If the `$record` number is specified, it sets the record to `$text`.

If `$record` number is not specified, it just sets the current memo record to `$text`.

`$text` is limited to 4095 characters and will be automatically trimmed if it is too long.


### `GetText($record = false)`

Returns the text of the memo record specified, if `$record` was set.

Otherwise, it returns the text of the current record.


Example
-------

    $pdb = new PalmMemo();
    $pdb->GoToRecord(0);
    $pdb->SetText("This is the first memo");
    $pdb->SetText("This is the second memo", 1);
    $pdb->GoToRecord(2);
    $pdb->SetText("This is the third memo");
    $pdb->DownloadPDB("MemoDB.pdb");


[API]: api.md
