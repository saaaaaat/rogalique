@echo off
:: Batch file to build the Game solution

:: 1. Define paths
set SOLUTION_NAME=Game.sln
set MSBUILD_PATH="C:\Program Files\Microsoft Visual Studio\18\Community\MSBuild\Current\Bin\amd64\MSBuild.exe"
set CONFIGURATION=Debug
set PLATFORM=x64

:: 2. Check if MSBuild exists
if not exist %MSBUILD_PATH% (
    echo MSBuild not found. Please check your Visual Studio installation.
    pause
    exit /b 1
)

:: 3. Build the solution
echo Building solution %SOLUTION_NAME%...
%MSBUILD_PATH% %SOLUTION_NAME% /p:Configuration=%CONFIGURATION% /p:Platform=%PLATFORM%

:: 4. Check result
if %ERRORLEVEL% neq 0 (
    echo Build failed with errors.
    pause
    exit /b %ERRORLEVEL%
)

:: 5. Copy SFML DLLs
if exist "D:\rogalic\SFML\SFML-2.5.1\bin" (
    xcopy /Y "D:\rogalic\SFML\SFML-2.5.1\bin\*.dll" "D:\rogalic\x64\%CONFIGURATION%\" >nul
    echo Copied SFML DLLs.
)

:: 6. Copy Resources
if exist "D:\rogalic\Resources" (
    xcopy /E /I /Y "D:\rogalic\Resources" "D:\rogalic\x64\%CONFIGURATION%\Resources" >nul
    echo Copied resources.
)

echo Build completed successfully.
pause