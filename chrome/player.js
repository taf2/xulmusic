function Player(){
}
Player.prototype = {
	// initialize the player
	init: function( playlist ){
		this.player = Components.classes["@XulMusic.org;Player;1"].getService(Components.interfaces.xmIPlayer);
		this.playlist = playlist;

		this.playPauseButton = document.getElementById( "track-play-pause" );
		this.tracker = document.getElementById( "curtrack-position" );
		this.trackTime = document.getElementById( "track-time" );
		this.trackTitle = document.getElementById( "track-title" );
		this.playPauseButton = document.getElementById( "track-play-pause" );
		
		// restore persisted volume
		this.player.setVolume( document.getElementById( "controls" ).getAttribute( "volume" ) );
	},
	// toggles between play and pause states
	playPause: function(){

		var track = this.player.track;

		// check if the player has a current track loaded
		if( track ){
			if( track.getState() == Components.interfaces.xmITrack.PAUSED ||
					track.getState() == Components.interfaces.xmITrack.STOPPED ){
				this.player.play();
				this.setProgressTimeout();
				this.playPauseButton.setAttribute( "mode", "pause" );
			}
			else{
				if( this.progTimeout ){
					window.clearTimeout( this.progTimeout );
					this.progTimeout = null;
				}
				this.player.pause();
				this.playPauseButton.setAttribute( "mode", "play" );
			}
		}
		else{  // no track is currently loaded
			var track = this.playlist.getFirstTrack();
			this.playTrack( track );
			this.playPauseButton.setAttribute( "mode", "pause" );
		}
	},
	// stops current track
	stop: function(){
		if( this.progTimeout ){
			window.clearTimeout( this.progTimeout );
			this.progTimeout = null;
		}
		this.tracker.setAttribute( "curpos", "0" );
		this.player.stop();
		this.playPauseButton.setAttribute( "mode", "play" );
	},
	// stops any currently playing tracks
	// and starts playing new track
	playTrack: function( track ){
		// reset the current track position
		this.tracker.setAttribute( "curpos", "0" );
		// set the current track
		this.player.track = track;
		// start playing
		this.player.play();
		this.playPauseButton.setAttribute( "mode", "pause" );
		this.trackTitle.setAttribute( "value", track.getTitle() );
		// start a timeout to update the tracks position
		this.setProgressTimeout();
	},
	setProgressTimeout: function(){
		var thisObj = this;
		function callProgressUpdate(){
			thisObj.progressUpdate();
		}
		window.clearTimeout( this.progTimeout );
		this.progTimeout = setTimeout( callProgressUpdate, 100 );
	},
	progressUpdate: function()
	{
		var track = this.player.track;
		if( !track ){
			window.clearTimeout( this.progTimeout );
			this.progTimeout = null;
			return;
		}

		var state = track.getState();
		var time = track.getPosition();
		var duration = track.getDuration();
		var percent = ( (time / duration) * 100.0 );
		
		var ctime = getTimeString( time );
		this.tracker.setAttribute( "curpos", percent );
		this.trackTime.setAttribute("value", ctime);

		if( percent >= 100 ){
			var player = this;

			// Give the UI a chance to display the final time
			function endTrack()
			{
				// step 1 show the final time for the track
				player.tracker.setAttribute( "curpos", 100 );
				player.trackTime.setAttribute("value",  getTimeString( duration ) );
				player.stop();
				player.playlist.nextTrack();
			}

			// stop polling for track progress
			window.clearTimeout( this.progTimeout );
			this.progTimeout = null;
			setTimeout( endTrack, (time / duration) * 1000.0 );
			return;
		}

		if( state == Components.interfaces.xmITrack.PLAYING )
			this.setProgressTimeout();
	},
	getVolume: function(){
		return this.player.getVolume();
	},
	setVolume: function( value ){
		return this.player.setVolume( value );
	},
	startPositionChange: function(){
		if( this.progTimeout ){
			dump( "clearing timeout\n" );
			window.clearTimeout( this.progTimeout );
			this.progTimeout = null;
		}
	},
	setNewPosition: function( pos ){
		var track = this.player.track;
		if( track ){
			var state = track.getState();

			if( state == Components.interfaces.xmITrack.PLAYING || 
					state == Components.interfaces.xmITrack.PAUSED ){
				var duration = track.getDuration();
				track.setPosition( duration * pos );
			}
			this.setProgressTimeout();
		}
	},
	player:null,
	track:null,
	tracker:null,
	trackTime:null,
	trackTitle:null,
	playlist:null,
	playPauseButton:null,
	progTimeout:0
};
