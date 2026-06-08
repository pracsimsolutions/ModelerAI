@echo off
setlocal enabledelayedexpansion

REM ============================================================================
REM rename-module.bat
REM Renames a FlexSim module across all files and folders.
REM
REM The current module name is auto-detected from the .fsx file.
REM
REM Usage:  rename-module.bat [NewName]
REM         rename-module.bat              (prompts interactively)
REM Example: rename-module.bat MyCustomModule
REM
REM This script is re-runnable: works for initial rename from template AND
REM for later renames as the design evolves.
REM ============================================================================

REM -- Resolve module root (one level up from scripts/)
set "MODULE_ROOT=%~dp0.."
pushd "%MODULE_ROOT%"
set "MODULE_ROOT=%CD%"
popd

REM -- Auto-detect current module name from .fsx file
set "OLD_NAME="
for %%F in ("%MODULE_ROOT%\*.fsx") do set "OLD_NAME=%%~nF"

if not defined OLD_NAME (
    echo ERROR: No .fsx file found in %MODULE_ROOT%. Cannot detect current module name.
    pause
    exit /b 1
)

REM -- Get new name from argument or prompt interactively
REM    Old name is ALWAYS auto-detected from .fsx — never manually specified.
REM    Usage: rename-module.bat [NewName]
if not "%~1"=="" (
    set "NEW_NAME=%~1"
) else (
    echo.
    echo ============================================================================
    echo  FlexSim Module Rename
    echo  Current module name: %OLD_NAME%
    echo ============================================================================
    echo.
    set /p "NEW_NAME=  Enter new module name: "
    if "!NEW_NAME!"=="" (
        echo ERROR: No name entered.
        pause
        exit /b 1
    )
)

if "%OLD_NAME%"=="%NEW_NAME%" (
    echo ERROR: Old name and new name are the same.
    pause
    exit /b 1
)

echo.
echo ============================================================================
echo  FlexSim Module Rename
echo  From: %OLD_NAME%
echo  To:   %NEW_NAME%
echo  Root: %MODULE_ROOT%
echo ============================================================================
echo.
echo  This will:
echo    - Replace "%OLD_NAME%" with "%NEW_NAME%" in all .h, .cpp, .fsx, .sln, .vcxproj files
echo    - Rename %OLD_NAME%.fsx, %OLD_NAME%.t, %OLD_NAME%.dll
echo    - Rename %OLD_NAME%DLL\ folder to %NEW_NAME%DLL\
echo.
set /p "CONFIRM=  Proceed? (Y/N): "
if /i not "%CONFIRM%"=="Y" (
    echo.
    echo  Cancelled.
    pause
    exit /b 0
)
echo.

REM ==========================================================================
REM Step 1: Replace in file contents
REM ==========================================================================
echo [Step 1] Replacing "%OLD_NAME%" with "%NEW_NAME%" in file contents...
echo.

REM -- Replace in .h and .cpp files in the DLL folder
if exist "%MODULE_ROOT%\%OLD_NAME%DLL" (
    for %%F in ("%MODULE_ROOT%\%OLD_NAME%DLL\*.h" "%MODULE_ROOT%\%OLD_NAME%DLL\*.cpp") do (
        if exist "%%F" (
            echo   Updating: %%~nxF
            powershell -NoProfile -Command "(Get-Content -Path '%%F' -Raw) -replace '%OLD_NAME%', '%NEW_NAME%' | Set-Content -Path '%%F' -NoNewline"
        )
    )
) else if exist "%MODULE_ROOT%\%NEW_NAME%DLL" (
    REM DLL folder may already be renamed in a partial re-run
    for %%F in ("%MODULE_ROOT%\%NEW_NAME%DLL\*.h" "%MODULE_ROOT%\%NEW_NAME%DLL\*.cpp") do (
        if exist "%%F" (
            echo   Updating: %%~nxF
            powershell -NoProfile -Command "(Get-Content -Path '%%F' -Raw) -replace '%OLD_NAME%', '%NEW_NAME%' | Set-Content -Path '%%F' -NoNewline"
        )
    )
) else (
    echo   WARNING: Neither %OLD_NAME%DLL nor %NEW_NAME%DLL folder found.
)

REM -- Determine which DLL folder currently exists for project files
set "DLL_DIR="
if exist "%MODULE_ROOT%\%OLD_NAME%DLL" set "DLL_DIR=%MODULE_ROOT%\%OLD_NAME%DLL"
if exist "%MODULE_ROOT%\%NEW_NAME%DLL" set "DLL_DIR=%MODULE_ROOT%\%NEW_NAME%DLL"

REM -- Replace in .fsx file
if exist "%MODULE_ROOT%\%OLD_NAME%.fsx" (
    echo   Updating: %OLD_NAME%.fsx
    powershell -NoProfile -Command "(Get-Content -Path '%MODULE_ROOT%\%OLD_NAME%.fsx' -Raw) -replace '%OLD_NAME%', '%NEW_NAME%' | Set-Content -Path '%MODULE_ROOT%\%OLD_NAME%.fsx' -NoNewline"
) else if exist "%MODULE_ROOT%\%NEW_NAME%.fsx" (
    echo   Updating: %NEW_NAME%.fsx
    powershell -NoProfile -Command "(Get-Content -Path '%MODULE_ROOT%\%NEW_NAME%.fsx' -Raw) -replace '%OLD_NAME%', '%NEW_NAME%' | Set-Content -Path '%MODULE_ROOT%\%NEW_NAME%.fsx' -NoNewline"
)

REM -- Replace in .sln file
set "SLN_FILE="
if exist "%DLL_DIR%\%OLD_NAME%.sln" (
    set "SLN_FILE=%DLL_DIR%\%OLD_NAME%.sln"
) else if exist "%DLL_DIR%\%NEW_NAME%.sln" (
    set "SLN_FILE=%DLL_DIR%\%NEW_NAME%.sln"
)
if defined SLN_FILE (
    echo   Updating: !SLN_FILE!
    powershell -NoProfile -Command "(Get-Content -Path '!SLN_FILE!' -Raw) -replace '%OLD_NAME%', '%NEW_NAME%' | Set-Content -Path '!SLN_FILE!' -NoNewline"
)

REM -- Replace in .vcxproj and .vcxproj.filters
if defined DLL_DIR (
    for %%F in ("%DLL_DIR%\*.vcxproj" "%DLL_DIR%\*.vcxproj.filters") do (
        if exist "%%F" (
            echo   Updating: %%~nxF
            powershell -NoProfile -Command "(Get-Content -Path '%%F' -Raw) -replace '%OLD_NAME%', '%NEW_NAME%' | Set-Content -Path '%%F' -NoNewline"
        )
    )
)

echo.

REM ==========================================================================
REM Step 2: Rename files
REM ==========================================================================
echo [Step 2] Renaming files...
echo.

REM -- Rename .fsx
if exist "%MODULE_ROOT%\%OLD_NAME%.fsx" (
    echo   Renaming: %OLD_NAME%.fsx  -^>  %NEW_NAME%.fsx
    ren "%MODULE_ROOT%\%OLD_NAME%.fsx" "%NEW_NAME%.fsx"
) else (
    echo   Skipped:  %OLD_NAME%.fsx (not found, may already be renamed)
)

REM -- Rename .t
if exist "%MODULE_ROOT%\%OLD_NAME%.t" (
    echo   Renaming: %OLD_NAME%.t  -^>  %NEW_NAME%.t
    ren "%MODULE_ROOT%\%OLD_NAME%.t" "%NEW_NAME%.t"
) else (
    echo   Skipped:  %OLD_NAME%.t (not found, may already be renamed)
)

REM -- Rename .dll (may not exist if in .gitignore)
if exist "%MODULE_ROOT%\%OLD_NAME%.dll" (
    echo   Renaming: %OLD_NAME%.dll  -^>  %NEW_NAME%.dll
    ren "%MODULE_ROOT%\%OLD_NAME%.dll" "%NEW_NAME%.dll"
) else (
    echo   Skipped:  %OLD_NAME%.dll (not found, may not be built yet)
)

REM -- Rename .sln inside DLL folder
if exist "%MODULE_ROOT%\%OLD_NAME%DLL\%OLD_NAME%.sln" (
    echo   Renaming: %OLD_NAME%DLL\%OLD_NAME%.sln  -^>  %NEW_NAME%.sln
    ren "%MODULE_ROOT%\%OLD_NAME%DLL\%OLD_NAME%.sln" "%NEW_NAME%.sln"
) else (
    echo   Skipped:  %OLD_NAME%DLL\%OLD_NAME%.sln (not found, may already be renamed)
)

echo.

REM ==========================================================================
REM Step 3: Rename DLL folder
REM ==========================================================================
echo [Step 3] Renaming DLL folder...
echo.

if exist "%MODULE_ROOT%\%OLD_NAME%DLL" (
    echo   Renaming: %OLD_NAME%DLL\  -^>  %NEW_NAME%DLL\
    ren "%MODULE_ROOT%\%OLD_NAME%DLL" "%NEW_NAME%DLL"
) else (
    echo   Skipped:  %OLD_NAME%DLL\ (not found, may already be renamed)
)

echo.

REM ==========================================================================
REM Step 4: Summary and manual steps
REM ==========================================================================
echo ============================================================================
echo  Rename complete: %OLD_NAME%  -^>  %NEW_NAME%
echo ============================================================================
echo.
echo  Manual steps remaining:
echo.
echo  1. Open %NEW_NAME%DLL\%NEW_NAME%.sln in Visual Studio and verify it loads.
echo  2. Rebuild the DLL (Build ^> Rebuild Solution).
echo  3. Open the module in FlexSim and verify it loads correctly.
echo  4. If you renamed the module folder itself, update any external references.
echo  5. Review any .pdb or build artifact names if needed.
echo.

endlocal
pause
exit /b 0

:usage
echo.
echo Usage: %~nx0 [NewName]
echo        %~nx0              (prompts interactively)
echo.
echo Renames a FlexSim module across all files, folders, and file contents.
echo The current module name is auto-detected from the .fsx file.
echo.
echo Arguments:
echo   NewName   The desired new module name (e.g., MyCustomModule)
echo.
echo Example:
echo   %~nx0 MyCustomModule
echo.
endlocal
pause
exit /b 1
