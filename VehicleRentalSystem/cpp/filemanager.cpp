#include "filemanager.h"
#include <fstream>
#include <sstream>
#include <iostream>

FileManager::FileManager(const std::string& vehiclesPath, const std::string& customersPath, 
                         const std::string& rentalsPath)
    : vPath(vehiclesPath), cPath(customersPath), rPath(rentalsPath) {
}

std::vector<Vehicle*> FileManager::loadVehicles() {
    std::vector<Vehicle*> vehicles;
    std::ifstream file(vPath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << vPath << std::endl;
        return vehicles;
    }

    std::string line;
    // Skip header line
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string id, type, brand, model, rentPerDayStr, status;

        // Parse CSV format: id, type, brand, model, rentPerDay, status
        std::getline(ss, id, ',');
        std::getline(ss, type, ',');
        std::getline(ss, brand, ',');
        std::getline(ss, model, ',');
        std::getline(ss, rentPerDayStr, ',');
        std::getline(ss, status, ',');

        double rentPerDay = std::stod(rentPerDayStr);

        // Create appropriate vehicle type based on type field
        Vehicle* vehicle = nullptr;
        if (type == "Car") {
            vehicle = new Car(id, brand, model, rentPerDay, status);
        } else if (type == "Bike") {
            vehicle = new Bike(id, brand, model, rentPerDay, status);
        } else if (type == "Truck") {
            vehicle = new Truck(id, brand, model, rentPerDay, status);
        }

        if (vehicle != nullptr) {
            vehicles.push_back(vehicle);
        }
    }

    file.close();
    return vehicles;
}

std::vector<Customer> FileManager::loadCustomers() {
    std::vector<Customer> customers;
    std::ifstream file(cPath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << cPath << std::endl;
        return customers;
    }

    std::string line;
    // Skip header line
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string id, name, phone, email;

        // Parse CSV format: id, name, phone, email
        std::getline(ss, id, ',');
        std::getline(ss, name, ',');
        std::getline(ss, phone, ',');
        std::getline(ss, email, ',');

        customers.emplace_back(id, name, phone, email);
    }

    file.close();
    return customers;
}

std::vector<Rental> FileManager::loadRentals() {
    std::vector<Rental> rentals;
    std::ifstream file(rPath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << rPath << std::endl;
        return rentals;
    }

    std::string line;
    // Skip header line
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string rentalId, customerId, vehicleId, startDate, endDate, totalCostStr;

        // Parse CSV format: rentalId, customerId, vehicleId, startDate, endDate, totalCost
        std::getline(ss, rentalId, ',');
        std::getline(ss, customerId, ',');
        std::getline(ss, vehicleId, ',');
        std::getline(ss, startDate, ',');
        std::getline(ss, endDate, ',');
        std::getline(ss, totalCostStr, ',');

        double totalCost = std::stod(totalCostStr);

        rentals.emplace_back(rentalId, customerId, vehicleId, startDate, endDate, totalCost);
    }

    file.close();
    return rentals;
}

void FileManager::saveVehicles(const std::vector<Vehicle*>& vehicles) {
    std::ofstream file(vPath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << vPath << " for writing" << std::endl;
        return;
    }

    // Write header
    file << "id,type,brand,model,rentPerDay,status\n";

    // Write each vehicle
    for (const auto& vehicle : vehicles) {
        // Determine the type of vehicle using dynamic casting to get the type name
        std::string type;
        if (dynamic_cast<Car*>(vehicle)) {
            type = "Car";
        } else if (dynamic_cast<Bike*>(vehicle)) {
            type = "Bike";
        } else if (dynamic_cast<Truck*>(vehicle)) {
            type = "Truck";
        }

        file << vehicle->getVehicleId() << ","
             << type << ","
             << vehicle->getBrand() << ","
             << vehicle->getModel() << ","
             << vehicle->getRentPerDay() << ","
             << vehicle->getAvailabilityStatus() << "\n";
    }

    file.close();
}

void FileManager::saveCustomers(const std::vector<Customer>& customers) {
    std::ofstream file(cPath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << cPath << " for writing" << std::endl;
        return;
    }

    // Write header
    file << "id,name,phone,email\n";

    // Write each customer
    for (const auto& customer : customers) {
        file << customer.getCustomerId() << ","
             << customer.getName() << ","
             << customer.getPhone() << ","
             << customer.getEmail() << "\n";
    }

    file.close();
}

void FileManager::saveRentals(const std::vector<Rental>& rentals) {
    std::ofstream file(rPath);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << rPath << " for writing" << std::endl;
        return;
    }

    // Write header
    file << "rentalId,customerId,vehicleId,startDate,endDate,totalCost\n";

    // Write each rental
    for (const auto& rental : rentals) {
        file << rental.getRentalId() << ","
             << rental.getCustomerId() << ","
             << rental.getVehicleId() << ","
             << rental.getStartDate() << ","
             << rental.getEndDate() << ","
             << rental.getTotalCost() << "\n";
    }

    file.close();
}
