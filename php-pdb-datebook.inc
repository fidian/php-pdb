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
 * values are marked with a ^.  For events without a time, use '' or do not
 * define both the StartTime and EndTime.
 *
 * Key           Example          Description
 * ------------------------------------------
 * StartTime     2:00             Starting time of event, 24 hour format
 * EndTime       13:00            Ending time of event, 24 hour format
 * Date          2001-01-23       Year-Month-Day of event
 * Description   Title            This is the title of the event
 * Alarm         5d               ^ Number of units (m=min, h=hours, d=days)
 * Repeat        ???              ^ Repeating event data (not supported)
 * Note          NoteNote         ^ A note for the event
 * Exceptions    ???              ^ Exceptions to the event (not supported)
 */
 
class PalmDatebook extends PalmDB {
   var $DatebookRecords;
   
   function PalmDatebook () {
      $this->Name = 'DatebookDB';
      $this->Type = 'date';
      $this->Creator = 'DATA';
      $this->RecordGetFunc = 'PalmDatebookRecordGet';
      $this->RecordSizeFunc = 'PalmDatebookRecordSize';
   }
   
   function PalmDatebookRecordGet($num) {
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
   
   function PalmDatebookRecordSize($num) {
   }
   
   function PalmDatebookSetRecord($RecordData) {
      $this->DatebookRecords[$this->CurrentRecord] = $RecordData;
   }
   
   function PalmDatebookGetRecord($RecordData) {
      if (! isset($this->DatebookRecords[$this->CurrentRecord]))
         return array();
      return $this->DatebookRecords[$this->CurrentRecord];
   }
}

?>
