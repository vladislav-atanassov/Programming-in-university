DROP DATABASE IF EXISTS vet;
 
CREATE DATABASE vet;
 
USE vet;
 
CREATE TABLE pet_owners (
	id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(255) NOT NULL,
    phone_number CHAR(10) NOT NULL,
    email VARCHAR(255) NOT NULL
);
 
CREATE TABLE pets (
	id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(255) NOT NULL,
    birth DATE NOT NULL,
    microchip_number CHAR(12) NOT NULL,
    owner_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY(owner_id) REFERENCES pet_owners(id)
);
 
CREATE TABLE vets (
	id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(255) NOT NULL
);
 
CREATE TABLE salaries (
	id INT PRIMARY KEY AUTO_INCREMENT,
    amount INT NOT NULL,
    vet_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY(vet_id) REFERENCES vets(id)
);
 
CREATE TABLE procedures_log (
	id INT PRIMARY KEY AUTO_INCREMENT,
    log_time DATE,
    vet_id INT NOT NULL,
    pet_id INT NOT NULL,
    procidure TEXT NOT NULL,
    CONSTRAINT FOREIGN KEY(pet_id) REFERENCES pets(id),
    CONSTRAINT FOREIGN KEY(vet_id) REFERENCES vets(id)
);

DROP TRIGGER IF EXISTS pet_added_trigger;
DELIMITER //

CREATE TRIGGER pet_added_trigger AFTER INSERT ON pets
FOR EACH ROW
BEGIN
    DECLARE vet_id INT;

    SELECT vets.id INTO vet_id
    FROM vets
    JOIN pet_owners ON pets.owner_id = pet_owners.id
    WHERE pet_owners.id = NEW.owner_id
    LIMIT 1;

    INSERT INTO procedures_log (log_time, vet_id, pet_id, procidure)
    VALUES (NOW(), vet_id, NEW.id, 'Pet added');
END
//
DELIMITER ;

INSERT INTO pet_owners (name, phone_number, email) VALUES ("Sasho", '1234567890', 'oeasnfj');

INSERT INTO pets (name, birth, microchip_number, owner_id) VALUES ('Maxi', '2016-01-15', '123456789222', 2);

SELECT * FROM procedures_log;

DROP PROCEDURE IF EXISTS add_pet;
delimiter |
CREATE PROCEDURE add_pet(IN vet_name VARCHAR(255), 
									IN pet_name VARCHAR(255), 
									IN pet_birth DATE,
                                    IN microchip CHAR(12),
                                    IN owner_name VARCHAR(255),
                                    IN owner_phone_num CHAR(10),
                                    IN owner_email VARCHAR(255),
                                    IN procidure_text TEXT )
PROC:BEGIN

	DECLARE done INT DEFAULT FALSE;
    DECLARE dog_exists INT DEFAULT FALSE;
    DECLARE pet_microchip CHAR(12);
    DECLARE owner_id INT;
	DECLARE vet_id INT;
    DECLARE pet_id INT;
    DECLARE pet_cursor CURSOR FOR 
    SELECT microchip FROM pets;
 
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

    OPEN pet_cursor;
    pet_loop: LOOP
		FETCH pet_cursor INTO pet_microchip;

        IF done THEN 
            LEAVE pet_loop;
        END IF;

		IF (pet_microchip = pets.microchip)
		THEN 
			SET dog_exists = TRUE;
            SIGNAL SQLSTATE '45000' SET MESSAGE_TEXT='Pet already present.';
            LEAVE pet_loop;
		END IF;
    END LOOP;
 
    CLOSE pet_cursor;
 
    SELECT id INTO owner_id FROM pet_owners pw WHERE pw.name = owner_name AND pw.phone_number = owner_phone_num;
    SELECT id INTO vet_id FROM vets WHERE vets.name = vet_name;
    SELECT id INTO pet_id FROM pets WHERE pets.name = pet_name;

    INSERT INTO pet_owners ( name, phone_number, email ) values ( owner_name, owner_phone_num, owner_email );
    INSERT INTO pets ( name, birth, microchip_number, owner_id ) values ( pet_name, pet_birth, microchip, owner_id );
END
|
delimiter ;

CALL add_pet('Dr. Smith', 'Maxi', '2017-01-15', '878997878997', 'Alex', '0888888880', 'alex@fake.com', 'Broken leg operation');
 
SELECT * from pets;
SELECT * FROM pet_owners;
---------------------------------------------------------------------------

DROP PROCEDURE IF EXISTS alter_pet_procedure;
delimiter |
CREATE PROCEDURE alter_pet_procedure(IN vet_name VARCHAR(255), IN pet_name VARCHAR(255), IN procedure_id INT, IN new_text TEXT)
begin
	DECLARE curr_vet_id INT;
    DECLARE curr_pet_id INT;
 
    SELECT id INTO curr_vet_id FROM vets WHERE name = vet_name;
    SELECT id INTO curr_pet_id FROM pets WHERE name = pet_name;
 
    UPDATE procedures_log SET procidure=new_text WHERE vet_id = curr_vet_id AND pet_id = curr_pet_id;
end
|
delimiter ;
 
DROP TRIGGER IF EXISTS log_on_pet_added;
delimiter |
CREATE TRIGGER log_on_pet_added AFTER INSERT ON pets
FOR EACH ROW
BEGIN
END
|
delimiter ;
 
CALL add_pet('Dr. Smith', 'Maxi', '2017-01-15', '878997878997', 'Alex', '0888888880', 'alex@fake.com', 'Broken leg operation');
 
SELECT * from pets;
