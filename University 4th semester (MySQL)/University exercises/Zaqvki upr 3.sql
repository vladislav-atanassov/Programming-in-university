USE school_sport_clubs;

-- DONE
SELECT DISTINCT students.name, sports.name
FROM students JOIN student_sport
ON students.id = student_sport.student_id
JOIN sportgroups 
ON student_sport.sportGroup_id = sportgroups.id
JOIN sports
ON sportgroups.sport_id = sports.id;

-- DONE
SELECT students.name, students.class, student_sport.sportGroup_id
FROM students JOIN student_sport
ON students.id = student_sport.student_id
JOIN sportgroups
ON student_sport.sportGroup_id = sportgroups.sport_id AND sportgroups.dayOfWeek = 'Monday';

-- DONE
SELECT coaches.name, sports.name
FROM coaches JOIN sports
ON coaches.id IN(
SELECT coach_id FROM sportgroups
WHERE sportgroups.sport_id = sports.id AND sports.name = 'Football');

-- DONE
SELECT sportgroups.location, sportgroups.hourOfTraining, sportgroups.dayOfWeek
FROM sportgroups JOIN sports
WHERE sports.name = 'Volleyball';

-- DONE
SELECT students.name, sports.name
FROM students JOIN student_sport
ON students.id = student_sport.student_id
JOIN sportgroups 
ON student_sport.sportGroup_id = sportgroups.id
JOIN sports
ON sportgroups.sport_id= sports.id AND students.name = 'Iliyan Ivanov';

-- DONE
SELECT students.name
FROM students JOIN student_sport
ON students.id = student_sport.student_id
JOIN sportgroups 
ON student_sport.sportGroup_id = sportgroups.id
JOIN sports
ON sportgroups.sport_id= sports.id AND sports.name = 'Football'
JOIN coaches 
ON sportgroups.coach_id = coaches.id AND coaches.name = 'Ivan Todorov Petkov';


