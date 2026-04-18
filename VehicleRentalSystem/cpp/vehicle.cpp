#include "vehicle.h"

Vehicle::Vehicle(const std::string& id, const std::string& br, const std::string& md, 
                 double rate, const std::string& status)
    : vehicleId(id), brand(br), model(md), rentPerDay(rate), availabilityStatus(status) {
    // Constructor body - initialization done in member initializer list
}

std::string Vehicle::getVehicleId() const {
    return vehicleId;
}

std::string Vehicle::getBrand() const {
    return brand;
}

std::string Vehicle::getModel() const {
    return model;
}

double Vehicle::getRentPerDay() const {
    return rentPerDay;
}

std::string Vehicle::getAvailabilityStatus() const {
    return availabilityStatus;
}

void Vehicle::setVehicleId(const std::string& id) {
    vehicleId = id;
}

void Vehicle::setBrand(const std::string& br) {
    brand = br;
}

void Vehicle::setModel(const std::string& md) {
    model = md;
}

void Vehicle::setRentPerDay(double rate) {
    rentPerDay = rate;
}

void Vehicle::setAvailabilityStatus(const std::string& status) {
    availabilityStatus = status;
}
