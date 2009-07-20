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
 * The Original Code is Todd A. Fisher code.
 *
 * The Initial Developer of the Original Code is Todd A. Fisher.
 * Portions created by the Initial Developer are Copyright (C) 2005
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s): 
 *
 */

var playList = null;
var player = null;

/*
 * initialize the player
 * called by xul-music onload
 */
function init()
{
	playList = new PlayList();
	playList.init();
	playList.load( getXmlPlayListFileURL() );
	playList.updated = false;
	player = playList.player;
}

/*
 * save any changes to the playlist
 * called by xul-music onunload
 */
function cleanup()
{
	if( playList.updated )
		playList.save( getXmlPlayListFile() );
}

/*
 * Load the next track in the list
 */
function nextTrack()
{
	playList.nextTrack();
}

/*
 * Load the previous track in the list
 */
function previousTrack()
{
	playList.previousTrack();
}

/*
 * Add a track to the playlist
 * Opens the file picker dialog
 */
function addTrack()
{
	var fpicker = Components.classes["@mozilla.org/filepicker;1"].
								createInstance(Components.interfaces.nsIFilePicker);

	fpicker.init( window, "Select Music Files",
								Components.interfaces.nsIFilePicker.modeOpen |
								Components.interfaces.nsIFilePicker.modeOpenMultiple );

	// XXX: determine what file types are supported by the underlying
	// implementation. e.g. add something to xmIPlayer to check what types
	// are supported.
	fpicker.appendFilter( "MP3", "*.mp3" );
	fpicker.appendFilter( "Ogg Vorbis", "*.ogg" );

	var ret = fpicker.show();

	if( ret == Components.interfaces.nsIFilePicker.returnOK ||
			ret == Components.interfaces.nsIFilePicker.returnReplace ){

		var files = fpicker.files;

		while( files.hasMoreElements() ){
			var file = files.getNext().QueryInterface( Components.interfaces.nsIFile );
			dump( file.path + "\n" );
			playList.addTrackByURL( fileToURL( file ) );
		}
	}

}

function removeSelectedTrack()
{
	playList.removeSelectedTrack();
}

function showHidePlayList()
{
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
}

function savePlayList()
{
}

function getXmlPlayListFile()
{
	// get profile directory
	var file = Components.classes["@mozilla.org/file/directory_service;1"].
												getService(Components.interfaces.nsIProperties).
												get("ProfD", Components.interfaces.nsIFile);
	
	file.append( "xmplaylist.xml" );
	if( !file.exists() ){
		file.create( Components.interfaces.nsIFile.NORMAL_FILE_TYPE, 0664 );
	}
	return file;
}

function getXmlPlayListFileURL()
{
	var ios = Components.classes["@mozilla.org/network/io-service;1"]
											.getService(Components.interfaces.nsIIOService);
	return ios.newFileURI(getXmlPlayListFile());
}

function updateCurrentTrackDuration()
{
	var track = player.track;
	if( track ){
		var duration = track.getDuration();
		var cells = activeTrackItem.getElementsByTagName( "listcell" );
		if( cells.length > 1 ){
			var cell = cells[1];
			cell.setAttribute( "value", duration );
			cell.setAttribute( "label", getTimeString(duration) );
		}
		else{
			var cell = document.createElement( "listcell" );
			cell.setAttribute( "label", getTimeString(duration) );
			cell.setAttribute( "value", duration );
			activeTrackItem.appendChild( cell );
		}
		playListUpdate = true;
	}
}

function getCurrentTrackDuration()
{
	var cells = activeTrackItem.getElementsByTagName( "listcell" );
	if( cells.length > 1 ){
		var cell = cells[1];
		return cell.getAttribute( "value" );
	}
	
	return 0;
}

function getTimeString( seconds )
{
	var min = Math.floor( seconds / 60 );
	var sec = seconds % 60;

	return min + ":" + sec;
}

/*
 * Play/Pause button: track-play-pause
 */
function play()
{
	playList.playPause();
}

/*
 * when a user double clicks on the playlist
 */
function playSelected()
{
	dump( "callling playselected\n" );
	playList.playSelected();
}


var trackDuration = 0;
/*
 * start a track position change
 * user onmousedown on the curtrack-position control
 */
function startPositionChange( slider )
{
	slider.setAttribute("changing","true");
	player.startPositionChange();
	if( player.player.track ){
		trackDuration = player.player.track.getDuration();
		slider.setAttribute("onmousemove","onPositionChange(this);");
	}
}

/* 
 * when the slider is moving we track it's
 * position and update the track time by 
 * multipling the track duration times the
 * slider position
 */
function onPositionChange( slider )
{
	var time = Math.round( trackDuration * (slider.getAttribute( "curpos" )/100.0) );
	var ctime = getTimeString( time );
	player.trackTime.setAttribute("value", ctime);
}
/*
 * end a track position change
 * user onmouseup on the curtrack-position control
 * get the control position. convert it to a percentage
 * multiple the percent times the total track duration
 * to get the new track position.
 */
function endPositionChange( slider )
{
	slider.setAttribute("onmousemove","");
	slider.setAttribute("changing","false");
	var pos = slider.getAttribute("curpos") / 100;
	player.setNewPosition( pos );
}

/* 
 * user clicks on the stop button: track-stop
 */
function stop()
{
	playList.stop();
}

/*
 * start a change in current track volume
 */
function startVolumeChange(scroll)
{
	// set the onmousemove handler
	scroll.setAttribute( "onmousemove", "updateVolume(this)" );
	scroll.setAttribute("changing","true");
}
/*
 * update the current volume
 */
function updateVolume(scroll)
{
	player.setVolume( scroll.getAttribute("curpos") );
	//dump( "updating volume:" + player.getVolume() + " scroll volume:" + scroll.getAttribute("curpos") + "\n" );
	document.getElementById( "controls" ).setAttribute( "volume", scroll.getAttribute("curpos") );
}
/*
 * remove the onmousemove handler
 */
function endVolumeChange(scroll)
{
	scroll.setAttribute("changing","false");
	scroll.setAttribute( "onmousemove", "" );
	updateVolume(scroll);
}

function setVolume()
{
	// sync volume
	var volume = document.getElementById( "volume-scroll" );
	volume.setAttribute( "curpos", player.getVolume() );
	//dump( player.getVolume() + "\n" );
}

function openXulDialog( uri, args )
{
	window.openDialog( uri, "_blank", "chrome, dialog, centerscreen, modal, resizable=no", args );
}

function addPodCast()
{
	dump( "called addPodCast\n" );
	var podcastURL = new Object();
	podcastURL.value = "";
	openXulDialog( "chrome://xulmusic/chrome/podcast.xul", podcastURL );
	if( podcastURL.value == "" )
		return;
	dump( "loading podcast url: " + podcastURL.value + "\n" );

	// the following is the plan:
	// 
	// step 1: download the feed.
	// 
	// step 2: parse the feed for it's title and create a new item in our playlist.
	//
	// step 3: begin looping though the list of all items within the feed
	//  			 creating subitems from the root item created in step 2.
	//
	// step 4: start each new child item off downloading the audio file in each enclosure
	//				 the download will have a progress bar in the playlist for each file as it is
	//  			 downloaded.
	// 
	// audio files are saved in the audio folder set in the options dialog.

	var xmlhttp = new XMLHttpRequest();

	xmlhttp.onreadystatechange = function(){
		// status == 0 when reading from a file
		if( xmlhttp.readyState == 4 && (xmlhttp.status == 0 | xmlhttp.status == 200) ){
			// feed was succesfully download continue to step 2
			var feedItem = addNewFeedToPlayList( xmlhttp.responseXML );
			// next start the process of creating the subitems
			addChildFeeds( feedItem, xmlhttp.responseXML );
		}
	};
	xmlhttp.open( "GET", podcastURL.value, true );
	xmlhttp.overrideMimeType("text/xml");
	xmlhttp.send(null);
}
// step 2: parse the feed for it's title and create a new item in our playlist.
function addNewFeedToPlayList( feed )
{
	var titles = feed.getElementsByTagName( "title" );
	var links = feed.getElementsByTagName( "link" );
	//var item = createTreeItem( titles[0].firstChild.nodeValue, links[0].firstChild.nodeValue, "", links[0].firstChild.nodeValue );

	var item = createTreeSubItem( titles[0].firstChild.nodeValue, links[0].firstChild.nodeValue );
	var children = document.createElement( "treechildren" );

	item.appendChild( children );

	playList.items.appendChild( item );
	playList.updated = true;
	return children;
}

// step 3: begin looping though the list of all items within the feed
//  			 creating subitems from the root item created in step 2.
// step 4: start each new child item off downloading the audio file in each enclosure
//				 the download will have a progress bar in the playlist for each file as it is
//  			 downloaded.
function addChildFeeds( feedItem, feed )
{
	var items = feed.getElementsByTagName( "item" );

	document.getElementById( "podcast-download" ).setAttribute( "hidden", "false" );

	for( var i = 0; i < items.length; ++i ){
		var item = items[i];
		// XXX: be smarter about how we extract this information
		var title = item.getElementsByTagName( "title" )[0].firstChild.nodeValue;
		var enclosures = item.getElementsByTagName( "enclosure" );
		if( enclosures.length > 0 ){
			dump( "processing item: " + title + "\n" );

			// get the URL object
			var url = Components.classes["@mozilla.org/network/standard-url;1"].createInstance(Components.interfaces.nsIURL);
			// XXX: some what validate the URL
			url.spec =  enclosures[0].getAttribute("url");
			
			// create the child node
			var item = createTreeItem( title, url.spec, "", "" );
			var progcell = document.createElement( "treecell" );
			progcell.setAttribute( "mode", "normal" );
			item.firstChild.appendChild( progcell ); // append the cell to the treerow
			feedItem.appendChild( item ); // append the item to the treechildren
			// create a new downloader object
			var downloader = new Downloader( updateDownloadProgress, downloadComplete );
			downloader.progcell = progcell; // set the progress cell
			downloader.pathcell = item.firstChild.firstChild; // set the first cell that contains the path
			var audioFile = getAudioFolder();
			audioFile.append( decodeURI( url.fileBaseName ) + "." + url.fileExtension );
			downloader.start( url.spec, audioFile );
		}
	}
}

function updateDownloadProgress( percent )
{
	try{
		this.progcell.setAttribute( "value", percent );
	}catch(e){
		dump( e + "\n" );
	}
}

function downloadComplete( file )
{
	try{
		this.progcell.setAttribute( "value", 100 );
		this.pathcell.setAttribute( "value", fileToURL( file ).spec );
	}catch(e){
		dump( e + "\n" );
	}
}
