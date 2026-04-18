# Vehicle Rental System - Project Report

## 1. Introduction

The Vehicle Rental System is a comprehensive software solution designed to manage vehicle rental operations efficiently. It combines a powerful C++ console backend with a modern Flask web dashboard, enabling both operational staff to manage rentals and administrators to monitor fleet analytics.

**What the system does:**
- **Backend**: Manages vehicles (add/view), customers, rental transactions with polymorphic cost calculations
- **Frontend**: Provides a responsive web dashboard with real-time statistics, searchable data tables, and visual analytics
- **Data Layer**: Shares information through CSV files, allowing seamless integration between components

**Why it matters:**
The system demonstrates how modern object-oriented principles (inheritance, polymorphism, encapsulation) combine with practical web technologies to solve real-world business problems. It's a complete learning project showcasing professional software architecture patterns.

---

## 2. Problem Statement

Traditional vehicle rental systems often suffer from:
- **Fragmented Architecture**: Backend and frontend tightly coupled or using complex database queries
- **Inflexible Pricing**: Simple fixed rates don't account for vehicle type variations (economy vs. luxury)
- **Poor Data Management**: Manually updating customer and rental records
- **Limited Insights**: No visibility into fleet utilization or financial metrics

This project addresses these gaps by:
- **Modular Design**: Independent C++ backend and Flask frontend communicating only through CSV
- **Polymorphic Pricing**: Different vehicle types (Car, Bike, Truck) have distinct rental cost calculations
- **Automated Persistence**: CSV-based storage ensures data consistency across components
- **Real-time Analytics**: Dashboard provides instant visibility into fleet status and revenue

---

## 3. OOP Concepts Used

### 3.1 Polymorphism (Method Overriding)

**Vehicle Hierarchy Example:**

```cpp
// Abstract base class with pure virtual method
class Vehicle {
    virtual double calculateRentalCost(int days) const = 0;
};

// Concrete implementations
class Car : public Vehicle {
    double calculateRentalCost(int days) const override {
        return getRentPerDay() * days;  // No discount
    }
};

class Bike : public Vehicle {
    double calculateRentalCost(int days) const override {
        return 0.8 * getRentPerDay() * days;  // 20% discount
    }
};

class Truck : public Vehicle {
    double calculateRentalCost(int days) const override {
        return 1.3 * getRentPerDay() * days;  // 30% surcharge
    }
};

// Usage - Polymorphic behavior
Vehicle* vehicle = new Car("V001", "Toyota", "Camry", 50, "Available");
double cost = vehicle->calculateRentalCost(3);  // Calls Car::calculateRentalCost()
// Returns: 50 * 3 = $150.00
```

**Why Polymorphism:**
- Each vehicle type has different pricing logic
- Code using Vehicle pointers doesn't need to know the actual type
- New vehicle types can be added without modifying existing code

### 3.2 Encapsulation (Data Hiding)

**Customer Class Example:**

```cpp
class Customer {
private:
    std::string customerId;  // PRIVATE - hidden from outside
    std::string name;
    std::string phone;
    std::string email;

public:
    // PUBLIC getters - controlled read access
    std::string getCustomerId() const { return customerId; }
    std::string getName() const { return name; }
    
    // PUBLIC setters - controlled write access with validation opportunity
    void setName(const std::string& nm) { name = nm; }
};

// Usage
Customer c("C001", "John Smith", "555-1001", "john@email.com");
std::cout << c.getName();           // OK - uses getter
// c.name = "Jane Smith";            // COMPILE ERROR - private field
c.setName("Jane Smith");             // OK - uses setter
```

**Why Encapsulation:**
- Prevents invalid data corruption
- Allows future validation logic in setters
- Implements data hiding principle (external code shouldn't know internal structure)

### 3.3 Inheritance (IS-A Relationship)

**Vehicle Type Hierarchy:**

```cpp
// Base class defines interface and common behavior
class Vehicle {
protected:
    std::string vehicleId;
    std::string brand;
    std::string model;
    double rentPerDay;
    std::string availabilityStatus;

public:
    Vehicle(...) { ... }
    virtual double calculateRentalCost(int days) const = 0;
};

// Derived classes inherit from Vehicle
class Car : public Vehicle {
    // Inherits: getters, setters, all fields
    // Implements: calculateRentalCost() specific to cars
};

class Truck : public Vehicle {
    // Inherits: getters, setters, all fields
    // Implements: calculateRentalCost() specific to trucks
};
```

**Why Inheritance:**
- Defines contract all vehicle types must follow
- Reduces code duplication (common fields, getters/setters)
- Creates type hierarchy for polymorphic behavior

### 3.4 Composition (Has-A Relationship)

**RentalManager composing Customers:**

```cpp
class RentalManager {
private:
    std::vector<Customer> customers;  // COMPOSITION - owns customers
    
public:
    void addCustomer(const Customer& customer) {
        customers.push_back(customer);
    }
    
    ~RentalManager() {
        customers.clear();  // Customers deleted when manager destroyed
    }
};
```

**Why Composition:**
- RentalManager is responsible for Customer lifecycle
- When manager is destroyed, customers are automatically cleaned up
- Clear ownership semantics

### 3.5 Aggregation (Weak Ownership)

**RentalManager aggregating Vehicles:**

```cpp
class RentalManager {
private:
    std::vector<Vehicle*> vehicles;  // AGGREGATION - manages but doesn't own
    
public:
    void addVehicle(Vehicle* vehicle) {
        vehicles.push_back(vehicle);  // Takes existing vehicle pointer
    }
    
    ~RentalManager() {
        for (auto v : vehicles) {
            delete v;  // Explicit cleanup needed
        }
    }
};

// Usage
auto* car = new Car(...);
manager.addVehicle(car);  // Manager groups vehicle but may not own its full lifecycle
```

**Why Aggregation:**
- Vehicles might outlive the manager in some scenarios
- Manager groups related objects without strict ownership
- More flexible lifecycle management

---

## 4. System Design

### Data Flow Architecture

```
┌──────────────────────────────────────────────────────────────────┐
│                    SHARED DATA LAYER (CSV FILES)                 │
│  ┌────────────────┐  ┌──────────────────┐  ┌──────────────────┐ │
│  │ vehicles.csv   │  │ customers.csv    │  │ rentals.csv      │ │
│  │ (8 columns)    │  │ (4 columns)      │  │ (6 columns)      │ │
│  └────────────────┘  └──────────────────┘  └──────────────────┘ │
└──────────┬───────────────────────────────────────────────────┬───┘
           │                                                   │
    ┌──────▼──────────────────┐                ┌──────────────▼─────┐
    │   C++ BACKEND           │                │   FLASK DASHBOARD   │
    │ ┌────────────────────┐  │                │ ┌────────────────┐  │
    │ │ RentalManager      │  │ ◇──────┐       │ │ app.py         │  │
    │ │ + vehicles (list)  │  │        │       │ │ + load_*()     │  │
    │ │ + customers (list) │  │        └──────────► + render_*()  │  │
    │ │ + rentals (list)   │  │                │ │ + api routes   │  │
    │ │                    │  │                │ └────────────────┘  │
    │ └────────────────────┘  │                │ ┌────────────────┐  │
    │ ┌────────────────────┐  │                │ │ Templates      │  │
    │ │ FileManager        │  │ ◇──────┐       │ │ + base.html    │  │
    │ │ + loadVehicles()   │  │        │       │ │ + home.html    │  │
    │ │ + loadCustomers()  │  │        │       │ │ + data_view    │  │
    │ │ + loadRentals()    │  │        │       │ │ + statistics   │  │
    │ │ + save*()          │  │        └──────────► (display data)│  │
    │ └────────────────────┘  │                │ └────────────────┘  │
    │ ┌────────────────────┐  │                │ ┌────────────────┐  │
    │ │ Console Menu       │  │                │ │ Static Assets  │  │
    │ │ (8 options)        │  │                │ │ + style.css    │  │
    │ └────────────────────┘  │                │ │ + charts.js    │  │
    └──────────────────────────┘                └────────────────────┘
```

### Component Responsibilities

**C++ Backend:**
- Read/write CSV files on startup and shutdown
- Validate vehicle availability before rentals
- Calculate polymorphic rental costs
- Maintain in-memory collections during runtime
- Provide user menu interface

**Flask Dashboard:**
- Periodically read CSV files for latest data
- Serve responsive HTML templates
- Provide JSON endpoints for chart data
- Implement search and filtering on tables
- Display real-time statistics and KPIs

**CSV Data Layer:**
- Single source of truth for persistent data
- No locks/transactions (simple file-based)
- Formats: standard comma-separated values
- Readable by both C++ (fstream) and Python (csv module)

---

## 5. UML Diagram Explanation

### Inheritance Structure
- **Vehicle** (Abstract): Base class defining the rental cost calculation interface
- **Car, Bike, Truck** (Concrete): Each implements calculateRentalCost() with business rules
- Arrow direction: Derived → Base (IS-A relationship)

### Composition: RentalManager ◆── Customer
- RentalManager owns Customers completely
- Customers created and destroyed with the manager
- Strong lifecycle coupling

### Aggregation: RentalManager ◇── Vehicle and RentalManager ◇── Rental
- Manager groups these objects but doesn't strictly own them
- Vehicles can exist independently
- Rentals created dynamically during transactions

### Data Flow
- C++ backend polls CSV, modifies in memory, saves back
- Flask backend polls CSV independently, no direct backend access
- CSV is the message queue between components

---

## 6. Dashboard Explanation

### Home Page (`/`)
- **Stat Cards**: Total Vehicles (blue), Total Customers (green), Total Rentals (teal), Available (yellow), Rented (red)
- **System Status**: Fleet size, utilization percentage, registered customers, transaction count
- **Quick Links**: Navigation to Data View and Statistics pages

### Data View Page (`/data`)
- **Searchable Tables**: Three tabs for Vehicles, Customers, Rentals
- **Client-side Filtering**: Real-time search as user types in search box
- **Status Badges**: Visual indicators for vehicle availability (green=Available, red=Rented)
- **Empty States**: Graceful messages when no data exists

### Statistics Page (`/statistics`)
- **KPI Cards**: Total Revenue, Most Rented Type, Total Customers, Total Rentals
- **Vehicle Availability Chart** (Pie): Doughnut chart showing Available vs. Rented ratio
- **Vehicle Type Distribution** (Bar): Horizontal bar chart counting Cars, Bikes, Trucks
- **System Insights**: Fleet efficiency %, average rental cost, system health status
- **Auto-refresh**: Charts update every 10 seconds with fresh CSV data

---

## 7. Conclusion

### Lessons Learned

1. **OOP is Powerful**: Polymorphism enabled elegant vehicle type handling without complex if-else chains
2. **Separation of Concerns**: Independent backend and frontend components made testing easier
3. **CSV as Transfer Format**: Simple, human-readable, accessible from multiple languages
4. **Encapsulation Prevents Bugs**: Private fields with public getters/setters caught data integrity issues early
5. **Test with Real Data**: Sample CSV files with realistic values exposed edge cases

### Improvements for Production

1. **Database**: Replace CSV with relational database (PostgreSQL) for:
   - ACID transactions across multiple reads/writes
   - Concurrent access without file locking issues
   - Complex queries (e.g., "rentals by customer this month")

2. **API**: Instead of CSV, implement REST API so backend and frontend communicate directly with JSON

3. **Authentication**: Add user login, admin roles, customer accounts

4. **Error Handling**: More graceful failures, logging system, monitoring dashboard

5. **Performance**: 
   - Index CSV data for faster lookups
   - Cache frequently accessed data
   - Implement pagination for large datasets

6. **Validation**: Add business rule validation (dates, email format, phone numbers)

7. **Deployment**: Docker containers for easy deployment, GitHub Actions for CI/CD

### Project Completion Summary

✓ C++ backend compiles without warnings (g++ -std=c++17)
✓ 8 OOP classes implemented with proper separation
✓ 3 derived Vehicle types with polymorphic cost calculation
✓ 8-option console menu fully functional
✓ Automatic CSV loading/saving
✓ Flask dashboard with 3 pages working
✓ Searchable/sortable data tables
✓ Real-time statistics with Chart.js visualization
✓ 100% of specification requirements met

---

**Project Repository**: c:\Users\prady\OneDrive\Desktop\oops\VehicleRentalSystem\
**Build Date**: April 15, 2026
**Status**: Complete and Tested
