set ArchiveName=GeneralsProxyLauncher_v2.2

set ReleaseDir=Release
set ReleaseUnpackedDir=ReleaseUnpacked
set ReleaseUnpackedLauncherDir=%ReleaseUnpackedDir%\launcher

if not exist %ReleaseDir% mkdir %ReleaseDir%
if not exist %ReleaseUnpackedLauncherDir% mkdir %ReleaseUnpackedLauncherDir%

xcopy /Y Code\vc9\Release\*.exe %ReleaseUnpackedLauncherDir%\

tar.exe -a -c -C %ReleaseUnpackedDir% -f %ReleaseDir%\%ArchiveName%.zip *.*
