#include "customer.h"

Customer::Customer(const std::string& id, const std::string& nm, const std::string& ph, 
                   const std::string& em)
    : customerId(id), name(nm), phone(ph), email(em) {
    // Constructor body - initialization done in member initializer list
}

std::string Customer::getCustomerId() const {
    return customerId;
}

std::string Customer::getName() const {
    return name;
}

std::string Customer::getPhone() const {
    return phone;
}

std::string Customer::getEmail() const {
    return email;
}

void Customer::setCustomerId(const std::string& id) {
    customerId = id;
}

void Customer::setName(const std::string& nm) {
    name = nm;
}

void Customer::setPhone(const std::string& ph) {
    phone = ph;
}

void Customer::setEmail(const std::string& em) {
    email = em;
}
