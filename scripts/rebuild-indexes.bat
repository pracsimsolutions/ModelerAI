@echo off
setlocal enabledelayedexpansion

REM ============================================================================
REM rebuild-indexes.bat
REM Parses FlexSim's XML help files into grep-friendly flat text indexes.
REM
REM Usage:  scripts\rebuild-indexes.bat
REM
REM Requires: FlexSim offline help installed (Help > Download Offline Help)
REM Output:   docs/reference-cache/command-index.txt
REM           docs/reference-cache/api-index.txt
REM ============================================================================

REM -- Resolve module root (one level up from scripts/)
set "MODULE_ROOT=%~dp0.."
pushd "%MODULE_ROOT%"
set "MODULE_ROOT=%CD%"
popd

REM -- Resolve FlexSim root
set "FLEXSIM_ROOT=%MODULE_ROOT%\..\.."
pushd "%FLEXSIM_ROOT%"
set "FLEXSIM_ROOT=%CD%"
popd

echo.
echo ============================================================================
echo  FlexSim Command ^& API Index Builder
echo  Module Root:  %MODULE_ROOT%
echo  FlexSim Root: %FLEXSIM_ROOT%
echo ============================================================================
echo.

REM -- Check that help files exist
set "COMMANDS_XML=%FLEXSIM_ROOT%\help\CommandReference\Commands.xml"
set "API_ROOT=%FLEXSIM_ROOT%\help\FlexScriptAPIReference"

if not exist "%COMMANDS_XML%" (
    echo ERROR: FlexSim offline help not found.
    echo.
    echo   Expected: %COMMANDS_XML%
    echo.
    echo   To fix: Open FlexSim ^> Help ^> Download Offline Help
    echo   Then re-run this script.
    echo.
    pause
    exit /b 1
)

if not exist "%API_ROOT%" (
    echo ERROR: FlexScript API reference not found.
    echo.
    echo   Expected: %API_ROOT%
    echo.
    echo   To fix: Open FlexSim ^> Help ^> Download Offline Help
    echo   Then re-run this script.
    echo.
    pause
    exit /b 1
)

REM -- Create output directory
set "OUTPUT_DIR=%MODULE_ROOT%\docs\reference-cache"
if not exist "%OUTPUT_DIR%" mkdir "%OUTPUT_DIR%"

REM ==========================================================================
REM Step 1: Parse Commands.xml into command-index.txt
REM ==========================================================================
echo [Step 1] Parsing Commands.xml...
echo.

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0parse-commands.ps1" "%COMMANDS_XML%" "%OUTPUT_DIR%\command-index.txt"

if errorlevel 1 (
    echo.
    echo ERROR: Failed to parse Commands.xml. Check PowerShell output above.
    pause
    exit /b 1
)

echo.

REM ==========================================================================
REM Step 2: Parse FlexScriptAPIReference into api-index.txt
REM ==========================================================================
echo [Step 2] Parsing FlexScript API Reference...
echo.

powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0parse-api.ps1" "%API_ROOT%" "%OUTPUT_DIR%\api-index.txt"

if errorlevel 1 (
    echo.
    echo ERROR: Failed to parse API reference. Check PowerShell output above.
    pause
    exit /b 1
)

echo.
echo ============================================================================
echo  Index rebuild complete.
echo  Output: docs\reference-cache\command-index.txt
echo          docs\reference-cache\api-index.txt
echo ============================================================================
echo.

endlocal
pause
exit /b 0
