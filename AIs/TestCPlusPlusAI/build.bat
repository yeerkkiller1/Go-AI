:start

call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" x64

cl "TestCPlusPlusAI\EntryPoint.cpp" "TestCPlusPlusAI\Location.cpp" "TestCPlusPlusAI\Piece.cpp" "TestCPlusPlusAI\libertyGroup.cpp" "TestCPlusPlusAI\Board.cpp" "TestCPlusPlusAI\MontecarloSimulation.cpp" "TestCPlusPlusAI\Prune.cpp" "TestCPlusPlusAI\q.cpp" "TestCPlusPlusAI\FastRandom.cpp" "TestCPlusPlusAI\FastSetReplacement.cpp" /EHsc /Ox

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