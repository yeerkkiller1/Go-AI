:start

call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" x64

cl /Tc "TestCPlusPlusAI\EntryPoint.cpp" /Tc "TestCPlusPlusAI\Location.cpp" /Tc "TestCPlusPlusAI\Piece.cpp" /Tc "TestCPlusPlusAI\libertyGroup.cpp" /Tc "TestCPlusPlusAI\Board.cpp" /Tc "TestCPlusPlusAI\MontecarloSimulation.cpp" /Tc "TestCPlusPlusAI\Prune.cpp" /Tc "TestCPlusPlusAI\q.cpp" /Tc "TestCPlusPlusAI\FastRandom.cpp" /Tc "TestCPlusPlusAI\FastSetReplacement.cpp" /Ox

echo.
echo ---------------------------------------
echo Running Now
echo ---------------------------------------
echo.

EntryPoint.exe

echo.
echo.
echo ---------------------------------------
echo Done running, continue to compile and run again
echo ---------------------------------------
echo.
echo.

pause

goto start