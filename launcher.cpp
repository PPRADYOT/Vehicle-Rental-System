#include <iostream>
#include <fstream>
#include <chrono>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <windows.h>

// ANSI Color codes for cross-platform compatibility
#define RESET "\033[0m"
#define CYAN "\033[36m"
#define GREEN "\033[32m"
#define RED "\033[31m"
#define YELLOW "\033[33m"
#define BRIGHT_CYAN "\033[96m"

// Get current timestamp
std::string getTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// Log message to console and file
void log(const std::string& message, const std::string& level = "INFO", std::ofstream* logFile = nullptr) {
    std::string timestamp = getTimestamp();
    std::string logMessage = "[" + timestamp + "] [" + level + "] " + message;
    
    // Choose color based on level
    std::string color = CYAN;
    if (level == "SUCCESS") color = GREEN;
    else if (level == "ERROR") color = RED;
    else if (level == "WARNING") color = YELLOW;
    
    // Print to console with color
    std::cout << color << logMessage << RESET << std::endl;
    
    // Write to log file if provided
    if (logFile && logFile->is_open()) {
        *logFile << logMessage << std::endl;
    }
}

// Run a system command
bool runCommand(const std::string& command, const std::string& description, std::ofstream* logFile = nullptr) {
    log("Starting: " + description, "INFO", logFile);
    
    int result = system(command.c_str());
    
    if (result == 0) {
        log("Completed: " + description, "SUCCESS", logFile);
        return true;
    } else {
        log("Failed: " + description + " (Exit code: " + std::to_string(result) + ")", "ERROR", logFile);
        return false;
    }
}

int main(int argc, char* argv[]) {
    // Get project root directory (simplified path handling)
    std::string projectRoot = "C:\\Users\\prady\\OneDrive\\Desktop\\oops\\VehicleRentalSystem";
    std::string cppDir = projectRoot + "\\cpp";
    std::string webDir = projectRoot + "\\web";
    std::string dataDir = cppDir + "\\data";
    std::string logsDir = projectRoot + "\\logs";
    
    // Create logs directory if it doesn't exist
    system(("if not exist \"" + logsDir + "\" mkdir \"" + logsDir + "\"").c_str());
    
    // Create log file with timestamp
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y%m%d_%H%M%S");
    std::string timestamp = ss.str();
    
    std::string logFile = logsDir + "\\launcher_" + timestamp + ".log";
    std::ofstream log_stream(logFile, std::ios::app);
    
    // Display header
    std::cout << "\n" << BRIGHT_CYAN << "╔═══════════════════════════════════════════════════════╗" << RESET << std::endl;
    std::cout << BRIGHT_CYAN << "║   VEHICLE RENTAL SYSTEM - LAUNCHER (C++)              ║" << RESET << std::endl;
    std::cout << BRIGHT_CYAN << "║   Automatic Compilation & Deployment                  ║" << RESET << std::endl;
    std::cout << BRIGHT_CYAN << "╚═══════════════════════════════════════════════════════╝" << RESET << std::endl;
    
    log("=== Launcher Started ===", "INFO", &log_stream);
    log("Project Root: " + projectRoot, "INFO", &log_stream);
    log("Log File: " + logFile, "INFO", &log_stream);
    
    // ========================================================================
    // STEP 1: Verify Project Structure
    // ========================================================================
    std::cout << "\n" << CYAN << "[STEP 1] Verifying Project Structure..." << RESET << std::endl;
    log("Verifying project structure", "INFO", &log_stream);
    
    bool structureValid = true;
    std::string checkCpp = "cd " + cppDir + " && echo checked";
    std::string checkWeb = "cd " + webDir + " && echo checked";
    std::string checkData = "cd " + dataDir + " && echo checked";
    
    if (system(("if not exist \"" + cppDir + "\" (exit /b 1)").c_str()) != 0) {
        log("C++ directory not found: " + cppDir, "ERROR", &log_stream);
        structureValid = false;
    }
    if (system(("if not exist \"" + webDir + "\" (exit /b 1)").c_str()) != 0) {
        log("Web directory not found: " + webDir, "ERROR", &log_stream);
        structureValid = false;
    }
    if (system(("if not exist \"" + dataDir + "\" (exit /b 1)").c_str()) != 0) {
        log("Data directory not found: " + dataDir, "ERROR", &log_stream);
        structureValid = false;
    }
    
    if (!structureValid) {
        log("=== Launcher Failed ===", "ERROR", &log_stream);
        log_stream.close();
        std::cout << RED << "✗ Project structure validation failed" << RESET << std::endl;
        system("pause");
        return 1;
    }
    
    log("✓ Project structure verified", "SUCCESS", &log_stream);
    std::cout << GREEN << "✓ Project structure validated" << RESET << std::endl;
    
    // ========================================================================
    // STEP 2: Compile C++ Backend
    // ========================================================================
    std::cout << "\n" << CYAN << "[STEP 2] Compiling C++ Backend..." << RESET << std::endl;
    log("Attempting to compile C++ backend", "INFO", &log_stream);
    
    std::string compileCmd = "cd \"" + cppDir + "\" && g++ -std=c++17 -o rental_system.exe "
                             "main.cpp vehicle.cpp car.cpp bike.cpp truck.cpp "
                             "customer.cpp rental.cpp rentalmanager.cpp filemanager.cpp 2>&1";
    
    if (runCommand(compileCmd, "C++ Compilation", &log_stream)) {
        std::cout << GREEN << "✓ C++ backend compiled successfully" << RESET << std::endl;
        
        // Check executable exists
        std::string exePath = cppDir + "\\rental_system.exe";
        if (system(("if exist \"" + exePath + "\" (exit /b 0)").c_str()) == 0) {
            log("Executable created: " + exePath, "SUCCESS", &log_stream);
        }
    } else {
        std::cout << RED << "✗ C++ compilation failed. See log for details." << RESET << std::endl;
        log("=== Launcher Failed ===", "ERROR", &log_stream);
        log_stream.close();
        system("pause");
        return 1;
    }
    
    // ========================================================================
    // STEP 3: Display Startup Info
    // ========================================================================
    std::cout << "\n" << CYAN << "[STEP 3] Preparing Environment..." << RESET << std::endl;
    log("Environment preparation", "INFO", &log_stream);
    
    std::cout << GREEN << "✓ C++ Backend Ready" << RESET << std::endl;
    std::cout << "  Location: " << cppDir << std::endl;
    
    std::cout << GREEN << "✓ Data Layer Ready" << RESET << std::endl;
    std::cout << "  Location: " << dataDir << std::endl;
    std::cout << "    - vehicles.csv" << std::endl;
    std::cout << "    - customers.csv" << std::endl;
    std::cout << "    - rentals.csv" << std::endl;
    
    std::cout << GREEN << "✓ Web Dashboard Ready" << RESET << std::endl;
    std::cout << "  Location: " << webDir << std::endl;
    
    // ========================================================================
    // STEP 4: Show Summary
    // ========================================================================
    std::cout << "\n" << BRIGHT_CYAN << "╔═══════════════════════════════════════════════════════╗" << RESET << std::endl;
    std::cout << BRIGHT_CYAN << "║         SYSTEM READY FOR DEPLOYMENT                   ║" << RESET << std::endl;
    std::cout << BRIGHT_CYAN << "╚═══════════════════════════════════════════════════════╝" << RESET << std::endl;
    
    std::cout << "\n" << CYAN << "📋 Next Steps:" << RESET << std::endl;
    std::cout << "  1. Run C++ Backend:" << std::endl;
    std::cout << "     cd " << cppDir << std::endl;
    std::cout << "     .\\rental_system.exe" << std::endl;
    std::cout << "\n  2. Run Flask Dashboard (in another terminal):" << std::endl;
    std::cout << "     cd " << webDir << std::endl;
    std::cout << "     set VEHICLE_RENTAL_CSV_PATH=" << dataDir << std::endl;
    std::cout << "     python app.py" << std::endl;
    std::cout << "\n  3. Open browser:" << std::endl;
    std::cout << "     http://localhost:5000" << std::endl;
    
    std::cout << "\n" << CYAN << "📝 Logging:" << RESET << std::endl;
    std::cout << "  All operations logged to:" << std::endl;
    std::cout << "  " << logFile << std::endl;
    
    std::cout << "\n" << CYAN << "⏱️  Build Time: " << getTimestamp() << RESET << std::endl;
    
    log("=== Launcher Completed Successfully ===", "SUCCESS", &log_stream);
    log_stream.close();
    
    std::cout << "\n" << YELLOW << "Press Enter to close this window..." << RESET << std::endl;
    system("pause");
    
    return 0;
}
