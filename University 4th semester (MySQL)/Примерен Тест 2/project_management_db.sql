DROP DATABASE IF EXISTS project_managment;
CREATE DATABASE project_managment;
USE project_managment;

CREATE TABLE department(
    id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
    name VARCHAR(100) NOT NULL,
    location VARCHAR(100) NOT NULL
);

CREATE TABLE employee(
    id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
    firstName VARCHAR(50) NOT NULL,
    lastName VARCHAR(50) NOT NULL,
    dept_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (dept_id) REFERENCES department(id)
);

CREATE TABLE project(
    id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
    name VARCHAR(255) NOT NULL,
    budget DECIMAL(10, 2) NOT NULL,
    priority INT NOT NULL,
    isFinished BOOLEAN NOT NULL DEFAULT FALSE,
    startDate DATE NOT NULL,
    endDate DATE NOT NULL 
);

CREATE TABLE task(
    tackDescr TEXT NOT NULL,
    hours INT NOT NULL,
    month INT NOT NULL,
    year INT NOT NULL,
    empl_id INT NOT NULL,
    project_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (empl_id) REFERENCES employee(id),
    CONSTRAINT FOREIGN KEY (project_id) REFERENCES project(id)
);

CREATE TABLE bonuses(
    id INT AUTO_INCREMENT PRIMARY KEY NOT NULL,
    totalHours INT NULL DEFAULT NULL,
    bonusPoints DOUBLE DEFAULT 0,
    amount DECIMAL(10, 2) DEFAULT 0,
    month INT NOT NULL,
    year INT NOT NULL,
    empl_id INT NOT NULL,
    CONSTRAINT UNIQUE (month, year, empl_id),
        FOREIGN KEY (empl_id) REFERENCES employee(id)
);
