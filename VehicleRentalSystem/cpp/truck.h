#ifndef TRUCK_H
#define TRUCK_H

#include "vehicle.h"

// Derived class for Truck vehicles
// Implements calculateRentalCost() with 30% surcharge
class Truck : public Vehicle {
public:
    // Constructor - calls Vehicle base class constructor
    Truck(const std::string& id, const std::string& brand, const std::string& model, 
          double rentPerDay, const std::string& status);

    // Override pure virtual function from Vehicle
    // Truck rental cost = 1.3 × base rate per day × number of days (30% surcharge)
    double calculateRentalCost(int days) const override;
};

#endif // TRUCK_H
