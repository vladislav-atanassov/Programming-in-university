DROP DATABASE IF EXISTS Agency_Property;
CREATE DATABASE Agency_Property;
USE Agency_Property;

CREATE TABLE customers (
	id INT AUTO_INCREMENT PRIMARY KEY,
	email VARCHAR(30) UNIQUE NOT NULL,
    phone VARCHAR(20) NOT NULL,
    name VARCHAR(50) NOT NULL
);

CREATE TABLE types (
	id INT AUTO_INCREMENT PRIMARY KEY,
    typeName VARCHAR(50) NOT NULL
);

CREATE TABLE properties (
	id INT 	AUTO_INCREMENT PRIMARY KEY,
    area VARCHAR(100) NOT NULL,
    price DECIMAL(20, 2) NOT NULL,
    location VARCHAR(100) NOT NULL,
    description VARCHAR(100) NOT NULL,
    customer_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (customer_id) REFERENCES Customers(id),
    type_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (type_id) REFERENCES Types(id)
); 

CREATE TABLE actions (
	id INT AUTO_INCREMENT PRIMARY KEY,
    actionType VARCHAR(50) NOT NULL
);

CREATE TABLE ads (
	id INT AUTO_INCREMENT PRIMARY KEY,
    publicationDate DATETIME NOT NULL,
    isActual BOOLEAN NOT NULL,
    property_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (property_id) REFERENCES Properties(id),
    action_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (action_id) REFERENCES Actions(id)
);

CREATE TABLE employees (
	id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(50) NOT NULL,
    position VARCHAR(50) NOT NULL
);

CREATE TABLE deals (
	id INT AUTO_INCREMENT PRIMARY KEY,
    dealDate DATETIME NOT NULL,
    paymentType VARCHAR(100) NOT NULL,
    ad_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (ad_id) REFERENCES Ads(id),
    employee_id INT NULL DEFAULT NULL,
    CONSTRAINT FOREIGN KEY (employee_id) REFERENCES Employees(id)
);

CREATE TABLE salaryPayments (
    id INT AUTO_INCREMENT PRIMARY KEY,
    salaryAmount DECIMAL(20, 2) NOT NULL,
    monthlyBonuses DECIMAL(20, 2) NOT NULL DEFAULT 0,
    yearOfPayment YEAR NOT NULL,
    monthOfPayment INT NOT NULL,
    dateOfPayment DATETIME NOT NULL,
    employee_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (employee_id) REFERENCES Employees(id),
    CONSTRAINT UNIQUE (employee_id, yearOfPayment, monthOfPayment, dateOfPayment)
);

INSERT INTO customers (email, phone, name) VALUES 
('john.doe@example.com', '555-1234', 'John Doe'),
('jane.doe@example.com', '555-5678', 'Jane Doe'),
('alice.jones@example.com', '555-9101', 'Alice Jones'),
('bob.smith@example.com', '555-1122', 'Bob Smith'),
('carol.white@example.com', '555-3344', 'Carol White'),
('dave.brown@example.com', '555-5566', 'Dave Brown'),
('emma.green@example.com', '555-7788', 'Emma Green'),
('frank.black@example.com', '555-9900', 'Frank Black'),
('grace.hall@example.com', '555-2233', 'Grace Hall'),
('henry.adams@example.com', '555-4466', 'Henry Adams');

INSERT INTO types (typeName) VALUES 
('парцел'),
('къща'),
('сграда'),
('апартамент'),
('мезонет');

INSERT INTO properties (area, price, location, description, customer_id, type_id) VALUES 
('120 sqm', 15000.00, 'Downtown', 'Fully renovated apartment', 1, 2),
('99 sqm', 300000.00, 'Suburban Area', 'Spacious house with garden', 2, 2),
('450 sqm', 50000.00, 'Lakeside', 'Exclusive villa with private dock', 3, 2),
('80 sqm', 1000.00, 'Near park', 'Cozy cottage perfect for weekends', 4, 2),
('200 sqm', 2000.00, 'Central', 'Modern duplex with city views', 5, 2),
('150 sqm', 180000.00, 'Residential area', 'Townhouse with community pool', 6, 2),
('100 sqm', 200000.00, 'Downtown', 'High-rise condo', 7, 5),
('500 sqm', 150000.00, 'Rural', 'Land ready for building', 8, 4),
('600 sqm', 350000.00, 'Outskirts', 'Farm with facilities', 9, 3),
('50 sqm', 90000.00, 'City center', 'Compact studio apartment', 10, 3);

INSERT INTO actions (actionType) VALUES 
('купува'),
('продава'),
('отдавна под наем');

INSERT INTO ads (publicationDate, isActual, property_id, action_id) VALUES 
('2024-05-07 10:00:00', TRUE, 1, 1),
('2023-02-07 10:00:00', TRUE, 2, 1),
('2024-03-06 10:00:00', TRUE, 3, 1),
('2023-05-05 10:00:00', FALSE, 4, 2),
('2024-05-05 10:00:00', FALSE, 5, 3),
('2023-06-03 10:00:00', TRUE, 6, 3),
('2024-05-02 10:00:00', TRUE, 7, 3),
('2023-05-01 10:00:00', FALSE, 8, 3),
('2024-05-01 10:00:00', TRUE, 9, 2),
('2023-10-01 10:00:00', TRUE, 10, 3);

INSERT INTO employees (name, position) VALUES 
('Tom Hardy', 'Agent'),
('Natalie Portman', 'Broker'),
('Chris Evans', 'Manager'),
('Emma Watson', 'Receptionist'),
('Robert Downey', 'Finance Officer'),
('Scarlett Johansson', 'Marketing Manager'),
('Chris Hemsworth', 'IT Support'),
('Mark Ruffalo', 'Maintenance'),
('Jeremy Renner', 'Director'),
('Elizabeth Olsen', 'HR Manager');

INSERT INTO deals (dealDate, paymentType, ad_id, employee_id) VALUES 
('2024-05-15 12:00:00', 'Cash', 1, 1),
('2024-05-15 12:00:00', 'Credit', 2, 3),
('2024-05-15 12:00:00', 'Debit', 4, 3),
('2024-05-15 12:00:00', 'Cash', 5, 2),
('2024-05-15 12:00:00', 'Credit', 6, 2),
('2024-05-15 12:00:00', 'Debit', 7, 3),
('2024-07-15 12:00:00', 'Cash', 1, 3),
('2024-08-15 12:00:00', 'Credit', 8, 3),
('2021-09-15 12:00:00', 'Debit', 9, 3),
('2024-10-15 12:00:00', 'Cash', 10, 2);

INSERT INTO salaryPayments (salaryAmount, yearOfPayment, monthOfPayment, dateOfPayment, employee_id) VALUES 
(3000.00, 2024, 5, '2024-05-31', 1),
(3200.00, 2024, 5, '2024-05-31', 2),
(2800.00, 2024, 5, '2024-05-31', 3),
(2500.00, 2024, 5, '2024-05-31', 4),
(3500.00, 2024, 5, '2024-05-31', 5),
(2700.00, 2024, 5, '2024-05-31', 6),
(2900.00, 2024, 5, '2024-05-31', 7),
(2600.00, 2024, 5, '2024-05-31', 8),
(3800.00, 2024, 5, '2024-05-31', 9),
(3100.00, 2024, 5, '2024-05-31', 10);
