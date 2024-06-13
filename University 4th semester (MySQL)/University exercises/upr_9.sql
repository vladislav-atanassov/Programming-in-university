DROP DATABASE IF EXISTS school_sport_clubs;
CREATE DATABASE school_sport_clubs;
USE school_sport_clubs;

CREATE TABLE school_sport_clubs.sports(
	id INT AUTO_INCREMENT PRIMARY KEY ,
	name VARCHAR(255) NOT NULL
)Engine = Innodb;

CREATE TABLE school_sport_clubs.coaches(
	id INT AUTO_INCREMENT PRIMARY KEY ,
	name VARCHAR(255) NOT NULL ,
	egn VARCHAR(10) NOT NULL UNIQUE CONSTRAINT EGN CHECK(CHAR_LENGTH(egn) = 10),
	month_salary DECIMAL , 
	hour_salary DECIMAL
)Engine = Innodb;

CREATE TABLE school_sport_clubs.students(
	id INT AUTO_INCREMENT PRIMARY KEY ,
	name VARCHAR(255) NOT NULL ,
	egn VARCHAR(10) NOT NULL UNIQUE ,
	address VARCHAR(255) NOT NULL ,
	phone VARCHAR(20) NULL DEFAULT NULL ,
	class VARCHAR(10) NULL DEFAULT NULL   
)Engine = Innodb;

CREATE TABLE school_sport_clubs.sportGroups(
	id INT AUTO_INCREMENT PRIMARY KEY ,
	location VARCHAR(255) NOT NULL ,
	dayOfWeek ENUM('Monday','Tuesday','Wednesday','Thursday','Friday','Saturday','Sunday') ,
	hourOfTraining TIME NOT NULL ,
	sport_id INT NOT NULL ,
	coach_id INT NOT NULL ,
	UNIQUE KEY(location,dayOfWeek,hourOfTraining)  ,
	CONSTRAINT FOREIGN KEY(sport_id) 
		REFERENCES sports(id) ,
	CONSTRAINT FOREIGN KEY (coach_id) 
		REFERENCES coaches(id)
)Engine = Innodb;

CREATE TABLE school_sport_clubs.student_sport(
	student_id INT NOT NULL , 
	sportGroup_id INT NOT NULL ,  
	CONSTRAINT FOREIGN KEY (student_id) 
		REFERENCES students(id) ,	
	CONSTRAINT FOREIGN KEY (sportGroup_id) 
		REFERENCES sportGroups(id) ,
	PRIMARY KEY(student_id,sportGroup_id)
)Engine = Innodb;

CREATE TABLE taxesPayments(
	id INT AUTO_INCREMENT PRIMARY KEY,
	student_id INT NOT NULL,
	group_id INT NOT NULL,
	paymentAmount DOUBLE NOT NULL,
	month TINYINT,
	year YEAR,
	dateOfPayment DATETIME NOT NULL ,
	CONSTRAINT FOREIGN KEY (student_id) 
		REFERENCES students(id),
	CONSTRAINT FOREIGN KEY (group_id) 
		REFERENCES sportgroups(id)
)Engine = Innodb;

CREATE TABLE salaryPayments(
	id INT AUTO_INCREMENT PRIMARY KEY,
	coach_id INT NOT NULL,
	month TINYINT,
	year YEAR,
	salaryAmount DOUBLE CONSTRAINT salaryCantBeNegative CHECK(salaryAmount >= 0),
	dateOfPayment DATETIME not null,
	CONSTRAINT FOREIGN KEY (coach_id) 
		REFERENCES coaches(id),
	UNIQUE KEY(`coach_id`,`month`,`year`)    
)Engine = Innodb;
		
create table coach_work(
	id INT auto_increment primary key,
    coach_id INT not null,
    group_id INT not null,
    number_of_hours INT not null default 1,
	date DATETIME not null,
	isPayed BOOLEAN NOT NULL DEFAULT 0,
    foreign key (coach_id) references coaches(id),
    foreign key (group_id) references sportgroups(id)
)Engine = Innodb;

create table salarypayments_log(
id INT auto_increment primary key,
operation ENUM('INSERT','UPDATE','DELETE') not null,
old_coach_id INT,
new_coach_id INT,
old_month INT,
new_month INT,
old_year INT,
new_year INT,
old_salaryAmount DECIMAL,
new_salaryAmount DECIMAL,
old_dateOfPayment DATETIME,
new_dateOfPayment DATETIME,
dateOfLog DATETIME
)Engine = Innodb;

INSERT INTO sports
VALUES 	(NULL, 'Football') ,
		(NULL, 'Volleyball'),
		(NULL, 'Tennis');
		
INSERT INTO coaches  (name, egn)
VALUES 	('Ivan Todorov Petkov', '7509041245') ,
		('georgi Ivanov Todorov', '8010091245') ,
		('Ilian Todorov Georgiev', '8407106352') ,
		('Petar Slavkov Yordanov', '7010102045') ,
		('Todor Ivanov Ivanov', '8302160980') , 
		('Slavi Petkov Petkov', '7106041278');
		
INSERT INTO students (name, egn, address, phone, class) 
VALUES 	('Iliyan Ivanov', '9401150045', 'Sofia-Mladost 1', '0893452120', '10') ,
		('Ivan Iliev Georgiev', '9510104512', 'Sofia-Liylin', '0894123456', '11') ,
		('Elena Petrova Petrova', '9505052154', 'Sofia-Mladost 3', '0897852412', '11') ,
		('Ivan Iliev Iliev', '9510104542', 'Sofia-Mladost 3', '0894123457', '11') ,
		('Maria Hristova Dimova', '9510104547', 'Sofia-Mladost 4', '0894123442', '11') ,
		('Antoaneta Ivanova Georgieva', '9411104547', 'Sofia-Krasno selo', '0874526235', '10');
		
INSERT INTO sportGroups
VALUES 	(NULL, 'Sofia-Mladost 1', 'Monday', '08:00:00', 1, 1 ) ,
		(NULL, 'Sofia-Mladost 1', 'Monday', '09:30:00', 1, 2 ) ,
		(NULL, 'Sofia-Liylin 7', 'Sunday', '08:00:00', 2, 1) ,
		(NULL, 'Sofia-Liylin 7', 'Sunday', '09:30:00', 2, 2) ,		
		(NULL, 'Plovdiv', 'Monday', '12:00:00', '1', '1');
		
INSERT INTO student_sport 
VALUES 	(1, 1),
		(2, 1),
		(3, 1),
		(4, 2),
		(5, 2),
		(6, 2),
		(1, 3),
		(2, 3),
		(3, 3);
		
INSERT INTO `school_sport_clubs`.`taxespayments` 
VALUES	(NULL, '1', '1', '200', '1', 2023, now()),
		(NULL, '1', '1', '200', '2', 2023, now()),
		(NULL, '1', '1', '200', '3', 2023, now()),
		(NULL, '1', '1', '200', '4', 2023, now()),
		(NULL, '1', '1', '200', '5', 2023, now()),
		(NULL, '1', '1', '200', '6', 2023, now()),
		(NULL, '1', '1', '200', '7', 2023, now()),
		(NULL, '1', '1', '200', '8', 2023, now()),
		(NULL, '1', '1', '200', '9', 2023, now()),
		(NULL, '1', '1', '200', '10', 2023, now()),
		(NULL, '1', '1', '200', '11', 2023, now()),
		(NULL, '1', '1', '200', '12', 2023, now()),
		(NULL, '2', '1', '250', '1', 2023, now()),
		(NULL, '2', '1', '250', '2', 2023, now()),
		(NULL, '2', '1', '250', '3', 2023, now()),
		(NULL, '2', '1', '250', '4', 2023, now()),
		(NULL, '2', '1', '250', '5', 2023, now()),
		(NULL, '2', '1', '250', '6', 2023, now()),
		(NULL, '2', '1', '250', '7', 2023, now()),
		(NULL, '2', '1', '250', '8', 2023, now()),
		(NULL, '2', '1', '250', '9', 2023, now()),
		(NULL, '2', '1', '250', '10', 2023, now()),
		(NULL, '2', '1', '250', '11', 2023, now()),
		(NULL, '2', '1', '250', '12', 2023, now()),
		(NULL, '3', '1', '250', '1', 2023, now()),
		(NULL, '3', '1', '250', '2', 2023, now()),
		(NULL, '3', '1', '250', '3', 2023, now()),
		(NULL, '3', '1', '250', '4', 2023, now()),
		(NULL, '3', '1', '250', '5', 2023, now()),
		(NULL, '3', '1', '250', '6', 2023, now()),
		(NULL, '3', '1', '250', '7', 2023, now()),
		(NULL, '3', '1', '250', '8', 2023, now()),
		(NULL, '3', '1', '250', '9', 2023, now()),
		(NULL, '3', '1', '250', '10', 2023, now()),
		(NULL, '3', '1', '250', '11', 2023, now()),
		(NULL, '3', '1', '250', '12', 2023, now()),
		(NULL, '1', '2', '200', '1', 2023, now()),
		(NULL, '1', '2', '200', '2', 2023, now()),
		(NULL, '1', '2', '200', '3', 2023, now()),
		(NULL, '1', '2', '200', '4', 2023, now()),
		(NULL, '1', '2', '200', '5', 2023, now()),
		(NULL, '1', '2', '200', '6', 2023, now()),
		(NULL, '1', '2', '200', '7', 2023, now()),
		(NULL, '1', '2', '200', '8', 2023, now()),
		(NULL, '1', '2', '200', '9', 2023, now()),
		(NULL, '1', '2', '200', '10', 2023, now()),
		(NULL, '1', '2', '200', '11', 2023, now()),
		(NULL, '1', '2', '200', '12', 2023, now()),
		(NULL, '4', '2', '200', '1', 2023, now()),
		(NULL, '4', '2', '200', '2', 2023, now()),
		(NULL, '4', '2', '200', '3', 2023, now()),
		(NULL, '4', '2', '200', '4', 2023, now()),
		(NULL, '4', '2', '200', '5', 2023, now()),
		(NULL, '4', '2', '200', '6', 2023, now()),
		(NULL, '4', '2', '200', '7', 2023, now()),
		(NULL, '4', '2', '200', '8', 2023, now()),
		(NULL, '4', '2', '200', '9', 2023, now()),
		(NULL, '4', '2', '200', '10', 2023, now()),
		(NULL, '4', '2', '200', '11', 2023, now()),
		(NULL, '4', '2', '200', '12', 2023, now()),
		/**2022**/
		(NULL, '1', '1', '200', '1', 2022, now()),
		(NULL, '1', '1', '200', '2', 2022, now()),
		(NULL, '1', '1', '200', '3', 2022, now()),
		(NULL, '1', '1', '200', '4', 2022, now()),
		(NULL, '1', '1', '200', '5', 2022, now()),
		(NULL, '1', '1', '200', '6', 2022, now()),
		(NULL, '1', '1', '200', '7', 2022, now()),
		(NULL, '1', '1', '200', '8', 2022, now()),
		(NULL, '1', '1', '200', '9', 2022, now()),
		(NULL, '1', '1', '200', '10', 2022, now()),
		(NULL, '1', '1', '200', '11', 2022, now()),
		(NULL, '1', '1', '200', '12', 2022, now()),
		(NULL, '2', '1', '250', '1', 2022, now()),
		(NULL, '2', '1', '250', '2', 2022, now()),
		(NULL, '2', '1', '250', '3', 2022, now()),
		(NULL, '2', '1', '250', '4', 2022, now()),
		(NULL, '2', '1', '250', '5', 2022, now()),
		(NULL, '2', '1', '250', '6', 2022, now()),
		(NULL, '2', '1', '250', '7', 2022, now()),
		(NULL, '2', '1', '250', '8', 2022, now()),
		(NULL, '2', '1', '250', '9', 2022, now()),
		(NULL, '2', '1', '250', '10', 2022, now()),
		(NULL, '2', '1', '250', '11', 2022, now()),
		(NULL, '2', '1', '250', '12', 2022, now()),
		(NULL, '3', '1', '250', '1', 2022, now()),
		(NULL, '3', '1', '250', '2', 2022, now()),
		(NULL, '3', '1', '250', '3', 2022, now()),
		(NULL, '3', '1', '250', '4', 2022, now()),
		(NULL, '3', '1', '250', '5', 2022, now()),
		(NULL, '3', '1', '250', '6', 2022, now()),
		(NULL, '3', '1', '250', '7', 2022, now()),
		(NULL, '3', '1', '250', '8', 2022, now()),
		(NULL, '3', '1', '250', '9', 2022, now()),
		(NULL, '3', '1', '250', '10', 2022, now()),
		(NULL, '3', '1', '250', '11', 2022, now()),
		(NULL, '3', '1', '250', '12', 2022, now()),
		(NULL, '1', '2', '200', '1', 2022, now()),
		(NULL, '1', '2', '200', '2', 2022, now()),
		(NULL, '1', '2', '200', '3', 2022, now()),
		(NULL, '1', '2', '200', '4', 2022, now()),
		(NULL, '1', '2', '200', '5', 2022, now()),
		(NULL, '1', '2', '200', '6', 2022, now()),
		(NULL, '1', '2', '200', '7', 2022, now()),
		(NULL, '1', '2', '200', '8', 2022, now()),
		(NULL, '1', '2', '200', '9', 2022, now()),
		(NULL, '1', '2', '200', '10', 2022, now()),
		(NULL, '1', '2', '200', '11', 2022, now()),
		(NULL, '1', '2', '200', '12', 2022, now()),
		(NULL, '4', '2', '200', '1', 2022, now()),
		(NULL, '4', '2', '200', '2', 2022, now()),
		(NULL, '4', '2', '200', '3', 2022, now()),
		(NULL, '4', '2', '200', '4', 2022, now()),
		(NULL, '4', '2', '200', '5', 2022, now()),
		(NULL, '4', '2', '200', '6', 2022, now()),
		(NULL, '4', '2', '200', '7', 2022, now()),
		(NULL, '4', '2', '200', '8', 2022, now()),
		(NULL, '4', '2', '200', '9', 2022, now()),
		(NULL, '4', '2', '200', '10', 2022, now()),
		(NULL, '4', '2', '200', '11', 2022, now()),
		(NULL, '4', '2', '200', '12', 2022, now()),
		/**2021**/
		(NULL, '1', '1', '200', '1', 2021, now()),
		(NULL, '1', '1', '200', '2', 2021, now()),
		(NULL, '1', '1', '200', '3', 2021, now()),
		(NULL, '2', '1', '250', '1', 2021, now()),
		(NULL, '3', '1', '250', '1', 2021, now()),
		(NULL, '3', '1', '250', '2', 2021, now()),
		(NULL, '1', '2', '200', '1', 2021, now()),
		(NULL, '1', '2', '200', '2', 2021, now()),
		(NULL, '1', '2', '200', '3', 2021, now()),
		(NULL, '4', '2', '200', '1', 2021, now()),
		(NULL, '4', '2', '200', '2', 2021, now());
		
INSERT INTO `school_sport_clubs`.`salaryPayments` 
VALUES	(NULL, '1', 1, 2024, 2500, now()),
		(NULL, '1', 2, 2024, 2500, now()),
		(NULL, '1', 3, 2024, 2500, now()),
		(NULL, '1', 4, 2024, 2500, now()),
		(NULL, '2', 1, 2024, 3600, now()),
		(NULL, '2', 2, 2024, 3600, now()),
		(NULL, '2', 3, 2024, 3600, now()),
		(NULL, '2', 4, 2024, 3600, now()),
		(NULL, '3', 1, 2024, 1500, now()),
		(NULL, '3', 2, 2024, 1500, now()),
		(NULL, '3', 3, 2024, 1500, now()),
		(NULL, '3', 4, 2024, 1500, now()),
		(NULL, '4', 1, 2024, 900, now()),
		(NULL, '4', 2, 2024, 900, now()),
		(NULL, '4', 3, 2024, 900, now()),
		(NULL, '5', 1, 2024, 1200, now()),
		(NULL, '5', 2, 2024, 1200, now()),
		(NULL, '5', 3, 2024, 1200, now()),
		(NULL, '5', 4, 2024, 1200, now()),
		(NULL, '6', 1, 2024, 5500, now()),
		(NULL, '6', 2, 2024, 5500, now()),
		(NULL, '6', 3, 2024, 5500, now()),
		(NULL, '6', 4, 2024, 5500, now());
        
UPDATE `school_sport_clubs`.`coaches` SET `month_salary`='2200', `hour_salary`='24' WHERE `id`='1';
UPDATE `school_sport_clubs`.`coaches` SET `month_salary`='2300', `hour_salary`='25' WHERE `id`='2';
UPDATE `school_sport_clubs`.`coaches` SET `month_salary`='2800', `hour_salary`='28' WHERE `id`='3';
UPDATE `school_sport_clubs`.`coaches` SET `month_salary`='3000', `hour_salary`='30' WHERE `id`='4';
UPDATE `school_sport_clubs`.`coaches` SET `month_salary`='2450', `hour_salary`='26' WHERE `id`='5';

INSERT INTO `school_sport_clubs`.`coach_work` (`coach_id`, `group_id`, `number_of_hours`, `date`) VALUES ('1', '1', '2', '2023-03-07 08:45:55');
INSERT INTO `school_sport_clubs`.`coach_work` (`coach_id`, `group_id`, `number_of_hours`, `date`) VALUES ('1', '1', '2', '2023-03-14 08:45:55');
INSERT INTO `school_sport_clubs`.`coach_work` (`coach_id`, `group_id`, `number_of_hours`, `date`) VALUES ('1', '1', '2', '2023-03-21 08:45:55');
INSERT INTO `school_sport_clubs`.`coach_work` (`coach_id`, `group_id`, `number_of_hours`, `date`) VALUES ('1', '1', '2', '2023-03-28 08:45:55');
INSERT INTO `school_sport_clubs`.`coach_work` (`coach_id`, `group_id`, `number_of_hours`, `date`) VALUES ('1', '1', '2', '2023-04-04 08:45:55');
INSERT INTO `school_sport_clubs`.`coach_work` (`coach_id`, `group_id`, `number_of_hours`, `date`) VALUES ('1', '1', '2', '2023-04-11 08:45:55');
INSERT INTO `school_sport_clubs`.`coach_work` (`coach_id`, `group_id`, `number_of_hours`, `date`) VALUES ('2', '2', '2', '2023-03-07 08:45:55');
INSERT INTO `school_sport_clubs`.`coach_work` (`coach_id`, `group_id`, `number_of_hours`, `date`) VALUES ('2', '2', '2', '2023-03-14 08:45:55');
INSERT INTO `school_sport_clubs`.`coach_work` (`coach_id`, `group_id`, `number_of_hours`, `date`) VALUES ('2', '2', '2', '2023-03-21 08:45:55');
INSERT INTO `school_sport_clubs`.`coach_work` (`coach_id`, `group_id`, `number_of_hours`, `date`) VALUES ('2', '2', '2', '2023-03-28 08:45:55');
INSERT INTO `school_sport_clubs`.`coach_work` (`coach_id`, `group_id`, `number_of_hours`, `date`) VALUES ('2', '2', '2', '2023-04-04 08:45:55');
INSERT INTO `school_sport_clubs`.`coach_work` (`coach_id`, `group_id`, `number_of_hours`, `date`) VALUES ('2', '2', '2', '2023-04-11 08:45:55');
INSERT INTO `school_sport_clubs`.`coach_work` (`coach_id`, `group_id`, `number_of_hours`, `date`) VALUES ('2', '3', '2', '2023-04-02 08:45:55');
INSERT INTO `school_sport_clubs`.`coach_work` (`coach_id`, `group_id`, `number_of_hours`, `date`) VALUES ('2', '3', '2', '2023-04-09 08:45:55');

DROP TRIGGER if exists after_salarypayment_update;
delimiter |
CREATE TRIGGER after_salarypayment_update AFTER UPDATE ON salarypayments
FOR EACH ROW 
BEGIN
INSERT INTO salarypayments_log(operation,
old_coach_id,
new_coach_id,
old_month,
new_month,
old_year,
new_year,
old_salaryAmount,
new_salaryAmount,
old_dateOfPayment,
new_dateOfPayment,
dateOfLog)
VALUES ('UPDATE',
OLD.coach_id,
CASE NEW.coach_id WHEN OLD.coach_id THEN NULL ELSE NEW.coach_id END,
OLD.month,
CASE NEW.month WHEN OLD.month THEN NULL ELSE NEW.month END,
OLD.year,
CASE NEW.year WHEN OLD.year THEN NULL ELSE NEW.year END,
OLD.salaryAmount,
CASE NEW.salaryAmount WHEN OLD.salaryAmount THEN NULL ELSE NEW.salaryAmount END,
OLD.dateOfPayment,
CASE NEW.dateOfPayment WHEN OLD.dateOfPayment THEN NULL ELSE NEW.dateOfPayment END,
NOW());
END
|
Delimiter ;

SELECT * FROM salarypayments_log;

UPDATE school_sport_clubs.salarypayments SET month='4' WHERE id='15';

DROP TRIGGER if exists after_salarypayment_delete;
delimiter |
CREATE TRIGGER after_salarypayment_delete AFTER DELETE ON salarypayments
FOR EACH ROW 
BEGIN
INSERT INTO salarypayments_log(operation,
old_coach_id,
new_coach_id,
old_month,
new_month,
old_year,
new_year,
old_salaryAmount,
new_salaryAmount,
old_dateOfPayment,
new_dateOfPayment,
dateOfLog)
VALUES ('DELETE',
OLD.coach_id,
OLD.month,
OLD.year,
OLD.salaryAmount,
OLD.dateOfPayment,
NOW());
END
|
Delimiter ;

DROP TRIGGER if exists after_salarypayment_delete_recovery;
delimiter |
CREATE TRIGGER after_salarypayment_delete_recovery AFTER DELETE ON salarypayments
FOR EACH ROW 
BEGIN
INSERT INTO salarypayments(coach_id,
	month,
	year,
	salaryAmount,
	dateOfPayment) FROM salarypayments_log
VALUES ('RECOVER',
old_coach_id,
old_month,
old_year,
old_salaryAmount,
old_dateOfPayment,
dateOfLog);
END
|
Delimiter ;

DROP TRIGGER if exists restriction_on_student_training_in_more_groups;
delimiter |
CREATE TRIGGER restriction_on_student_training_in_more_groups 
	BEFORE UPDATE ON sportgroups
FOR EACH ROW 
BEGIN
	IF (SELECT COUNT(id) FROM students
	JOIN student_sport stud_sp ON students.id = stud_sp.student_id
	JOIN sportgroups sp_gr ON stud_sp.sportGroup_id = sp_gr.id) > 2
	END IF 


END
|
Delimiter ;

CREATE VIEW 

use school_sport_clubs;

DROP TRIGGER if exists after_salarypayment_delete;
delimiter |
CREATE TRIGGER after_salarypayment_delete AFTER DELETE ON salarypayments
FOR EACH ROW 
BEGIN
INSERT INTO salarypayments_log(
	operation,
	old_coach_id,
	new_coach_id,
	old_month,
	new_month,
	old_year,
	new_year,
	old_salaryAmount,
	new_salaryAmount,
	old_dateOfPayment,
	new_dateOfPayment,
	dateOfLog
)
VALUES (
	'DELETE',
	OLD.coach_id,
    NULL,
	OLD.month,
    NULL,
	OLD.year,
    NULL,
	OLD.salaryAmount,
    NULL,
	OLD.dateOfPayment,
    NULL,
	NOW()
);
END;
|
Delimiter ;

INSERT INTO salarypayments (coach_id, month, year, salaryAmount, dateOfPayment)
select old_coach_id as coach_id, old_month as month, old_year as year, old_salaryAmount as salaryAmount,
       old_dateOfPayment as dateOfPayment from salarypayments_log
where salarypayments_log.operation = "DELETE";

DROP TRIGGER if exists before_student_sport_insert;
delimiter |
CREATE TRIGGER before_student_sport_insert BEFORE INSERT ON student_sport
FOR EACH ROW 
BEGIN
	IF ((SELECT COUNT(sportGroup_id) from student_sport where student_id = NEW.student_id) >= 2)
    THEN 
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'The student is already registered in 2 groups.';
    end if;
END
|
delimiter ;

--=============================================================================

DROP TRIGGER if exists after_salarypayment_delete;
delimiter |
CREATE TRIGGER after_salarypayment_delete AFTER DELETE ON salarypayments
FOR EACH ROW 
BEGIN
INSERT INTO salarypayments_log(
	operation,
	old_coach_id,
	new_coach_id,
	old_month,
	new_month,
	old_year,
	new_year,
	old_salaryAmount,
	new_salaryAmount,
	old_dateOfPayment,
	new_dateOfPayment,
	dateOfLog
)
VALUES (
	'DELETE',
	OLD.coach_id,
    NULL,
	OLD.month,
    NULL,
	OLD.year,
    NULL,
	OLD.salaryAmount,
    NULL,
	OLD.dateOfPayment,
    NULL,
	NOW()
);
END;
|
Delimiter ;

INSERT INTO salarypayments (coach_id, month, year, salaryAmount, dateOfPayment)
select old_coach_id as coach_id, old_month as month,
	old_year as year, old_salaryAmount as salaryAmount,
    old_dateOfPayment as dateOfPayment from salarypayments_log
where salarypayments_log.operation = "DELETE";

DROP TRIGGER if exists before_student_sport_insert;
delimiter |
CREATE TRIGGER before_student_sport_insert BEFORE INSERT ON student_sport
FOR EACH ROW 
BEGIN
	IF ((SELECT COUNT(sportGroup_id) from student_sport where student_id = NEW.student_id) >= 2)
    THEN 
		SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT = 'The student is already registered in 2 groups.';
    end if;
END
|
delimiter ;

SELECT sportGroup_id from student_sport where student_id = 1;
insert into student_sport(student_id, sportGroup_id) VALUES (1, 1);
insert into student_sport(student_id, sportGroup_id) VALUES (1, 2);
insert into student_sport(student_id, sportGroup_id) VALUES (1, 3);
insert into student_sport(student_id, sportGroup_id) VALUES (1, 4);
insert into student_sport(student_id, sportGroup_id) VALUES (1, 5);
insert into student_sport(student_id, sportGroup_id) VALUES (1, 6);

create view view4 as
select students.name, count(student_sport.sportGroup_id) 
from students 
join student_sport on student_sport.student_id = students.id
group by students.id;

DROP PROCEDURE if exists proc5;
delimiter |
create procedure proc5(in coach_name VARCHAR(100))
begin
	select student_sport.sportGroup_id, sports.name, students.name
    from coaches
    join sportgroups on sportgroups.coach_id = coaches.id
    join sports on sports.id = sportgroups.sport_id
    join student_sport on student_sport.sportGroup_id = sportgroups.id
    join students on students.id = student_sport.student_id
    where coaches.name = coach_name;
end |
delimiter ;

DROP PROCEDURE if exists proc6;
delimiter |
create procedure proc6(in sport_name VARCHAR(100))
begin
	select coaches.name, sportgroups.location, 
	sportgroups.location, sportgroups.hourOfTraining, sportgroups.dayOfWeek
    from sports
    join sportgroups on sports.id = sportgroups.sport_id
    join coaches on sportgroups.coach_id = coaches.id
    where sports.name = sport_name;
end |
delimiter ;
call task6("Football");

