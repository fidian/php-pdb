<?php
/* The data for SetRecordRaw and from GetRecordRaw should be/return a
 * special array, detailed below.  Optional values can be set to '' or not
 * defined.  If they are anything else (including zero), they are considered
 * to be 'set'.  Optional values are marked with a ^.
 *
 * Key            Example             Description
 * ----------------------------------------------
 * description    ToDo                This is the ToDo text
 * note           Note                ^ A note for the todo
 * due_date       2002-06-03          ^ Year-Month-Day of the todo
 * completed      1                   ^ 1/0 (default 0 not completed)
 * priority       3                   ^ 1-5 (default 1)
 * category       2                   ^ 0-15 (default 0)
 * attributes     ???                 ^ Array of attributes
 *
 * The attributes array could be like this.
 * 
 * Key            Example              Description
 * -----------------------------------------------
 * expunged       1                    ^ 1/0/'' (default '' not set)
 * deleted        1                    ^ 1/0/'' (default '' not set)
 * arvhive        1                    ^ 1/0/'' (default '' not set)
 * dirty          1                    ^ 1/0/'' (default '' not set)
 * private        1                    ^ 1/0/'' (default '' not set)
 */
class PalmTodoList extends PalmDB {
  var $CategoryList;
  var $FirstDay;

  // Constructor.
  function PalmTodoList() {
    PalmDB::PalmDB('DATA','todo','ToDoDB');
    
    //Set a default CategoryList array.
    $this->CategoryList[1]['Name'] = 'Bussines';
    $this->CategoryList[1]['Renamed'] = false;
    $this->CategoryList[2]['Name'] = 'Personal';
    $this->CategoryList[2]['Renamed'] = false;

    $this->FirstDay = 0;

  }

   // Returns a new array with default data for a new record.
   // This doesn't actually add the record.
   function NewRecord() {
     //No due date by default.
     $Item['due_date'] = '';

     $Item['completed'] = 0;
     $Item['prioity'] = 1;

     $Item['description'] = '';
     $Item['note'] = '';

     $Item['category'] = 0;

     return $Item;
   }

   // Converts a date string ( YYYY-MM-DD )( "2001-10-31" )
   // into bitwise ( YYYY YYYM MMMD DDDD )
   // Should only be used when saving
   function DateToInt16($date) {
      $YMD = explode('-', $date);
      return ($YMD[0] - 1904) * 512 + $YMD[1] * 32 + $YMD[2];
   }
   
   
   // Converts a bitwise date ( YYYY YYYM MMMD DDDD )
   // Into the human readable date string ( YYYY-MM-DD )( "2001-10-31" )
   // Should only be used when loading
   function Int16ToDate($number) {
      $year = $number / 512;
      settype($year, "integer");
      $year += 1904;
      $number = $number % 512;
      $month = $number / 32;
      settype($month, "integer");
      $day = $number % 32;
      return $year . '-' . $month . '-' . $day;
   }  

   // Overrides the GetRecordSize method.
   // Probably should only be used when saving
   function GetRecordSize($num = false) {
     if ($num === false)
       $num = $this->CurrentRecord;
     
     if (! isset($this->Records[$num]) || ! is_array($this->Records[$num]))
       return PalmDB::GetRecordSize($num);

     $data = $this->Records[$num];

     // Due_date (1).
     // Completed and Priority (2).
     $Bytes = 3;

     if ($data['description'] != '') {
       $Bytes += strlen($data['description']);
     }
     $Bytes += 1; //NULL termination

     if ($data['note'] != '') {
       $Bytes += strlen($data['note']);
     }
     $Bytes += 1; //NULL termination

     return $Bytes;
   }

   // Overrides the GetRecord method. We store data in associative arrays.
   // Just convert the data into the proper format and then return the
   // generated string.
   function GetRecord($num = false) {
     if ($num === false) {
       $num = $this->CurrentRecord;
     }

     if (! isset($this->Records[$num]) || ! is_array($this->Records[$num])) {
       return PalmDB::GetRecord($num);
     }

     $data = $this->Records[$num];
     $RecordString = '';

     if (isset($data['due_date'])) {
       list($Y,$M,$D) = split('-',$data['due_date']);
       $rawDate = ($D & 0x001f) |
          (($M & 0x000f) << 5) |
          ((($Y - 1904) & 0x007f) << 9);
     } else {
       $rawDate = 0xffff;
     }
     $RecordString .= $this->Int16($rawDate);

     $priority = $data['priority'] & 0x7f;
     if ($data['completed']) {
       $priority |= 0x80;
     }
     $RecordString .= $this->Int8($priority);

     $RecordString .= $this->String($data['description']);
     $RecordString .= $this->Int8(0);
     $RecordString .= $this->String($data['note']);
     $RecordString .= $this->Int8(0);

     return $RecordString;
   }

   // Sets the data for the current record
   function SetRecordRaw($A, $B = false) {
     if ($B === false) {
       $B = $A;
       $A = $this->CurrentRecord;
     }
     if (!isset($B['priority'])) {
       $B['priority'] = 1;
     }
     $this->Records[$A] = $B;

     if (isset($B['attributes'])) {
       $atts = 0;
       if ($B['attributes']['expunged'] || $B['attributes']['deleted']) {
         if ($B['attributes']['archive']) {
           $atts |= 0x08;
         }
       } else {
         $atts = $B['category'] & 0x0f;
       }
       if ($B['attributes']['expunged']) {
         $atts |= 0x80;
       }
       if ($B['attributes']['dirty']) {
         $atts |= 0x40;
       }
       if ($B['attributes']['deleted']) {
         $atts |= 0x20;
       }
       if ($B['attributes']['private']) {
         $atts |= 0x10;
       }

       $this->RecordAttrs[$A] = $atts;
     } else {
       $this->RecordAttrs[$A] = $B['category'] & 0x0f;
     }
   }

   // Returns the size of the AppInfo block.  It is the size of the
   // category list plus four bytes.
   function GetAppInfoSize() {
      return PDB_CATEGORY_SIZE + 6;
   }

   // Returns the AppInfo block. It is composed of the category list.
   function GetAppInfo() {
     // Category List.
     $this->AppInfo = $this->CreateCategoryData($this->CategoryList);

     // Unknown thing (first_day_in_week)
     // 00 00 00 00 FD 00 == where FD is the first day in week.
     // I'm using 0 as the default value since I don't know what it should be
     $this->AppInfo .= $this->Int16(0);
     $this->AppInfo .= $this->Int16(0);
     $this->AppInfo .= $this->Int8(count($this->CategoryList) - 1);
     $this->AppInfo .= $this->Int8(0);
     return $this->AppInfo;
   }

   // Sets the category list.
   function SetCategoryList($list) {
     if (!is_array($list)) {
       return false;
     }

     // Validates if the first element is empty.
     $i = $list[0] != '' ? 1 : 0;

     $keys = array_keys($list);
     sort($keys);
     foreach ($keys as $id) {
       $this->CategoryList[$id+$i]['Name'] = $list[$id];
       $this->CategoryList[$id+$i]['Renamed'] = false;
     }
     return true;
   }

   // Returns the category list.
   function GetCategoryList() {
     return $this->CategoryList;
   }

   // Parse $fileData for the information we need when loading a todolist
   // file.
   function LoadAppInfo($fileData) {
     $this->AppInfo = bin2hex($fileData);
     $this->CategoryList = $this->LoadCategoryData($fileData);
     $fileData = substr($fileData, PDB_CATEGORY_SIZE + 4);
     $this->FirstDay = $this->LoadInt8($fileData);
   }

   // Converts the todo record data loaded from a file into the internal
   // storage method that is useed for the rest of the class and for ease of
   // use.
   // Return false to signal no error.
   function LoadRecord($fileData, $RecordInfo) {
     $this->RecordAttrs[$RecordInfo['UID']] = $RecordInfo['Attrs'];

     $NewRec = $this->NewRecord();

     //Load date
     $date = $this->LoadInt16(substr($fileData,0,2));
     if ($date != 0xffff) {
       $day = $date & 0x001f;
       $month = ($date >> 5) & 0x000f;
       $year = ($date >> 9) & 0x007f;
       $year += 1904;
       $NewRec['due_date'] = "$year-$month-$day";
     }

     $priority = $this->LoadInt8(substr($fileData,2,1));
     $completed = $priority & 0x80;
     $priority &= 0x7f;
     $NewRec['completed'] = $completed ? 1 : 0;
     $NewRec['priority'] = $priority;

     $i = 3;
     $description = '';
     while ($fileData[$i] != "\0") {
       $description .= $fileData[$i];
       $i++;
     }
     $i++;
     $note = '';
     while ($fileData[$i] != "\0") {
       $note .= $fileData[$i];
       $i++;
     }
     $NewRec['description'] = $description;
     $NewRec['note'] = $note;

     $NewRec['attributes']['expunged'] = $RecordInfo['Attrs'] & 0x80 ? 1 : 0;
     $NewRec['attributes']['dirty'] = $RecordInfo['Attrs'] & 0x40 ? 1 : 0;
     $NewRec['attributes']['deleted'] = $RecordInfo['Attrs'] & 0x20 ? 1 : 0;
     $NewRec['attributes']['private'] = $RecordInfo['Attrs'] & 0x10 ? 1 : 0;
     if (($RecordInfo['Attrs'] & 0xa0) == 0) {
       $NewRec['category'] = $RecordInfo['Attrs'] & 0x0f;
     } else {
       $NewRec['attributes']['archive'] = $RecordInfo['Attrs'] & 0x08 ? 1 : 0;
     }
     $this->Records[$RecordInfo['UID']] = $NewRec;
   }

}


/* WRITE EXAMPLE
 *
$todo = new PalmTodoList();
$categorias = array('Visita','Fax','Correo');
$todo->SetCategoryList($categorias);
$record = array('description' => 'Enviar Fax',
                'note' => "25\nProbar palm",
                'priority' => 2,
                'completed' => 0,
                'category' => 2,
                'due_date' => '');
$todo->SetRecordRaw($record);
$todo->GoToRecord('+1');
$record = array('description' => 'Llamar a juan',
                'note' => '35',
                'category' => 0,
                'due_date' => '2002-5-31',
                'attributes' => array('private' => 1,
                                      'dirty' => 1));
$todo->SetRecordRaw($record);
$fp = fopen('./pdbs/todo.pdb','wb');
$todo->WriteToFile($fp);
fclose($fp);
 *
 * READ EXAMPLE
 *
$pdb = new PalmTodoList();
$fp = fopen('./tdread.pdb','r');
$pdb->ReadFile($fp);
fclose($fp);

echo "Nombre: $pdb->Name<BR>";
echo "Tipo: $pdb->TypeID<br>";
echo "Creator: $pdb->CreatorID<br>";
echo "Atributos: $pdb->Atributes<br>";
echo "Version: $pdb->Version<br>";
echo "ModNum: $pdb->ModNumber<br>";
echo "CreationTime: $pdb->CreationTime<br>";
echo "ModTime: $pdb->ModificationTime<br>";
echo "BackTime: $pdb->BackupTime<br>";
echo 'NumRec: '.$pdb->GetRecordCount().'<br>';
$recids = $pdb->GetRecordIDs();
$record1 = $pdb->GetRecordRaw($recids[0]);
echo "record1 = $record1<br>";
echo "Desc: ".$record1['description'].'<br>';
echo "Nota: ".$record1['note'].'<br>';
echo 'Fecha: '.$record1['due_date'].'<br>';
echo 'Cat: '.$record1['category'].'<br>';
$categorias = $pdb->GetCategoryList();
echo "NumCat = ".count($categorias).'<br>';
while (list($k,$v) = each($categorias)) {
  echo "categorias[$k] = $v<br>";
  while (list($key,$val) = each($categorias[$k])) {
    echo "  categoria[$key] = $val<br>";
  }
}
 *
 */
?o
[5~[5~[5~
