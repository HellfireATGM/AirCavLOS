@set root=%cd%

@echo Setting up Visual Studio 2015 environment
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\Tools\vsvars32"

cd %root%
devenv AirCavLOS.sln /rebuild "Release|Win32" 
devenv AirCavLOS.sln /rebuild "Debug|Win32" 

devenv AirCavLOS.sln /rebuild "Release|x64" 
devenv AirCavLOS.sln /rebuild "Debug|x64" 

:end
cd %root%

call copy_to_dist.bat

@pause
