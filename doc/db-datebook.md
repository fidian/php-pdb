Datebook Module
===============

This class uses a completely different structure for storing information about events.  It is supposed to make things easier, but might be a bit complex to learn at first.

This module's output will overwrite existing databases.  Please read the [overwrite warning](overwrite.md) for more information.


Including into your program
---------------------------

    include 'php-pdb.inc';
    include 'modules/datebook.inc';


Creating a new database
-----------------------

Since a Datebook has a specified type, creator, and name, the class takes
care of knowing what they are.  Creation of a new Datebook is a snap.

    $DB = new PalmDatebook();


Writing the database
--------------------

This is the same as the base class.  See the [API] for more information.


Loading the database
--------------------

This is the same as the base class.  See the [API] for more information.


Other functions
---------------


### `GetRecordRaw()`, `SetRecordRaw()`

Please see the [API] for how to use these functions.  You use both of these to get/set records in the database.


### `NewRecord()`

Returns an array with some default data for a new Datebook record.  Does not actually add the record.  Use `SetRecordRaw()` to add the record to the database.


Record Format
-------------

The data for `GetRecordRaw` and the data returned from `SetRecordRaw` is a specially formatted array, as detailed below.  Optional values can be set to '' or not defined.  If an optional value is anything else (including zero), it is considered to be set.

| Key         | Example        | Description                                                                                        |
|-------------|----------------|----------------------------------------------------------------------------------------------------|
| StartTime   | 2:00           | (Optional) Starting time of the event, 24 hour format                                              |
| Date        | 2001-01-23     | Year, month, and day of the event                                                                  |
| Description | Eat at Joe's   | The title or the name of the event                                                                 |
| EndTime     | 13:00          | (Optional) Ending time of the event, 24 hour format                                                |
| Alarm       | 5d             | (Optional) A number of units before the event to sound an alarm (m = minutes, h = hours, d = days) |
| Repeat      | *Special*      | (Optional) An array detailing how the event should repeat                                          |
| Note        | Order a burger | (Optional) A note for the event                                                                    |
| Exceptions  | *Special*      | (Optional) Exceptions when the event should not happen                                             |
| WhenChanged | 1              | (Optional) True if the "when info" for the event has changed                                       |
| Flags       | 3              | (Optional) User flags for the event                                                                |

`EndTime` must happen at or after `StartTime`.  The time the event occurs may not pass midnight (0:00).  If the event doesn't have a time (an all-day event), use '' or do not define `StartTime` and `EndTime`.

Exceptions are specified in an array consisting of dates the event occurred and did not happen or should not be shown.  Dates are in the format `YYYY-MM-DD`, just like the Date attribute of the record.

Repeating events are special, and the Repeat attribute of the record is set to an array.

    // No repeat (or just leave Repeat undefined):
    $repeat['Type'] = PDB_DATEBOOK_REPEAT_NONE;

    // Daily repeating events:
    $repeat['Type'] = PDB_DATEBOOK_REPEAT_DAILY;
    $repeat['Frequency'] = FREQUENCY; // Explained below
    $repeat['End'] = END_DATE; // Explained below

    // Weekly repeating events:
    $repeat['Type'] = PDB_DATEBOOK_REPEAT_Weekly;
    $repeat['Frequency'] = FREQUENCY; // Explained below
    $repeat['End'] = END_DATE; // Explained below
    $repeat['Days'] = DAYS; // Explained below
    $repeat['StartOfWeek'] = SOW; // Explained below

    // "Monthly by day" repeating events:  (happens on a specific weekday)
    $repeat['Type'] = PDB_DATEBOOK_REPEAT_MONTH_BY_DAY;
    $repeat['Frequency'] = FREQUENCY; // Explained below
    $repeat['End'] = END_DATE; // Explained below
    $repeat['WeekNum'] = WEEKNUM; // Explained below
    $repeat['DayNum'] = DAYNUM; // Explained below

    // "Monthly by date" repeating events:  (happens on a specific numbered day)
    $repeat['Type'] = PDB_DATEBOOK_REPEAT_MONTH_BY_DATE;
    $repeat['Frequency'] = FREQUENCY; // Explained below
    $repeat['End'] = END_DATE; // Explained below

    // Yearly repeating events:
    $repeat['Type'] = PDB_DATEBOOK_REPEAT_YEARLY;
    $repeat['Frequency'] = FREQUENCY; // Explained below
    $repeat['End'] = END_DATE; // Explained below

**FREQUENCY** - The frequency of the event.  If it is a daily event and you want it to happen every other day, set Frequency to 2.  This will default to 1 if not specified.

**END_DATE** - The last day, month, and year on which the event occurs.  Format is `YYYY-MM-DD`.  If not specified, no end date will be set.  If the record was loaded from a file and there was no end date for a repeating event, this array element will not be set.

**DAYS** - What days during the week the event occurs.  This is a string of numbers from 0 to 6.  I'm not sure if 0 is Sunday or if 0 is the start of the week from the preferences.  If you have a weekly repeating event and you want it to repeat on Thursday, you set this to "4".

**SOW** - As quoted from P5-Palm:  "I'm not sure what this is, but the Datebook app appears to perform some hairy calculations involving this."

**WEEKNUM** - The number of the week on which the event occurs.  5 is the last week of the month.  0 is the first.

**DAYNUM** - The day of the week on which the event occurs.  Again, I don't know if 0 is Sunday or if 0 is the start of the week from the preferences.

There are also two mysterious 'unknown' fields for the repeat event and they will be populated if the database is loaded from a file.  They will otherwise default to 0.  They are `unknown1` and `unknown2`.


Example
-------

    // Create an instance of the class
    $pdb = new PalmDatebook();

    // Create a repeating event that happens every other Friday.
    // I want it to be an all-day event that says "Pay Day!"
    $Repeat = array(
       "Type" => PDB_DATEBOOK_REPEAT_WEEKLY,
       "Frequency" => 2,
       "Days" => "5",
       "StartOfWeek" => 0
    );
    $Record = array(
       "Date" => "2001-11-2",
       "Description" => "Pay Day!",
       "Repeat" => $Repeat
    );

    // Add the record to the datebook
    $pdb->SetRecordRaw($Record);

    // Advance to the next record just in case we want to add more events
    $pdb->GoToRecord("+1");


[API]: api.md
