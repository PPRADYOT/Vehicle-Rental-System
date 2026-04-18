# Vehicle Rental System - Automated Launcher Guide

## Quick Start (3 Methods)

You have **3 ways** to launch the entire system automatically with logging:

---

## Method 1: **Easiest** - Double-Click launcher.exe

### Steps:
1. Open File Explorer
2. Navigate to: `C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem\`
3. **Double-click** `launcher.exe`

### What Happens:
- ✅ Automatically compiles C++ backend
- ✅ Shows real-time compilation logs
- ✅ Creates launcher window with status
- ✅ Saves all logs to `logs/` directory
- ✅ Displays next steps with terminal commands

### Output:
```
╔═══════════════════════════════════════════════════════╗
║   VEHICLE RENTAL SYSTEM - LAUNCHER (C++)             ║
║   Automatic Compilation & Deployment                 ║
╚═══════════════════════════════════════════════════════╝

✓ Project structure validated
✓ C++ backend compiled successfully
✓ Environment preparation complete
  
📋 Next Steps:
  1. Run C++ Backend:
     cd C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem\cpp
     .\rental_system.exe
  
  2. Run Flask Dashboard:
     cd C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem\web
     set VEHICLE_RENTAL_CSV_PATH=C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem\cpp\data
     python app.py
     
  3. Open Browser:
     http://localhost:5000
```

---

## Method 2: **Automated** - Double-Click launcher.bat

### Steps:
1. Open File Explorer
2. Navigate to: `C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem\`
3. **Double-click** `launcher.bat`

### What Happens:
- ✅ Wrapper batch file calls PowerShell launcher
- ✅ Shows compilation status
- ✅ Logs operations to file
- ✅ Displays system readiness

### Output:
```
ℹ Starting launcher script...

[14:30:45] [INFO] === Launcher Started ===
[14:30:45] [INFO] Project Root: C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem
[14:30:46] [INFO] Starting: C++ Compilation
[14:30:50] [SUCCESS] Completed: C++ Compilation
[14:30:50] [SUCCESS] ✓ Project structure verified
...

📌 Next Steps:
  1. Check the Flask window for server status
  2. Check the C++ window for console menu
  3. Open browser to: http://localhost:5000
  4. Logs are saved to: logs\launcher_20260415_143050.log
```

---

## Method 3: **Full Control** - PowerShell Script launcher.ps1

### Steps:

#### Option A: Direct Execution
```powershell
# Open PowerShell as Administrator
# Navigate to project folder
cd C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem

# Run launcher script
.\launcher.ps1
```

#### Option B: With Specific Flags
```powershell
# Compile C++ only (skip Flask)
.\launcher.ps1 -NoCPP

# Start Flask only (skip C++ compilation)
.\launcher.ps1 -NOCPP

# Full execution (default)
.\launcher.ps1
```

### Features:
- ✅ Colored console output (green/red/yellow/cyan)
- ✅ Full compilation output logging
- ✅ Environment verification
- ✅ Automatic Flask installation (if missing)
- ✅ Configurable via flags
- ✅ Comprehensive log files

### Output:
```
[2026-04-15 14:30:45] [INFO] === Launcher Started ===
[2026-04-15 14:30:45] [INFO] Project Root: C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem
[2026-04-15 14:30:45] [INFO] Log File: C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem\logs\launcher_20260415_143050.log

[STEP 1] Verifying Project Structure...
✓ Project structure validated

[STEP 2] Compiling C++ Backend...
  Compiling C++ files...
✓ C++ backend compiled successfully
  Executable size: 271KB

[STEP 3] Checking Flask Environment...
✓ Python found
✓ Flask is ready

[STEP 4] Starting C++ Console Application...
✓ Starting rental_system.exe...
  → Window will appear shortly

[STEP 5] Starting Flask Web Dashboard...
✓ Flask server started

════════════════════════════════════════════════════════
         VEHICLE RENTAL SYSTEM - RUNNING
════════════════════════════════════════════════════════

📋 System Components:
  ✓ C++ Console Backend (rental_system.exe)
    Location: C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem\cpp
  ✓ Flask Web Dashboard
    URL: http://localhost:5000
    Location: C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem\web

📊 Data Layer:
  ✓ Shared CSV Files
    Location: C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem\cpp\data
    - vehicles.csv (5 lines)
    - customers.csv (6 lines)
    - rentals.csv (5 lines)

📝 Logging:
  ✓ All operations logged to:
    C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem\logs\launcher_20260415_143050.log
```

---

## Log File Locations

All launcher activities are logged to:
```
C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem\logs\launcher_YYYYMMDD_HHMMSS.log
```

### Examples:
- `launcher_20260415_143050.log` — April 15, 2:30:50 PM
- `launcher_20260415_150230.log` — April 15, 3:02:30 PM

### View Recent Logs:
```powershell
# List all logs (most recent first)
Get-ChildItem C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem\logs -Name | Sort-Object -Descending

# View a specific log
Get-Content C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem\logs\launcher_20260415_143050.log

# Follow log updates in real-time
Get-Content -Path C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem\logs\launcher_20260415_143050.log -Wait
```

---

## Troubleshooting

### Issue: "g++ not found"
**Solution:** Install MinGW-w64
```
Download: https://www.mingw-w64.org/
```

### Issue: "Python not found"
**Solution:** Install Python 3.8+
```
Download: https://www.python.org/downloads/
Make sure to check "Add Python to PATH" during installation
```

### Issue: Compilation fails with errors
**Solution:** Check the launcher log file
```powershell
# Find the most recent log
$latestLog = Get-ChildItem C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem\logs -Name | Sort-Object -Descending | Select-Object -First 1
Get-Content "C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem\logs\$latestLog"
```

### Issue: Flask port 5000 already in use
**Solution #1:** Stop the Flask process
```powershell
Get-Process python | Stop-Process -Force
```

**Solution #2:** Use a different port
```powershell
cd C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem\web
python -c "import app; app.app.run(port=5001)"
```

### Issue: CSV path not found by Flask
**Solution:** Verify environment variable is set
```powershell
# In PowerShell
$env:VEHICLE_RENTAL_CSV_PATH
# Should output: C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem\cpp\data
```

---

## File Sizes Reference

| File | Size | Purpose |
|------|------|---------|
| `launcher.exe` | ~125 KB | Compiled C++ launcher |
| `launcher.ps1` | ~12 KB | PowerShell launcher script |
| `launcher.bat` | ~1 KB | Batch wrapper script |
| `launcher.cpp` | ~8 KB | Launcher source code |
| `cpp/rental_system.exe` | ~271 KB | Compiled C++ backend |

---

## Recommended Usage

### For Daily Development:
```powershell
# Double-click launcher.exe
# It compiles everything and shows status
```

### For CI/CD or Automation:
```powershell
# Use launcher.ps1 with flags
.\launcher.ps1 -NoFlask  # Compile only
.\launcher.ps1 -NoCPP     # Run Flask only
```

### For Debugging:
```powershell
# Run launcher.ps1 and watch logs
.\launcher.ps1
# Review logs directory for detailed output
```

---

## What Gets Logged?

The launcher logs capture:
- ✅ Timestamp of each operation
- ✅ Compilation status and diagnostics
- ✅ Environment variable settings
- ✅ Python and Flask verification
- ✅ System paths and file locations
- ✅ Success/Error status for each step
- ✅ Total execution time

This makes troubleshooting fast and debugging simple!
