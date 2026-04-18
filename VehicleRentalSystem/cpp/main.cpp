#include "rentalmanager.h"
#include "filemanager.h"
#include <iostream>
#include <sstream>
#include <cstdlib>

// Function prototypes
void displayMenu();
std::string getInput(const std::string& prompt);
void pauseScreen();

int main() {
    // Initialize FileManager with CSV paths
    FileManager fm("data/vehicles.csv", "data/customers.csv", "data/rentals.csv");

    // Initialize RentalManager
    RentalManager rm;

    // Load existing data from CSV files
    rm.loadData(fm);

    int choice = 0;

    std::cout << "\n";
    std::cout << "============================================\n";
    std::cout << "   VEHICLE RENTAL SYSTEM - CONSOLE BACKEND\n";
    std::cout << "============================================\n\n";

    while (true) {
        displayMenu();
        std::cout << "Enter your choice (1-8): ";
        std::cin >> choice;
        std::cin.ignore();  // Clear newline from input buffer

        switch (choice) {
        case 1: {
            // Add Vehicle
            std::cout << "\n--- Add Vehicle ---\n";
            std::string vehicleId = getInput("Enter Vehicle ID: ");
            std::string type = getInput("Enter Vehicle Type (Car/Bike/Truck): ");
            std::string brand = getInput("Enter Brand: ");
            std::string model = getInput("Enter Model: ");
            std::cout << "Enter Rent Per Day: ";
            double rentPerDay;
            std::cin >> rentPerDay;
            std::cin.ignore();

            Vehicle* vehicle = nullptr;
            if (type == "Car") {
                vehicle = new Car(vehicleId, brand, model, rentPerDay, "Available");
            } else if (type == "Bike") {
                vehicle = new Bike(vehicleId, brand, model, rentPerDay, "Available");
            } else if (type == "Truck") {
                vehicle = new Truck(vehicleId, brand, model, rentPerDay, "Available");
            } else {
                std::cerr << "Invalid vehicle type.\n";
                break;
            }

            rm.addVehicle(vehicle);
            std::cout << "Vehicle added successfully!\n";
            pauseScreen();
            break;
        }

        case 2: {
            // View All Vehicles
            std::cout << "\n--- All Vehicles ---\n";
            rm.displayAllVehicles();
            pauseScreen();
            break;
        }

        case 3: {
            // Rent a Vehicle
            std::cout << "\n--- Rent a Vehicle ---\n";
            std::string vehicleId = getInput("Enter Vehicle ID: ");
            std::string customerId = getInput("Enter Customer ID: ");
            std::string startDate = getInput("Enter Start Date (YYYY-MM-DD): ");
            std::string endDate = getInput("Enter End Date (YYYY-MM-DD): ");
            rm.rentVehicle(vehicleId, customerId, startDate, endDate);
            pauseScreen();
            break;
        }

        case 4: {
            // Return a Vehicle
            std::cout << "\n--- Return a Vehicle ---\n";
            std::string vehicleId = getInput("Enter Vehicle ID: ");
            rm.returnVehicle(vehicleId);
            pauseScreen();
            break;
        }

        case 5: {
            // Add Customer
            std::cout << "\n--- Add Customer ---\n";
            std::string customerId = getInput("Enter Customer ID: ");
            std::string name = getInput("Enter Name: ");
            std::string phone = getInput("Enter Phone: ");
            std::string email = getInput("Enter Email: ");

            Customer customer(customerId, name, phone, email);
            rm.addCustomer(customer);
            std::cout << "Customer added successfully!\n";
            pauseScreen();
            break;
        }

        case 6: {
            // View All Customers
            std::cout << "\n--- All Customers ---\n";
            rm.displayAllCustomers();
            pauseScreen();
            break;
        }

        case 7: {
            // Generate Rental Report
            std::cout << "\n";
            rm.generateRentalReport();
            pauseScreen();
            break;
        }

        case 8: {
            // Save & Exit
            std::cout << "\nSaving data...\n";
            rm.saveData(fm);
            std::cout << "Data saved successfully!\n";
            std::cout << "Thank you for using Vehicle Rental System. Goodbye!\n";
            return 0;
        }

        default:
            std::cerr << "Invalid choice. Please try again.\n";
            pauseScreen();
        }
    }

    return 0;
}

void displayMenu() {
    std::cout << "\n";
    std::cout << "========== MAIN MENU ==========\n";
    std::cout << "1. Add Vehicle\n";
    std::cout << "2. View All Vehicles\n";
    std::cout << "3. Rent a Vehicle\n";
    std::cout << "4. Return a Vehicle\n";
    std::cout << "5. Add Customer\n";
    std::cout << "6. View All Customers\n";
    std::cout << "7. Generate Rental Report\n";
    std::cout << "8. Save & Exit\n";
    std::cout << "===============================\n";
}

std::string getInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void pauseScreen() {
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}
