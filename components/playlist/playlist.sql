/*
 * "The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * The Original Author is Todd A. Fisher
 *
 * The Initial Developer of the Original Code is Todd A. Fisher.
 * Portions created by the Initial Developer are Copyright (C) 2005
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s): 
 *
 */

create table tracks (
	id integer primary key autoincrement,
	duration integer not null default 0,
	title varchar(40) not null default "Unknown",
	url varchar(256) not null,
	artist_id integer not null default 0,
	album_id integer not null default 0,
	genre_id integer not null default 0,
	playlist_id integer not null default 0 -- the playlist associated to this track
);

create table albums (
	id integer primary key autoincrement,
	album varchar(30) not null
);
insert into albums (id,album) values(0,"Unknown");

create table artists (
	id integer primary key autoincrement,
	artist varchar(30) not null
);
insert into artists (id,artist) values(0,"Unknown");

create table genres (
	id integer primary key autoincrement,
	genre varchar(30) not null
);
insert into genres (id,genre) values(0,"Unknown");

create table playlists (
	id integer primary key autoincrement,
	playlist varchar(30) not null
);
insert into playlists (id,playlist) values(0,"default");
