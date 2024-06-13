DROP DATABASE IF EXISTS films;
CREATE DATABASE films;
USE films;

CREATE TABLE studios(
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    address VARCHAR(255) NOT NULL,
    bulstad VARCHAR(255) NOT NULL,
    producer VARCHAR(255) NOT NULL
);

CREATE TABLE actors(
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    address VARCHAR(255) NOT NULL,
    gender ENUM('male', 'female') NOT NULL,
    birth DATE NOT NULL,
    studio_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (studio_id) REFERENCES studios(id)
); 

CREATE TABLE films.film(
    id INT AUTO_INCREMENT PRIMARY KEY,
    title VARCHAR(100) NOT NULL,
    year_of_production YEAR NOT NULL,
    length_minutes INT NOT NULL,
    producer VARCHAR(100) NOT NULL,
    budget DECIMAL NOT NULL,
    actor_id INT NOT NULL,
    studio_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (actor_id) REFERENCES actors(id),
    CONSTRAINT FOREIGN KEY (studio_id) REFERENCES studios(id)
);

SELECT actors.name AS ActorName
FROM actors 
WHERE actors.gender = 'male';

SELECT film.title FROM film
WHERE film.year_of_production > 1990 
AND film.year_of_production < 2000
ORDER BY film.budget
LIMIT 3;

SELECT film.title AS FilmTitle, actors.name AS ActorName
FROM film JOIN actors
ON film.actor_id = actors.id
JOIN studios
ON actors.studio_id = studios.id
WHERE studios.name = 'IFS-200';

SELECT film.title AS FilmTitle, 
    studios.name AS StudioName, 
    studios.producer AS ProducerName
FROM film JOIN studios
ON film.studio_id = studios.id;

SELECT actors.name, film.title, film.budget
FROM actors JOIN film
ON actors.id = film.actor_id
ORDER BY film.budget;

SELECT actors.name AS ActorName, AVG(film.length_minutes) AS AverageLength
FROM actors JOIN film 
ON actors.id = film.actor_id
WHERE film.length_minutes > (
    SELECT AVG(length_minutes)
    FROM film
    WHERE year_of_production < 2000
)
GROUP BY actors.id;

SELECT actors.name 
FROM actors JOIN film 
ON actors.id = film.actor_id
GROUP BY actors.id
HAVING COUNT(film.id) = 0;

SELECT actors.name 
FROM actors JOIN film 
ON actors.id = film.actor_id
GROUP BY actors.id
ORDER BY COUNT(film.id)
LIMIT 1;

SELECT actors.name 
FROM actors JOIN film 
ON actors.id = film.actor_id
WHERE actors.gender = 'male'
GROUP BY actors.id
ORDER BY actors.birth ASC, COUNT(film.id) DESC
LIMIT 1;
-- Or
SELECT actors.name 
FROM actors JOIN film 
ON actors.id = film.actor_id
WHERE actors.gender = 'male'
AND actors.birth = (
    SELECT MIN(birth) 
    FROM actors 
    WHERE gender = 'male'
)
GROUP BY actors.id
ORDER BY COUNT(film.id) DESC
LIMIT 1;