/*
Напишете процедура която да извежда летателните часове на пилот по подадено id
*/

USE airport;

-- Тестови заявки за проверка на резилтата от процедурата
INSERT INTO flights (take_off_of_flight, arrival_of_flight,
    take_off_location, arrival_loccation) 
VALUES 
('2024-04-28 08:00:00', '2024-04-28 11:30:00', 'New York', 'Los Angeles');

INSERT INTO employees_to_flights (employee_id, flight_id) VALUES (1, 4);

DROP PROCEDURE IF EXISTS fligh_hours_on_pilot_id;
DELIMITER |
CREATE PROCEDURE fligh_hours_on_pilot_id(IN pilot_id INT)
BEGIN
    DECLARE done INT DEFAULT FALSE;
    DECLARE take_off_t DATETIME;
    DECLARE arrival_t DATETIME;
    DECLARE pilot_flight_hours INT DEFAULT 0;
    DECLARE cur_flight_duration INT DEFAULT 0;

    DECLARE flight_hours_cursor CURSOR FOR
        SELECT take_off_of_flight, arrival_of_flight FROM flights
        JOIN employees_to_flights ON flights.id = employees_to_flights.flight_id
        JOIN employees ON employees_to_flights.employee_id = employees.id
        WHERE employees.department_id = 1
        AND employees.id = pilot_id;

    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

    OPEN flight_hours_cursor;
    
    flight_loop: LOOP
        FETCH flight_hours_cursor INTO take_off_t, arrival_t;

        IF done THEN
            LEAVE flight_loop;
        END IF;

        SELECT TIMESTAMPDIFF(HOUR, take_off_t, arrival_t) INTO cur_flight_duration;

        SET pilot_flight_hours = pilot_flight_hours + cur_flight_duration;

    END LOOP;

    SELECT pilot_flight_hours;
    
    CLOSE flight_hours_cursor;
END |
DELIMITER ;

SET @pilot_id = 1;

CALL fligh_hours_on_pilot_id(@pilot_id); -- Очакван резултат pilot_flight_hours: 3
