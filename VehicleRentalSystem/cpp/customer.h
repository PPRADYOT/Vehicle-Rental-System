#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>

// Class representing a customer in the rental system
// Maintains customer information with full encapsulation
class Customer {
private:
    std::string customerId;  // Unique identifier for the customer
    std::string name;        // Customer's full name
    std::string phone;       // Customer's phone number
    std::string email;       // Customer's email address

public:
    // Constructor
    Customer(const std::string& id, const std::string& nm, const std::string& ph, 
             const std::string& em);

    // Getters (read-only access to private fields)
    std::string getCustomerId() const;
    std::string getName() const;
    std::string getPhone() const;
    std::string getEmail() const;

    // Setters (controlled write access to private fields)
    void setCustomerId(const std::string& id);
    void setName(const std::string& nm);
    void setPhone(const std::string& ph);
    void setEmail(const std::string& em);
};

#endif // CUSTOMER_H
