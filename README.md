PHP-PDB
=======

This is being moved over from SourceForge.  See all of the documentation at [php-pdb.sourceforge.net/](http://php-pdb.sourceforge.net/) until it appears here.

PHP-PDB is a set of [PHP] classes that manipulate [Palm OS] databases.  It lets you read, write, alter, and easily use data that is meant to be sent to or retrieved from a handheld.  It requires PHP 4.0.1 or later, but due to security problems with PHP 4, I'd suggest you have version 4.1.2 or patch your current version.


Directly Supported Databases
----------------------------

These types of databases have had separate classes written to extend the base class and make life easier for the developer.  PHP-PDB can be used with your own [custom database](doc/db-custom.md) type as well.

* [Address Book](doc/db-addrbook.md) - Reading and writing fully supported.
* [Datebook](doc/db-datebook.md) - Reading and writing fully supported.
* [DOC](doc/db-doc.md) - Can read and write compressed and uncompressed doc files.  Can use embedded and stored bookmarks.
* [List](doc/db-list.md) - Reading and writing fully supported.
* [MobileDB](doc/db-mobiledb.md) - Can read and write databases.  No utility functions yet, so this class is difficult to use.
* [SmallBASIC](doc/db-smallbasic.md) - Can read and write databases.  Easily convert from/to this format.


Features
--------

* The base class can be extended to provide easy support for specific types of databases.
* The class supports reading databases as well as writing databases.
* Easily be able to pipe a Palm OS database to the browser or out to a file.
* Category support, but it is currently somewhat limited.
* Limited appinfo and sortinfo support exists.


As Seen In ...
--------------

This PHP class works quite well and it is being used in the following manners.  If you want your site listed here, I'd love to add it -- tell me about it!

* [Ampoliros](http://www.ampoliros.com/) - Used as a package for a web-based system that will be used by other various modules to import/export data.
* [Marco](http://rumkin.com/software/marco/) - Data point conversion from different formats into the internal format used by Marco.
* [LDS Trivia](http://www.shiblon.com/trivia/) - Real-time trivia database creation from questions submitted on the web site.
* [Spade](esamples/spade.php) - A simple address book editor that has enough potential, especially if you use Coldsync or another tool to synchronize databases.
* [Twister!](examples/twister.php) - Web page, text and [Project Gutenberg](http://promo.net/pg/) file conversion to DOC or SmallBASIC format.


Credits
-------

This is all new code (not based on another project), but credit should be given to people who wrote other open-source programs that I learned from.  Also, this list is to give thanks to people and companies who have helped me out with PHP-PDB.

* Arensburger, Andrew - Thanks for making [P5-Palm](P5-Palm)!  I used several ideas when creating PHP-PDB, and a few comments really helped me out.
* Barth, Christoph - Wrote a [patch](tools/mgw_palm_pdb_patch.tar.gz) to add export to Palm for [moregroupware](http://www.moregroupware.org/).  Here's the [description](tools/mgw_palm_pdb_patch.txt) that I wrote about it.
* Beirne, Pat - Created the first [DOC](doc/db-doc.md) compression scheme.  See it [here](http://www.linuxmafia.com/pub/palmos/development/pilotdoc-compression.html).
* Christopoulos, Nicholas - He is the "development team" of [SmallBASIC](http://smallbasic.sourceforge.net/), and he explained the [SmallBASIC record layout](http://smallbasic.sourceforge.net/cgi-bin/wboard.cgi?board=wpalm?action=display?num=239) further for me so the [SmallBASIC module](doc/db-smallbasic.md) could be created.
* Dean, Wayne - Helped me out with finding out why the Desktop 4.0 didn't like installing PHP-PDB generated .pdb files.  (Padding issue.)
* Dittgen, Pierre - His [palmlib](http://mmmm.free.fr/palm/download/) (aka [ToPTIP](http://depot.free-system.com/index.php3?projectid=2&action=show")) was handy to have around when initially adding [DOC](doc/db-doc.md) support to compare output and make sure mine worked.
* Gupta, Kartikaya - Fixed some problems when dealing is DOC databases.
* [Handmark](http://www.handmark.com/) - Provided detailed database structure [information](http://www.handmark.com/products/mobiledb/dbstructure.htm) for [MobileDB](http://www.handmark.com/products/mobiledb/).  Cassidy Lackey answered many of my questions.  Thanks for helping to get the [MobileDB module](doc/db-modbiledb.md) class going!
* Low, Andrew - Created [List](http://www.magma.ca/~roo/list/list.html).
* Martinez, Eduardo Pascual - Created the classes to handle [Todo](doc/db-todo.md) and [Address Book](doc/db-addrbook.md) formats.
* Nuernberg, Cristiano - Submitted a much better example for MobileDB than what I had previously.
* [Palm Computing, Inc.](http://www.palm.com) - Without them, there would not be a [Palm OS].  Also, they provided [documentation](http://www.palmos.com/dev/tech/docs/) (especially detailing the [file formats](http://www.palmos.com/dev/tech/docs/FileFormats/FileFormatsTOC.html)) that made my life significantly easier.
* Pereira, Jaime - Gave me the code to LoadDouble().
* [Pyrite](http://www.pyrite.org/) - They provided a great [description](http://www.pyrite.org/doc_format.php) about the compression format and stored bookmarks which helped a lot for the [DOC module](doc/db-doc.md).
* Schindlauer, Roman -- Worked out problems regarding address books and categories.  Also wrote [Spade](examples/spade.php).


License
-------

PHP-PDB is licensed under the GNU LGPL version 2.  See [doc/LEGAL](doc/LEGAL) for the full text of the license.


[PHP]: http://www.php.net/
[Palm OS]: http://www.palmos.com/
