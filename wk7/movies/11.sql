SELECT DISTINCT title FROM movies
   ...> JOIN ratings ON movies.id = ratings.movie_id
   ...> JOIN stars ON movies.id = stars.movie_id
   ...> JOIN people ON stars.person_id = people.id
   ...> ORDER BY rating DESC
   ...> WHERE name='Chadwick Boseman'
   ...> LIMIT 5;
