#include "rentalmanager.h"
#include "filemanager.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <algorithm>

RentalManager::RentalManager() {
}

RentalManager::~RentalManager() {
    // Free all dynamically allocated Vehicle objects
    for (auto v : vehicles) {
        delete v;
    }
    vehicles.clear();
}

void RentalManager::loadData(FileManager& fm) {
    // Load all data from CSV files
    vehicles = fm.loadVehicles();
    customers = fm.loadCustomers();
    rentals = fm.loadRentals();
}

void RentalManager::addVehicle(Vehicle* vehicle) {
    if (vehicle != nullptr) {
        vehicles.push_back(vehicle);
    }
}

void RentalManager::addCustomer(const Customer& customer) {
    customers.push_back(customer);
}

void RentalManager::displayAllVehicles() const {
    if (vehicles.empty()) {
        std::cout << "No vehicles in the system.\n";
        return;
    }

    std::cout << "\n";
    std::cout << std::string(100, '-') << "\n";
    std::cout << std::left << std::setw(8) << "ID"
              << std::setw(10) << "Type"
              << std::setw(15) << "Brand"
              << std::setw(15) << "Model"
              << std::setw(12) << "Rent/Day"
              << std::setw(15) << "Status\n";
    std::cout << std::string(100, '-') << "\n";

    for (const auto& vehicle : vehicles) {
        // Determine type for display
        std::string type;
        if (dynamic_cast<Car*>(vehicle)) {
            type = "Car";
        } else if (dynamic_cast<Bike*>(vehicle)) {
            type = "Bike";
        } else if (dynamic_cast<Truck*>(vehicle)) {
            type = "Truck";
        }

        std::cout << std::left << std::setw(8) << vehicle->getVehicleId()
                  << std::setw(10) << type
                  << std::setw(15) << vehicle->getBrand()
                  << std::setw(15) << vehicle->getModel()
                  << std::setw(12) << std::fixed << std::setprecision(2) << vehicle->getRentPerDay()
                  << std::setw(15) << vehicle->getAvailabilityStatus() << "\n";
    }
    std::cout << std::string(100, '-') << "\n\n";
}

void RentalManager::displayAllCustomers() const {
    if (customers.empty()) {
        std::cout << "No customers in the system.\n";
        return;
    }

    std::cout << "\n";
    std::cout << std::string(80, '-') << "\n";
    std::cout << std::left << std::setw(8) << "ID"
              << std::setw(20) << "Name"
              << std::setw(15) << "Phone"
              << std::setw(30) << "Email\n";
    std::cout << std::string(80, '-') << "\n";

    for (const auto& customer : customers) {
        std::cout << std::left << std::setw(8) << customer.getCustomerId()
                  << std::setw(20) << customer.getName()
                  << std::setw(15) << customer.getPhone()
                  << std::setw(30) << customer.getEmail() << "\n";
    }
    std::cout << std::string(80, '-') << "\n\n";
}

int RentalManager::calculateDays(const std::string& startDate, const std::string& endDate) const {
    // Parse dates in format YYYY-MM-DD
    int startYear, startMonth, startDay;
    int endYear, endMonth, endDay;

    sscanf(startDate.c_str(), "%d-%d-%d", &startYear, &startMonth, &startDay);
    sscanf(endDate.c_str(), "%d-%d-%d", &endYear, &endMonth, &endDay);

    // Simple calculation: crude but works for same year comparisons
    // For production, use proper date library
    struct tm startTm = {};
    struct tm endTm = {};

    startTm.tm_year = startYear - 1900;
    startTm.tm_mon = startMonth - 1;
    startTm.tm_mday = startDay;

    endTm.tm_year = endYear - 1900;
    endTm.tm_mon = endMonth - 1;
    endTm.tm_mday = endDay;

    time_t startTime = mktime(&startTm);
    time_t endTime = mktime(&endTm);

    int days = static_cast<int>(difftime(endTime, startTime) / 86400);
    return (days > 0) ? days : 1;  // At least 1 day
}

void RentalManager::rentVehicle(const std::string& vehicleId, const std::string& customerId,
                                const std::string& startDate, const std::string& endDate) {
    // Find vehicle
    Vehicle* vehicle = findVehicle(vehicleId);
    if (vehicle == nullptr) {
        std::cerr << "Error: Vehicle not found.\n";
        return;
    }

    // Check if vehicle is available
    if (vehicle->getAvailabilityStatus() != "Available") {
        std::cerr << "Error: Vehicle is not available.\n";
        return;
    }

    // Find customer
    Customer* customer = findCustomer(customerId);
    if (customer == nullptr) {
        std::cerr << "Error: Customer not found.\n";
        return;
    }

    // Calculate rental cost using polymorphism
    int days = calculateDays(startDate, endDate);
    double cost = vehicle->calculateRentalCost(days);

    // Create rental record
    std::string rentalId = "R" + std::to_string(rentals.size() + 1);
    Rental newRental(rentalId, customerId, vehicleId, startDate, endDate, cost);
    rentals.push_back(newRental);

    // Update vehicle status
    vehicle->setAvailabilityStatus("Rented");

    std::cout << "\nVehicle rented successfully!\n";
    std::cout << "Rental ID: " << rentalId << "\n";
    std::cout << "Days: " << days << "\n";
    std::cout << "Total Cost: $" << std::fixed << std::setprecision(2) << cost << "\n\n";
}

void RentalManager::returnVehicle(const std::string& vehicleId) {
    // Find vehicle
    Vehicle* vehicle = findVehicle(vehicleId);
    if (vehicle == nullptr) {
        std::cerr << "Error: Vehicle not found.\n";
        return;
    }

    // Find active rental for this vehicle
    Rental* rental = findRentalByVehicle(vehicleId);
    if (rental == nullptr) {
        std::cerr << "Error: No active rental found for this vehicle.\n";
        return;
    }

    // Update vehicle status
    vehicle->setAvailabilityStatus("Available");

    std::cout << "\nVehicle returned successfully!\n";
    std::cout << "Rental ID: " << rental->getRentalId() << "\n";
    std::cout << "Total Cost: $" << std::fixed << std::setprecision(2) << rental->getTotalCost() << "\n\n";
}

void RentalManager::generateRentalReport() const {
    if (rentals.empty()) {
        std::cout << "No rentals to report.\n";
        return;
    }

    double totalRevenue = 0;
    int carCount = 0, bikeCount = 0, truckCount = 0;

    std::cout << "\n";
    std::cout << std::string(130, '=') << "\n";
    std::cout << std::string(40, ' ') << "RENTAL REPORT\n";
    std::cout << std::string(130, '=') << "\n\n";

    std::cout << std::left << std::setw(10) << "Rental ID"
              << std::setw(12) << "Customer ID"
              << std::setw(10) << "Vehicle ID"
              << std::setw(15) << "Start Date"
              << std::setw(15) << "End Date"
              << std::setw(15) << "Total Cost\n";
    std::cout << std::string(130, '-') << "\n";

    for (const auto& rental : rentals) {
        totalRevenue += rental.getTotalCost();

        // Count vehicle types from associated vehicles
        Vehicle* v = findVehicle(rental.getVehicleId());
        if (v) {
            if (dynamic_cast<Car*>(v)) carCount++;
            else if (dynamic_cast<Bike*>(v)) bikeCount++;
            else if (dynamic_cast<Truck*>(v)) truckCount++;
        }

        std::cout << std::left << std::setw(10) << rental.getRentalId()
                  << std::setw(12) << rental.getCustomerId()
                  << std::setw(10) << rental.getVehicleId()
                  << std::setw(15) << rental.getStartDate()
                  << std::setw(15) << rental.getEndDate()
                  << std::setw(15) << std::fixed << std::setprecision(2) << rental.getTotalCost() << "\n";
    }

    std::cout << std::string(130, '=') << "\n";
    std::cout << "\nSummary:\n";
    std::cout << "  Total Rentals: " << rentals.size() << "\n";
    std::cout << "  Total Revenue: $" << std::fixed << std::setprecision(2) << totalRevenue << "\n";
    std::cout << "  Cars Rented: " << carCount << "\n";
    std::cout << "  Bikes Rented: " << bikeCount << "\n";
    std::cout << "  Trucks Rented: " << truckCount << "\n\n";
}

void RentalManager::saveData(FileManager& fm) const {
    fm.saveVehicles(vehicles);
    fm.saveCustomers(customers);
    fm.saveRentals(rentals);
}

Vehicle* RentalManager::findVehicle(const std::string& vehicleId) const {
    for (auto& v : vehicles) {
        if (v->getVehicleId() == vehicleId) {
            return v;
        }
    }
    return nullptr;
}

Customer* RentalManager::findCustomer(const std::string& customerId) const {
    for (auto& c : customers) {
        if (c.getCustomerId() == customerId) {
            return const_cast<Customer*>(&c);
        }
    }
    return nullptr;
}

Rental* RentalManager::findRentalByVehicle(const std::string& vehicleId) const {
    for (auto& r : rentals) {
        if (r.getVehicleId() == vehicleId && r.getEndDate() == "") {
            return const_cast<Rental*>(&r);
        }
    }
    return nullptr;
}
