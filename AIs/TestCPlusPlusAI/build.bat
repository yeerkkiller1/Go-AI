:start

call "C:\Program Files (x86)\Microsoft Visual Studio 11.0\VC\vcvarsall.bat" x64

cl "C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\EntryPoint.cpp" "C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\Metadata.cpp" "C:\Users\QuentinBrooks\Dropbox\Go AI\AIs\TestCPlusPlusAI\TestCPlusPlusAI\MontecarloSimulation.cpp" /I "C:\Program Files\boost_1_49_0\boost_1_49_0" /EHsc /Ox /Gs /link shell32.lib

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