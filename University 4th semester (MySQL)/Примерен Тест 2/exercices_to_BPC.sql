USE Agency_Property;

DROP VIEW IF EXISTS monthlyDeals;
CREATE VIEW monthlyDeals
AS
SELECT 
    c.name, 
    c.phone, 
    p.location, 
    p.area, 
    p.price, 
    e.name AS EmployeeName
FROM 
    customers c 
JOIN properties p ON c.id = p.customer_id
JOIN ads ON p.id = ads.property_id
JOIN deals ON ads.id = deals.ad_id
JOIN employees e ON deals.employee_id = e.id
JOIN actions ON ads.action_id = actions.id
JOIN types ON p.type_id = types.id
WHERE e.position = 'Broker' -- Ensure position is spelled correctly
AND actions.actionType = 'купува' -- Use the correct actionType
AND types.typeName = 'къща' -- Use the correct typeName
AND p.area > 100
AND MONTH(deals.dealDate) = MONTH(NOW())
AND YEAR(deals.dealDate) = YEAR(NOW())
ORDER BY p.price;

DROP PROCEDURE IF EXISTS commissionPayment;
DELIMITER |
CREATE PROCEDURE commissionPayment(IN p_month INT, IN p_year INT)
BEGIN

    DECLARE done INT DEFAULT FALSE;
    DECLARE deal_id INT;
    DECLARE cur_price DECIMAL(20, 2); 
    DECLARE cur_salary_id INT;
    DECLARE emp_id INT;
    DECLARE bonus DECIMAL(10, 2);
    DECLARE cur_count INT DEFAULT 0;
    DECLARE number_of_deals INT;
    DECLARE perc_bonus FLOAT;
    
    DECLARE dealsCursor CURSOR FOR
    SELECT id FROM deals 
    WHERE MONTH(dealDate) = p_month
    AND YEAR(dealDate) = p_year;
    
    DECLARE bonusCursor CURSOR FOR
    SELECT s.id, s.montlyBonus FROM salaryPayments s
    JOIN employees e ON s.employee_id = e.id
    WHERE MONTH(deals.dealDate) = p_month
    AND YEAR(deals.dealDate) = p_year
    ORDER BY s.montlyBonus DESC
    LIMIT 3;

    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

    OPEN dealsCursor;

    deals_loop: LOOP
        FETCH dealsCursor INTO deal_id;

        IF done THEN
            LEAVE deals_loop;
        END IF;

        SELECT price INTO cur_price FROM properties p
        JOIN ads ON p.id = ads.property_id
        WHERE ads.id = deal_id;

        SELECT employee_id INTO cur_salary_id FROM deals
        WHERE id = deal_id;

        START TRANSACTION;
        
        IF cur_price < 100000 THEN 
            UPDATE salaryPayments
            SET montlyBonus = montlyBonus + cur_price * 0.02
            WHERE employee_id = cur_salary_id;
        ELSE 
            UPDATE salaryPayments
            SET montlyBonus = montlyBonus + cur_price * 0.03
            WHERE employee_id = cur_salary_id;
        END IF;

        IF (ROW_COUNT() = 1) THEN
            COMMIT;
        ELSE   
            ROLLBACK;
        END IF;

    END LOOP;    

    CLOSE dealsCursor;

    SET done = FALSE;

    OPEN bonusCursor;

    bonus_loop: LOOP
        FETCH bonusCursor INTO emp_id, bonus;

        IF done_bonus THEN
            LEAVE bonus_loop;
        END IF;

        SELECT COUNT(*) INTO number_of_deals FROM deals 
        WHERE employee_id = emp_id
        AND MONTH(dealDate) = p_month
        AND YEAR(dealDate) = p_year;

        IF cur_count = 0 THEN
            SET perc_bonus = 1.15;
        ELSEIF cur_count = 1 THEN
            SET perc_bonus = 1.10;
        ELSEIF cur_count = 2 THEN 
            SET perc_bonus = 1.05;
        END IF; 

        START TRANSACTION;

        UPDATE salaryPayments
        SET montlyBonus = bonus + (bonus / number_of_deals) * perc_bonus
        WHERE id = emp_id;

        IF (ROW_COUNT() = 1) THEN
            COMMIT;
        ELSE   
            ROLLBACK;
        END IF;
        SET cur_count = cur_count + 1;

    END LOOP; 

    CLOSE bonusCursor;

END |
DELIMITER ;


USE Agency_Property;

DELIMITER |
CREATE PROCEDURE SendEmailToCustomers(IN ad_id INT, IN property_id INT, IN discount DECIMAL(10, 2))
BEGIN
    SELECT CONCAT('Email sent') AS Confirmation;
END |
DELIMITER ;

DELIMITER |
CREATE TRIGGER if_customer_used_agency AFTER INSERT ON ads
FOR EACH ROW
BEGIN
    
    DECLARE deals_count INT DEFAULT 0;
    DECLARE discount DECIMAL(5, 2) DEFAULT 0;
    DECLARE t_customer_id INT;

    SELECT customer_id
    INTO t_customer_id
    FROM customers AS c
    JOIN properties AS p ON p.customer_id = c.id
    WHERE p.id = NEW.property_id;

    SELECT COUNT(*) INTO deals_count
    FROM Deals AS d
    JOIN ads ON ads.id = d.ad_id
    JOIN actions AS a ON a.id = Ads.action_id
    JOIN properties AS p ON p.id = Ads.property_id
    JOIN customers AS c ON c.id = p.customer_id
    WHERE actionType = "продава"
        AND customer_id = t_customer_id;

    IF (deals_count > 0) 
        THEN 
            IF (1 <= deals_count AND deals_count <= 5)
                THEN
                    SET discount = 0.5;
            ELSEIF (deals_count > 5)
                THEN
                    SET discount = 0.01;
            END IF;

        CALL SendEMailToCustomer(NEW.property_id, t_customer_id , discount);
    END IF;
END |
DELIMITER ;

DROP TRIGGER IF EXISTS validation_for_under_two_ads;
DELIMITER |
CREATE TRIGGER validation_for_under_two_ads BEFORE INSERT ON ads
FOR EACH ROW
BEGIN

    DECLARE number_of_ads INT;

    SELECT COUNT(p.customer_id) INTO number_of_ads FROM customers c
    JOIN properties p ON c.id = p.customer_id
    JOIN ads ON p.id = ads.property_id
    JOIN actions ON ads.action_id = actions.id
    WHERE ads.id = NEW.id
    AND ads.isActual = TRUE
    AND actions.name = 'отдавна под наем';

    IF number_of_ads >= 2 THEN
        SIGNAL SQLSTATE '45000' 
            SET MESSAGE_TEXT = 'Customer already has 2 or more active ads!';
    END IF;

END |
DELIMITER ;
