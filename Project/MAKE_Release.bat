set ArchiveName=GeneralsProxyLauncher_v2.0

set ReleaseDir=Release
set ReleaseUnpackedDir=ReleaseUnpacked
set ReleaseUnpackedLauncherDir=%ReleaseUnpackedDir%\launcher
set ReleaseUnpackedLauncherShellDir=%ReleaseUnpackedDir%\launcherShell

if not exist %ReleaseDir% mkdir %ReleaseDir%
if not exist %ReleaseUnpackedLauncherDir% mkdir %ReleaseUnpackedLauncherDir%
if not exist %ReleaseUnpackedLauncherShellDir% mkdir %ReleaseUnpackedLauncherShellDir%

xcopy /Y Code\vc9\Release\*.exe %ReleaseUnpackedLauncherDir%\
xcopy /Y Code\vc9\Release_ShellExecute\*.exe %ReleaseUnpackedLauncherShellDir%\

tar.exe -a -c -C %ReleaseUnpackedDir% -f %ReleaseDir%\%ArchiveName%.zip *.*
