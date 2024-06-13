USE school_sport_clubs;

-- Full JOIN
(SELECT s.name as Sport, sg.location as Place
FROM sports as s LEFT JOIN sportgroups as sg
ON s.id = sg.sport_id) 
UNION
(SELECT s.name as Sport, sg.location as Place
FROM sports as s RIGHT JOIN sportgroups as sg
ON s.id = sg.sport_id);

-- Self JOIN
SELECT firstStud.name AS Student1, secondStud.name AS Student2, sports.name AS Sport
FROM students AS firstStud JOIN students AS secondStud
ON firstStud.id > secondStud.id
JOIN sports ON(
	firstStud.id IN( -- We want the students to train in the same group
		SELECT student_id
        FROM student_sport
        WHERE sportGroup_id IN(
			SELECT id FROM sportgroups	
            WHERE sportgroups.sport_id = sports.id)
					)
AND (secondStud.id IN( -- Same nested SELECTs as the above 
	SELECT student_id
    FROM student_sport
	WHERE sportGroup_id IN(
		SELECT id FROM sportgroups	
		WHERE sportgroups.sport_id = sports.id)
					)		)
) WHERE firstStud.id IN(
	SELECT student_id
	FROM student_sport
    WHERE sportGroup_id IN(
		SELECT sportGroup_id
		FROM student_sport
        WHERE student_id = secondStud.id)
) ORDER BY Sport;

-- DONE
SELECT students.name, students.class, students.phone
FROM students JOIN student_sport
ON students.id = student_sport.student_id
JOIN sportgroups 
ON student_sport.sportGroup_id = sportgroups.id
JOIN sports
ON sportgroups.sport_id = sports.id 
WHERE sports.name = 'Football';
/* OR
SELECT students.name, students.class, students.phone
FROM students WHERE students.id IN (
	SELECT student_id FROM student_sport
		WHERE student_sport.sportgroup_id IN (
		SELECT id FROM sportgroups
		WHERE sportgroups.id = student_sport.sportgroup_id
        AND sportgroups.sport_id IN (
			SELECT id FROM sports
			WHERE sports.name = 'Football')));
*/

-- DONE 
SELECT DISTINCT coaches.name
FROM coaches JOIN sportgroups
ON coaches.id = sportgroups.coach_id
JOIN sports
ON sports.id = sportgroups.sport_id 
WHERE sports.name = 'Volleyball';
/* OR
SELECT coaches.name
FROM coaches JOIN sports
ON coaches.id IN(
	SELECT coach_id FROM sportgroups
	WHERE sports.name = 'Volleyball');
*/

-- DONE
SELECT DISTINCT coaches.name, sports.name
FROM coaches JOIN sportgroups
ON coaches.id = sportgroups.coach_id
JOIN sports
ON sportgroups.sport_id = sports.id
JOIN student_sport
ON sportgroups.id = student_sport.student_id
JOIN students
ON student_sport.student_id = students.id 
WHERE students.name = 'Iliyan Ivanov';
-- //TODO with SELECT

-- DONE
SELECT students.name, students.class, sportgroups.location, coaches.name
FROM students JOIN student_sport
ON students.id = student_sport.student_id
JOIN sportgroups
ON student_sport.sportGroup_id = sportgroups.id
JOIN coaches
ON sportgroups.coach_id = coaches.id
JOIN sports
ON sportgroups.sport_id = sports.id 
WHERE sportgroups.hourOfTraining = '08:00:00';
-- //TODO with nested SELECT

    DROP DATABASE IF EXISTS hospital;
CREATE DATABASE hospital;
USE hospital;

CREATE TABLE hospital.doctors(
    id INT AUTO_INCREMENT PRIMARY KEY,
    firstName VARCHAR(50) NOT NULL,
    secondName VARCHAR(50) NOT NULL,
    lastName VARCHAR(50) NOT NULL,
    room VARCHAR(10) NOT NULL,
    specialization VARCHAR(50) NOT NULL,
    working_with_NHC BOOLEAN NOT NULL,
    phone VARCHAR(15) NOT NULL,
    email VARCHAR(50) NOT NULL
);

CREATE TABLE hospital.patients(
    id INT AUTO_INCREMENT PRIMARY KEY,
    firstName VARCHAR(50) NOT NULL,
    secondName VARCHAR(50) NOT NULL,
    lastName VARCHAR(50) NOT NULL,
    address VARCHAR(255) NOT NULL,
    egn CHAR(10) NOT NULL
);

CREATE TABLE hospital.doctor_to_patient(
    doctor_id INT NOT NULL,
    patient_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (doctor_id) REFERENCES doctors(id),    
    CONSTRAINT FOREIGN KEY (patient_id) REFERENCES patients(id),
    PRIMARY KEY(doctor_id, patient_id)
);

CREATE TABLE hospital.diagnoses(
    id INT AUTO_INCREMENT PRIMARY KEY,
    patient_id INT NOT NULL,
    diagnose TEXT NULL,
    CONSTRAINT FOREIGN KEY (patient_id) REFERENCES patients(id)
);

CREATE TABLE hospital.period_of_treatment(
    id INT AUTO_INCREMENT PRIMARY KEY,
    patient_id INT NOT NULL,
    period INT NOT NULL,
    CONSTRAINT FOREIGN KEY (patient_id) REFERENCES patients(id)
);

CREATE TABLE hospital.prescribed_treatments(
    id INT AUTO_INCREMENT PRIMARY KEY,
    patient_id INT NOT NULL,
    treatment TEXT NULL,
    CONSTRAINT FOREIGN KEY (patient_id) REFERENCES patients(id)
);

