<?PHP
/* Documentation for PHP-PDB library -- Datebook module
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See http://php-pdb.sourceforge.net/ for more information about the library
 */

include("../functions.inc");

StandardHeader('Datebook Files', 'modules');

?>

<p>This class uses a completely different for for storing information about
events.  It is supposed to make things easier, but might be a bit complex to
learn at first.</p>

<?PHP ShowOverwriteWarning() ?>

<h3>Including into your program</h3>

<?PHP

ShowExample('
include \'php-pdb.inc\';
include \'modules/datebook.inc\';
');

?>

<h3>Creating a new database</h3>

<P>Since a datebook has a specified type, creator, and name, the class takes
care of knowing what they are.  Creation of a new datebook is a snap.

<?PHP

ShowExample('
$DB = new PalmDatebook();
');

?>

<h3>Writing the database</h3>

<p>This is the same as the base class.  See <a href="../example.php">Basic
Use</a> for more information.</p>

<h3>Loading the database</h3>

<p>This is the same as the base class.  See <a href="../example.php">Basic
Use</a> for more information.</p>

<h3>Other functions</h3>

<dl>

<dt><b>GetRecordRaw()<br>
SetRecordRaw()</b></dt>
<dd>Please see <a href="../example.php">Basic Use</a> for how to use these
functions.  You use both of these to get/set records in the database.</dd>

<dt><b>NewRecord()</b></dt>
<dd>Returns an array with some default data for a new Datebook record.  Does
not actually add the record.  Use SetRecordRaw() for that.</dd>

</dl>

<h3>Record Format</h3>

<p>The data for the GetRecordRaw and the data returned from SetRecordRaw is
a specially formatted array, as detailed below.  Optional values can be set
to '' or not defined.  If an optional value is anything else (including
zero), it is considered to be set.</p>

<table align=center cellpadding=4 cellspacing=0 border=1 bgcolor=#FFDFDF>
<tr>
<th>Key</th><th>Example</th><th>Description</th>
</tr>
<tr><td>StartTime</td><td>2:00</td><td>Starting time of the event, 24 hour
format</td></tr>
<tr><td>Endtime</td><td>13:00</td><td>Ending time of the event, 24 hour
format</td></tr>
<tr><td>Date</td><td>2001-01-23</td><td>Year, month, and day of the
event</td></tr>
<tr><td>Description</td><td>Eat At Joe's</td><td>The title or the name of 
the event</td></tr>
<tr><td>Alarm</td><td>5d</td><td>[Optional] A number of units before the
event to sound an alarm (m = minutes, h = hours, d = days)</td></tr>
<tr><td>Repeat</td><td><i>Special</i></td><td>[Optional] An array detailing
how the event should repeat</td></tr>
<tr><td>Note</td><td>Order a burger and fries</td><td>[Optional] A note for
the event</td></tr>
<tr><td>Exceptions</td><td><i>Special</i></td><td>[Optional] Exceptions when
the event should not happen</td></tr>
<tr><td>WhenChanged</td><td>1</td><td>[Optional] True if the "when info" for
the event has changed.</td></tr>
<tr><td>Flags</td><td>3</td><td>[Optional] User flags for the event</td></tr>
</table>

<p>EndTime must happen at or after StartTime.  The time the event occurs may
not pass midnight (0:00).  If the event doesn't have a time (an all-day 
event), use '' or do not define StartTime and EndTime.</p>

<p>Exceptions are specified in an array consisting of dates the event
occured and did not happen or should not be shown.  Dates are in the format
<tt>YYYY-MM-DD</tt>, just like the Date attribute of the record.</p>

<p>Repeating events are special, and the Repeat attribute of the record is
set to an array.

<dl>
<dt>No repeat (or just leave Repeat undefined):</dt>
<dd>$repeat['Type'] = PDB_DATEBOOK_REPEAT_NONE;</dd>
<dt>Daily repeating events:</dt>
<dd>$repeat['Type'] = PDB_DATEBOOK_REPEAT_DAILY;</dd>
<dd>$repeat['Frequency'] = FREQUENCY; // Explained below</dd>
<dd>$repeat['End'] = END_DATE; // Explained below</dd>
<dt>Weekly repeating events:</dt>
<dd>$repeat['Type'] = PDB_DATEBOOK_REPEAT_Weekly;</dd>
<dd>$repeat['Frequency'] = FREQUENCY; // Explained below</dd>
<dd>$repeat['End'] = END_DATE; // Explained below</dd>
<dd>$repeat['Days'] = DAYS; // Explained below</dd>
<dd>$repeat['StartOfWeek'] = SOW; // Explained below</dd>
<dt>"Monthly by day" repeating events:  (happens on a specific weekday)</dt>
<dd>$repeat['Type'] = PDB_DATEBOOK_REPEAT_MONTH_BY_DAY;</dd>
<dd>$repeat['Frequency'] = FREQUENCY; // Explained below</dd>
<dd>$repeat['End'] = END_DATE; // Explained below</dd>
<dd>$repeat['WeekNum'] = WEEKNUM; // Explained below</dd>
<dd>$repeat['DayNum'] = DAYNUM; // Explained below</dd>
<dt>"Monthly by date" repeating events:  (happens on a specific numbered 
day)</dt>
<dd>$repeat['Type'] = PDB_DATEBOOK_REPEAT_MONTH_BY_DATE;</dd>
<dd>$repeat['Frequency'] = FREQUENCY; // Explained below</dd>
<dd>$repeat['End'] = END_DATE; // Explained below</dd>
<dt>Yearly repeating events:</dt>
<dd>$repeat['Type'] = PDB_DATEBOOK_REPEAT_YEARLY;</dd>
<dd>$repeat['Frequency'] = FREQUENCY; // Explained below</dd>
<dd>$repeat['End'] = END_DATE; // Explained below</dd>
</dl>

<dl>
<dt>FREQUENCY</dt>
<dd>The frequency of the event.  If it is a daily event and you want it to
happen every other day, set Frequency to 2.  This will default to 1 if not
specified.</dd>
<dt>END_DATE</dt>
<dd>The last day, month, and year on which the event occurs.  Format is
<tt>YYYY-MM-DD</tt>.  If not specified, no end date will be set.</dd>
<dt>DAYS</dt>
<dd>What days during the week the event occurs.  This is a string of numbers
from 0 to 6.  I'm not sure if 0 is Sunday or if 0 is the start of the week
from the preferences.  If you have a weekly repeating event and you want it
to repeat on Thursday, you set this to "4".</dd>
<dt>SOW</dt>
<dd>As quoted from P5-Palm:<br>
<blockquote>"I'm not sure what this is, but the Datebook app appears to
perform some hairy calculations involving this."</blockquote></dd>
<dt>WEEKNUM</dt>
<dd>The number of the week on which the event occurs.  5 is the last week of
the month.  0 is the first.</dd>
<dt>DAYNUM</dt>
<dd>The day of the week on which the event occurs.  Again, I don't know if 0
is Sunday or if 0 is the start of the week from the preferences.</dd>
</dl>

<p>There are also two mysterious 'unknown' fields for the repeat event and
they will be populated if the database is loaded from a file.  They will
otherwise default to 0.  They are 'unknown1' and 'unknown2'.</p>


<h3>Example</h3>

<?PHP

ShowExample('
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
');

StandardFooter();

?>
