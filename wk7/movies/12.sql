SELECT title FROM movies
   ...> JOIN stars ON movies.id = movie_id
   ...> JOIN people ON stars.person_id = people.id
   ...> WHERE name IN ('Bradley Cooper', 'Jennifer Lawrence');
   GROUP BY title
   HAVING COUNT(DISTINCT name) = 2;
