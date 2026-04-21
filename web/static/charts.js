// Vehicle Rental System - Client-side Chart and Utility Functions

// Configuration for chart styling
const chartColors = {
    primary: '#0d6efd',
    secondary: '#6c757d',
    success: '#198754',
    info: '#0dcaf0',
    warning: '#ffc107',
    danger: '#dc3545',
};

const Chart_Config = {
    responsive: true,
    maintainAspectRatio: false,
    plugins: {
        legend: {
            display: true,
            labels: {
                padding: 15,
                font: {
                    size: 12,
                    weight: '500'
                },
                usePointStyle: true
            }
        }
    }
};

// Utility function to format currency
function formatCurrency(value) {
    return new Intl.NumberFormat('en-US', {
        style: 'currency',
        currency: 'USD',
        minimumFractionDigits: 2,
        maximumFractionDigits: 2
    }).format(value);
}

// Utility function to format numbers with commas
function formatNumber(value) {
    return new Intl.NumberFormat('en-US').format(value);
}

// Utility function to format dates
function formatDate(dateString) {
    if (!dateString) return '-';
    const date = new Date(dateString);
    return new Intl.DateTimeFormat('en-US', {
        year: 'numeric',
        month: 'short',
        day: 'numeric'
    }).format(date);
}

// Utility function to calculate days between dates
function calculateDaysDifference(startDate, endDate) {
    const start = new Date(startDate);
    const end = new Date(endDate);
    const diffTime = Math.abs(end - start);
    const diffDays = Math.ceil(diffTime / (1000 * 60 * 60 * 24));
    return diffDays;
}

// Function to display loading spinner
function showLoadingSpinner(elementId) {
    const element = document.getElementById(elementId);
    if (element) {
        element.innerHTML = `
            <div class="spinner-border text-primary" role="status">
                <span class="visually-hidden">Loading...</span>
            </div>
        `;
    }
}

// Function to show error message
function showErrorMessage(elementId, message) {
    const element = document.getElementById(elementId);
    if (element) {
        element.innerHTML = `
            <div class="alert alert-danger" role="alert">
                <i class="bi bi-exclamation-triangle"></i> ${message}
            </div>
        `;
    }
}

// Function to show success message
function showSuccessMessage(message, duration = 3000) {
    const alertDiv = document.createElement('div');
    alertDiv.className = 'alert alert-success alert-dismissible fade show position-fixed';
    alertDiv.style.top = '20px';
    alertDiv.style.right = '20px';
    alertDiv.style.zIndex = '9999';
    alertDiv.innerHTML = `
        <i class="bi bi-check-circle"></i> ${message}
        <button type="button" class="btn-close" data-bs-dismiss="alert"></button>
    `;
    document.body.appendChild(alertDiv);

    setTimeout(() => {
        alertDiv.remove();
    }, duration);
}

// Enhanced table search function with debounce
function createTableSearch(inputId, tableId) {
    const input = document.getElementById(inputId);
    const table = document.getElementById(tableId);

    if (!input || !table) return;

    let debounceTimer;
    input.addEventListener('keyup', function() {
        clearTimeout(debounceTimer);
        debounceTimer = setTimeout(() => {
            filterTable(tableId, this.value);
        }, 300);
    });
}

// Filter table rows based on search text
function filterTable(tableId, searchText) {
    const table = document.getElementById(tableId);
    if (!table) return;

    const rows = table.getElementsByTagName('tbody')[0].getElementsByTagName('tr');
    const lowerSearchText = searchText.toLowerCase();
    let visibleCount = 0;

    for (let row of rows) {
        const text = row.textContent.toLowerCase();
        const shouldShow = text.includes(lowerSearchText);
        row.style.display = shouldShow ? '' : 'none';
        if (shouldShow) visibleCount++;
    }

    // Show message if no results
    const tbody = table.getElementsByTagName('tbody')[0];
    let noResultsRow = tbody.querySelector('.no-results');
    
    if (visibleCount === 0 && searchText) {
        if (!noResultsRow) {
            noResultsRow = document.createElement('tr');
            noResultsRow.className = 'no-results';
            noResultsRow.innerHTML = '<td colspan="100%" class="text-center text-muted py-4">No results found</td>';
            tbody.appendChild(noResultsRow);
        }
        noResultsRow.style.display = '';
    } else if (noResultsRow) {
        noResultsRow.style.display = 'none';
    }
}

// Sort table by column
function sortTable(tableId, columnIndex, isNumeric = false) {
    const table = document.getElementById(tableId);
    if (!table) return;

    const tbody = table.getElementsByTagName('tbody')[0];
    const rows = Array.from(tbody.getElementsByTagName('tr'));

    rows.sort((rowA, rowB) => {
        const cellA = rowA.getElementsByTagName('td')[columnIndex];
        const cellB = rowB.getElementsByTagName('td')[columnIndex];

        let valueA = cellA.textContent.trim();
        let valueB = cellB.textContent.trim();

        if (isNumeric) {
            valueA = parseFloat(valueA.replace(/[^0-9.-]/g, '')) || 0;
            valueB = parseFloat(valueB.replace(/[^0-9.-]/g, '')) || 0;
        }

        if (valueA > valueB) return 1;
        if (valueA < valueB) return -1;
        return 0;
    });

    rows.forEach(row => tbody.appendChild(row));
}

// Export table to CSV
function exportTableToCSV(tableId, filename = 'export.csv') {
    const table = document.getElementById(tableId);
    if (!table) return;

    let csv = [];
    const rows = table.querySelectorAll('tr');

    for (let row of rows) {
        let csvRow = [];
        const cells = row.querySelectorAll('td, th');

        for (let cell of cells) {
            csvRow.push('"' + cell.textContent.trim().replace(/"/g, '""') + '"');
        }

        csv.push(csvRow.join(','));
    }

    // Create download link
    const csvContent = 'data:text/csv;charset=utf-8,' + csv.join('\n');
    const link = document.createElement('a');
    link.setAttribute('href', encodeURI(csvContent));
    link.setAttribute('download', filename);
    link.click();
}

// Initialize tooltips (Bootstrap 5)
function initializeTooltips() {
    const tooltipTriggerList = [].slice.call(document.querySelectorAll('[data-bs-toggle="tooltip"]'));
    tooltipTriggerList.map((tooltipTriggerEl) => new bootstrap.Tooltip(tooltipTriggerEl));
}

// Initialize popovers (Bootstrap 5)
function initializePopovers() {
    const popoverTriggerList = [].slice.call(document.querySelectorAll('[data-bs-toggle="popover"]'));
    popoverTriggerList.map((popoverTriggerEl) => new bootstrap.Popover(popoverTriggerEl));
}

// Document ready function
document.addEventListener('DOMContentLoaded', function() {
    initializeTooltips();
    initializePopovers();
});
