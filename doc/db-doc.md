DOC File Module
===============

The DOC file format is the standard way of storing text files on a handheld.  You will need a DOC reader in order to see any DOC file you have loaded onto your device.  There are several ones listed below.  To create DOC files, there are many programs already out there that will work for you.  Additionally, you can use [Twister](../samples/twister.php), which just uses PHP and this PHP-PDB library to create DOC files from plain text files, web pages, and files from [Project Gutenberg](http://promo.net/pg/).


Including into your program
---------------------------

    include 'php-pdb.inc';
    include 'modules/doc.inc';


Creating a new database
-----------------------

DOC files already have a specified type and creator ID.  The only thing
that is left for you to specify is the name of the document you wish to
create.

    $DB = new PalmDoc("Name Of Document");
      // Typical usage to create a compressed DOC file

    $DB = new PalmDoc("Uncompressed DOC", false);
      // This is how you create an uncompressed DOC file

    $pdb = new PalmDoc();
      // Special:  If you want to create an instance of the class
      // and then use ReadFile() to load the database information


Writing the database
--------------------

This is the same as the base class.  See the [API] for more information.

If the `$pdb` was set to be a compressed file, the contents will be transparently compressed.  Also, you can further manipulate the text normally after writing -- it will just be recompressed every time you write the database if the contents were changed.

<h3>Loading the database</h3>

This is the same as the base class.  See the [API] for more information.

<h3>Category Support and Record Attributes</h3>

DOC files do not support categories nor record attributes.

<h3>Adding Bookmarks</h3>

In DOC files, there are two ways to make bookmarks -- stored and embedded.  Stored bookmarks are where additional records are added to the output file.  These are nicer to use, and most DOC readers support them.  Embedded bookmarks are embedded in the text and require the DOC reader to scan the entire DOC file the first time you read it.  They use a unique character at the beginning of a line to mark that line as a bookmark.  Then, to signify which character is the "bookmark" character, you include it at the end of the DOC file in angle brackets.

Make sure to just pick one type of bookmarks to use!  In the [tests](../samples/bookmark_test.php) that I have performed, it appears that if the bookmark reader can handle both types of bookmarks, it will only search for embedded bookmarks if there are no stored bookmarks.

If you decide to use embedded bookmarks, be very careful of the bookmark character that you pick, because some doc readers don't check to see if that character is at the beginning of a line before blindly adding it to the bookmark list.  If you pick a character like an apostrophe or a period, then you are potentially in for a huge surprise.

Stored bookmark names are limited to 15 characters.  The maximum length for embedded bookmark names could vary.


Doc Readers and Results of Bookmark Test
----------------------------------------

This list is not a comprehensive list of DOC readers.  If you know of a
DOC reader that is not yet on this list, just email me the program name and
URL, and I'll test it to see what kinds of bookmarks are supported.  You
can also see my test [here](../samples/bookmark_test.php).

| Program                  | Stored | Embedded | Notes                       |
|--------------------------|:------:|:--------:|-----------------------------|
| AportisDoc 2.2.3         |   Yes  |    Yes   | The one that started it all |
| CSpotRun 1.1             |   Yes  |    No    | Freeware, open source       |
| iSilo 3.05               |   Yes  |    No    | Paid version of iSilo       |
| iSilo Free 1.5           |   No   |    No    | Free version of iSilo       |
| MiniWrite 1.4            |   Yes  |    No    | DOC reader / writer         |
| QED 2.62                 |   Yes  |    Yes   | DOC reader / writer         |
| ReadThemAll 1.65         |   No   |    No    |                             |
| RichReader 1.62 Freeware |   Yes  |    No    | Free version of RichReader  |
| Smoothy 1.5.0            |   Yes  |    No    |                             |
| SuperDoc 1.4             |   Yes  |    Yes   |                             |
| TealDoc 4.51D            |   Yes  |    Yes   |                             |


Other functions
---------------


### `AddBookmark($name, $position = false)`

If `$position` is specified, adds a bookmark at `$position` bytes into the file.  This is the uncompressed count of bytes, not the number of bytes of compressed text.

If `$position` is not specified, adds a bookmark at the current position.  I strongly recommend using this form.

`$name` is limited to 15 characters and will automatically be trimmed if it is too long.


### `AddText($string)`

Adds the specified text to the end of the doc file.  To get newlines, use "\n".  This function can add single words, entire lines, paragraphs, or all of the text at once.  There is no size nor line limit.  It would be wise to have less than a few hundred kilobytes of total text, but that's at your discretion.


### `EraseText()`

Erases all text from the document being generated.


### `GetText()`

Returns a single string (potentially very large) that contains the entire document's text.


Example
-------

    $pdb = new PalmDoc("Doc Test");
    $pdb->AddText("This is a test.  This is a test of the PHP-PDB DOC class.\n");
    $pdb->AddText("This is only a test.\n");
    $pdb->AddText("This DOC will be automatically compressed.");
    $pdb->DownloadPDB("doc_test.pdb");

Another example is the [bookmark test](../samples/bookmark_test.php) that I used in order to find out what bookmark types are supported with each listed DOC reader.


[API]: api.md
