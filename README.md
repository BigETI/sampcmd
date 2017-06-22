# sampcmd
"sampcmd" is a command line tool to launch San Andreas Multiplayer.

# Why not "samp.exe"?
"samp.exe" is the UI launcher for San Andreas Multiplayer. If it's invoked with parameters to launch, the UI flashes for a single frame. And it is not possible to use different names, unless the entry within the registry is modified.

# How does it work?
The process of GTA San Andreas is launched (suspended) and "samp.dll" is inserted into the remote process.

# How to use
`sampcmd.exe -c -h 127.0.0.1 -p 7777 -n Username`
