:start


"C:\Program Files (x86)\tcc\tcc.exe" "TestCPlusPlusAI\EntryPoint.cpp" "TestCPlusPlusAI\Location.cpp" "TestCPlusPlusAI\Piece.cpp" "TestCPlusPlusAI\libertyGroup.cpp" "TestCPlusPlusAI\Board.cpp" "TestCPlusPlusAI\MontecarloSimulation.cpp" "TestCPlusPlusAI\Prune.cpp" "TestCPlusPlusAI\q.cpp" "TestCPlusPlusAI\FastRandom.cpp" "TestCPlusPlusAI\FastSetReplacement.cpp" /I "C:\Program Files\boost_1_49_0\boost_1_49_0" /EHsc /Ox /Gs /link shell32.lib

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