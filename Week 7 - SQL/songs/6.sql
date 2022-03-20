SELECT name
FROM songs
WHERE (
    SELECT id
    FROM artists
    WHERE name = "Post Malone"
) = artist_id;