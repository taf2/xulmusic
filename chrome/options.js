
// array of each option as it exists in the UI
var options = null;
var updated = false;

/* 
 * initialize options by reading xmoptions.xml file from user profile 
 */
function init()
{
	var optionFile = getOptionsFile();
	dump( "check exists\n" );

	if( !optionFile.exists() ){
		options = new Array();
		optionFile.create( Components.interfaces.nsIFile.NORMAL_FILE_TYPE, 0664 );
		dump( "options doesn't exists\n" );
		var audioPathURL = fileToURL( getDefaultAudioFolder() );
		// initialize each option
		document.getElementById( "audio-folder" ).value = audioPathURL.spec;
		options.push( "audio-folder" );
		updated = true;
		save();
		updated = false;
	}

	var xmlhttp = new XMLHttpRequest();
	options = new Array();


	xmlhttp.onreadystatechange = function(){
		// status == 0 when reading from a file
		if( xmlhttp.readyState == 4 && 
				(xmlhttp.status == 0 | xmlhttp.status == 200) ){
			// get the items from the play list
			var opts = xmlhttp.responseXML.getElementsByTagName( "option" );
			for( var i = 0; i < opts.length; ++i ){
				var id = opts[i].getAttribute("id");
				var value = opts[i].getAttribute("value");
				var option = document.getElementById( id );
				// set the option in the UI
				option.value = value;
				option.onchange();
				// save the option id for when it's time to save
				options.push( id );
			}
		}
	};
	xmlhttp.open( "GET", getOptionsFileURL().spec, true );
	xmlhttp.overrideMimeType("text/xml");
	xmlhttp.send(null);
}

function save()
{
	if( !updated ){
		return;
	}

	var file = getOptionsFile();
	var out = Components.classes["@mozilla.org/network/file-output-stream;1"]
		.createInstance(Components.interfaces.nsIFileOutputStream);
	out.init(file, 0x02 | 0x08 | 0x20, 0664, 0); // write, create, truncate
	writeToStream( out, "<?xml version='1.0'?>\n" );
	writeToStream( out, "<options>\n" );
	for( var i = 0; i < options.length; ++i ){
		writeToStream( out, "\t<option id='" + options[i] + 
															"' value='" + document.getElementById( options[i] ).value + "'/>\n" );
	}
	writeToStream( out, "</options>" );
	out.close();
}

function cancel()
{
}

function setAudioFolder()
{
	var fpicker = Components.classes["@mozilla.org/filepicker;1"].
								createInstance(Components.interfaces.nsIFilePicker);

	fpicker.init( window, "PodCast Folder", Components.interfaces.nsIFilePicker.modeGetFolder );
	var ret = fpicker.show();

	if( ret == Components.interfaces.nsIFilePicker.returnOK ){
		document.getElementById( "audio-folder" ).value = fpicker.fileURL.spec;
		document.getElementById( "audio-folder" ).onchange();
		updated = true;
	}
}

function setDisplayLabelFromLabel( copyFrom, destID )
{
	document.getElementById( destID ).value = fileURLSpecToFile( copyFrom.value ).path;
}
