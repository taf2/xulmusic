// Manage Each Playlist
// I'm not thinking we'll have multiple playlists
// but this does open the door for it
function PlayList()
{
}

PlayList.prototype = {
	init:function(){

		this.player = new Player();
		this.tracks = new Array();

		this.player.init(this);

		this.loop = document.getElementById( "playlist-loop" );
		this.tree = document.getElementById( "play-list" );
		this.view = this.tree.view;
	},
	// load the playlist from source url
	load:function( sourceURL ){

		// initialize this.tree and this.items
		this.tree = document.getElementById( "play-list" );
		this.items = document.getElementById( "play-list-items" );
		this.view = this.tree.view;

		var xmlhttp = new XMLHttpRequest();
		xmlhttp.tree = this.tree;
		xmlhttp.items = this.items;
		xmlhttp.obj = this;

		xmlhttp.onreadystatechange = function(){
			// status == 0 when reading from a file
			if( xmlhttp.readyState == 4 && (xmlhttp.status == 0 || xmlhttp.status == 200) ){
				// get the items from the play list
				var items = xmlhttp.responseXML.getElementsByTagName( "item" );
				var cells = null;
				var item = null;
				dump( "loaded: " + items.length + "\n" );
				if( items.length == 0 ){
					var parser = new DOMParser();
					var doc = parser.parseFromString(xmlhttp.responseText, "text/xml");
					items = doc.getElementsByTagName( "item" );
					dump( "loaded: " + items.length + "\n" );
					dump( xmlhttp.responseText + "\n" );
				}
				
				for( var i = 0; i < items.length; ++i ){
					cells = items[i].getElementsByTagName( "cell" );
					dump( "cells.length: " + cells.length + "\n" );
					if( cells.length == 2 ){
						item = 
						createTreeItem( decodeURI( cells[0].getAttribute("label") ), 
														cells[0].getAttribute("value"),
														cells[1].getAttribute("label"),
														cells[1].getAttribute("value") );
					}
					else{ // is it a pod cast?
						item = createTreeSubItem( cells[0].getAttribute("label"), 
																			cells[0].getAttribute("value") );
						var children = document.createElement( "treechildren" );
						var subitems = items[i].getElementsByTagName( "subitem" );
						for( var j = 0; j < subitems.length; ++j ){
							cells = subitems[j].getElementsByTagName( "cell" );
							var subitem =
							createTreeItem( decodeURI( cells[0].getAttribute("label") ), 
															cells[0].getAttribute("value"),
															cells[1].getAttribute("label"),
															cells[1].getAttribute("value") );
							children.appendChild( subitem );
						}
						item.appendChild( children );
					}

					xmlhttp.items.appendChild( item );

				}
				xmlhttp.obj.updated = false;
			}
		};
		dump( sourceURL.spec + "\n" );
		xmlhttp.open( "GET", sourceURL.spec, true );
		xmlhttp.overrideMimeType("text/xml");
		xmlhttp.send(null);
	},
	// saves the playlist to the source url
	save:function( sourceFile ){
		var file = sourceFile;
		var out = openWriteStream( file );

		writeToStream( out, "<?xml version=\"1.0\"?>\n<items>\n" );

		var items = this.items.childNodes;
		this.writePlayListItems( items, out, false );
		writeToStream( out, "</items>\n" );
		out.close();
	},
	writePlayListItems:function(items, out, sub){
		for( var i = 0; i < items.length; ++i ){
			var cells = items[i].getElementsByTagName( "treerow" )[0].getElementsByTagName("treecell");
			writeToStream( out, ( sub ? "\t\t<subitem>\n" : "\t<item>\n" ) );
			for( var j = 0; j < cells.length; ++j ){
				var tmp = encodeURI( cells[j].getAttribute( "label" ) );
				var title = "";
				for( var k = 0; k < tmp.length; ++k ){
					if( tmp[k] == '&' ){
						title += '&amp;';
					}
					else
						title += tmp[k];
				}
				writeToStream( out, ( sub ? "\t" : "" ) + "\t\t<cell label='" + title +
														"' value='" + cells[j].getAttribute( "value" ) + "'/>\n" );
			}
			var subitems = items[i].getElementsByTagName( "treeitem" );
			this.writePlayListItems( subitems, out, true );
			writeToStream( out, ( sub ? "\t\t</subitem>\n" : "\t</item>\n" ) );
		}
	},
	// toggles between hidden and visible
	toggleVisible:function(){
		// TODO: create artwork for the playlist closed state
		// toggle playlist closed state
		var playList = document.getElementById( "play-list-shell" );
		var control = document.getElementById( "playlist-show-hide" );
		var controls = document.getElementById( "controls" );
		var playlistHeight = controls.getAttribute( "playlist-height" );
		var xulWindow = document.getElementById( "xul-music" );

		if( playList.getAttribute("hidden") == "true" ){
			playList.setAttribute( "hidden", "false" );
			xulWindow.setAttribute( "height", playlistHeight );
			window.resizeTo( window.outerWidth, playlistHeight );
		}
		else{
			document.getElementById( "controls" ).setAttribute( "playlist-height", 
																						xulWindow.getAttribute("height") );
			var diff = window.outerHeight - window.innerHeight;
			var controlsHeight = window.getComputedStyle(controls,"").getPropertyValue("height");
			var height = parseInt( controlsHeight ) + diff;

			playList.setAttribute( "hidden", "true" );
			xulWindow.setAttribute( "height", height );
			window.resizeTo( window.outerWidth, height );
		}
	},
	// add a new track given a URL
	addTrackByURL:function( url ){
		if( !this.items || !this.tree ){
			// initialize this.tree and this.items
			this.tree = document.getElementById( "play-list" );
			this.items = document.getElementById( "play-list-items" );
			this.view = this.tree.view;
		}
		var player = Components.classes["@XulMusic.org;Player;1"].getService(Components.interfaces.xmIPlayer);

		var track = this.tracks[url.spec] = player.newTrack( url );
		var seconds = track.getDuration();
		this.items.appendChild( createTreeItem( track.getTitle(), url.spec, getTimeString( seconds ), seconds ) );
		dump( "adding track " + url.spec + ", title: " + track.getTitle() + ", seconds: " + getTimeString( seconds ) + "\n" );
		this.updated = true;
	},
	// remove the currently selected track and return true
	// if no tracks are selected return's false
	removeSelectedTrack:function(){
		if( this.tree.currentIndex > 0 ){
			var items = this.items.getElementsByTagName("treeitem");
			dump( "item count: " + items.length + "\n" );
			var item = items[this.tree.currentIndex];//this.tree.getItemAtIndex( this.tree.currentIndex );
			dump( "removing: " + item + "\n" );
			this.items.removeChild( item );
			this.updated = true;
		}
	},
	// skip to the next track and start playing
	// if no tracks are currently selected play the
	// first track in the list.  Return the new track.
	nextTrack:function(){
		var track = this.player.player.track;
		dump( track + "\n" );
		if( !track ){
			return;
		}
		var rows = this.items.getElementsByTagName("treerow");
		var index = this.getTrackRowIndex( track );
		var next = index + 1;

		dump( "row index: " + next + "\n" );

		if( next >= rows.length ){
			if( this.loop.getAttribute( "checked" ) == "true" )
				next = 0; // loop
			else{
				return;
			}
		}
		// XXX: in some conditions tree is null, this only seems to happen when 
		// we hit the end of a track and nextTrack is called from the timeout
		if( !this.tree || !this.view ){
			this.tree = document.getElementById( "play-list" );
			this.view = this.tree.view;
		}
		if( this.tree.getAttribute( "hidden" ) == "false" ){
			this.view.selection.select( next );
		}
		this.playSelected( next );
	},
	// same as nextTrack just goes in the opposite direction
	previousTrack:function(){
		var track = this.player.player.track;
		if( !track ){
			return;
		}
		var rows = this.items.getElementsByTagName("treerow");
		var index = this.getTrackRowIndex( track );
		var prev = index - 1;
		var looping = this.loop.getAttribute( "checked" ) == "true";

		if( prev >= rows.length ){
			if( looping )
				prev = rows.length - 1; // loop
			else
				return;
		}
		else if( prev < 0 ){
			if( looping )
				prev = rows.length - 1; // loop
			else
				return;
		}
		if( this.tree.getAttribute( "hidden" ) == "false" ){
			this.view.selection.select( prev );
		}
		this.playSelected( prev );
	},
	playSelected: function( index ){
		var track = this.getSelectedTrack( index );
		if( track ){
			this.player.stop();
			this.player.playTrack( track );
			return true;
		}
		return false;
	},
	playPause: function(){
		this.player.playPause();
	},
	stop: function(){
		this.player.stop();
	},
	getTrackRowIndex: function( track ){
		var rows = this.items.getElementsByTagName( "treerow" );
		for( var i = 0; i < rows.length; ++i ){
			var cells = rows[i].getElementsByTagName( "treecell" );
			//dump( "comparing: '" +  cells[0].getAttribute( "value" ) + "' == '" + track.getURLSpec() + "'\n" );
			if( cells[0].getAttribute( "value" ) == track.getURLSpec() ){
				return i;
			}
		}
		return -1;
	},
	getSelectedTrack: function( index ){
		var selected;

		if( index )
			selected = index;
		else
			selected = this.tree.currentIndex;

		if( selected == -1 ){
			return null;
		}

		var rows = this.items.getElementsByTagName( "treerow" );
		var cells = rows[selected].getElementsByTagName( "treecell" );
		var player = Components.classes["@XulMusic.org;Player;1"].getService(Components.interfaces.xmIPlayer);
		var spec = cells[0].getAttribute( "value" );

		var track = this.tracks[spec];

		if( track )
			return track;

		var url = Components.classes["@mozilla.org/network/standard-url;1"].createInstance(Components.interfaces.nsIURL);
		url.spec = spec;
		track = this.tracks[url.spec] = player.newTrack( url );
		return track;
	},
	getFirstTrack: function(){
		var rows = this.items.getElementsByTagName( "treerow" );
		if( rows.length > 0 ){
			var cells = rows[0].getElementsByTagName( "treecell" );
			// cell 0 has path and title
			var player = Components.classes["@XulMusic.org;Player;1"].getService(Components.interfaces.xmIPlayer);
			var url = Components.classes["@mozilla.org/network/standard-url;1"].createInstance(Components.interfaces.nsIURL);
			url.spec = cells[0].getAttribute( "value" );
			var track = this.tracks[url.spec];
			if( track )
				return track;
			track = this.tracks[url.spec] = player.newTrack( url );
			return track;
		}
		return null;
	},
	getTrackDuration: function( track ){
		var trackIndex = this.getTrackRowIndex( track );
		var rows = this.items.getElementsByTagName( "treerow" );
		var cells = rows[trackIndex].getElementsByTagName( "treecell" );
		if( cells.length > 1 ){
			var cell = cells[1];
			// this is a little freaky when playing a variable bit rate mp3
			return cell.getAttribute( "value" );
		}
		return 0;
	},
	setTrackDuration: function( track, duration ){
		var trackIndex = this.getTrackRowIndex( track );
		var rows = this.items.getElementsByTagName( "treerow" );
		var cells = rows[trackIndex].getElementsByTagName( "treecell" );
		if( cells.length > 1 ){
			var cell = cells[1];
			// this is a little freaky when playing a variable bit rate mp3
			if( cell.getAttribute( "value" ) != duration ){
				var dtime = getTimeString( duration );
				cell.setAttribute( "label", dtime );
				cell.setAttribute( "value", duration );
				this.updated = true;
			}
		}
	},
	updateTrackDuration: function( track ){
		var duration = track.getDuration();
		this.setTrackDuration( track, duration );
		return duration;
	},
	tree:null, // playlist xul tree
	view:null,
	items:null, // playlist xul treechildern
	tracks:null,
	updated:false, // true if the playlist was updated
	player:null, // the music player
	track:null, // the active track
	loop:false
};

// helper function for creating a new row in a tree
function createTreeItem( title, path, time, timeVal )
{
	var item = document.createElement( "treeitem" );
	var row = document.createElement( "treerow" );
	var cell = document.createElement( "treecell" );

	cell.setAttribute( "label", decodeURI( title ) );
	cell.setAttribute( "value", path );
	row.appendChild( cell );

	cell = document.createElement( "treecell" );
	cell.setAttribute( "label", time );
	cell.setAttribute( "value", timeVal );
	row.appendChild( cell );

	item.appendChild( row );

	return item;
}
function createTreeSubItem( title, path )
{
	var item = document.createElement( "treeitem" );
	var row = document.createElement( "treerow" );
	var cell = document.createElement( "treecell" );

	item.setAttribute( "container", "true" );
	item.setAttribute( "open", "true" );

	cell.setAttribute( "label", title );
	cell.setAttribute( "value", path );
	row.appendChild( cell );
	item.appendChild( row );

	return item;
}
