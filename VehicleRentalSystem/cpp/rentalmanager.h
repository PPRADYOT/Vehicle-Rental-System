#ifndef RENTALMANAGER_H
#define RENTALMANAGER_H

#include "vehicle.h"
#include "customer.h"
#include "rental.h"
#include <vector>
#include <string>

// Manager class for handling all rental system operations
// Uses polymorphic Vehicle pointers; composition with Customer; aggregation with Rental
class RentalManager {
private:
    std::vector<Vehicle*> vehicles;      // Owns Vehicle objects (polymorphic)
    std::vector<Customer> customers;     // Composition: owns Customer objects
    std::vector<Rental> rentals;         // Aggregation: owns Rental objects

public:
    // Constructor
    RentalManager();

    // Destructor - Important: cleanup dynamically allocated Vehicle pointers
    ~RentalManager();

    // Initialize manager with data from files
    void loadData(class FileManager& fm);

    // Add a new vehicle to the system
    void addVehicle(Vehicle* vehicle);

    // Add a new customer to the system
    void addCustomer(const Customer& customer);

    // View all vehicles with their details
    void displayAllVehicles() const;

    // View all customers with their details
    void displayAllCustomers() const;

    // Rent a vehicle - updates availability status and creates a rental record
    void rentVehicle(const std::string& vehicleId, const std::string& customerId, 
                    const std::string& startDate, const std::string& endDate);

    // Return a vehicle - marks as available again
    void returnVehicle(const std::string& vehicleId);

    // Generate and display rental report showing all rental transactions
    void generateRentalReport() const;

    // Save all data to CSV files
    void saveData(class FileManager& fm) const;

    // Helper methods
    Vehicle* findVehicle(const std::string& vehicleId) const;
    Customer* findCustomer(const std::string& customerId) const;
    Rental* findRentalByVehicle(const std::string& vehicleId) const;

    // Utility: calculate number of days between two dates (YYYY-MM-DD format)
    int calculateDays(const std::string& startDate, const std::string& endDate) const;
};

#endif // RENTALMANAGER_H
