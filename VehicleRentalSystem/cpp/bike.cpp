#include "bike.h"

Bike::Bike(const std::string& id, const std::string& brand, const std::string& model, 
           double rentPerDay, const std::string& status)
    : Vehicle(id, brand, model, rentPerDay, status) {
    // Constructor body - initialization delegated to Vehicle base class
}

double Bike::calculateRentalCost(int days) const {
    // Bike rental: 0.8 × base rate × days (20% discount)
    return 0.8 * getRentPerDay() * days;
}
