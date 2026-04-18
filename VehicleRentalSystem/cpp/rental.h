#ifndef RENTAL_H
#define RENTAL_H

#include <string>

// Class representing a rental transaction
// Tracks rental details: customer, vehicle, dates, and calculated cost
class Rental {
private:
    std::string rentalId;    // Unique identifier for the rental
    std::string customerId;  // ID of the customer renting
    std::string vehicleId;   // ID of the vehicle being rented
    std::string startDate;   // Rental start date (format: YYYY-MM-DD)
    std::string endDate;     // Rental end date (format: YYYY-MM-DD)
    double totalCost;        // Total rental cost calculated

public:
    // Constructor
    Rental(const std::string& rid, const std::string& cid, const std::string& vid,
           const std::string& start, const std::string& end, double cost);

    // Getters (read-only access to private fields)
    std::string getRentalId() const;
    std::string getCustomerId() const;
    std::string getVehicleId() const;
    std::string getStartDate() const;
    std::string getEndDate() const;
    double getTotalCost() const;

    // Setters (controlled write access to private fields)
    void setRentalId(const std::string& rid);
    void setCustomerId(const std::string& cid);
    void setVehicleId(const std::string& vid);
    void setStartDate(const std::string& start);
    void setEndDate(const std::string& end);
    void setTotalCost(double cost);
};

#endif // RENTAL_H
