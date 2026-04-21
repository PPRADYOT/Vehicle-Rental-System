// Vehicle Rental System - Node.js Web Server
// Simple HTTP server that serves the dashboard and reads CSV data

const http = require('http');
const fs = require('fs');
const path = require('path');
const { parse } = require('url');

// Configuration
const PORT = 5000;
const CSV_PATH = process.env.VEHICLE_RENTAL_CSV_PATH || path.join(__dirname, '..', 'cpp', 'data');

// Parse CSV file
function parseCSV(filePath) {
  try {
    const data = fs.readFileSync(filePath, 'utf-8');
    const lines = data.split('\n').map(line => line.replace('\r', '')).filter(line => line.trim());
    if (lines.length === 0) return [];
    
    const headers = lines[0].split(',');
    const rows = lines.slice(1).map(line => {
      const values = line.split(',');
      const obj = {};
      headers.forEach((header, index) => {
        obj[header] = values[index] || '';
      });
      return obj;
    });
    return rows;
  } catch (err) {
    console.error(`Error reading CSV: ${err.message}`);
    return [];
  }
}

// Write data to CSV file
function writeToCSV(filePath, headers, data) {
  try {
    const csvContent = [headers.join(','), ...data.map(row => headers.map(h => row[h] || '').join(','))].join('\n');
    fs.writeFileSync(filePath, csvContent, 'utf-8');
    return true;
  } catch (err) {
    console.error(`Error writing CSV: ${err.message}`);
    return false;
  }
}

// Parse request body
function parseBody(req, callback) {
  let body = '';
  req.on('data', chunk => {
    body += chunk.toString();
  });
  req.on('end', () => {
    try {
      callback(JSON.parse(body));
    } catch (err) {
      callback(null);
    }
  });
}

// Load all data
function loadAllData() {
  return {
    vehicles: parseCSV(path.join(CSV_PATH, 'vehicles.csv')),
    customers: parseCSV(path.join(CSV_PATH, 'customers.csv')),
    rentals: parseCSV(path.join(CSV_PATH, 'rentals.csv'))
  };
}

// Serve static files
function serveFile(filePath, contentType, res) {
  fs.readFile(filePath, (err, data) => {
    if (err) {
      res.writeHead(404, { 'Content-Type': 'text/plain' });
      res.end('404 - File Not Found');
      return;
    }
    res.writeHead(200, { 'Content-Type': contentType });
    res.end(data);
  });
}

// Create HTTP server
const server = http.createServer((req, res) => {
  const parsedUrl = parse(req.url, true);
  const pathname = parsedUrl.pathname;

  // Enable CORS
  res.setHeader('Access-Control-Allow-Origin', '*');
  res.setHeader('Access-Control-Allow-Methods', 'GET, POST, OPTIONS, PUT, DELETE');
  res.setHeader('Access-Control-Allow-Headers', 'Content-Type');

  if (req.method === 'OPTIONS') {
    res.writeHead(200);
    res.end();
    return;
  }

  // Routes
  if (pathname === '/' || pathname === '/home' || pathname === '/data' || pathname === '/statistics') {
    serveFile(path.join(__dirname, 'index.html'), 'text/html', res);
  } else if (pathname === '/admin') {
    serveFile(path.join(__dirname, 'admin.html'), 'text/html', res);
  } else if (pathname === '/static/style.css') {
    serveFile(path.join(__dirname, 'static', 'style.css'), 'text/css', res);
  } else if (pathname === '/static/charts.js') {
    serveFile(path.join(__dirname, 'static', 'charts.js'), 'application/javascript', res);
  }
  // POST: Add Vehicle
  else if (pathname === '/api/vehicles' && req.method === 'POST') {
    parseBody(req, (data) => {
      if (!data) {
        res.writeHead(400, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Invalid request' }));
        return;
      }
      const vehicles = parseCSV(path.join(CSV_PATH, 'vehicles.csv'));
      vehicles.push(data);
      const headers = ['id', 'type', 'brand', 'model', 'rentPerDay', 'status'];
      const success = writeToCSV(path.join(CSV_PATH, 'vehicles.csv'), headers, vehicles);
      res.writeHead(success ? 201 : 500, { 'Content-Type': 'application/json' });
      res.end(JSON.stringify({ success, message: success ? 'Vehicle added' : 'Error adding vehicle' }));
    });
  }
  // POST: Add Customer
  else if (pathname === '/api/customers' && req.method === 'POST') {
    parseBody(req, (data) => {
      if (!data) {
        res.writeHead(400, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Invalid request' }));
        return;
      }
      const customers = parseCSV(path.join(CSV_PATH, 'customers.csv'));
      customers.push(data);
      const headers = ['id', 'name', 'phone', 'email'];
      const success = writeToCSV(path.join(CSV_PATH, 'customers.csv'), headers, customers);
      res.writeHead(success ? 201 : 500, { 'Content-Type': 'application/json' });
      res.end(JSON.stringify({ success, message: success ? 'Customer added' : 'Error adding customer' }));
    });
  }
  // POST: Add Rental
  else if (pathname === '/api/rentals' && req.method === 'POST') {
    parseBody(req, (data) => {
      if (!data) {
        res.writeHead(400, { 'Content-Type': 'application/json' });
        res.end(JSON.stringify({ error: 'Invalid request' }));
        return;
      }
      const rentals = parseCSV(path.join(CSV_PATH, 'rentals.csv'));
      rentals.push(data);
      const headers = ['rentalId', 'customerId', 'vehicleId', 'startDate', 'endDate', 'totalCost'];
      const success = writeToCSV(path.join(CSV_PATH, 'rentals.csv'), headers, rentals);
      
      // Update vehicle status
      if (data.vehicleId) {
        const vehicles = parseCSV(path.join(CSV_PATH, 'vehicles.csv'));
        const vehicleIdx = vehicles.findIndex(v => v.id === data.vehicleId);
        if (vehicleIdx >= 0) {
          vehicles[vehicleIdx].status = 'Rented';
          writeToCSV(path.join(CSV_PATH, 'vehicles.csv'), ['id', 'type', 'brand', 'model', 'rentPerDay', 'status'], vehicles);
        }
      }
      
      res.writeHead(success ? 201 : 500, { 'Content-Type': 'application/json' });
      res.end(JSON.stringify({ success, message: success ? 'Rental added' : 'Error adding rental' }));
    });
  }
  // GET API Routes
  else if (pathname === '/api/stats') {
    const data = loadAllData();
    const available = data.vehicles.filter(v => v.status === 'Available').length;
    const rented = data.vehicles.filter(v => v.status === 'Rented').length;
    const totalRevenue = data.rentals.reduce((sum, r) => sum + parseFloat(r.totalCost || 0), 0);
    
    const stats = {
      totalVehicles: data.vehicles.length,
      totalCustomers: data.customers.length,
      totalRentals: data.rentals.length,
      availableVehicles: available,
      rentedVehicles: rented,
      totalRevenue: totalRevenue.toFixed(2)
    };
    
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify(stats));
  } 
  else if (pathname === '/api/vehicles') {
    const data = loadAllData();
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify(data.vehicles));
  }
  else if (pathname === '/api/customers') {
    const data = loadAllData();
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify(data.customers));
  }
  else if (pathname === '/api/rentals') {
    const data = loadAllData();
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify(data.rentals));
  }
  else if (pathname === '/api/vehicle-status-chart') {
    const data = loadAllData();
    const available = data.vehicles.filter(v => v.status === 'Available').length;
    const rented = data.vehicles.filter(v => v.status === 'Rented').length;
    
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({
      labels: ['Available', 'Rented'],
      datasets: [{
        data: [available, rented],
        backgroundColor: ['#28a745', '#dc3545']
      }]
    }));
  }
  else if (pathname === '/api/vehicle-type-chart') {
    const data = loadAllData();
    const types = {};
    data.vehicles.forEach(v => {
      const type = v.type || 'Unknown';
      types[type] = (types[type] || 0) + 1;
    });
    
    res.writeHead(200, { 'Content-Type': 'application/json' });
    res.end(JSON.stringify({
      labels: Object.keys(types),
      datasets: [{
        label: 'Count',
        data: Object.values(types),
        backgroundColor: ['#007bff', '#ffc107', '#17a2b8']
      }]
    }));
  }
  else {
    res.writeHead(404, { 'Content-Type': 'text/plain' });
    res.end('404 - Not Found');
  }
});

// Start server
server.listen(PORT, () => {
  console.log(`
╔════════════════════════════════════════════════════════════╗
║   VEHICLE RENTAL SYSTEM - NODE.JS WEB SERVER               ║
╚════════════════════════════════════════════════════════════╝

✅ Server running at: http://localhost:${PORT}

📊 Available Routes:
  ✓ Home:        http://localhost:${PORT}/
  ✓ Data View:   http://localhost:${PORT}/data
  ✓ Statistics:  http://localhost:${PORT}/statistics
  ✓ Admin Panel: http://localhost:${PORT}/admin

📡 API Endpoints:
  ✓ /api/stats               (Dashboard statistics)
  ✓ /api/vehicles            (All vehicles - GET/POST)
  ✓ /api/customers           (All customers - GET/POST)
  ✓ /api/rentals             (All rentals - GET/POST)
  ✓ /api/vehicle-status-chart   (Available vs Rented)
  ✓ /api/vehicle-type-chart     (Vehicle type distribution)

📁 CSV Path: ${CSV_PATH}

🔐 Admin Credentials:
  Username: admin
  Password: password

🛑 Press Ctrl+C to stop the server
  `);
});

server.on('error', (err) => {
  if (err.code === 'EADDRINUSE') {
    console.error(`❌ Port ${PORT} is already in use!`);
    console.error('   Try: netstat -ano | findstr :5000');
  } else {
    console.error(`❌ Server error: ${err.message}`);
  }
  process.exit(1);
});
