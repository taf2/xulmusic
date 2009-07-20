Playlist:

A Playlist should allow users to view their music by artist, album, or genre

The database should live in the users profile folder


Database Tables:

Artist
------
id, name

Album
------
id, name

Genre
------
id, name

Track
------
id, title, path, duration, artistId, albumId, genreId

create table artist(
	id integer primary key autoincrement,
	name varchar(40)
);

create table album(
	id integer primary key autoincrement,
	name varchar(40)
);

create table genre(
	id integer primary key autoincrement,
	name varchar(40)
);

create table track(
	id integer primary key autoincrement,
	title varchar(40),
	path varchar(255),
	duration integer not null,
	artistId integer not null,
	albumId integer not null,
	genreId integer not null
);

create table playlist(
	id integer primary key autoincrement,
	name varchar(30) not null
);

create table playlistmap(
	id integer primary key autoincrement,
	playlistId integer not null,
	order integer not null,
	trackid integer not null
);

some use cases:

display user defined playlist:

select 
	t.title, t.path, t.duration 
from
	playlist p, track t
where
	p.id = PlaylistID
orderby
	p.order

list all tracks by artist

select 
	a.name, t.title, t.path, t.duration 
from 
	track t, artist a, playlist p
where
	p.id = PlaylistID
orderby
	a.name
and
	p.order

list all tracks by album

select 
	a.name, t.title, t.path, t.duration 
from 
	track t, album a 
orderby
	a.name

list all tracks by genre

select 
	g.name, t.title, t.path, t.duration 
from 
	track t, genre g 
orderby
	g.name

list all tracks by title

select 
	t.title, t.path, t.duration 
from 
	track t
orderby
	t.title
