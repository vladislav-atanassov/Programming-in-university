SELECT count(*) FROM sportGroups;

-- Executes the function to group 1
SELECT group_id AS GroupID, SUM(paymentAmount) AS payment
FROM taxesPayments
WHERE group_id = 1;

-- Executes the function on all of the id's (GROUP BY)
SELECT group_id AS GroupID, SUM(paymentAmount) AS payment
FROM taxesPayments
GROUP BY group_id
HAVING payment > 11000; -- Using having to limit the output / final filtration

CREATE VIEW sportPlace
AS
SELECT s.name AS Sport, sg.location AS Place
FROM sports AS s JOIN sportGroups as sg 
ON s.id = sg.sport_id;
-- Deletes with DROP VIEW name_of_view

DROP DATABASE IF EXISTS cinema;
CREATE DATABASE cinema;
USE cinema;

CREATE TABLE place (
    id INT AUTO_INCREMENT PRIMARY KEY,
    number_of_spectators INT NULL
);

CREATE TABLE film (
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    year_of_production YEAR NOT NULL,
    country VARCHAR(255) NOT NULL
);

CREATE TABLE place_to_film (
    place_id INT NOT NULL,
    film_id INT NOT NULL,
    FOREIGN KEY (place_id) REFERENCES place(id),
    FOREIGN KEY (film_id) REFERENCES film(id),
    PRIMARY KEY (place_id, film_id)
);

CREATE TABLE place_info (
    time_of_show DATETIME NOT NULL,
    name VARCHAR(50) NOT NULL,
    location VARCHAR(255) NOT NULL,
    status ENUM('Deluxe', 'VIP', 'Standard'), 
    PRIMARY KEY (place_id, film_id),
    place_id INT NOT NULL,
    film_id INT NOT NULL,
    FOREIGN KEY (place_id) REFERENCES place(id),
    FOREIGN KEY (film_id) REFERENCES film(id)
);

SELECT place_info.name, place_info.place_id, place_info.time_of_show
FROM place_info 
JOIN film 
ON place_info.film_id = film.id 
WHERE film.name = 'Final Destination 7'
AND (place_info.status = 'Deluxe' OR place_info.status = 'VIP')
ORDER BY place_info.name, place_info.place_id;

SELECT place.number_of_spectators
FROM place JOIN place_info
ON place.id = place_info.place_id
JOIN film
ON place_info.film_id = film.id
WHERE film.name = 'Final Destination 7' 
AND (place_info.status = 'VIP' AND place_info.name = 'Arena Maldost');


-- school_sport_clubs 

USE school_sport_clubs;

SELECT firstStud.name AS Student1, secondStud.name AS Student2, sports.name AS Sport
FROM students AS firstStud JOIN students AS secondStud
ON firstStud.id > secondStud.id
JOIN sports ON(
	firstStud.id IN( -- We want the students to train in the same group
		SELECT student_id
        FROM student_sport
        WHERE sportGroup_id IN(
			SELECT id FROM sportgroups	
            WHERE sportgroups.sport_id = sports.id IN(
                SELECT name FROM sports
                WHERE sports.name = 'Football'
            ))
					)
AND (secondStud.id IN( -- Same nested SELECTs as the above 
	SELECT student_id
    FROM student_sport
	WHERE sportGroup_id IN(
		SELECT id FROM sportgroups	
		WHERE sportgroups.sport_id = sports.id IN(
                SELECT name FROM sports
                WHERE sports.name = 'Football'
            ))
					)		)
) WHERE firstStud.id IN(
	SELECT student_id
	FROM student_sport
    WHERE sportGroup_id IN(
		SELECT sportGroup_id
		FROM student_sport
        WHERE student_id = secondStud.id)
) ORDER BY Sport;

CREATE VIEW sportAtEight
AS
SELECT students.name AS Students_name, students.class, sportgroups.location, coaches.name AS Coaches_name
FROM students JOIN student_sport
ON students.id = student_sport.student_id
JOIN sportgroups
ON student_sport.sportGroup_id = sportgroups.id
JOIN coaches
ON sportgroups.coach_id = coaches.id
JOIN sports
ON sportgroups.sport_id = sports.id 
WHERE sportgroups.hourOfTraining = '08:00:00';

-- DONE
SELECT sports.name, COUNT(students.id) FROM students
JOIN student_sport
ON students.id = student_sport.student_id
JOIN sportgroups 
ON student_sport.sportGroup_id = sportgroups.id
JOIN sports
ON sportgroups.sport_id = sports.id
GROUP BY sports.id;

