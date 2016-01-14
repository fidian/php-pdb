<?PHP
/* Detail page describing Spade
 *
 * Copyright (C) 2001 - PHP-PDB development team
 * Licensed under the GNU LGPL software license.
 * See the doc/LEGAL file for more information
 * See https://github.com/fidian/php-pdb for more information about the library
 *
 * Please note that this page *IS* covered by the LGPL, but Spade itself
 * is not (or at least isn't yet).
 */

include '../functions.inc';

StandardHeader('Spade', 'samples');

?>

<p>Spade is a <b>s</b>imple <b>ad</b>dress book <b>e</b>ditor, written by
Roman Schindlauer &lt;<?PHP HideEmail('roman', 'fusion.at') ?>&gt;  It's
just getting started and shows a great deal of potential.</p>

<p>The current version is 0.1:</p>

<ul>
<li><a href="../tools/spade.tar.gz">Download</a></li>
<li><a href="spade/">Online Demo</a></li>
</ul>

<p><i>Warning:</i> Since people can edit the records however they see fit,
there might be vulgarity or other assorted nastiness.  Also, since Spade
is somewhat new, this might crash.  However, the database is cleared
nightly so problems will dissapear and any edits will eventually be lost.</p>

<p>I have made a couple changes to Spade so that it works without errors
on my setup, but the changes are very minimal.</p>

</ul>

<p>As quoted from his email to me:</p>

<?PHP

ShowExample("
i attached my Simple ADdressbook Editor (spade :). it expects php-pdb to
be a subdirectory, but this path can be changed at the top of index.php,
as well as the path of the addressbook pdb file itself. the code is very
beta, so don't use your one and only pdb with it :) but actually, i
didn't have any critical problems concerning loosing data. if it reaches
a release-able stage and you think it fits into the source bundle, you
can of course include it! eventually this would be an incentive to make
something real useful out of it!

there is no documentation included yet. if you click on a name of the
first column, you can edit this record. a deleted record can be
undeleted (unless synchronized yet). deleted and edited records are
highlighted in the list. there is an http-authentication included,
user=\"user\" and password=\"pwd\", both can be changed in auth.php. that's
all, basically.

regards,
roman
");

StandardFooter();
