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
 * Alarm         5                ^ Number of minutes before event for alarm
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
      if (! isset($data['StartTime']) || ! isset($data['EndTime']) ||
          strpos($data['StartTime'], ':') === false ||
	  strpos($data['EndTime'], ':') === false) {
	 $this->AppendInt8(255);
	 $this->AppendInt8(255);
      }
      ///////////////////////////////////////////////////////////////
      ///////////////////////////////////////////////////////////////
      ///////////////////////////////////////////////////////////////
      ///////////////////////////////////////////////////////////////
      ///////////////////////////////////////////////////////////////
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
