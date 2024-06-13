USE Agency_Property;
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