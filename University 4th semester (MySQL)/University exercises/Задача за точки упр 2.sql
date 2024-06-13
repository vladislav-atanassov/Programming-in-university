DROP DATABASE IF EXISTS news;
CREATE DATABASE news;
USE news;

CREATE TABLE types_of_news(
	id INT AUTO_INCREMENT PRIMARY KEY,
    name ENUM('Crimes', 'Sports')
);

CREATE TABLE video_section(
	id INT AUTO_INCREMENT PRIMARY KEY,
	name VARCHAR(255),
    link VARCHAR(255)
);

CREATE TABLE videos(
	id INT AUTO_INCREMENT PRIMARY KEY,
    video_id INT NOT NULL,
    type_of_news_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (video_id) REFERENCES video_section(id),
    CONSTRAINT FOREIGN KEY (type_of_news_id) REFERENCES types_of_news(id)
);

CREATE TABLE image_section(
	id INT AUTO_INCREMENT PRIMARY KEY,
	name VARCHAR(255),
    link VARCHAR(255)
);

CREATE TABLE images(
	id INT AUTO_INCREMENT PRIMARY KEY,
	image_id INT NOT NULL,
    type_of_news_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (image_id) REFERENCES image_section(id),
	CONSTRAINT FOREIGN KEY (type_of_news_id) REFERENCES types_of_news(id)
);

CREATE TABLE comment_section(
	id INT AUTO_INCREMENT PRIMARY KEY,
    comment TEXT
);

CREATE TABLE comments(
	id INT AUTO_INCREMENT PRIMARY KEY,
    comment_id INT NOT NULL, 
    type_of_news_id INT NOT NULL,
    comment TEXT,
    CONSTRAINT FOREIGN KEY (comment_id) REFERENCES comment_section(id),
    CONSTRAINT FOREIGN KEY (type_of_news_id) REFERENCES types_of_news(id)
);

CREATE TABLE user_roles(
	id INT AUTO_INCREMENT PRIMARY KEY,
    name ENUM('User', 'Admin', 'Author')
);

CREATE TABLE users(
	id INT AUTO_INCREMENT PRIMARY KEY,
	name VARCHAR(255) NOT NULL ,
	address VARCHAR(255) NOT NULL ,
	phone VARCHAR(20) NULL DEFAULT NULL ,
    password VARCHAR(24) NOT NULL,
    added_by_admin INT NOT NULL,
	role_id INT NOT NULL,
    image_id INT NOT NULL,
    video_id INT NOT NULL,
    comment_id INT NOT NULL,
    CONSTRAINT FOREIGN KEY (role_id) REFERENCES user_roles(id),
    CONSTRAINT FOREIGN KEY (image_id) REFERENCES image_section(id),
    CONSTRAINT FOREIGN KEY (video_id) REFERENCES video_section(id),
    CONSTRAINT FOREIGN KEY (comment_id) REFERENCES comment_section(id)
);

DELIMITER |
DROP TRIGGER IF EXISTS validation_before_insert_of_user;
CREATE TRIGGER validation_before_insert_of_user BEFORE INSERT ON news.users
BEGIN



END |
DELIMITER ;