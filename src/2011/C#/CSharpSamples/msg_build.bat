

rem http://re-thinkthings.spaces.live.com/blog/cns!1C5AE98DA3645F42!511.entry

set TargetDir=d:\temp\msg
set TargetName=msg_test
set MessageFile=%TargetName%.mc
set ResourceFile=%TargetName%.rc
set ResourceScriptFile=%TargetName%.res

call "C:\Program Files\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86

cd %TargetDir%

mc -s %MessageFile%
rc %ResourceFile%
link /DLL /SUBSYSTEM:WINDOWS /NOENTRY /MACHINE:x86 %ResourceScriptFile%

