# sampcmd
`sampcmd` is a command line tool to launch San Andreas Multiplayer.

# Why not `samp.exe`?
`samp.exe` is the UI launcher for San Andreas Multiplayer. If it's invoked with additional parameters, the UI flashes for a single frame.
It is not possible to use different names, unless the entry within the registry is modified.

# How does it work?
The process of GTA San Andreas is launched (detached and suspended) and "samp.dll" is inserted into the remote process with a new remote thread and waited till it's finished. After that the main thread of the remote process is resumed.

# Where do I put `sampcmd.exe`
Put "sampcmd.exe" into the root directory of GTA San Andreas, where San Andreas Multiplayer is installed.

# How to use?
For example: `sampcmd.exe -c -h 127.0.0.1 -p 7777 -n Username`

# Where can I download a `sampcmd` build?
http://bigeti.de/samp/sampcmd.exe
