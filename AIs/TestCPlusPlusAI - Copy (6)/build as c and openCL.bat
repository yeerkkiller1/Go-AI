:start

:OptimizedBuild
call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" x64
cl /Tc "TestCPlusPlusAI\EntryPoint.cpp" /Tc "TestCPlusPlusAI\Location.cpp" /Tc "TestCPlusPlusAI\Piece.cpp" /Tc "TestCPlusPlusAI\libertyGroup.cpp" /Tc "TestCPlusPlusAI\Board.cpp" /Tc "TestCPlusPlusAI\MontecarloSimulation.cpp" /Tc "TestCPlusPlusAI\Prune.cpp" /Tc "TestCPlusPlusAI\q.cpp" /Tc "TestCPlusPlusAI\FastRandom.cpp" /Tc "TestCPlusPlusAI\FastSetReplacement.cpp" /Ox /Ob2


:CreateSource
echo.
set combineError = "C:\Users\QuentinBrooks\Dropbox\Go AI\New C++ AI Build\OpenCL Compiler\Debug\OpenCLC2.exe" include_entrypoint nobuild writesource
IF [combineError]=="/?" echo worked
echo.

:CombineBuild
call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" x64
cl /Tc "TestCPlusPlusAI\COMBINED_SOURCE.cpp" /Ox /Ob2

:OpenCLBuild
echo.
"C:\Users\QuentinBrooks\Dropbox\Go AI\New C++ AI Build\OpenCL Compiler\Debug\OpenCLC2.exe"
echo.



echo.
echo ---------------------------------------
echo Running Now
echo ---------------------------------------
echo.

echo ---------Non-optimized Build------------
call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" x64
cl /Tc "TestCPlusPlusAI\EntryPoint.cpp" /Tc "TestCPlusPlusAI\Location.cpp" /Tc "TestCPlusPlusAI\Piece.cpp" /Tc "TestCPlusPlusAI\libertyGroup.cpp" /Tc "TestCPlusPlusAI\Board.cpp" /Tc "TestCPlusPlusAI\MontecarloSimulation.cpp" /Tc "TestCPlusPlusAI\Prune.cpp" /Tc "TestCPlusPlusAI\q.cpp" /Tc "TestCPlusPlusAI\FastRandom.cpp" /Tc "TestCPlusPlusAI\FastSetReplacement.cpp"

echo -------------Running Now---------------
EntryPoint.exe
echo.


echo -------------Running Now---------------
EntryPoint.exe
echo.



echo -------Running Combined Source----------
COMBINED_SOURCE.exe

echo.
echo.
echo ---------------------------------------
echo Done running, continue to compile and run again
echo ---------------------------------------
echo.
echo.

pause

goto start