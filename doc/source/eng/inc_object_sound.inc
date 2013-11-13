*grp Sound functions

*met PlaySound(Filename, Looping, LoopStart)
*mt2 PlaySound(Looping, LoopStart)
*des Plays a sound. If the filename is omitted, the currently assigned sound is played (if any).
*par Filename The filename of the sound file to be played
*par Looping Whether the sound should be looped (optional, default = false).
*par LoopStart The point from which the sound restarts when looping, in milliseconds (optional, default=0).
*ret If the method succeeds, the return value is <b>true</b>.

*met PlaySoundEvent(Filename, EventName)
*mt2 PlaySoundEvent(EventName)
*des Plays a sound and triggers an event when the sound is over. If the filename is omitted, the currently assigned sound is played (if any).
*par Filename The filename of the sound file to be played
*par EventName The name of the event to be triggered after the sound finishes.
*ret If the method succeeds, the return value is <b>true</b>.

*met StopSound()
*des Stops the currently playing sound (if any).
*ret If the method succeeds, the return value is <b>true</b>.

*met PauseSound()
*des Pauses the currently playing sound.
*ret If the method succeeds, the return value is <b>true</b>.

*met ResumeSound()
*des Resumes a paused sound playback.
*ret If the method succeeds, the return value is <b>true</b>.

*met IsSoundPlaying()
*des Queries whether a sound playback is in progress.
*ret If the sound is playing, the return value is <b>true</b>.

*met SetSoundPosition(Time)
*des Sets the sound's current playing position.
*par Time A new playing position (in milliseconds)
*ret If the method succeeds, the return value is <b>true</b>.

*met GetSoundPosition()
*des Queries the sound's current playing position
*ret Returns the current playing position (in milliseconds).

*met SetSoundVolume(Volume)
*des Sets a volume of the currently playing sound.
*par Volume A new sound volume (in percent).
*ret If the method succeeds, the return value is <b>true</b>.

*met GetSoundVolume()
*des Gets a volume of the currently playing sound.
*ret Returns the volume of the currently playing sound (in percent).

*met LoadSound(Filename)
*des Initializes a sound from file and makes it ready to be played.
*par Filename The sound file to be loaded
*ret If the method succeeds, the return value is <b>true</b>.
*rem Use this method to pre-cache a sound file. Initializing the sound takes some time and sometimes it's desirable to move all sound initialization to e.g. scene start so that the subsequent PlaySound calls don't slow the game down. That's what LoadSound is for. To play the pre-cached sound simply call PlaySound without specifying a filename, for example SomeObject.PlaySound(false);

*grp Sound effects functions

*met SoundFXNone()
*des Removes any sound effect currently assigned to this object.

*met SoundFXEcho(WetDryMix, Feedback, LeftDelay, RightDelay)
*des Assigns an echo effect to the sounds played by this object.
*par WetDryMix Wet/Dry Mix, in percent (0 to 100)
*par Feedback Feedback, in percent (0 to 100)
*par LeftDelay Left delay, in milliseconds (1 to 2000)
*par RightDelay Right delay, in milliseconds (1 to 2000)
*rem Example: <b>actor.SoundFXEcho(50, 50);</b>


*met SoundFXReverb(InGain, ReverbMix, ReverbTime, HighFreqRTRatio)
*des Assigns a reverb effect to the sounds played by this object.
*par InGain In gain, in dB (-96 to 0)
*par ReverbMix Reverb mix, in dB (-96 to 0)
*par ReverbTime Reverb time, in milliseconds (0.001 to 3000)
*par HighFreqRTRatio HighFreq RT Ratio (0.001 to 0.999)
*rem Example: <b>actor.SoundFXReverb(0, 0, 2000);</b>
