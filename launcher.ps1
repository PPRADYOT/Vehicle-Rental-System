# Vehicle Rental System - Automated Launcher with Logging
# This script compiles the C++ backend, starts Flask, and logs all operations

param(
    [switch]$NoFlask = $false,  # Skip Flask startup (only compile C++)
    [switch]$NoCPP = $false     # Skip C++ compilation (only run Flask)
)

# Configuration
$ProjectRoot = Split-Path -Parent $MyInvocation.MyCommandPath
$CppDir = Join-Path $ProjectRoot "cpp"
$WebDir = Join-Path $ProjectRoot "web"
$DataDir = Join-Path $CppDir "data"
$LogDir = Join-Path $ProjectRoot "logs"
$Timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
$LogFile = Join-Path $LogDir "launcher_${Timestamp}.log"

# Ensure log directory exists
if (-not (Test-Path $LogDir)) {
    New-Item -ItemType Directory -Path $LogDir -Force | Out-Null
}

# Function to log messages
function Write-Log {
    param(
        [string]$Message,
        [ValidateSet("INFO", "SUCCESS", "ERROR", "WARNING")]
        [string]$Level = "INFO"
    )
    
    $Timestamp = Get-Date -Format "yyyy-MM-dd HH:mm:ss"
    $LogMessage = "[$Timestamp] [$Level] $Message"
    
    # Write to console with colors
    switch ($Level) {
        "SUCCESS" { Write-Host $LogMessage -ForegroundColor Green }
        "ERROR" { Write-Host $LogMessage -ForegroundColor Red }
        "WARNING" { Write-Host $LogMessage -ForegroundColor Yellow }
        default { Write-Host $LogMessage -ForegroundColor Cyan }
    }
    
    # Write to log file
    Add-Content -Path $LogFile -Value $LogMessage
}

# Function to run command and log output
function Invoke-LoggedCommand {
    param(
        [string]$Description,
        [string]$CommandPath,
        [string[]]$Arguments,
        [string]$WorkingDirectory
    )
    
    Write-Log "Starting: $Description" "INFO"
    Add-Content -Path $LogFile -Value "========================================"
    
    try {
        $output = & $CommandPath @Arguments -ErrorAction Stop 2>&1
        
        if ($output) {
            Add-Content -Path $LogFile -Value $output
        }
        
        Write-Log "Completed: $Description" "SUCCESS"
        return $true
    }
    catch {
        Write-Log "Failed: $Description - $_" "ERROR"
        Add-Content -Path $LogFile -Value "ERROR: $_"
        return $false
    }
}

# Display header
Clear-Host
Write-Host "╔═══════════════════════════════════════════════════════╗" -ForegroundColor Cyan
Write-Host "║   VEHICLE RENTAL SYSTEM - AUTOMATED LAUNCHER         ║" -ForegroundColor Cyan
Write-Host "║   Built with C++ Backend & Flask Web Dashboard       ║" -ForegroundColor Cyan
Write-Host "╚═══════════════════════════════════════════════════════╝" -ForegroundColor Cyan
Write-Log "=== Launcher Started ===" "INFO"
Write-Log "Project Root: $ProjectRoot" "INFO"
Write-Log "Log File: $LogFile" "INFO"
Write-Log "Timestamp: $Timestamp" "INFO"

# ============================================================================
# STEP 1: Verify Project Structure
# ============================================================================
Write-Host "`n[STEP 1] Verifying Project Structure..." -ForegroundColor Cyan
Write-Log "Verifying project structure..." "INFO"

$errors = @()

if (-not (Test-Path $CppDir)) { $errors += "C++ directory not found: $CppDir" }
if (-not (Test-Path $WebDir)) { $errors += "Web directory not found: $WebDir" }
if (-not (Test-Path $DataDir)) { $errors += "Data directory not found: $DataDir" }

if ($errors.Count -gt 0) {
    foreach ($err in $errors) {
        Write-Log $err "ERROR"
    }
    Write-Log "=== Launcher Failed ===" "ERROR"
    exit 1
}

Write-Log "✓ Project structure verified" "SUCCESS"
Write-Host "✓ Project structure validated" -ForegroundColor Green

# ============================================================================
# STEP 2: Compile C++ Backend
# ============================================================================
if (-not $NoCPP) {
    Write-Host "`n[STEP 2] Compiling C++ Backend..." -ForegroundColor Cyan
    Write-Log "Attempting to compile C++ backend..." "INFO"
    
    Push-Location $CppDir
    
    # Check for g++
    $gppPath = (Get-Command g++ -ErrorAction SilentlyContinue).Source
    if (-not $gppPath) {
        Write-Log "g++ compiler not found in PATH" "ERROR"
        Write-Host "✗ g++ not found. Install MinGW-w64 or a C++ compiler." -ForegroundColor Red
        Write-Host "  Download: https://www.mingw-w64.org/" -ForegroundColor Yellow
        Pop-Location
        exit 1
    }
    
    Write-Log "g++ found at: $gppPath" "INFO"
    
    # List all C++ files to compile
    $cppFiles = @(
        "main.cpp", "vehicle.cpp", "car.cpp", "bike.cpp", "truck.cpp",
        "customer.cpp", "rental.cpp", "rentalmanager.cpp", "filemanager.cpp"
    )
    
    $allExist = $true
    foreach ($file in $cppFiles) {
        if (-not (Test-Path $file)) {
            Write-Log "Missing C++ source file: $file" "ERROR"
            $allExist = $false
        }
    }
    
    if (-not $allExist) {
        Write-Log "Some C++ source files are missing" "ERROR"
        Pop-Location
        exit 1
    }
    
    Write-Log "All source files found" "SUCCESS"
    Write-Host "  Compiling C++ files..." -ForegroundColor Gray
    
    # Compile
    $compileOutput = & g++ -std=c++17 -o rental_system.exe $cppFiles 2>&1
    
    if ($LASTEXITCODE -eq 0) {
        Write-Log "C++ compilation successful" "SUCCESS"
        Write-Host "✓ C++ backend compiled successfully" -ForegroundColor Green
        
        # Verify executable
        if (Test-Path "rental_system.exe") {
            $exeSize = (Get-Item "rental_system.exe").Length / 1KB
            Write-Log "Executable created: rental_system.exe (${exeSize}KB)" "SUCCESS"
            Write-Host "  Executable size: $([Math]::Round($exeSize))KB" -ForegroundColor Gray
        }
    }
    else {
        Write-Log "C++ compilation failed:" "ERROR"
        Add-Content -Path $LogFile -Value $compileOutput
        Write-Host "✗ Compilation failed. See log for details." -ForegroundColor Red
        Pop-Location
        exit 1
    }
    
    Pop-Location
}
else {
    Write-Host "⊘ C++ compilation skipped (-NoCPP flag)" -ForegroundColor Yellow
    Write-Log "C++ compilation skipped" "WARNING"
}

# ============================================================================
# STEP 3: Verify Flask Setup
# ============================================================================
Write-Host "`n[STEP 3] Checking Flask Environment..." -ForegroundColor Cyan
Write-Log "Checking Flask and Python..." "INFO"

$pythonPath = (Get-Command python -ErrorAction SilentlyContinue).Source
if (-not $pythonPath) {
    $pythonPath = (Get-Command python3 -ErrorAction SilentlyContinue).Source
}

if (-not $pythonPath) {
    Write-Log "Python not found in PATH" "ERROR"
    Write-Host "✗ Python not found. Install Python 3.8+." -ForegroundColor Red
    Write-Host "  Download: https://www.python.org/downloads/" -ForegroundColor Yellow
    exit 1
}

Write-Log "Python found at: $pythonPath" "INFO"
Write-Host "✓ Python found" -ForegroundColor Green

# Check Flask
$flaskCheck = & python -c "import flask; print('OK')" 2>&1
if ($LASTEXITCODE -ne 0) {
    Write-Log "Flask is not installed" "WARNING"
    Write-Host "⚠ Flask not installed. Installing Flask..." -ForegroundColor Yellow
    & python -m pip install flask -q 2>&1 | Out-Null
    
    $flaskCheck = & python -c "import flask; print('OK')" 2>&1
    if ($LASTEXITCODE -eq 0) {
        Write-Log "Flask installed successfully" "SUCCESS"
        Write-Host "✓ Flask installed" -ForegroundColor Green
    }
    else {
        Write-Log "Failed to install Flask" "ERROR"
        Write-Host "✗ Could not install Flask" -ForegroundColor Red
        exit 1
    }
}
else {
    Write-Host "✓ Flask is ready" -ForegroundColor Green
    Write-Log "Flask is installed" "SUCCESS"
}

# ============================================================================
# STEP 4: Start C++ Console Application (in background)
# ============================================================================
if (-not $NoCPP) {
    Write-Host "`n[STEP 4] Starting C++ Console Application..." -ForegroundColor Cyan
    Write-Log "Launching C++ rental system console..." "INFO"
    
    $exePath = Join-Path $CppDir "rental_system.exe"
    if (Test-Path $exePath) {
        Write-Host "✓ Starting rental_system.exe..." -ForegroundColor Green
        Write-Log "Launching: $exePath" "INFO"
        
        # Start C++ app in a new window
        Start-Process -FilePath $exePath -WorkingDirectory $CppDir
        
        Write-Log "C++ application launched in separate window" "SUCCESS"
        Write-Host "  → Window will appear shortly" -ForegroundColor Gray
        Start-Sleep -Seconds 1
    }
    else {
        Write-Log "Executable not found: $exePath" "ERROR"
        Write-Host "✗ Executable not found" -ForegroundColor Red
    }
}

# ============================================================================
# STEP 5: Start Flask Web Dashboard
# ============================================================================
if (-not $NoFlask) {
    Write-Host "`n[STEP 5] Starting Flask Web Dashboard..." -ForegroundColor Cyan
    Write-Log "Launching Flask web server..." "INFO"
    
    $flaskStart = {
        Set-Location $using:WebDir
        $env:VEHICLE_RENTAL_CSV_PATH = $using:DataDir
        Write-Host "[Flask] Starting server from: $using:WebDir" -ForegroundColor Cyan
        Write-Host "[Flask] CSV path: $using:DataDir" -ForegroundColor Cyan
        Write-Host "[Flask] Press Ctrl+C to stop the server" -ForegroundColor Yellow
        & python app.py 2>&1
    }
    
    # Start Flask in a new PowerShell window
    $flaskPath = Join-Path $WebDir "app.py"
    if (Test-Path $flaskPath) {
        Write-Log "Launching Flask from: $WebDir" "INFO"
        Write-Log "Setting CSV path: $DataDir" "INFO"
        
        Start-Process -FilePath "powershell.exe" `
                     -ArgumentList "-NoExit", "-Command", `
                     "Set-Location '$WebDir'; `$env:VEHICLE_RENTAL_CSV_PATH='$DataDir'; python app.py" `
                     -WorkingDirectory $WebDir
        
        Write-Log "Flask server launched in separate window" "SUCCESS"
        Write-Host "✓ Flask server started" -ForegroundColor Green
        Start-Sleep -Seconds 2
    }
    else {
        Write-Log "Flask app.py not found: $flaskPath" "ERROR"
        Write-Host "✗ Flask app.py not found" -ForegroundColor Red
        exit 1
    }
}
else {
    Write-Host "⊘ Flask startup skipped (-NoFlask flag)" -ForegroundColor Yellow
    Write-Log "Flask startup skipped" "WARNING"
}

# ============================================================================
# STEP 6: Display Summary
# ============================================================================
Write-Host "`n" -ForegroundColor Cyan
Write-Host "╔═══════════════════════════════════════════════════════╗" -ForegroundColor Green
Write-Host "║         VEHICLE RENTAL SYSTEM - RUNNING              ║" -ForegroundColor Green
Write-Host "╚═══════════════════════════════════════════════════════╝" -ForegroundColor Green

Write-Host "`n📋 System Components:" -ForegroundColor Cyan
if (-not $NoCPP) {
    Write-Host "  ✓ C++ Console Backend (rental_system.exe)" -ForegroundColor Green
    Write-Host "    Location: $CppDir" -ForegroundColor Gray
}
if (-not $NoFlask) {
    Write-Host "  ✓ Flask Web Dashboard" -ForegroundColor Green
    Write-Host "    URL: http://localhost:5000" -ForegroundColor Gray
    Write-Host "    Location: $WebDir" -ForegroundColor Gray
}

Write-Host "`n📊 Data Layer:" -ForegroundColor Cyan
Write-Host "  ✓ Shared CSV Files" -ForegroundColor Green
Write-Host "    Location: $DataDir" -ForegroundColor Gray
$csvFiles = Get-ChildItem $DataDir -Filter "*.csv" -ErrorAction SilentlyContinue
foreach ($csv in $csvFiles) {
    $lineCount = (Get-Content $csv.FullName | Measure-Object -Line).Lines
    Write-Host "    - $($csv.Name) ($lineCount lines)" -ForegroundColor Gray
}

Write-Host "`n📝 Logging:" -ForegroundColor Cyan
Write-Host "  ✓ All operations logged to:" -ForegroundColor Green
Write-Host "    $LogFile" -ForegroundColor Gray

Write-Host "`n⏱️  Timeline:" -ForegroundColor Cyan
Write-Host "  Start time: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')" -ForegroundColor Gray
Write-Host "  Execution: $([Math]::Round((Get-Date - [datetime]::ParseExact($Timestamp, 'yyyyMMdd_HHmmss', $null)).TotalSeconds, 2))s" -ForegroundColor Gray

Write-Host "`n📌 Next Steps:" -ForegroundColor Cyan
Write-Host "  1. Check the Flask window for server status" -ForegroundColor Yellow
Write-Host "  2. Check the C++ window for console menu" -ForegroundColor Yellow
Write-Host "  3. Open browser to: http://localhost:5000" -ForegroundColor Yellow
Write-Host "  4. Logs are saved to: $LogFile" -ForegroundColor Yellow

Write-Log "=== Launcher Completed Successfully ===" "SUCCESS"

Write-Host "`n 💡 Tip: Keep this window open to see logs. Close it to stop monitoring." -ForegroundColor Cyan
Write-Host "`n Press Enter to view log file or close this window..." -ForegroundColor Gray
Read-Host

# Optional: Show log file
if ($?) {
    if (Test-Path $LogFile) {
        Invoke-Item $LogFile
    }
}
