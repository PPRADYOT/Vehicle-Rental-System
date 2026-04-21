#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <cstdio>

// ========================== Vehicle (Abstract Base) ==========================
class Vehicle {
    std::string vehicleId, brand, model, availabilityStatus;
    double rentPerDay;
public:
    Vehicle(const std::string& id, const std::string& br, const std::string& md,
            double rate, const std::string& status)
        : vehicleId(id), brand(br), model(md), availabilityStatus(status), rentPerDay(rate) {}
    virtual ~Vehicle() = default;
    virtual double calculateRentalCost(int days) const = 0;
    virtual std::string getType() const = 0;
    std::string getVehicleId() const { return vehicleId; }
    std::string getBrand() const { return brand; }
    std::string getModel() const { return model; }
    double getRentPerDay() const { return rentPerDay; }
    std::string getAvailabilityStatus() const { return availabilityStatus; }
    void setVehicleId(const std::string& id) { vehicleId = id; }
    void setBrand(const std::string& br) { brand = br; }
    void setModel(const std::string& md) { model = md; }
    void setRentPerDay(double rate) { rentPerDay = rate; }
    void setAvailabilityStatus(const std::string& status) { availabilityStatus = status; }
};

// ========================== Car ==========================
class Car : public Vehicle {
public:
    Car(const std::string& id, const std::string& brand, const std::string& model,
        double rentPerDay, const std::string& status)
        : Vehicle(id, brand, model, rentPerDay, status) {}
    double calculateRentalCost(int days) const override { return getRentPerDay() * days; }
    std::string getType() const override { return "Car"; }
};

// ========================== Bike ==========================
class Bike : public Vehicle {
public:
    Bike(const std::string& id, const std::string& brand, const std::string& model,
         double rentPerDay, const std::string& status)
        : Vehicle(id, brand, model, rentPerDay, status) {}
    double calculateRentalCost(int days) const override { return 0.8 * getRentPerDay() * days; }
    std::string getType() const override { return "Bike"; }
};

// ========================== Truck ==========================
class Truck : public Vehicle {
public:
    Truck(const std::string& id, const std::string& brand, const std::string& model,
          double rentPerDay, const std::string& status)
        : Vehicle(id, brand, model, rentPerDay, status) {}
    double calculateRentalCost(int days) const override { return 1.3 * getRentPerDay() * days; }
    std::string getType() const override { return "Truck"; }
};

// ========================== Customer ==========================
class Customer {
    std::string customerId, name, phone, email;
public:
    Customer(const std::string& id, const std::string& nm, const std::string& ph,
             const std::string& em)
        : customerId(id), name(nm), phone(ph), email(em) {}
    std::string getCustomerId() const { return customerId; }
    std::string getName() const { return name; }
    std::string getPhone() const { return phone; }
    std::string getEmail() const { return email; }
    void setCustomerId(const std::string& id) { customerId = id; }
    void setName(const std::string& nm) { name = nm; }
    void setPhone(const std::string& ph) { phone = ph; }
    void setEmail(const std::string& em) { email = em; }
};

// ========================== Rental ==========================
class Rental {
    std::string rentalId, customerId, vehicleId, startDate, endDate;
    double totalCost;
public:
    Rental(const std::string& rid, const std::string& cid, const std::string& vid,
           const std::string& start, const std::string& end, double cost)
        : rentalId(rid), customerId(cid), vehicleId(vid), startDate(start), endDate(end), totalCost(cost) {}
    std::string getRentalId() const { return rentalId; }
    std::string getCustomerId() const { return customerId; }
    std::string getVehicleId() const { return vehicleId; }
    std::string getStartDate() const { return startDate; }
    std::string getEndDate() const { return endDate; }
    double getTotalCost() const { return totalCost; }
    void setRentalId(const std::string& rid) { rentalId = rid; }
    void setCustomerId(const std::string& cid) { customerId = cid; }
    void setVehicleId(const std::string& vid) { vehicleId = vid; }
    void setStartDate(const std::string& start) { startDate = start; }
    void setEndDate(const std::string& end) { endDate = end; }
    void setTotalCost(double cost) { totalCost = cost; }
};

// ========================== FileManager ==========================
class FileManager {
    std::string vPath, cPath, rPath;

    static Vehicle* createVehicle(const std::string& type, const std::string& id,
                                  const std::string& brand, const std::string& model,
                                  double rent, const std::string& status) {
        if (type == "Car")   return new Car(id, brand, model, rent, status);
        if (type == "Bike")  return new Bike(id, brand, model, rent, status);
        if (type == "Truck") return new Truck(id, brand, model, rent, status);
        return nullptr;
    }
public:
    FileManager(const std::string& vehiclesPath, const std::string& customersPath,
                const std::string& rentalsPath)
        : vPath(vehiclesPath), cPath(customersPath), rPath(rentalsPath) {}

    std::vector<Vehicle*> loadVehicles() {
        std::vector<Vehicle*> vehicles;
        std::ifstream file(vPath);
        if (!file.is_open()) { std::cerr << "Error: Could not open " << vPath << std::endl; return vehicles; }
        std::string line;
        std::getline(file, line); // skip header
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string id, type, brand, model, rentStr, status;
            std::getline(ss, id, ',');    std::getline(ss, type, ',');
            std::getline(ss, brand, ','); std::getline(ss, model, ',');
            std::getline(ss, rentStr, ','); std::getline(ss, status, ',');
            Vehicle* v = createVehicle(type, id, brand, model, std::stod(rentStr), status);
            if (v) vehicles.push_back(v);
        }
        return vehicles;
    }

    std::vector<Customer> loadCustomers() {
        std::vector<Customer> customers;
        std::ifstream file(cPath);
        if (!file.is_open()) { std::cerr << "Error: Could not open " << cPath << std::endl; return customers; }
        std::string line;
        std::getline(file, line); // skip header
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string id, name, phone, email;
            std::getline(ss, id, ',');    std::getline(ss, name, ',');
            std::getline(ss, phone, ','); std::getline(ss, email, ',');
            customers.emplace_back(id, name, phone, email);
        }
        return customers;
    }

    std::vector<Rental> loadRentals() {
        std::vector<Rental> rentals;
        std::ifstream file(rPath);
        if (!file.is_open()) { std::cerr << "Error: Could not open " << rPath << std::endl; return rentals; }
        std::string line;
        std::getline(file, line); // skip header
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            std::stringstream ss(line);
            std::string rid, cid, vid, start, end, costStr;
            std::getline(ss, rid, ',');   std::getline(ss, cid, ',');
            std::getline(ss, vid, ',');   std::getline(ss, start, ',');
            std::getline(ss, end, ',');   std::getline(ss, costStr, ',');
            rentals.emplace_back(rid, cid, vid, start, end, std::stod(costStr));
        }
        return rentals;
    }

    void saveVehicles(const std::vector<Vehicle*>& vehicles) {
        std::ofstream file(vPath);
        if (!file.is_open()) { std::cerr << "Error: Could not open " << vPath << " for writing" << std::endl; return; }
        file << "id,type,brand,model,rentPerDay,status\n";
        for (const auto& v : vehicles)
            file << v->getVehicleId() << "," << v->getType() << "," << v->getBrand() << ","
                 << v->getModel() << "," << v->getRentPerDay() << "," << v->getAvailabilityStatus() << "\n";
    }

    void saveCustomers(const std::vector<Customer>& customers) {
        std::ofstream file(cPath);
        if (!file.is_open()) { std::cerr << "Error: Could not open " << cPath << " for writing" << std::endl; return; }
        file << "id,name,phone,email\n";
        for (const auto& c : customers)
            file << c.getCustomerId() << "," << c.getName() << "," << c.getPhone() << "," << c.getEmail() << "\n";
    }

    void saveRentals(const std::vector<Rental>& rentals) {
        std::ofstream file(rPath);
        if (!file.is_open()) { std::cerr << "Error: Could not open " << rPath << " for writing" << std::endl; return; }
        file << "rentalId,customerId,vehicleId,startDate,endDate,totalCost\n";
        for (const auto& r : rentals)
            file << r.getRentalId() << "," << r.getCustomerId() << "," << r.getVehicleId() << ","
                 << r.getStartDate() << "," << r.getEndDate() << "," << r.getTotalCost() << "\n";
    }
};

// ========================== RentalManager ==========================
class RentalManager {
    std::vector<Vehicle*> vehicles;
    std::vector<Customer> customers;
    std::vector<Rental> rentals;

    int calculateDays(const std::string& startDate, const std::string& endDate) const {
        int sy, sm, sd, ey, em, ed;
        std::sscanf(startDate.c_str(), "%d-%d-%d", &sy, &sm, &sd);
        std::sscanf(endDate.c_str(), "%d-%d-%d", &ey, &em, &ed);
        struct tm st = {}, et = {};
        st.tm_year = sy - 1900; st.tm_mon = sm - 1; st.tm_mday = sd;
        et.tm_year = ey - 1900; et.tm_mon = em - 1; et.tm_mday = ed;
        int days = static_cast<int>(difftime(mktime(&et), mktime(&st)) / 86400);
        return (days > 0) ? days : 1;
    }
public:
    RentalManager() = default;
    ~RentalManager() { for (auto v : vehicles) delete v; }

    void loadData(FileManager& fm) {
        vehicles = fm.loadVehicles();
        customers = fm.loadCustomers();
        rentals = fm.loadRentals();
    }

    void addVehicle(Vehicle* v) { if (v) vehicles.push_back(v); }
    void addCustomer(const Customer& c) { customers.push_back(c); }

    void displayAllVehicles() const {
        if (vehicles.empty()) { std::cout << "No vehicles in the system.\n"; return; }
        std::cout << "\n" << std::string(100, '-') << "\n";
        std::cout << std::left << std::setw(8) << "ID" << std::setw(10) << "Type"
                  << std::setw(15) << "Brand" << std::setw(15) << "Model"
                  << std::setw(12) << "Rent/Day" << std::setw(15) << "Status\n";
        std::cout << std::string(100, '-') << "\n";
        for (const auto& v : vehicles)
            std::cout << std::left << std::setw(8) << v->getVehicleId()
                      << std::setw(10) << v->getType()
                      << std::setw(15) << v->getBrand()
                      << std::setw(15) << v->getModel()
                      << std::setw(12) << std::fixed << std::setprecision(2) << v->getRentPerDay()
                      << std::setw(15) << v->getAvailabilityStatus() << "\n";
        std::cout << std::string(100, '-') << "\n\n";
    }

    void displayAllCustomers() const {
        if (customers.empty()) { std::cout << "No customers in the system.\n"; return; }
        std::cout << "\n" << std::string(80, '-') << "\n";
        std::cout << std::left << std::setw(8) << "ID" << std::setw(20) << "Name"
                  << std::setw(15) << "Phone" << std::setw(30) << "Email\n";
        std::cout << std::string(80, '-') << "\n";
        for (const auto& c : customers)
            std::cout << std::left << std::setw(8) << c.getCustomerId()
                      << std::setw(20) << c.getName()
                      << std::setw(15) << c.getPhone()
                      << std::setw(30) << c.getEmail() << "\n";
        std::cout << std::string(80, '-') << "\n\n";
    }

    void rentVehicle(const std::string& vehicleId, const std::string& customerId,
                     const std::string& startDate, const std::string& endDate) {
        Vehicle* v = findVehicle(vehicleId);
        if (!v) { std::cerr << "Error: Vehicle not found.\n"; return; }
        if (v->getAvailabilityStatus() != "Available") { std::cerr << "Error: Vehicle is not available.\n"; return; }
        if (!findCustomer(customerId)) { std::cerr << "Error: Customer not found.\n"; return; }
        int days = calculateDays(startDate, endDate);
        double cost = v->calculateRentalCost(days);
        std::string rid = "R" + std::to_string(rentals.size() + 1);
        rentals.emplace_back(rid, customerId, vehicleId, startDate, endDate, cost);
        v->setAvailabilityStatus("Rented");
        std::cout << "\nVehicle rented successfully!\n"
                  << "Rental ID: " << rid << "\nDays: " << days
                  << "\nTotal Cost: $" << std::fixed << std::setprecision(2) << cost << "\n\n";
    }

    void returnVehicle(const std::string& vehicleId) {
        Vehicle* v = findVehicle(vehicleId);
        if (!v) { std::cerr << "Error: Vehicle not found.\n"; return; }
        Rental* r = findRentalByVehicle(vehicleId);
        if (!r) { std::cerr << "Error: No active rental found for this vehicle.\n"; return; }
        v->setAvailabilityStatus("Available");
        std::cout << "\nVehicle returned successfully!\n"
                  << "Rental ID: " << r->getRentalId()
                  << "\nTotal Cost: $" << std::fixed << std::setprecision(2) << r->getTotalCost() << "\n\n";
    }

    void generateRentalReport() const {
        if (rentals.empty()) { std::cout << "No rentals to report.\n"; return; }
        double totalRevenue = 0;
        int carCount = 0, bikeCount = 0, truckCount = 0;
        std::cout << "\n" << std::string(130, '=') << "\n"
                  << std::string(40, ' ') << "RENTAL REPORT\n"
                  << std::string(130, '=') << "\n\n";
        std::cout << std::left << std::setw(10) << "Rental ID" << std::setw(12) << "Customer ID"
                  << std::setw(10) << "Vehicle ID" << std::setw(15) << "Start Date"
                  << std::setw(15) << "End Date" << std::setw(15) << "Total Cost\n";
        std::cout << std::string(130, '-') << "\n";
        for (const auto& r : rentals) {
            totalRevenue += r.getTotalCost();
            Vehicle* v = findVehicle(r.getVehicleId());
            if (v) {
                std::string t = v->getType();
                if (t == "Car") carCount++;
                else if (t == "Bike") bikeCount++;
                else if (t == "Truck") truckCount++;
            }
            std::cout << std::left << std::setw(10) << r.getRentalId()
                      << std::setw(12) << r.getCustomerId()
                      << std::setw(10) << r.getVehicleId()
                      << std::setw(15) << r.getStartDate()
                      << std::setw(15) << r.getEndDate()
                      << std::setw(15) << std::fixed << std::setprecision(2) << r.getTotalCost() << "\n";
        }
        std::cout << std::string(130, '=') << "\n\nSummary:\n"
                  << "  Total Rentals: " << rentals.size() << "\n"
                  << "  Total Revenue: $" << std::fixed << std::setprecision(2) << totalRevenue << "\n"
                  << "  Cars Rented: " << carCount << "\n"
                  << "  Bikes Rented: " << bikeCount << "\n"
                  << "  Trucks Rented: " << truckCount << "\n\n";
    }

    void saveData(FileManager& fm) const {
        fm.saveVehicles(vehicles);
        fm.saveCustomers(customers);
        fm.saveRentals(rentals);
    }

    Vehicle* findVehicle(const std::string& id) const {
        for (auto& v : vehicles) if (v->getVehicleId() == id) return v;
        return nullptr;
    }
    Customer* findCustomer(const std::string& id) const {
        for (auto& c : customers) if (c.getCustomerId() == id) return const_cast<Customer*>(&c);
        return nullptr;
    }
    Rental* findRentalByVehicle(const std::string& vid) const {
        for (auto& r : rentals) if (r.getVehicleId() == vid && r.getEndDate() == "") return const_cast<Rental*>(&r);
        return nullptr;
    }
};

// ========================== Helpers ==========================
std::string getInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void displayMenu() {
    std::cout << "\n========== MAIN MENU ==========\n"
              << "1. Add Vehicle\n2. View All Vehicles\n3. Rent a Vehicle\n"
              << "4. Return a Vehicle\n5. Add Customer\n6. View All Customers\n"
              << "7. Generate Rental Report\n8. Save & Exit\n"
              << "===============================\n";
}

void pauseScreen() {
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}

// ========================== Main ==========================
int main() {
    FileManager fm("data/vehicles.csv", "data/customers.csv", "data/rentals.csv");
    RentalManager rm;
    rm.loadData(fm);
    int choice = 0;
    std::cout << "\n============================================\n"
              << "   VEHICLE RENTAL SYSTEM - CONSOLE BACKEND\n"
              << "============================================\n\n";
    while (true) {
        displayMenu();
        std::cout << "Enter your choice (1-8): ";
        std::cin >> choice;
        std::cin.ignore();
        switch (choice) {
        case 1: {
            std::cout << "\n--- Add Vehicle ---\n";
            std::string id = getInput("Enter Vehicle ID: ");
            std::string type = getInput("Enter Vehicle Type (Car/Bike/Truck): ");
            std::string brand = getInput("Enter Brand: ");
            std::string model = getInput("Enter Model: ");
            std::cout << "Enter Rent Per Day: ";
            double rent; std::cin >> rent; std::cin.ignore();
            Vehicle* v = nullptr;
            if (type == "Car") v = new Car(id, brand, model, rent, "Available");
            else if (type == "Bike") v = new Bike(id, brand, model, rent, "Available");
            else if (type == "Truck") v = new Truck(id, brand, model, rent, "Available");
            else { std::cerr << "Invalid vehicle type.\n"; break; }
            rm.addVehicle(v);
            std::cout << "Vehicle added successfully!\n";
            pauseScreen(); break;
        }
        case 2: std::cout << "\n--- All Vehicles ---\n"; rm.displayAllVehicles(); pauseScreen(); break;
        case 3: {
            std::cout << "\n--- Rent a Vehicle ---\n";
            std::string vid = getInput("Enter Vehicle ID: ");
            std::string cid = getInput("Enter Customer ID: ");
            std::string start = getInput("Enter Start Date (YYYY-MM-DD): ");
            std::string end = getInput("Enter End Date (YYYY-MM-DD): ");
            rm.rentVehicle(vid, cid, start, end);
            pauseScreen(); break;
        }
        case 4: {
            std::cout << "\n--- Return a Vehicle ---\n";
            rm.returnVehicle(getInput("Enter Vehicle ID: "));
            pauseScreen(); break;
        }
        case 5: {
            std::cout << "\n--- Add Customer ---\n";
            std::string cid = getInput("Enter Customer ID: ");
            std::string name = getInput("Enter Name: ");
            std::string phone = getInput("Enter Phone: ");
            std::string email = getInput("Enter Email: ");
            rm.addCustomer(Customer(cid, name, phone, email));
            std::cout << "Customer added successfully!\n";
            pauseScreen(); break;
        }
        case 6: std::cout << "\n--- All Customers ---\n"; rm.displayAllCustomers(); pauseScreen(); break;
        case 7: std::cout << "\n"; rm.generateRentalReport(); pauseScreen(); break;
        case 8:
            std::cout << "\nSaving data...\n"; rm.saveData(fm);
            std::cout << "Data saved successfully!\nThank you for using Vehicle Rental System. Goodbye!\n";
            return 0;
        default: std::cerr << "Invalid choice. Please try again.\n"; pauseScreen();
        }
    }
    return 0;
}
