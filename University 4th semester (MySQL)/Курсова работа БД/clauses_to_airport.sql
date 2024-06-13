USE airport;

-- Query with using restricting statement
SELECT e.first_name, e.surname,
    d.name AS dept_name, s.curr_month, s.salary_amount 
FROM employees e
JOIN departments d ON e.department_id = d.id
JOIN salaries s ON e.id = s.employee_id
WHERE s.curr_month = MONTH(NOW())
AND s.curr_year = YEAR(NOW())
AND s.salary_amount >= 5000;

-- Query using GROUP BY and aggregate function 'AVG()'
SELECT d.name AS dept_name, AVG(s.salary_amount) AS avg_salary
FROM employees e
JOIN departments d ON e.department_id = d.id
JOIN salaries s ON e.id = s.employee_id
GROUP BY d.name;

-- Query with INNER JOIN
SELECT e.first_name, e.surname, s.salary_amount
FROM employees e INNER JOIN salaries s
ON e.id = s.employee_id;

-- Query with OUTER JOIN
SELECT e.first_name, e.surname, 
    d.name AS manager_dept, s.salary_amount
FROM employees e
LEFT JOIN departments d ON e.department_id = d.id
LEFT JOIN salaries s ON e.id = s.employee_id
WHERE e.id != d.manager_id;

-- Query with nested SELECT
SELECT COUNT(*) AS num_steward_on_flight_2
FROM (
    SELECT employee_id
    FROM employees_to_flights
    WHERE flight_id = 2) AS flight_2
JOIN employees e ON flight_2.employee_id = e.id
JOIN departments d ON e.department_id = d.id
WHERE e.id != d.manager_id
AND d.name = 'Stewards';

-- Query with JOIN and aggregate function
SELECT e.first_name, e.surname, 
    COUNT(ef.flight_id) AS num_flights
FROM employees e
LEFT JOIN employees_to_flights ef ON e.id = ef.employee_id
JOIN departments d ON e.department_id = d.id
WHERE e.id != d.manager_id
GROUP BY 
    e.first_name,
    e.surname;

-- Trigger that increments automaticaly the flight hours of the planes
DROP TRIGGER IF EXISTS increment_flight_hours;
DELIMITER |
CREATE TRIGGER increment_flight_hours AFTER INSERT ON flights_to_planes
FOR EACH ROW
BEGIN
    DECLARE flight_duration INT;
    
    SELECT TIMESTAMPDIFF(HOUR, f.take_off_of_flight, f.arrival_of_flight) INTO flight_duration
    FROM flights f
    WHERE f.id = NEW.flight_id;
    
    UPDATE planes
    SET flight_hours = flight_hours + flight_duration
    WHERE id = NEW.plane_id;
END
|
DELIMITER ;

-- Tests for trigger increment_flight_hours
/* SELECT id AS plane_id, flight_hours FROM planes
WHERE planes.id = 1;

INSERT INTO flights(take_off_of_flight, arrival_of_flight,
    take_off_location, arrival_loccation) 
VALUES 
    ('2024-04-23 18:00:00', '2024-04-23 20:00:00', 'Tokyo', 'Seoul');

INSERT INTO flights_to_planes (flight_id, plane_id)
VALUES
(1, 1);

SELECT id AS plane_id, flight_hours FROM planes
WHERE planes.id = 1; */

-- Procedure for adding the extra hours of work to tha salaries 
SET @base_bonus = 50;
DROP PROCEDURE IF EXISTS update_salary_based_on_extra_work;
DELIMITER |
CREATE PROCEDURE update_salary_based_on_extra_work(IN base_bonus INT)
BEGIN
    DECLARE done INT DEFAULT FALSE;
    DECLARE emp_id INT;
    DECLARE sal_amount DECIMAL;
    DECLARE extra_hours INT;
    DECLARE dept_id INT;
    DECLARE exp_years INT;
    DECLARE dept_multiplier FLOAT;
    DECLARE exp_multiplier FLOAT DEFAULT 1.0;

    DECLARE cur_salary CURSOR FOR
        SELECT s.employee_id, s.salary_amount, 
            s.extra_hours_work, e.department_id, e.work_experience
        FROM salaries s
        JOIN employees e ON s.employee_id = e.id
        WHERE s.extra_hours_work IS NOT NULL;

    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;
    
    OPEN cur_salary;
    
    salary_loop: LOOP
        FETCH cur_salary INTO emp_id, sal_amount, extra_hours, dept_id, exp_years;
        IF done THEN
            LEAVE salary_loop;
        END IF;
        
        CASE dept_id
            WHEN 1 THEN
                SET dept_multiplier = 2.5;
            WHEN 2 THEN
                SET dept_multiplier = 1.0;
            WHEN 3 THEN
                SET dept_multiplier = 1.1;
            WHEN 4 THEN
                SET dept_multiplier = 1.75;
        END CASE;

        IF exp_years >= 10 THEN
            SET exp_multiplier = 1.5;
        ELSEIF exp_years >= 5 THEN
            SET exp_multiplier = 1.3;
        ELSEIF exp_years >= 3 THEN
            SET exp_multiplier = 1.1;
        END IF;
        
        UPDATE salaries 
        SET salary_amount = sal_amount + (base_bonus * dept_multiplier * exp_multiplier) 
        WHERE employee_id = emp_id;

        UPDATE salaries
        SET extra_hours_work = 0
        WHERE employee_id = emp_id;

    END LOOP;
    
    CLOSE cur_salary;
END |
DELIMITER ;

-- Event to call the adding of the extra work to the salaries every month on the first
CREATE EVENT IF NOT EXISTS update_salaries_event
ON SCHEDULE
    EVERY 1 MONTH
    STARTS CONCAT(YEAR(CURRENT_DATE()), '-', LPAD(MONTH(CURRENT_DATE()), 2, '0'), '-01')
DO
    CALL update_salary_based_on_extra_work(@base_bonus);

SET GLOBAL event_scheduler = ON;

-- Test for the procedure update_salary_based_on_extra_work
/*
SELECT * FROM salaries
LIMIT 3;

CALL update_salary_based_on_extra_work(@base_bonus);

SELECT * FROM salaries
LIMIT 3; */