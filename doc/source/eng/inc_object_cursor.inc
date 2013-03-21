*grp Cursor functions

*met SetCursor(Filename)
*des Sets the standard cursor.
*par Filename Filename of a sprite file to be used as a cursor pointer.
*ret Returns <b>true</b> if the cursor has been set succesfuly.

*met GetCursor()
*des Returns the filename of the cursor sprite (or null if no cursor is set)

*met GetCursorObject()
*des Returns a reference to the cursor sprite (or null if no cursor is set)

*met RemoveCursor()
*des Removes the standard cursor.

*met HasCursor()
*des Queries whether the standard cursor is set.
*ret Returns <b>true</b> if the object has a custom mouse pointer assigned.

