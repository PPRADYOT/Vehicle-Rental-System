#include "rental.h"

Rental::Rental(const std::string& rid, const std::string& cid, const std::string& vid,
               const std::string& start, const std::string& end, double cost)
    : rentalId(rid), customerId(cid), vehicleId(vid), startDate(start), endDate(end), totalCost(cost) {
    // Constructor body - initialization done in member initializer list
}

std::string Rental::getRentalId() const {
    return rentalId;
}

std::string Rental::getCustomerId() const {
    return customerId;
}

std::string Rental::getVehicleId() const {
    return vehicleId;
}

std::string Rental::getStartDate() const {
    return startDate;
}

std::string Rental::getEndDate() const {
    return endDate;
}

double Rental::getTotalCost() const {
    return totalCost;
}

void Rental::setRentalId(const std::string& rid) {
    rentalId = rid;
}

void Rental::setCustomerId(const std::string& cid) {
    customerId = cid;
}

void Rental::setVehicleId(const std::string& vid) {
    vehicleId = vid;
}

void Rental::setStartDate(const std::string& start) {
    startDate = start;
}

void Rental::setEndDate(const std::string& end) {
    endDate = end;
}

void Rental::setTotalCost(double cost) {
    totalCost = cost;
}
