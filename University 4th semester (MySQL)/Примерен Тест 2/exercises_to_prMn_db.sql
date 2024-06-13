USE project_managment;

DROP VIEW IF EXISTS month_empl_project_info;
CREATE VIEW month_empl_project_info AS
SELECT e.id AS EmpNo, CONCAT(e.firstName, ' ', e.lastName) AS EmplName, pr.name AS ProjectName, 
    pr.priority AS Priority, pr.budget AS Budget, SUM(task.hours) AS Hours, 
    task.month AS CurrentMonth, task.year AS CurrentYear
FROM employee e 
JOIN task ON e.id = task.empl_id
JOIN project pr ON task.project_id = pr.id
WHERE task.month = MONTH(NOW())
AND task.year = YEAR(NOW())
GROUP BY pr.id, e.id, task.hours, task.month, task.year;

DROP TRIGGER IF EXISTS before_insert_empl_validate_hours;
DELIMITER |
CREATE TRIGGER before_insert_empl_validate_hours BEFORE INSERT ON task
FOR EACH ROW
BEGIN

    DECLARE emplHours INT;

    SELECT IFNULL(SUM(hours), 0) INTO emplHours 
    FROM task 
    WHERE empl_id = NEW.empl_id
    AND month = MONTH(NOW())
    AND year = YEAR(NOW()); 

    IF emplHours >= 176 THEN
        SIGNAL SQLSTATE '45000' 
            SET MESSAGE_TEXT = 'Employee cannot work more than 176 hours in a month.';
    END IF;

END |
DELIMITER ;

DROP PROCEDURE IF EXISTS prepareBonuses;
DELIMITER |
CREATE PROCEDURE prepareBonuses(IN curr_month INT, IN curr_year INT)
PROC: BEGIN

    DECLARE done INT DEFAULT FALSE;
    DECLARE view_month INT;
    DECLARE view_year INT;
    DECLARE view_hours INT DEFAULT NULL;
    DECLARE view_priority INT;
    DECLARE view_id INT;
    DECLARE cal_bonusPoints DOUBLE;
    DECLARE bonus_exists INT;
    DECLARE wrong_date_handler CONDITION FOR SQLSTATE "45000";

    DECLARE empl_cursor CURSOR FOR
    SELECT Hours, Priority, EmpNo FROM month_empl_project_info;

    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;
    
    DECLARE CONTINUE HANDLER FOR wrong_date_handler
    BEGIN
        SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = "Year and month are not the same as the view!";
    END; 

    SELECT CurrentMonth, CurrentYear INTO view_month, view_year FROM month_empl_project_info LIMIT 1;

    -- Validate the parameters of the procedure
    IF view_month != curr_month OR view_year != curr_year THEN
        SIGNAL wrong_date_handler;
        LEAVE PROC;
    END IF;

    OPEN empl_cursor;

    empl_loop: LOOP

        FETCH empl_cursor INTO view_hours, view_priority, view_id;
        IF done THEN
            LEAVE empl_loop;
        END IF;

        SET cal_bonusPoints = view_hours * 0.08 * view_priority;

        SELECT COUNT(*) INTO bonus_exists 
        FROM bonuses 
        WHERE empl_id = view_id 
        AND month = curr_month 
        AND year = curr_year;

        IF bonus_exists = 0 THEN
            -- If no record exists insert a new record
            INSERT INTO bonuses (totalHours, bonusPoints, amount, month, year, empl_id)
            VALUES (view_hours, cal_bonusPoints, cal_bonusPoints * 10, curr_month, curr_year, view_id);
        ELSE
            -- If a record exists update the existing record
            START TRANSACTION;
                UPDATE bonuses 
                SET bonusPoints = bonusPoints + cal_bonusPoints,
                    amount = amount + (cal_bonusPoints * 10),
                    totalHours = totalHours + view_hours
                WHERE empl_id = view_id AND month = curr_month AND year = curr_year;

                IF ROW_COUNT() > 0 THEN
                    COMMIT;
                ELSE    
                    ROLLBACK;
                END IF;
        END IF;

    END LOOP;

    CLOSE empl_cursor;

END |
DELIMITER ;

-- Inserting some departments
INSERT INTO department (name, location) 
VALUES 
('Engineering', 'Headquarters'),
('HR', 'Headquarters'),
('Marketing', 'Branch Office');

-- Inserting some employees in different departments
INSERT INTO employee (firstName, lastName, dept_id) 
VALUES 
('John', 'Doe', 1),
('Jane', 'Smith', 2),
('Michael', 'Johnson', 3);

-- Inserting projects for the employees to work on
INSERT INTO project (name, budget, priority, isFinished, startDate, endDate) 
VALUES 
('Project A', 10000.00, 1, FALSE, '2024-05-01', '2024-06-30'),
('Project A', 10000.00, 1, FALSE, '2024-05-02', '2024-06-30'),
('Project B', 15000.00, 2, FALSE, '2024-05-01', '2024-06-30'), 
('Project C', 20000.00, 3, FALSE, '2024-06-01', '2024-07-31');

-- Inserting tasks for the employees for the current month
-- Assuming each employee has worked some hours on their respective projects
-- Replace the values with the actual hours worked by the employees
INSERT INTO task (tackDescr, hours, month, year, empl_id, project_id) 
VALUES 
('Task A description', 160, MONTH(NOW()), YEAR(NOW()), 1, 1), 
('Task d description', 160, MONTH(NOW()), YEAR(NOW()), 1, 1), 
('Task B description', 170, MONTH(NOW()), YEAR(NOW()), 2, 2), 
('Task C description', 150, MONTH(NOW()), YEAR(NOW()), 3, 3);

INSERT INTO bonuses (totalHours, month, year, empl_id) 
VALUES 
(200, MONTH(NOW()), YEAR(NOW()), 1),
(250, MONTH(NOW()), YEAR(NOW()), 2);

CALL prepareBonuses(5, 2024);

SELECT * FROM month_empl_project_info;