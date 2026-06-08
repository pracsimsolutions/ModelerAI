@echo off
setlocal enabledelayedexpansion

REM ============================================================================
REM build-module.bat
REM Cleans build artifacts and compiles the FlexSim module DLL via MSBuild.
REM
REM Usage:  Run from a Visual Studio Developer Command Prompt.
REM         scripts\build-module.bat
REM
REM The script auto-detects the *DLL/ folder and *.sln file.
REM ============================================================================

REM -- Resolve module root (one level up from scripts/)
set "MODULE_ROOT=%~dp0.."
pushd "%MODULE_ROOT%"
set "MODULE_ROOT=%CD%"
popd

echo.
echo ============================================================================
echo  FlexSim Module Build
echo  Root: %MODULE_ROOT%
echo ============================================================================
echo.

REM -- Auto-detect the DLL folder (*DLL/ pattern)
set "DLL_DIR="
for /d %%D in ("%MODULE_ROOT%\*DLL") do (
    set "DLL_DIR=%%D"
)
if not defined DLL_DIR (
    echo ERROR: Could not find a *DLL/ folder in %MODULE_ROOT%.
    echo        Expected a folder matching the pattern *DLL ^(e.g., ModelerAIDLL^).
    pause
    exit /b 1
)
for %%D in ("%DLL_DIR%") do set "DLL_DIR_NAME=%%~nxD"
echo  DLL folder: %DLL_DIR_NAME%

REM -- Auto-detect the .sln file inside the DLL folder
set "SLN_FILE="
for %%F in ("%DLL_DIR%\*.sln") do (
    set "SLN_FILE=%%F"
)
if not defined SLN_FILE (
    echo ERROR: Could not find a .sln file inside %DLL_DIR_NAME%\.
    pause
    exit /b 1
)
for %%F in ("%SLN_FILE%") do set "SLN_NAME=%%~nxF"
echo  Solution:   %SLN_NAME%

REM -- Derive the module name from the DLL folder (strip trailing "DLL")
set "MODULE_NAME=%DLL_DIR_NAME:~0,-3%"
echo  Module:     %MODULE_NAME%
echo.

REM ==========================================================================
REM Step 1: Clean build artifacts
REM ==========================================================================
echo [Step 1] Cleaning build artifacts...
echo.

REM -- Remove x64/ folder (Debug/Release output)
if exist "%DLL_DIR%\x64" (
    echo   Removing: %DLL_DIR_NAME%\x64\
    rmdir /s /q "%DLL_DIR%\x64"
)

REM -- Remove .vs/ folder (VS temp files)
if exist "%DLL_DIR%\.vs" (
    echo   Removing: %DLL_DIR_NAME%\.vs\
    rmdir /s /q "%DLL_DIR%\.vs"
)

REM -- Delete .pdb, .obj, .log, .idb, .ilk files in DLL dir
for %%E in (pdb obj log idb ilk) do (
    for %%F in ("%DLL_DIR%\*.%%E") do (
        echo   Deleting: %DLL_DIR_NAME%\%%~nxF
        del /q "%%F"
    )
)

REM -- Delete old .dll from module root
if exist "%MODULE_ROOT%\%MODULE_NAME%.dll" (
    echo   Deleting: %MODULE_NAME%.dll ^(module root^)
    del /q "%MODULE_ROOT%\%MODULE_NAME%.dll"
)

echo.
echo   Clean complete.
echo.

REM ==========================================================================
REM Step 2: Build via MSBuild
REM ==========================================================================
echo [Step 2] Building %SLN_NAME%...
echo.

REM -- Find MSBuild: check PATH first, then search standard VS install locations
set "MSBUILD_EXE="
where msbuild >nul 2>&1
if not errorlevel 1 (
    set "MSBUILD_EXE=msbuild"
)

if not defined MSBUILD_EXE (
    echo   MSBuild not on PATH. Searching Visual Studio installations...
    echo.
    REM Try vswhere (ships with VS 2017+ installer)
    set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
    if exist "!VSWHERE!" (
        for /f "usebackq delims=" %%I in (`"!VSWHERE!" -latest -requires Microsoft.Component.MSBuild -find MSBuild\**\Bin\MSBuild.exe`) do (
            set "MSBUILD_EXE=%%I"
        )
    )
)

if not defined MSBUILD_EXE (
    REM Fallback: search common install paths
    for %%E in (Enterprise Professional Community BuildTools) do (
        for %%V in (2022 2019) do (
            set "TRY=%ProgramFiles%\Microsoft Visual Studio\%%V\%%E\MSBuild\Current\Bin\MSBuild.exe"
            if exist "!TRY!" (
                set "MSBUILD_EXE=!TRY!"
            )
        )
    )
)

if not defined MSBUILD_EXE (
    echo ERROR: Could not find MSBuild anywhere.
    echo.
    echo   Checked:
    echo     - PATH
    echo     - vswhere.exe ^(Visual Studio Installer^)
    echo     - Standard VS 2022/2019 install paths
    echo.
    echo   Make sure Visual Studio 2022 is installed with the C++ workload.
    pause
    exit /b 1
)

echo   Using: !MSBUILD_EXE!
echo.

"!MSBUILD_EXE!" "%SLN_FILE%" /p:Configuration=Release /p:Platform=x64 /v:minimal
if errorlevel 1 (
    echo.
    echo ERROR: Build failed. Review the output above for details.
    pause
    exit /b 1
)

echo.
echo   Build succeeded.
echo.

REM ==========================================================================
REM Step 3: Verify DLL in module root
REM ==========================================================================
echo [Step 3] Verifying DLL...
echo.

REM MSBuild outputs directly to the module root (vcxproj OutDir = $(SolutionDir)..\)
if exist "%MODULE_ROOT%\%MODULE_NAME%.dll" (
    echo   Found: %MODULE_NAME%.dll in module root
) else (
    echo ERROR: %MODULE_NAME%.dll not found in module root after build.
    echo        Check the vcxproj OutDir setting.
    pause
    exit /b 1
)
echo.

REM ==========================================================================
REM Done
REM ==========================================================================
echo ============================================================================
echo  Build complete: %MODULE_NAME%.dll
echo ============================================================================
echo.

endlocal
pause
exit /b 0
