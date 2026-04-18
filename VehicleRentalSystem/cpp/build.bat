@echo off
cd /d "C:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem\cpp"
echo Compiling C++ Vehicle Rental System...
g++ -std=c++17 -Wall main.cpp vehicle.cpp car.cpp bike.cpp truck.cpp customer.cpp rental.cpp rentalmanager.cpp filemanager.cpp -o rental_system.exe
if %ERRORLEVEL% EQU 0 (
    echo Compilation successful!
    dir /B rental_system.exe
) else (
    echo Compilation failed!
)
pause
