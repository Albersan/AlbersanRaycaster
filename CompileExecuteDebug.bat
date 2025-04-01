@echo off
echo *********************************************************************************************************************
echo ************************************************** Compile Project **************************************************
echo *********************************************************************************************************************
echo.
:PROMPT
cmake --build build --config Debug

if not exist "build\Debug\maps" mkdir "build\Debug\maps"
if not exist "build\Debug\sounds" mkdir "build\Debug\sounds"
if not exist "build\Debug\textures" mkdir "build\Debug\textures"

xcopy /E /I /Y "src\maps\*" "build\Debug\maps\"
xcopy /E /I /Y "src\sounds\*" "build\Debug\sounds\"
xcopy /E /I /Y "src\textures\*" "build\Debug\textures\"
xcopy /I /Y "libs\SDL2\bin\*.dll" "build\Debug\" 
cd build/Debug/   
"AlbersanRaycaster.exe"
:END