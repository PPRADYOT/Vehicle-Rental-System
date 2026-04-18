#include "car.h"

Car::Car(const std::string& id, const std::string& brand, const std::string& model, 
         double rentPerDay, const std::string& status)
    : Vehicle(id, brand, model, rentPerDay, status) {
    // Constructor body - initialization delegated to Vehicle base class
}

double Car::calculateRentalCost(int days) const {
    // Car rental: base rate × days (no discount)
    return getRentPerDay() * days;
}
