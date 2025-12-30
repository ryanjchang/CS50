SELECT AVG(rating) from ratings
   ...> JOIN movies ON movie_id = movies.id
   ...> WHERE year=2012;
