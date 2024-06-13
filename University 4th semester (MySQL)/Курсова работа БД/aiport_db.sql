DROP DATABASE IF EXISTS airport;
CREATE DATABASE airport;
USE airport;

CREATE TABLE departments(
    id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
    name VARCHAR(50) NOT NULL,
    manager_id INT DEFAULT NULL
);

CREATE TABLE employees(
    id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
    first_name VARCHAR(50) NOT NULL,
    second_name VARCHAR(50) NOT NULL,
    surname VARCHAR(50) NOT NULL,
    started_in_company DATE NOT NULL,
    work_experience INT NOT NULL,
    specific_information TEXT NULL,
    department_id INT DEFAULT NULL,
    FOREIGN KEY (department_id) REFERENCES departments(id)
);

ALTER TABLE departments ADD CONSTRAINT FK_employee_id
FOREIGN KEY (id) REFERENCES employees(id);

CREATE TABLE planes(
    id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
    brand VARCHAR(255) NOT NULL,
    model VARCHAR(50) NOT NULL,
    plane_number INT NOT NULL,
    date_of_production DATE NOT NULL,
    producer VARCHAR(255) NOT NULL,
    number_of_passengers INT NOT NULL,
    flight_hours INT NOT NULL,
    maintanence_cost DECIMAL NOT NULL,
    repair_id INT NOT NULL
);

CREATE TABLE repairs(
    id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
    plane_id INT NOT NULL,
    repair_date DATE NOT NULL,
    repair_description TEXT,
    repair_cost DECIMAL NOT NULL,
    CONSTRAINT FOREIGN KEY (plane_id) REFERENCES planes(id)
);

CREATE TABLE flights(
    id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
    take_off_of_flight DATETIME NOT NULL,
    arrival_of_flight DATETIME NOT NULL,
    take_off_location VARCHAR(255) NOT NULL,
    arrival_loccation VARCHAR(255) NOT NULL
);

CREATE TABLE flights_to_planes (
    flight_id INT NOT NULL,
    plane_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (plane_id) REFERENCES planes(id),
    CONSTRAINT FOREIGN KEY (flight_id) REFERENCES flights(id)
);

CREATE TABLE employees_to_flights (
    employee_id INT NOT NULL,
    flight_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (employee_id) REFERENCES employees(id),
    CONSTRAINT FOREIGN KEY (flight_id) REFERENCES flights(id)
);

CREATE TABLE employees_to_planes (
    employee_id INT NOT NULL,
    plane_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (employee_id) REFERENCES employees(id),
    CONSTRAINT FOREIGN KEY (plane_id) REFERENCES planes(id)
);

CREATE TABLE salaries (
    id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
    salary_amount DECIMAL NOT NULL,
    extra_hours_work INT NULL,
    curr_month INT NOT NULL,
    curr_year INT NOT NULL,
    employee_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (employee_id) REFERENCES employees(id),
    UNIQUE KEY (employee_id, curr_month, curr_year)
);
