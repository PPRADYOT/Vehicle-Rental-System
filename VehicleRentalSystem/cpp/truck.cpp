#include "truck.h"

Truck::Truck(const std::string& id, const std::string& brand, const std::string& model, 
             double rentPerDay, const std::string& status)
    : Vehicle(id, brand, model, rentPerDay, status) {
    // Constructor body - initialization delegated to Vehicle base class
}

double Truck::calculateRentalCost(int days) const {
    // Truck rental: 1.3 × base rate × days (30% surcharge)
    return 1.3 * getRentPerDay() * days;
}
