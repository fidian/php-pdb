<?PHP

/* Class extender for PalmOS DOC files
 *
 * Copyright (C) 2001 Tyler Akins
 * Licensed under the GNU LGPL
 * See the LEGAL file for more information
 */
 
$PDB_DOC_RECORD_SIZE = 4096;
 
class PalmDoc extends PalmDB {
   function PalmDoc ($Title = '') {
      $this->Name = $Title;
      $this->Type = 'TEXt';
      $this->Creator = 'REAd';
      
      // Record 0 is reserved for header information
      $this->GoToRecord(1);
      $this->RecordGetFunc = 'PalmDocRecordGet';
      $this->RecordSizeFunc = 'PalmDocRecordSize';
   }
   
   function AddDocText ($String) {
      global $PDB_DOC_RECORD_SIZE;
      
      $SpaceLeft = $PDB_DOC_RECORD_SIZE - $this->GetRecordSize();
      while ($String) {
         if ($SpaceLeft > 0) {
	    $this->AppendString($String, $SpaceLeft);
	    $String = substr($String, $SpaceLeft);
	    $SpaceLeft = 0;
	 } else {
	    $Curr = $this->GoToRecord();
	    $this->GoToRecord($Curr + 1);
	    $SpaceLeft = $PDB_DOC_RECORD_SIZE;
	 }
      }
   }
   
   function MakeDocRecordZero () {
      global $PDB_DOC_RECORD_SIZE;

      $oldRec = $this->GoToRecord(0);
      $this->ClearCurrentRecord();
      $this->AppendInt16(1);  // Version
      $this->AppendInt16(0);  // Reserved
      
      $Content_Length = 0;
      $MaxIndex = 0;
      foreach ($this->Records as $index => $recInfo) {
         // Just because we're going through the array doesn't mean that
	 // we are going to get the indexes in order.
	 // Pity.
         if ($index != 0)
	    $Content_Length += $this->GetRecordSize($index);
	 if ($index > $MaxIndex)
	    $MaxIndex = $index;
	 $this->RecordAttrs[$index] = 0x40;  // dirty + private
      }
      $this->AppendInt32($Content_Length);       // Doc Size
      $this->AppendInt16($MaxIndex);             // Number of Records
      $this->AppendInt16($PDB_DOC_RECORD_SIZE);  // Record size
      $this->AppendInt32(0);                     // Reserved
      $this->GoToRecord($oldRec);
   }
   
   function PalmDocRecordSize($num) {
      if ($num == 0)
         return 16;
      return $this->GetRecordSize_Normal($num);
   }
   
   function PalmDocRecordGet($num) {
      if ($num == 0)
         $this->MakeDocRecordZero();
      if (! isset($this->Records[$num]))
         return '';
      return $this->Records[$num];
   }
}

?>
