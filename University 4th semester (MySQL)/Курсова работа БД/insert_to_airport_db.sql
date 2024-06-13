USE airport;

-- Insert clauses for the employees table
INSERT INTO employees (first_name, second_name, surname, started_in_company, 
    work_experience, specific_information) 
VALUES 
('John', 'Doe', 'Smith', '2020-05-15', 2, NULL),       
('Jane', 'Doe', 'Smith', '2019-10-20', 13, 'Experienced pilot'),  
('Alice', 'Johnson', 'Brown', '2021-01-10', 1, NULL),  
('Marry', 'Hana', 'Green', '2020-05-15', 2, NULL),      
('Harry', 'Doe', 'Thomsan', '2019-10-20', 3, NULL),     
('Elizabeth', 'Huffer', 'Summer', '2021-01-10', 1, NULL); 

INSERT INTO departments (name, manager_id) 
VALUES 
('Pilots', 1),         
('Workers',  2),        
('Administration', 3), 
('Stewards', 4);   

-- Update employees with department IDs
UPDATE employees
SET department_id = 
    CASE 
        WHEN first_name = 'John' AND second_name = 'Doe' 
            AND surname = 'Smith' THEN 1
        WHEN first_name = 'Jane' AND second_name = 'Doe' 
            AND surname = 'Smith' THEN 2
        WHEN first_name = 'Alice' AND second_name = 'Johnson' 
            AND surname = 'Brown' THEN 3
        WHEN first_name = 'Marry' AND second_name = 'Hana' 
            AND surname = 'Green' THEN 4
        WHEN first_name = 'Harry' AND second_name = 'Doe' 
            AND surname = 'Thomsan' THEN 2
        WHEN first_name = 'Elizabeth' AND second_name = 'Huffer' 
            AND surname = 'Summer' THEN 4
        ELSE department_id
    END;

-- Insert clauses for the planes table
INSERT INTO planes (brand, model, plane_number, date_of_production, producer,
    number_of_passengers, flight_hours, maintanence_cost, repair_id) 
VALUES 
('Boeing', '737', 1001, '2019-01-20', 'Boeing Company', 150, 5000, 2500.00, 1),
('Airbus', 'A320', 2001, '2020-05-15', 'Airbus SE', 180, 4500, 2800.00, 2),
('Boeing', '747', 3001, '2018-11-10', 'Boeing Company', 300, 7000, 4000.00, 3);

-- Insert clauses for the flights table
INSERT INTO flights (take_off_of_flight, arrival_of_flight,
    take_off_location, arrival_loccation) 
VALUES 
('2024-04-01 08:00:00', '2024-04-01 10:30:00', 'New York', 'Los Angeles'),
('2024-04-02 10:00:00', '2024-04-02 13:30:00', 'London', 'Paris'),
('2024-04-03 12:30:00', '2024-04-03 15:00:00', 'Tokyo', 'Beijing');

-- Insert clauses for the salaries table
INSERT INTO salaries (id, salary_amount, 
    extra_hours_work, curr_month, curr_year, employee_id) 
VALUES 
(1, 8000.00, 11, 4 , 2024, 1),
(2, 4000.00, 7, 4 , 2024, 2),
(3, 4500.00, NULL, 4 , 2024, 3),
(4, 5000.00, NULL, 4 , 2024, 4),
(5, 4000.00, 5, 4 , 2024, 5),
(6, 4500.00, 9, 4 , 2024, 6);

-- Insert statement for assigning employees to a flight
INSERT INTO employees_to_flights (employee_id, flight_id) 
VALUES
(2, 1),
(3, 1),
(6, 1),
(2, 2),
(3, 2),
(6, 2);

-- Insert statement for assigning employees to work on a plane
INSERT INTO employees_to_planes (employee_id, plane_id)
VALUES
(2, 1),
(3, 1),
(6, 1),
(2, 2),
(3, 2),
(6, 2);

-- Insert statement for assigning flights done on a plane
INSERT INTO flights_to_planes (flight_id, plane_id)
VALUES
(1, 1),
(1, 1),
(1, 1),
(2, 2),
(2, 2),
(2, 2);

INSERT INTO repairs (plane_id, repair_date, repair_description, repair_cost) 
VALUES 
(1, '2023-05-15', 'Engine maintenance', 5000.00),
(2, '2023-06-20', 'Replacement of avionics system', 8000.00),
(3, '2023-07-10', 'Repair of landing gear', 6000.00),
(1, '2023-08-05', 'Repair of hydraulic system', 4000.00),
(3, '2023-09-12', 'Structural repair', 7000.00);
