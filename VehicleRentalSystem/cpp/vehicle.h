#ifndef VEHICLE_H
#define VEHICLE_H

#include <string>

// Abstract base class representing a vehicle in the rental system
// Enforces polymorphic rental cost calculation for different vehicle types
class Vehicle {
private:
    std::string vehicleId;       // Unique identifier for the vehicle
    std::string brand;           // Manufacturer brand
    std::string model;           // Vehicle model name
    double rentPerDay;           // Base rental rate per day
    std::string availabilityStatus; // "Available" or "Rented"

public:
    // Constructor
    Vehicle(const std::string& id, const std::string& br, const std::string& md, 
            double rate, const std::string& status);

    // Virtual destructor for proper cleanup of derived classes
    virtual ~Vehicle() = default;

    // Pure virtual function - each vehicle type calculates cost differently
    virtual double calculateRentalCost(int days) const = 0;

    // Getters (read-only access to private fields)
    std::string getVehicleId() const;
    std::string getBrand() const;
    std::string getModel() const;
    double getRentPerDay() const;
    std::string getAvailabilityStatus() const;

    // Setters (controlled write access to private fields)
    void setVehicleId(const std::string& id);
    void setBrand(const std::string& br);
    void setModel(const std::string& md);
    void setRentPerDay(double rate);
    void setAvailabilityStatus(const std::string& status);
};

#endif // VEHICLE_H
