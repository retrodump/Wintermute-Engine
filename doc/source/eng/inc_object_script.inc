*grp Script functions

*met AttachScript(Filename)
*des Executes a script file and attaches it to the object.
*par Filename The filename of the script file to be executed.
*ret Returns <b>true</b> if the script has been executed succesfuly.
*rem Each WME object can have multiple scripts attached.

*met DetachScript(Filename, KillThreads)
*des Terminates a specified script file and detaches it from an object.
*par Filename The filename of the (running) script to be terminated and detached.
*par KillThreads Specifies whether to terminate all running threads of this script (optional, default=false)
*ret Returns <b>true</b> if the script has been detached succesfully.
*rem Script threads are currently executed event handlers and methods.

*met IsScriptRunning(Filename)
*des Queries whether a specified script file is running and attached to an object.
*par Filename The filename of the script to be queried.
*ret Returns <b>true</b> if a specified script file is attached to an object.

*met CanHandleMethod(MethodName)
*des Queries whether the object supports a method of a specified name.
*par MethodName The name of the method to be checked.
*ret Returns <b>true</b> if the object is able to handle the specified method.
