import os
import csv
import json
from flask import Flask, render_template, jsonify
from datetime import datetime
from collections import Counter

app = Flask(__name__)

# Configuration - uses environment variable for CSV path or defaults to data directory
CSV_PATH = os.getenv('VEHICLE_RENTAL_CSV_PATH', 'data')

def get_csv_path(filename):
    """Construct full path to CSV file from configured path"""
    return os.path.join(CSV_PATH, filename)

def load_vehicles():
    """Load vehicles from CSV file"""
    vehicles = []
    try:
        with open(get_csv_path('vehicles.csv'), 'r') as f:
            reader = csv.DictReader(f)
            for row in reader:
                if row:
                    vehicles.append(row)
    except FileNotFoundError:
        pass
    return vehicles

def load_customers():
    """Load customers from CSV file"""
    customers = []
    try:
        with open(get_csv_path('customers.csv'), 'r') as f:
            reader = csv.DictReader(f)
            for row in reader:
                if row:
                    customers.append(row)
    except FileNotFoundError:
        pass
    return customers

def load_rentals():
    """Load rentals from CSV file"""
    rentals = []
    try:
        with open(get_csv_path('rentals.csv'), 'r') as f:
            reader = csv.DictReader(f)
            for row in reader:
                if row:
                    rentals.append(row)
    except FileNotFoundError:
        pass
    return rentals

def count_available_vehicles():
    """Count vehicles that are available"""
    vehicles = load_vehicles()
    return sum(1 for v in vehicles if v.get('status') == 'Available')

def count_rented_vehicles():
    """Count vehicles that are currently rented"""
    vehicles = load_vehicles()
    return sum(1 for v in vehicles if v.get('status') == 'Rented')

def get_vehicle_type_counts():
    """Get counts of each vehicle type"""
    vehicles = load_vehicles()
    types = [v.get('type', 'Unknown') for v in vehicles]
    return Counter(types)

def calculate_total_revenue():
    """Calculate total revenue from all rentals"""
    rentals = load_rentals()
    total = sum(float(r.get('totalCost', 0)) for r in rentals if r.get('totalCost'))
    return total

def get_most_rented_type():
    """Get the vehicle type that has been rented the most"""
    rentals = load_rentals()
    if not rentals:
        return "None"
    
    vehicles = load_vehicles()
    vehicle_map = {v.get('id'): v.get('type') for v in vehicles}
    
    rented_types = [vehicle_map.get(r.get('vehicleId'), 'Unknown') for r in rentals]
    type_counts = Counter(rented_types)
    
    if type_counts:
        return type_counts.most_common(1)[0][0]
    return "None"

# Routes

@app.route('/')
def home():
    """Home page with system statistics"""
    vehicles = load_vehicles()
    customers = load_customers()
    rentals = load_rentals()
    
    stats = {
        'total_vehicles': len(vehicles),
        'total_customers': len(customers),
        'total_rentals': len(rentals),
        'available_vehicles': count_available_vehicles(),
        'rented_vehicles': count_rented_vehicles(),
    }
    
    return render_template('home.html', stats=stats)

@app.route('/data')
def data_view():
    """Data view page with tables for vehicles, customers, and rentals"""
    vehicles = load_vehicles()
    customers = load_customers()
    rentals = load_rentals()
    
    return render_template('data_view.html', 
                         vehicles=vehicles,
                         customers=customers,
                         rentals=rentals)

@app.route('/statistics')
def statistics():
    """Statistics page with charts and insights"""
    return render_template('statistics.html')

# API Routes for chart data

@app.route('/api/vehicle-status-chart')
def vehicle_status_chart():
    """JSON endpoint for vehicle availability chart data"""
    available = count_available_vehicles()
    rented = count_rented_vehicles()
    
    return jsonify({
        'labels': ['Available', 'Rented'],
        'data': [available, rented]
    })

@app.route('/api/vehicle-type-chart')
def vehicle_type_chart():
    """JSON endpoint for vehicle type distribution chart data"""
    type_counts = get_vehicle_type_counts()
    
    return jsonify({
        'labels': list(type_counts.keys()),
        'data': list(type_counts.values())
    })

@app.route('/api/statistics-summary')
def statistics_summary():
    """JSON endpoint for statistics summary KPIs"""
    total_revenue = calculate_total_revenue()
    most_rented = get_most_rented_type()
    
    return jsonify({
        'total_revenue': f"${total_revenue:.2f}",
        'most_rented_type': most_rented,
        'total_customers': len(load_customers()),
        'total_rentals': len(load_rentals())
    })

if __name__ == '__main__':
    app.run(debug=True, port=5000)
