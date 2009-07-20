
function fileToURL( file )
{
	var ios = Components.classes["@mozilla.org/network/io-service;1"].
											 getService(Components.interfaces.nsIIOService);
	var fileHandler = ios.getProtocolHandler("file").
												QueryInterface(Components.interfaces.nsIFileProtocolHandler);
	return fileHandler.newFileURI( file );
}

function fileURLSpecToFile( fileURLSpec )
{
	var ios = Components.classes["@mozilla.org/network/io-service;1"].
											 getService(Components.interfaces.nsIIOService);
	var fileHandler = ios.getProtocolHandler("file").
												QueryInterface(Components.interfaces.nsIFileProtocolHandler);
	return fileHandler.getFileFromURLSpec( fileURLSpec );
}

// create a new nsILocalFile
function newLocalFile( path )
{
	var file = Components.classes["@mozilla.org/file/local;1"].createInstance(
	                          Components.interfaces.nsILocalFile);
	file.initWithPath( path );
	return file;
}

function getDefaultAudioFolder()
{
	// This mess is to first get the home folder for the user
	// then to try and get the My Documents folder within the home
	// folder.  Clearly, this is only going to work for winxp.
	// we should really have some sort of system check to avoid doing the
	// My Documents check on other systems.
	var file = Components.classes["@mozilla.org/file/directory_service;1"].
		getService(Components.interfaces.nsIProperties).
			get("Home", Components.interfaces.nsIFile);
	var chkPath = file;

	// for windows we'll put podcasts in the My Music Folder
	chkPath.append( "My Documents" );
	chkPath.append( "My Music" );

	dump( "check exists: " + chkPath.path  + "\n" );

	if( chkPath.exists() ){
		return chkPath; // windows
	}
	else{
		// for linux/mac we'll put it in the profile/Music folder
		// open to better places
		file = Components.classes["@mozilla.org/file/directory_service;1"].
			getService(Components.interfaces.nsIProperties).
				get("ProfD", Components.interfaces.nsIFile);

		file.append( "Music" );
		return file; // all others
	}
}

// helper function for writing to a stream
function writeToStream( stream, data )
{
	stream.write( data, data.length );
}

function openWriteStream( file )
{
	var out = Components.classes["@mozilla.org/network/file-output-stream;1"]
		.createInstance(Components.interfaces.nsIFileOutputStream);
	out.init(file, 0x02 | 0x08 | 0x20, 0664, 0); // write, create, truncate
	return out;
}

function getOptionsFile()
{
	// get profile directory
	var file = Components.classes["@mozilla.org/file/directory_service;1"].
												getService(Components.interfaces.nsIProperties).
												get("ProfD", Components.interfaces.nsIFile);
	
	file.append( "xmoptions.xml" );
//	if( !file.exists() ){
//		file.create( Components.interfaces.nsIFile.NORMAL_FILE_TYPE, 0664 );
//	}
	return file;
}

function getOptionsFileURL()
{
	var ios = Components.classes["@mozilla.org/network/io-service;1"]
											.getService(Components.interfaces.nsIIOService);
	return ios.newFileURI(getOptionsFile());
}

function getAudioFolder()
{
	var xmlhttp = new XMLHttpRequest();
	options = new Array();

	xmlhttp.open( "GET", getOptionsFileURL().spec, false );
	xmlhttp.overrideMimeType("text/xml");
	xmlhttp.send(null);

	var opts = xmlhttp.responseXML.getElementsByTagName( "option" );
	for( var i = 0; i < opts.length; ++i ){
		if( "audio-folder" == opts[i].getAttribute("id") ){
			return fileURLSpecToFile( opts[i].getAttribute( "value" ) );
		}
	}
	return null;
}
