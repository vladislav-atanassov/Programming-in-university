CREATE VIEW dailyManpulations
AS
SELECT CONCAT(p.firstName, ' ',  p.lastName) AS Patient, 
    proc.name AS ProcedureName,
    proc.price AS Price,
    m.operatingDate AS OperationDate,
    m.operatingTime AS StartHour,
    d.name AS Doctor
FROM doctors d 
JOIN manipipulations m ON d.id = m.doctor_id
JOIN patients p ON m.patient_id = p.id
JOIN procedures proc ON m.procedure_id = proc.id
WHERE DATE(m.operatingDate) = DATE(NOW())
GROUP BY m.operatingDate, d.name;

DELIMITER |
CREATE PROCEDURE monthlyBonusPayment(IN p_month INT, IN p_year INT)
BEGIN

    DECLARE done INT DEFAULT FALSE;
    DECLARE c_doctor_id INT;
    DECLARE proc_count INT;

    DECLARE doctorCursor CURSOR FOR SELECT d.id FROM doctors d;

    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

    OPEN doctorCursor;

    doctor_loop: LOOP
        
        FETCH doctorCursor INTO c_doctor_id, c_bonus;
        
        IF done THEN
            LEAVE doctor_loop;
        END IF;

        IF (monthSalryPaymnet(p_month, p_year) = 1) THEN
            SELECT COUNT(m.doctor_id) INTO proc_count FROM doctors d 
            JOIN manipulations m ON d.id = m.doctor_id
            JOIN procedures proc ON m.procedure_id = proc.id
            WHERE d.id = c_doctor_id
            AND MONTH(m.operatingDate) = p_month
            AND YEAR(m.operatingDate) = p_year
            GROUP BY d.id, proc.id;

            START TRANSACTION;
            UPDATE salaryPayments s
            SET montlyBonus = montlyBonus + (proc_count * 1000)
            WHERE s.doctor_id = c_doctor_id;

            IF ROW_COUNT() = 1 THEN
                COMMIT;
            ELSE
                ROLLBACK;
            END IF;

        END IF;

    END LOOP;

    CLOSE doctorCursor;

END |
DELIMITER ;

DELIMITER |
CREATE TRIGGER 
