set ConfigurationName=Debug
set OutDir=..\bin\Debug
set SolutionDir=..\

xcopy "%OGRE_HOME%\Bin\%ConfigurationName%\*.dll" "%OutDir%" /s/e/d/i/y
xcopy "%SolutionDir%data" "%OutDir%\data\" /s/e/d/i/y
xcopy "%SolutionDir%config" "%OutDir%\config\" /s/e/d/i/y
xcopy "%SolutionDir%externals\Fmod\dll\*.dll" "%OutDir%" /s/e/d/i/y
xcopy "%SolutionDir%externals\Lua-5.0-SDK\lib\*.dll" "%OutDir%" /s/e/d/i/y
xcopy "%SolutionDir%externals\ParticleUniverse\bin\%ConfigurationName%\*.dll" "%OutDir%" /s/e/d/i/y
xcopy "%SolutionDir%externals\TheoraVideoSystem\bin\*.dll" "%OutDir%" /s/e/d/i/y

pause