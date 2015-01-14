Overwrite Warning
=================

If you install a database created with a module that cites this overwrite warning, it will *overwrite* any database that already exists on your handheld.  This is because a new database will have the same name as the current database on your handheld and you can't change the name of the database otherwise the application won't work.

This is obviously irritating, so please keep this warning in mind.

An ideal use for this PHP class would be for talking to a conduit, where the conduit would upload the current data, the server would parse it and add/modify/delete entries with this class, the server would send the modified file back to the conduit, and the conduit would replace the device's database with the modified one.
