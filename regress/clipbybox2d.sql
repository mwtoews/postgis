-- Overlap
SELECT ST_ClipByBox2d(ST_MakeEnvelope(0,0,10,10),ST_MakeEnvelope(5,5,20,20))::box2d;
-- Geom covers rect
SELECT ST_ClipByBox2d(ST_MakeEnvelope(0,0,10,10),ST_MakeEnvelope(5,5,8,8))::box2d;
-- Geom within rect
SELECT ST_ClipByBox2d(ST_MakeEnvelope(2,2,8,8),ST_MakeEnvelope(0,0,10,10))::box2d;
-- Multipoint with point inside, point outside and point on boundary
SELECT ST_AsText(ST_ClipByBox2d('MULTIPOINT(-1 -1, 0 0, 2 2)'::geometry,ST_MakeEnvelope(0,0,10,10)));
-- Invalid polygon (bow-tie) -- ST_Intersection throws an exception here
SELECT ST_AsText(ST_ClipByBox2d('POLYGON((0 0, 10 10, 0 10, 10 0, 0 0))', ST_MakeEnvelope(2,2,8,8)));
-- Invalid polygon (lineal self-intersection) -- ST_Intersection returns a collection
SELECT ST_AsText(ST_ClipByBox2d('POLYGON((0 0,5 4,5 6,0 10,10 10,5 6,5 4,10 0,0 0))', ST_MakeEnvelope(2,2,10,5)));

