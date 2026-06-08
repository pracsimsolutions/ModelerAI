@echo off
REM ============================================================
REM ModelerAI — one-click first-run setup
REM ============================================================
REM Double-click this file after cloning into FlexSim's modules
REM directory. It builds the knowledge-base index the AI uses
REM for lookups. Safe to re-run any time the FlexSim install
REM changes (new version, new modules, etc.).
REM
REM Does NOT pull FlexSim's written user manual by default. The
REM manual is FlexSim's intellectual property — we don't
REM redistribute it. If you want richer KB answers, see the
REM "Optional: include the FlexSim manual" section at the
REM bottom of the README.
REM ============================================================
setlocal enabledelayedexpansion

REM Move to the directory this .bat file lives in so the script
REM works whether you double-click it or run from somewhere else.
cd /d "%~dp0"

echo.
echo ============================================================
echo  ModelerAI setup
echo ============================================================
echo.

REM ---- Sanity check: Node.js installed? ----
where node >nul 2>&1
if errorlevel 1 (
    echo  ERROR: Node.js is not installed or not in your PATH.
    echo.
    echo  Install Node.js from https://nodejs.org/ ^(LTS is fine^),
    echo  then double-click this setup.bat again.
    echo.
    pause
    exit /b 1
)

REM ---- Sanity check: are we inside a FlexSim install? ----
REM build-kb.js expects the FlexSim help dir at <repo>/../../help
if not exist "..\..\help\FlexScriptAPIReference" (
    echo  ERROR: This repo doesn't look like it's inside a FlexSim install.
    echo.
    echo  Expected to find FlexSim help files at:
    echo    %~dp0..\..\help\FlexScriptAPIReference\
    echo.
    echo  Make sure you cloned this repo into the modules folder of your
    echo  FlexSim 2026 install ^(usually C:\Program Files\FlexSim 2026\modules\^),
    echo  not into Documents or anywhere else.
    echo.
    echo  See README.md "Step 3" for the right clone path.
    echo.
    pause
    exit /b 1
)

REM ---- Build the KB index ----
echo  Building the search index from FlexSim's API + command docs...
echo  ^(This is auto-generated from your local FlexSim install — nothing
echo   FlexSim-derived ships in this repo.^)
echo.

node tools\build-kb.js
if errorlevel 1 (
    echo.
    echo  Something went wrong. Scroll up to see the error.
    echo  If you get stuck, email Josh at josh@pracsimsolutions.com.
    echo.
    pause
    exit /b 1
)

echo.
echo ============================================================
echo  Done. Knowledge base ready.
echo ============================================================
echo.
echo  Next steps:
echo    1. Open ModelerAiDLL\ModelerAi.sln in Visual Studio 2022.
echo    2. Press Ctrl+Shift+B to build x64 ^(takes 1-3 minutes^).
echo    3. Open FlexSim 2026, open a model, open the FlexScript
echo       Console ^(View ^> Other ^> FlexScript Console^), type:
echo            addmodelerai
echo       and press Enter.
echo.
echo  Optional: if you want the FlexSim user manual searchable too
echo  ^(richer answers, but locally only — we don't redistribute it^),
echo  run:
echo            node tools\ingest-manual.js
echo            node tools\build-kb.js
echo.
echo  Full walkthrough: README.md
echo  Questions:        josh@pracsimsolutions.com
echo.
pause
