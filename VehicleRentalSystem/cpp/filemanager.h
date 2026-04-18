#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "vehicle.h"
#include "car.h"
#include "bike.h"
#include "truck.h"
#include "customer.h"
#include "rental.h"
#include <vector>
#include <string>

// Class responsible for reading and writing CSV data files
// Handles persistence for vehicles, customers, and rentals
class FileManager {
private:
    std::string vPath;  // Path to vehicles.csv
    std::string cPath;  // Path to customers.csv
    std::string rPath;  // Path to rentals.csv

public:
    // Constructor - accepts paths to CSV files
    FileManager(const std::string& vehiclesPath, const std::string& customersPath, 
                const std::string& rentalsPath);

    // Methods to load data from CSV files into vectors
    // Returns a vector of Vehicle pointers (polymorphic for Car, Bike, Truck)
    std::vector<Vehicle*> loadVehicles();

    // Returns a vector of Customer objects
    std::vector<Customer> loadCustomers();

    // Returns a vector of Rental objects
    std::vector<Rental> loadRentals();

    // Methods to save data to CSV files
    // Each method takes the vector and writes it back to the corresponding CSV
    void saveVehicles(const std::vector<Vehicle*>& vehicles);
    void saveCustomers(const std::vector<Customer>& customers);
    void saveRentals(const std::vector<Rental>& rentals);

    // Helper method to extract type from CSV line (Car, Bike, or Truck)
    std::string extractType(const std::string& line);
};

#endif // FILEMANAGER_H
