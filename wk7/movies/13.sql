SELECT DISTINCT name FROM people
   ...> JOIN stars ON people.id = stars.person_id
   ...> JOIN movies ON stars.movie_id = movies.id
   ...> WHERE movies.id IN
   ...> (SELECT id FROM movies WHERE name='Kevin Bacon' AND birth=1958);

Given kevin bacons id - 102
SELECT id FROM people
WHERE name='Kevin Bacon' AND birth=1958;

Find movies he starred in
   SELECT id FROM movies
   ...> JOIN stars ON movies.id = stars.movie_id
   ...> WHERE stars.person_id = 102;

Find names of stars who have that movie id
SELECT name FROM people
    JOIN stars ON people.id = stars.person_id
    JOIN movies ON stars.movie_id = movies.id
    WHERE stars.movie_id IN (list of above movie ids)

Exclude Kevin Bacon
AND name!='Kevin Bacon'

Final query:

SELECT DISTINCT name FROM people
   ...> JOIN stars on people.id = stars.person_id
   ...> WHERE stars.movie_id IN
   ...> (SELECT stars.movie_id FROM stars
   ...> JOIN people ON stars.person_id = people.id
   ...> WHERE name='Kevin Bacon' AND birth=1958)
   ...> AND name!='Kevin Bacon';

