
set ArchiveName=GeneralsProxyLauncher_v1.9
set ReleaseDir=Release
set ReleaseUnpackedDir=ReleaseUnpacked
set ReleaseUnpackedLauncherDir=%ReleaseUnpackedDir%\launcher

if not exist %ReleaseDir% mkdir %ReleaseDir%
if not exist %ReleaseUnpackedDir% mkdir %ReleaseUnpackedDir%
if not exist %ReleaseUnpackedLauncherDir% mkdir %ReleaseUnpackedLauncherDir%

xcopy /Y Code\vc9\Release\generals.exe %ReleaseUnpackedLauncherDir%\

tar.exe -a -c -C %ReleaseUnpackedDir% -f %ReleaseDir%\%ArchiveName%.zip *.*

pause
