<?PHP

/* Class extender for PalmOS Datebook files
 *
 * Copyright (C) 2001 Tyler Akins
 * Licensed under the GNU LGPL
 * See the LEGAL file for more information
 */
 
class PalmDatebook extends PalmDB {
   function PalmDatebook () {
      $this->Name = 'DatebookDB';
      $this->Type = 'date';
      $this->Creator = 'DATA';
   }
}

?>
