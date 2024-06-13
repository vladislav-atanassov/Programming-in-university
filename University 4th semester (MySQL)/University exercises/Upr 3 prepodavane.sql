USE school_sport_clubs;

/* LEFT takes also the left table values similar with RIGHT*/
SELECT sports.name, sportGroups.location
FROM sports JOIN sportGroups /* JOIN or INNER JOIN joins if both are present*/
ON sports.id= sportGroups.sport_id 
WHERE sportGroups.sport_id IS NULL; /* Takes where there is no match */

/* Variant 1 to join tables with no direct connection*/
SELECT sports.name, coaches.name 
FROM sports JOIN coaches
ON sports.id IN ( /*Using IN to get only unique results*/
SELECT sport_id FROM sportGroups 
WHERE sportGroups.coach_id = coaches.id);

/* Variant 2 to join tables with no direct connection*/
SELECT sports.name, coaches.name /* If we add DISTINCT after SELECT the result will be unique also*/
FROM sports JOIN sportGroups 
ON sports.id= sportGroups.sport_id /*Using the equals operator gets all results (expected more results)*/
JOIN coaches ON sportGroups.coach_id= coaches.id;