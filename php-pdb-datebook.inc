<?PHP

/* Class extender for PalmOS Datebook files
 *
 * Copyright (C) 2001 Tyler Akins
 * Licensed under the GNU LGPL
 * See the LEGAL file for more information
 */
 
/* The data for the GetRecord and SetRecord functions should return/be fed
 * an array.  Optional values can be set to '' or not defined.  If they are
 * anything else (including zero), they are considered to be 'set'.  Optional
 * values are marked with a ^.
 *
 * Key           Example          Description
 * ------------------------------------------
 * StartTime     2:00             Starting time of event, 24 hour format
 * EndTime       13:00            Ending time of event, 24 hour format
 * Date          2001-01-23       Year-Month-Day of event
 * Description   Title            This is the title of the event
 * Alarm         5d               ^ Number of units (m=min, h=hours, d=days)
 * Repeat        ???              ^ Repeating event data
 * Note          NoteNote         ^ A note for the event
 * Exceptions    ???              ^ Exceptions to the event
 *
 * EndTime must happen at or after StartTime.  The time the event occurs
 * may not pass midnight.  If the event doesn't have a time, use '' or do
 * not define StartTime and EndTime.
 * 
 * Repeat events can be:
 *            No repeat (unset or empty string)
 *   D        Daily repeat
 *   W[days]  Weekly repeat that happens on the specified days
 *   M[days]  Monthly repeat event *blahblah*
 *   Y        Yearly repeat
 * 
 * Repeat events also have a frequency.
 *
 * Exceptions are specified in an array consisting of dates the event occured
 * and did not happen or should not be shown.
 */
 
class PalmDatebook extends PalmDB {
   var $DatebookRecords;
   var $FirstDay;
   var $CategoryData;
   
   function PalmDatebook () {
      $this->Name = 'DatebookDB';
      $this->Type = 'date';
      $this->Creator = 'DATA';
      $this->FirstDay = 0;
      $this->CategoryData = array();
   }

   function GetRecord($num) {
      $this->ClearCurrentRecord();
      $data = $this->DatebookRecords[$this->CurrentRecord];
      if (! isset($data))
         return $this->Records[$this->CurrentRecord]);
	 
      // Start Time and End Time
      // 4 bytes
      // 0xFFFFFFFF if the event has no time
      if (! isset($data['StartTime']) || ! isset($data['EndTime']) ||
          strpos($data['StartTime'], ':') === false ||
	  strpos($data['EndTime'], ':') === false) {
	 $this->AppendInt16(65535);
	 $this->AppendInt16(65535);
      }
      
      // The day of the event
      // For repeating events, this is the first day the event occurs
      //
      // 'Date' = "2001-10-31"
      // bitwise:  [YYYY YYYM  MMMD DDDD]
      $YMD = explode('-', $data['Date']);
      $this->AppendInt16(($YMD[0] - 1904) * 512 + $YMD[1] * 32 + $YMD[2]);
      
      // Flags
      // 0x80 = When ("Was the 'when' updated?" or some junk)
      // 0x40 = Alarm (Is there one set?)
      // 0x20 = Repeat (When do I repeat and how often?)
      // 0x10 = Note (Is there a note attached?)
      // 0x08 = Exceptions (Are there any exceptions?)
      // 0x04 = Description (Mandatory, as far as I know)
      // 0x02 = Unknown
      // 0x01 = Unknown
      $Flags = 0x04;
      $this->AppendInt16($Flags);
      $this->AppendString($data['Description']);
      $this->AppendInt8(0);
   }
   
   function SetDatebookRecord($RecordData) {
      $this->DatebookRecords[$this->CurrentRecord] = $RecordData;
   }
   
   function GetDatebookRecord($RecordData) {
      if (! isset($this->DatebookRecords[$this->CurrentRecord]))
         return array();
      return $this->DatebookRecords[$this->CurrentRecord];
   }
   
   function GetAppInfoSize() {
      // FIXME
      return 0;
   }
   
   function GetAppInfo() {
      // Category list
      $this->AppInfo = PalmDB::CreateCategoryData($this->CategoryData);
      
      // Unknown thing (first_day_in_week)
      // 00 00 FD 00 == where FD is the first day in week.
      // I'm using 0 as the default value since I don't know what it should be
      $this->AppInfo .= $this->Int16(0);
      $this->AppInfo .= $this->Int8($this->FirstDay);
      $this->AppInfo .= $this->Int8(0);
      
      return $this->AppInfo;
   }
   
   function LoadAppInfo($fileData) {
      // Category list
      $this->CategoryData = PalmDB::LoadCategoryData($fileData);
      $fileData = substr($fileData, PDB_CATEGORY_SIZE + 2);
      if (strlen($fileData < 1))
         return;
      $this->FirstDay = $this->LoadInt8($fileData);
   }
}

?>
