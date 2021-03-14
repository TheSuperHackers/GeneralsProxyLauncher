Generals Proxy Launcher for COMMAND AND CONQUER GENERALS: ZERO HOUR

Author:
xezon

-----------------------------------------------------------------------------------------------
--- Install -----------------------------------------------------------------------------------

1. Copy the "launcher" folder and all its contents
   into the game install directory, for example:
   > C:\Program Files (x86)\EA Games\Command & Conquer Generals Zero Hour

   [!!] DO NOT extract the files (!) into the root directory of the game

   Directory hierarchy should look like this:
   > C:\Program Files (x86)\EA Games\Command & Conquer Generals Zero Hour\launcher\

2. Start GameRanger

3. Open Edit > Options > Games

4. Select the game from the list

5. Click Browse... and navigate to generals.exe in launcher directory, for example:
   > C:\Program Files (x86)\EA Games\Command & Conquer Generals Zero Hour\launcher\generals.exe


-----------------------------------------------------------------------------------------------
--- Changelog ---------------------------------------------------------------------------------

v1.0:
- Initial release

v1.1:
- Added commandline.txt to specify custom parameters along with default " -win"

v1.2
- Added proper MapsZH.big file format to avoid issues in generals file parser

v1.3
- Fixed Error 740: Added UAC Execution Level "highestAvailable"
- Changed project and strings to Unicode (commandline.txt remains ansi)
- Added transfer of correct default working directory to spawning generals.exe
- Added functionality to copy d3d8.dll from root dir to launcher dir

v1.4
- Reverted d3d8.dll copying

v1.5
- Use ShellExecute instead of CreateProcess launch method

v1.6
- Improved install instructions in ReadMe.txt
- Minor code refactoring

v1.7
- Added .big swap support for enabling and disabling .big files on execution (see big.txt file)

v1.8
- Added safety mechanism in .big file swap
- Added single instance safety

v1.9
- Use ShellExecute instead of CreateProcess launch method
