SELECT DISTINCT name
FROM people
JOIN directors ON people.id = directors.person_ID
JOIN ratings ON ratings.movie_id = directors.movie_id
WHERE rating >= 9;