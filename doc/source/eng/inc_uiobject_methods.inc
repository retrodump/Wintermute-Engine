;*grp Miscelaneous functions

*met SetFont(Filename)
*des Sets a normal font.
*par Filename A filename of a font file to be used.
*ret If the method succeeds, the return value is <b>true</b>.

*met SetImage(Filename)
*des Sets a background image (sprite).
*par Filename A filename of a sprite file to be used.
*ret If the method succeeds, the return value is <b>true</b>.

*met GetImage()
*des Returns the filename of the background sprite (or null if no sprite is set)

*met GetImageObject()
*des Returns a reference to the background sprite (or null if no sprite is set)


*met Focus()
*des Focuses the control.

*met MoveAfter(Control)
*mt2 MoveAfter(ControlName)
*des Moves the control in z-order after a given control.
*par Control A control object we will move our control after
*par ControlName A name of a control object we will move our control after
*ret If the method succeeds, the return value is <b>true</b>.

*met MoveBefore(Control)
*mt2 MoveBefore(ControlName)
*des Moves the control in z-order before a given control.
*par Control A control object we will move our control before
*par ControlName A name of a control object we will move our control before
*ret If the method succeeds, the return value is <b>true</b>.

*met MoveToTop()
*des Moves the control to the top of the z-order.
*ret If the method succeeds, the return value is <b>true</b>.

*met MoveToBottom()
*des Moves the control to the bottom of the z-order.
*ret If the method succeeds, the return value is <b>true</b>.



*grp 

*atr Parent
*des Returns a reference to a parent window of this control (or null)
*ro

*atr ParentNotify
*des Specifies whether this control should notify its parent about events

*atr Width
*des A width of the control

*atr Height
*des A height of the control

*atr Visible
*des Specifies whether the control is visible

*atr Disabled
*des Specifies whether the control is disabled

*atr Text
*des A text the control displays

*atr NextSibling
*des Returns a reference to the next control in z-order
*ro

*atr PrevSibling
*des Returns a reference to the previous control in z-order
*ro

