USE school_sport_clubs;

DELIMITER $
DROP PROCEDURE IF EXISTS testProc $

CREATE PROCEDURE testProc(INOUT testParam VARCHAR(255)) -- OUT makes the parameter NULL, IN passes it by value, INOUT passes by reference

BEGIN

SELECT testParam;
SET testParam = 'Georgi';
SELECT testParam;

END $
DELIMITER ;

SET @test = 'Ivan'; -- @ is used to initialize sesion variable, @@ is sed for global variable

CALL testProc(@test);

SELECT @test;

#drop procedure checkMothTax;
delimiter |
CREATE procedure checkMothTax(IN studId INT, IN groupId INT, IN paymentMonth INT, IN paymentYear INT)
BEGIN
DECLARE result char(1);
SET result = 0;
	IF( (SELECT paymentAmount
		FROM taxespayments
		WHERE student_id = studId
		AND group_id = groupId
		AND MONTH = paymentMonth
		AND year = paymentYear) IS NOT NULL)
    THEN
		SET result = 1;
	ELSE
		SET result = 0;
    END IF;
    
SELECT result as IsTaxPayed;
end |
delimiter ;

CALL `school_sport_clubs`.`checkMothTax`(1, 1,1,2022);

#drop procedure getPaymentPeriod;
delimiter |
CREATE procedure getPaymentPeriod(IN studId INT, IN groupId INT, IN paymentYear INT)
BEGIN
DECLARE countOfMonths tinyint;
DECLARE monthStr VARCHAR(10);
DECLARE yearStr varchar(10);
SET monthStr = 'MONTH';
SET yearStr = 'YEAR';

	SELECT COUNT(*)
    INTO countOfMonths
    FROM taxespayments
    WHERE student_id = studId
    AND group_id = groupId
    AND year = paymentYear;
    
    CASE countOfMonths
    WHEN 0 THEN SELECT 'This student has not paid for this group/year!' as PAYMENT_PERIOD;
    WHEN 1 THEN SELECT concat('ONE_', monthStr) as PAYMENT_PERIOD;
    WHEN 3 THEN SELECT concat('THREE_',monthStr, 'S') as PAYMENT_PERIOD;
    WHEN 6 THEN SELECT concat('SIX_',monthStr,'S') as PAYMENT_PERIOD;
    WHEN 12 THEN SELECT yearStr as PAYMENT_PERIOD;
    ELSE
		SELECT 	concat(countOfMonths,monthStr,'S') as PAYMENT_PERIOD;
	END CASE;
END |
DELIMITER ;

CALL getPaymentPeriod(1,1, 2022);


#drop procedure getAllPaymentsAmount;
delimiter |
CREATE procedure getAllPaymentsAmount(IN firstMonth INT, IN secMonth INT, IN paymentYear INT, IN studId INT)
BEGIN
	DECLARE iterator int;
	IF(firstMonth >= secMonth)
    THEN 
		SELECT 'Please enter correct months!' as RESULT;
	ELSE IF((SELECT COUNT(*)
			FROM taxesPayments
			WHERE student_id =studId ) = 0)
        THEN SELECT 'Please enter correct student_id!' as RESULT;
		ELSE
	
	SET ITERATOR = firstMonth;

		WHILE(iterator >= firstMonth AND iterator <= secMonth)
		DO
			SELECT student_id, group_id, paymentAmount, month
			FROM taxespayments
			WHERE student_id = studId
			AND year = paymentYear
			AND month = iterator;
    
			SET iterator = iterator + 1;
		END WHILE;
		END IF;
    
    END IF;
END |
DELIMITER ;

CALL getAllPaymentsAmount(1,6,2022,1);

#drop procedure getAllPaymentsAmountOptimized;
delimiter |
CREATE procedure getAllPaymentsAmountOptimized(IN firstMonth INT, IN secMonth INT, IN paymentYear INT, IN studId INT)
BEGIN
    DECLARE iterator int;
    CREATE TEMPORARY TABLE tempTbl(
    student_id int, 
    group_id int,
    paymentAmount double,
    month int
    ) ENGINE = Memory;
    
    
	IF(firstMonth >= secMonth)
    THEN 
		SELECT 'Please enter correct months!' as RESULT;
	ELSE IF((SELECT COUNT(*)
			FROM taxesPayments
			WHERE student_id =studId ) = 0)
        THEN SELECT 'Please enter correct student_id!' as RESULT;
		ELSE
	
	SET ITERATOR = firstMonth;

		WHILE(iterator >= firstMonth AND iterator <= secMonth)
		DO
			INSERT INTO tempTbl
			SELECT student_id, group_id, paymentAmount, month
			FROM taxespayments
			WHERE student_id = studId
			AND year = paymentYear
			AND month = iterator;
    
			SET iterator = iterator + 1;
		END WHILE;
		END IF;
    
    END IF;
		SELECT *
        FROM tempTbl;
        DROP TABLE tempTbl;
END |
DELIMITER ;

CALL getAllPaymentsAmountOptimized(1,6,2022,1);

----------------------------------------------------------------------------

DELIMITER $
DROP PROCEDURE IF EXISTS zad1 $

CREATE PROCEDURE zad1(IN testParam VARCHAR(255))
BEGIN

SELECT coaches.name, sports.name, sportGroups.location, 
    sportGroups.dayOfWeek, students.name, students.phone
FROM coaches JOIN sportGroups
ON coaches.id = sportgroups.coach_id
JOIN student_sport
ON sportGroups.sport_id = student_sport.sportGroup_id
JOIN students
ON student_sport.student_id = students.id
JOIN sports
ON sportGroups.sport_id = sports.id
WHERE coaches.name = testParam; 

END $
DELIMITER ;

SET @test = 'Ivan Todorov Petkov';

CALL zad1(@test);

SELECT @test;

DELIMITER $
DROP PROCEDURE IF EXISTS zad2 $

CREATE PROCEDURE zad2(IN testParam INT)
BEGIN

SELECT coaches.name, sports.name, sportGroups.location, 
    sportGroups.dayOfWeek, students.name, students.phone
FROM coaches JOIN sportGroups
ON coaches.id = sportgroups.coach_id
JOIN student_sport
ON sportGroups.sport_id = student_sport.sportGroup_id
JOIN students
ON student_sport.student_id = students.id
JOIN sports
ON sportGroups.sport_id = sports.id
WHERE sports.id = testParam; 

END $
DELIMITER ;

SET @test = 2;

CALL zad2(@test);

SELECT @test;

DELIMITER $
DROP PROCEDURE IF EXISTS zad3 $

CREATE PROCEDURE zad3(IN student_name VARCHAR(255),IN input_year INT)
BEGIN

SELECT AVG(taxesPayments.paymentAmount) AS AveragePayments
FROM taxesPayments JOIN students
ON taxesPayments.student_id = students.id
WHERE students.name = student_name
AND taxesPayments.year = input_year;

END $
DELIMITER ;

SET @test1 = 'Iliyan Ivanov';
SET @test2 = 2022;

CALL zad3(@test1, @test2);

SELECT @test1, @test2;

DELIMITER $
DROP PROCEDURE IF EXISTS zad4 $

CREATE PROCEDURE zad4(IN coach_name VARCHAR(255))
BEGIN

DECLARE result INT;
SET result = (SELECT COUNT(sportGroups.id) 
        FROM sportGroups JOIN coaches
        ON sportGroups.coach_id = coaches.id
        WHERE coaches.name = coach_name);

IF (result IS NOT NULL)
THEN
    SELECT result AS numberOfGroups;
ELSE
    SET result = 'No groups trained by this coach';
END IF;
END $
DELIMITER ;

SET @test = 'Ivan Todorov Petkov';

CALL zad4(@test);

SELECT @test;

USE transaction_test;

DELIMITER $
DROP PROCEDURE IF EXISTS zad5 $

CREATE PROCEDURE zad5(IN account_id_in INT, 
        IN account_id_out INT, IN transaction_sum INT)
BEGIN

DECLARE trasaction_validation INT;

SET trasaction_validation = (SELECT ROW_COUNT(customer_accounts.amount) 
                            FROM customer_accounts JOIN customers
                            WHERE customets.id = account_id_out);

IF(trasaction_validation >= transaction_sum)
THEN
    START TRANSACTION
        UPDATE customer_accounts
        JOIN customers
        SET customer_accounts.amount = customer_accounts.amount - transaction_sum
        WHERE customer_accounts.id = account_id_out;

        UPDATE customer_accounts
        JOIN customers
        SET customer_accounts.amount = customer_accounts.amount + transaction_sum
        WHERE customer_accounts.id = account_id_in;

    COMMIT;
END $
DELIMITER ;

SET @test1 = 1;
SET @test2 = 2;
SET transaction_sum = 10;

CALL zad5(@test1, @test2, @test3);

SELECT @test1, @test2, @test3;