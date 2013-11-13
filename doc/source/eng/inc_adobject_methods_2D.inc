*inc inc_adobject_methods.inc


;*grp Miscelaneous functions

*met PlayAnim(Filename)
*mt2 PlayAnimAsync(Filename)
*des Plays an animation.
*par Filename A filename of a sprite file to be played
*ret Returns <b>true</b> if the animation has been played successfuly.
*rem The PlayAnim method blocks the script execution until the animation is over, while the PlayAnimAsync method returns immediately.
*rem For actors, you can also specify an animation set name instead of filename.


*met ForceTalkAnim(Filename)
*des Force a special animation to be used for a subsequent Talk call.
*par Filename A filename of the sprite file to be used as a talking animation
*rem This setting has only effect on one subsequent Talk() method call. After the talking is over, the talking animation is reset back to default.

*met AddAttachment(EntityFile, PreDisplay, OffsetX, OffsetY)
*des Adds an attached entity to this object.
*par EntityFile The filename of the entity to be attached.
*par PreDisplay Specifies whether the attached entitiy is drawn below the object or on top of it (optional, default=true)
*par OffsetX The X offset of the attachment relative to object's position
*par OffsetY The Y offset of the attachment relative to object's position
*ret Returns <b>true</b> is the attachment has been successfuly attached.
*rem This method is typically used for attaching a "blob" shadow image to an actor or an entity.
*rem The X and Y offset is scaled accordingly to object's current scale.

*met RemoveAttachment(AttachmentName)
*mt2 RemoveAttachment(AttachmentIndex)
*des Removes an attached entity from this object.
*par AttachmentName The name of the entity to be detached
*par AttachmentIndex The index of the attachment to be detached
*ret Returns <b>true</b> if the attachment has been successfuly detached.

*met GetAttachment(AttachmentName)
*mt2 GetAttachment(AttachmentIndex)
*des Gets a reference to an attached entity object.
*par AttachmentName The name of the attached entity to be queried
*par AttachmentIndex The index of the attachment to be queried
*ret Returns a reference to the attached entity or <b>null</b> if specified attachement cannot be found.

