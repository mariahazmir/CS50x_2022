SELECT AVG(energy)
FROM songs
WHERE (
    SELECT id
    FROM artists
    WHERE name = "Drake"
) = artist_id;