#ifndef BIKE_H
#define BIKE_H

#include "vehicle.h"

// Derived class for Bike vehicles
// Implements calculateRentalCost() with 20% discount
class Bike : public Vehicle {
public:
    // Constructor - calls Vehicle base class constructor
    Bike(const std::string& id, const std::string& brand, const std::string& model, 
         double rentPerDay, const std::string& status);

    // Override pure virtual function from Vehicle
    // Bike rental cost = 0.8 × base rate per day × number of days (20% discount)
    double calculateRentalCost(int days) const override;
};

#endif // BIKE_H
