*grp Miscelaneous functions


*met Reset()
*des Cancels the action the object was currently performing (like talking, walking etc.).

*met IsTalking()
*des Queries whether the object is currently talking.
*ret Returns <b>true</b> if the object is talking.

*met StopTalking()
*des Cancels object's talking.
*rem You can also use StopTalk() abbreviation.

*met Talk(Text, SoundFilename, Duration, TalkStances, TextAlignment)
*mt2 TalkAsync(Text, SoundFilename, Duration, TalkStances, TextAlignment)
*des Makes the object talk.
*par Text A text to be used as a talk subtitle
*par SoundFilename A filename of a sound file to be used (oprtional, default is no sound)
*par Duration A subtitle duration in milliseconds (optional, default is 0)
*par TalkStances A comma separated list of talk "stances" to be used for the talking (optional, default is random stances)
*par TextAlignment A text alignment for the subtitle (0-left, 1-right, 2-center) (optional, default=2)
*rem Talk method blocks the script execution until the animation is over, while the TalkAsync method returns immediately. If the duration is set to zero, it's calculated automatically either from the sound file or from the length of the subtitle.


*met StickToRegion(Region)
*mt2 StickToRegion(RegionName)
*des Forces this actor/entity to act like it's inside a given region
*par Region Reference to a scene region object
*par RegionName Name of a scene region
*rem Use this method to "lock" a z-order value of an actor or an entity.

*met SetFont(Filename)
*des Sets font of this object.
*par Filename The name of the font to be set.
*ret If the method succeeds, the return value is <b>true</b>.


*met GetFont()
*des Gets a font assigned to this object.
*ret Returns the filename of currently assigned font.


*grp Particle functions

*met CreateParticleEmitter(FollowOwner, OffsetX, OffsetY)
*des Initializes the particle emitter attached to this object.
*par FollowOwner Specifies whether the particle emitter inherits position of its owner object
*par OffsetX If FollowOwner is true, this is the X offset to be added to owner's position when displaying particles
*par OffsetY If FollowOwner is true, this is the Y offset to be added to owner's position when displaying particles
*ret Returns the reference to the particle emitter object
*rem Once initialized with this method, the particle emitter object can be queried using the <b>ParticleEmitter</b> property.

*met DeleteParticleEmitter()
*des Deletes the particle emitter attached to this object.
*ret Returns <b>true</b> if the method succeeded.
