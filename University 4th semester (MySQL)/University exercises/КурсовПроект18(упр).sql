DROP DATABASE IF EXISTS catalog_sale_autoparts;
CREATE DATABASE catalog_sale_autoparts;
USE catalog_sale_autoparts;

CREATE TABLE employees (
    id INT PRIMARY KEY AUTO_INCREMENT NOT NULL,
    name VARCHAR(255) NOT NULL
);

CREATE TABLE customers (
    id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
    name VARCHAR(255) NOT NULL,
    email VARCHAR(50) NOT NULL,
    phone VARCHAR(20) NOT NULL,
    promo_code FLOAT NULL
);

CREATE TABLE products (
    id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
    product_type VARCHAR(255) NOT NULL,
    car_brand VARCHAR(255) NOT NULL,
    car_model VARCHAR(255) NOT NULL,
    description TEXT,
    catalog_number VARCHAR(50) NOT NULL,
    price DECIMAL(10, 2) NOT NULL
);

CREATE TABLE purchase_history (
    id INT AUTO_INCREMENT PRIMARY KEY,
    purchase_date DATE NOT NULL,
    purchase_price DECIMAL(10, 2) NOT NULL,
    price DECIMAL(10, 2) NOT NULL,
    discount FLOAT NULL,
    employee_id INT NOT NULL,
    FOREIGN KEY (employee_id) REFERENCES employees(id)
);

CREATE TABLE customers_employees (
    customer_id INT NOT NULL,
    employee_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (customer_id) REFERENCES customers(id),
    CONSTRAINT FOREIGN KEY (employee_id) REFERENCES employees(id)
);

CREATE TABLE customers_purchases (
    customer_id INT NOT NULL,
    purchase_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (customer_id) REFERENCES customers(id),
    CONSTRAINT FOREIGN KEY (purchase_id) REFERENCES purchase_history(id)
);

CREATE TABLE purchases_products (
    purchase_id INT NOT NULL,
    product_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (purchase_id) REFERENCES purchase_history(id),
    CONSTRAINT FOREIGN KEY (product_id) REFERENCES products(id)
);

DROP TRIGGER IF EXISTS add_promo_code_after_purchase;
DELIMITER |
CREATE TRIGGER add_promo_code_after_purchase AFTER INSERT ON customers_purchases
FOR EACH ROW
BEGIN
    DECLARE total_spent DECIMAL(10, 2);
    DECLARE existing_promo FLOAT;

    -- Calculate the total amount spent by the customer
    SELECT ph.purchase_price INTO total_spent
    FROM purchase_history ph
    JOIN customers_purchases cp ON ph.id = cp.purchase_id
    WHERE cp.customer_id = NEW.customer_id;
    
    -- Check if the total spent is over 30 and if the customer has an existing promo code
    SELECT promo_code INTO existing_promo
    FROM customers
    WHERE id = NEW.customer_id;

    IF total_spent >= 30 AND existing_promo IS NULL THEN
        -- Add promo code for the customer
        UPDATE customers
        SET promo_code = 5 -- Set the promo code value as desired
        WHERE id = NEW.customer_id;
    ELSE
        -- Remove existing discount
        UPDATE customers
        SET promo_code = NULL -- Set the new promo code value as desired
        WHERE id = NEW.customer_id;
    END IF;

    -- Remove the discount if the customer has used one before adding the new one
    IF total_spent >= 30 AND existing_promo IS NOT NULL THEN
        -- Remove existing discount
        UPDATE customers
        SET promo_code = 5 -- Set the new promo code value as desired
        WHERE id = NEW.customer_id;
    END IF;

END |
DELIMITER ;

-- Inserting test data into the employees table
INSERT INTO employees (name) VALUES 
('John Doe'), 
('Jane Smith'), 
('Robert Johnson');

-- Inserting test data into the customers table
INSERT INTO customers (name, email, phone, promo_code) VALUES
('Alice Brown', 'alice@example.com', '1234567890', NULL),
('Bob Green', 'bob@example.com', '9876543210', NULL),
('Charlie White', 'charlie@example.com', '4567890123', NULL);

-- Inserting test data into the products table
INSERT INTO products (product_type, car_brand, car_model, description, catalog_number, price) VALUES
('Engine Oil', 'Honda', 'Civic', 'High-quality synthetic engine oil', '12345', 30.00),
('Brake Pads', 'Toyota', 'Corolla', 'OEM replacement brake pads', '54321', 50.00),
('Air Filter', 'Ford', 'Fusion', 'Reusable high-flow air filter', '98765', 40.00);

-- Inserting test data into the purchase_history table
INSERT INTO purchase_history (purchase_date, purchase_price, price, discount, employee_id) VALUES
('2024-04-30', 25.00, 30.00, NULL, 1),
('2024-04-29', 50.00, 45.00, 5.00, 2),
('2024-04-28', 40.00, 40.00, NULL, 3);

-- Inserting test data into the customers_purchases table
INSERT INTO customers_purchases (customer_id, purchase_id) VALUES
(1, 1),
(2, 2),
(3, 3);

-- Inserting test data into the purchases_products table (many-to-many relationship)
INSERT INTO purchases_products (purchase_id, product_id) VALUES
(1, 1), -- Alice bought Engine Oil
(2, 2), -- Bob bought Brake Pads
(3, 3); -- Charlie bought Air Filter

SELECT * FROM customers;

