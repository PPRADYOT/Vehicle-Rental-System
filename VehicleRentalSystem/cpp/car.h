#ifndef CAR_H
#define CAR_H

#include "vehicle.h"

// Derived class for Car vehicles
// Implements calculateRentalCost() with base rate (no discount or surcharge)
class Car : public Vehicle {
public:
    // Constructor - calls Vehicle base class constructor
    Car(const std::string& id, const std::string& brand, const std::string& model, 
        double rentPerDay, const std::string& status);

    // Override pure virtual function from Vehicle
    // Car rental cost = base rate per day × number of days
    double calculateRentalCost(int days) const override;
};

#endif // CAR_H
