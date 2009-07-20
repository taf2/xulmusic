const nsIIOService = Components.interfaces.nsIIOService;
const nsIFileOutputStream = Components.interfaces.nsIFileOutputStream;
const nsIBufferedOutputStream = Components.interfaces.nsIBufferedOutputStream;
const CI_IOOService = "@mozilla.org/network/io-service;1";
const CI_FileOutput = "@mozilla.org/network/file-output-stream;1";
const CI_BufferedOutput = "@mozilla.org/network/buffered-output-stream;1";

function Downloader( progress, finished )
{
	this.progress = progress;
	this.finished = finished;
	this.start = Downloader_start;
}

function Downloader_start( downloadURL, file )
{
	var ios = Components.classes[CI_IOOService].getService(nsIIOService);
	var outstream = Components.classes[CI_FileOutput].createInstance(nsIFileOutputStream);

	this.downloadedFile = file;

	if( !file.exists() )
		file.create(Components.interfaces.nsIFile.NORMAL_FILE_TYPE, 0664);

	outstream.init( file, 0x02 | 0x08 | 0x20, 0664, 0);

	this.channel = ios.newChannel( downloadURL, null, null );
	this.output = Components.classes[CI_BufferedOutput].createInstance(nsIBufferedOutputStream);
	this.output.init( outstream, 4096 );

	this.channel.notificationCallbacks = this;
	this.channel.asyncOpen( this, this );
}

Downloader.prototype.onStartRequest = function ( req, context )
{
	dump( "starting request\n" );
}

Downloader.prototype.onStopRequest = function ( req, context, status, error )
{
	dump( "Status: " + status + ", stopping request: error ?= " + error + " \n" );

	//dump( "content length = " + this.channel.contentLength + "\n" );
	this.output.close();
	this.output = null;

	this.finished( this.downloadedFile );
}

Downloader.prototype.onDataAvailable = function ( req, context, input, offset, count )
{
	if( this.output ){
		this.output.writeFrom( input, count );
	}
}
// requester interface
Downloader.prototype.getInterface = function(uuid)
{
	var strUUID = uuid.toString();
	if( strUUID == "{dd47ee00-18c2-11d3-9337-00104ba0fd40}" ){
		return this;
	}
	return null;
}

Downloader.prototype.onProgress = function( request , context , progress , progressMax )
{
	var percent = Math.floor( (progress / progressMax)*100);
	dump( "progress: " + progress + " of " + progressMax + ", " + percent + "%\n" );
	this.progress( percent );
//	this.progress.setAttribute( "value", percent + "%" );
//	this.progressMsg.value = "Downloading " + percent + "% of Patch";
}

Downloader.prototype.onStatus = function( request , context , status , statusArg )
{
	dump( "status: " + status + "\n" );
}
