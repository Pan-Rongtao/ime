@echo off

set msbuild_execute="C:\Program Files (x86)\MSBuild\14.0\Bin\msbuild.exe"
set build_dir="build"
if not exist "%build_dir%" md "%build_dir%"
cd "%build_dir%"

cmake -DCMAKE_INSTALL_PREFIX=../dist -G "Visual Studio 14 2015" ..
@%msbuild_execute% INSTALL.vcxproj /p:Configuration="debug"
@%msbuild_execute% INSTALL.vcxproj /p:Configuration="release"

@pause