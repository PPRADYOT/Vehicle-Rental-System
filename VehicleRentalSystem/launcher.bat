@echo off
REM Vehicle Rental System - Quick Launcher Batch Script
REM This batch file provides easy double-click execution

setlocal enabledelayedexpansion

REM Get the directory where this batch file is located
set SCRIPT_DIR=%~dp0

REM Display header
cls
echo.
echo ╔═══════════════════════════════════════════════════════╗
echo ║   VEHICLE RENTAL SYSTEM - LAUNCHER                   ║
echo ║   C++ Backend + Flask Web Dashboard                  ║
echo ╚═══════════════════════════════════════════════════════╝
echo.

REM Check if PowerShell is available
where powershell >nul 2>nul
if errorlevel 1 (
    echo ERROR: PowerShell not found. This launcher requires PowerShell.
    echo.
    pause
    exit /b 1
)

REM Check if launcher.ps1 exists
if not exist "%SCRIPT_DIR%launcher.ps1" (
    echo ERROR: launcher.ps1 not found in %SCRIPT_DIR%
    echo.
    pause
    exit /b 1
)

echo ℹ Starting launcher script...
echo.

REM Run PowerShell with the launcher script
REM AllowSigned is not enforced; execution policy is temporarily bypassed for this command
powershell.exe -ExecutionPolicy Bypass -File "%SCRIPT_DIR%launcher.ps1"

REM Get exit code from PowerShell
set EXIT_CODE=%errorlevel%

if %EXIT_CODE% equ 0 (
    echo.
    echo ✓ Launcher completed successfully
) else (
    echo.
    echo ✗ Launcher encountered an error (Exit code: %EXIT_CODE%)
)

echo.
pause
exit /b %EXIT_CODE%
