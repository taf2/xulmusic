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
#include <nsIURL.h>
#include <nsIFile.h>
#include <nsMemory.h>
#include <nsString.h>
#include <nsIFile.h>
#include <nsIFileURL.h>
#include <nsIIOService.h>
#include <nsServiceManagerUtils.h>
#include <nsIProtocolHandler.h>
#include <nsIFileProtocolHandler.h>
#include <nsNetUtil.h>
#include <nsIClassInfoImpl.h>
#include "GstTrack.h"
#include <pthread.h>
#include <unistd.h>

/* Implementation file */
NS_IMPL_ISUPPORTS1_CI(GstTrack, xmITrack)

GstTrack::GstTrack()
  : xmITrack(), play(NULL),playState(STOPPED),needInit(true),volume(1.0),duration(0)
{
  /* member initializers and constructor code */
}

GstTrack::~GstTrack()
{
  /* destructor code */
	this->Stop();
}

/* AString getTitle (); */
NS_IMETHODIMP GstTrack::GetTitle(nsAString & _retval)
{
	_retval = this->title;
	return NS_OK;
}

/* AString getArtist (); */
NS_IMETHODIMP GstTrack::GetArtist(nsAString & _retval)
{
	_retval = this->artist;
	return NS_OK;
}

/* AString getURLSpec (); */
NS_IMETHODIMP GstTrack::GetURLSpec(nsAString & _retval)
{
	_retval = this->urlSpec;
	return NS_OK;
}

/* void setPosition (in long position); */
NS_IMETHODIMP GstTrack::SetPosition(PRInt32 position)
{
	if( !this->needInit ){
		gint64 loc = (gint64)((gint64)position * GST_MSECOND);
		gint64 duration;
		GstFormat form = GST_FORMAT_TIME;
		gst_element_query_duration( this->play, &form, &duration );

		if( loc >= duration )
			loc = duration-1000;

		if( loc < 0 )
			loc = 0;

		gst_element_seek_simple( this->play, form,
                             (GstSeekFlags)(GST_SEEK_FLAG_FLUSH |
                                            GST_SEEK_FLAG_KEY_UNIT),
                             loc);
	}
	return NS_OK;
}

/* long getPosition (); */
NS_IMETHODIMP GstTrack::GetPosition(PRInt32 *_retval)
{
	if( !this->needInit ){
		gint64 pos_time;
		GstFormat form = GST_FORMAT_TIME;
		gst_element_query_position( this->play, &form, &pos_time );
		*_retval = pos_time/GST_MSECOND;
	}
	else
		*_retval = 0;
	return NS_OK;
}

/* long getDuration (); */
NS_IMETHODIMP GstTrack::GetDuration(PRInt32 *_retval)
{
	if( !this->needInit && !this->duration ){
		gint64 total_time;
		GstFormat form = GST_FORMAT_TIME;
		gst_element_query_duration( this->play, &form, &total_time );
		this->duration = total_time/GST_MSECOND;
		if( this->duration < 0 )
			this->duration = 0;
	}

	*_retval = this->duration;

	return NS_OK;
}

/* short getState (); */
NS_IMETHODIMP GstTrack::GetState(PRInt16 *_retval)
{
	*_retval = (PRInt16)this->playState;
	return NS_OK;
}

void
GstTrack::
StreamError( GstElement *play, 
						 GstElement *src, 
						 GError *err, 
						 gchar *debug, 
						 GstTrack *track )
{
  g_print ("Error: %s\n", err->message);
}

void
GstTrack::
ProcessMediaTag( const GstTagList *list, 
								 const gchar *tag, 
								 GstTrack *track )
{
	const GValue *tagvalue = gst_tag_list_get_value_index( list, tag, 0 );

	if( tagvalue ){
		if( G_VALUE_HOLDS_STRING( tagvalue ) ){
			//printf( "tag: %s --> %s\n", tag, g_value_get_string( tagvalue ) );
			if( !strcmp( tag, "title" ) ){
				track->title =  NS_ConvertUTF8toUTF16( g_value_get_string( tagvalue ) );
			}
			else if( !strcmp( tag, "artist" ) ){
				track->artist =  NS_ConvertUTF8toUTF16( g_value_get_string( tagvalue ) );
			}
		}
	}
}

void
GstTrack::
ParsedTags( GstElement*play, 
						GstElement*source, 
						GstTagList*tags, 
						GstTrack *track )
{
	gst_tag_list_foreach( tags, (GstTagForeachFunc)ProcessMediaTag, track );
}

nsresult GstTrack::initTrack( nsIFile *file )
{
	nsresult rv;
	nsCString pathURL;
	nsCOMPtr<nsIIOService> ios = do_GetService( "@mozilla.org/network/io-service;1" );
	nsCOMPtr<nsIProtocolHandler> proto;
	// get the file url
	ios->GetProtocolHandler( "file", getter_AddRefs( proto ) );
	nsCOMPtr<nsIFileProtocolHandler> fileHandler( do_QueryInterface( proto, &rv ) );
	fileHandler->GetURLSpecFromFile( file, pathURL );

	printf( "initializing stream: %s\n", pathURL.get() );

	// create the player object
	this->play = gst_element_factory_make( "playbin", "play" );
	g_object_set( G_OBJECT( this->play ), "uri", pathURL.get(), NULL );
	printf( "connecting signals: play(%x)\n", (int)this->play );
	//gst_element_enable_threadsafe_properties( this->play );
	g_signal_connect( this->play, "error", G_CALLBACK( StreamError ), this );
	//g_signal_connect( this->play, "found-tag", G_CALLBACK( ParsedTags ), this );

	this->file = file;
	this->needInit = false;

	nsString path;
	this->file->GetPath( path );
	this->getID3Tags( path );
	return NS_OK;
}

static bool isStringVisible( const char *buf, int len )
{	
	int visible = 0;
	for( int i = 0; i < len; ++i ){
		visible += isprint( buf[i] );
		visible -= isspace( buf[i] );
	}
	return visible != 0;
}

// XXX: share this code with win32 and other imps
bool GstTrack::getID3Tags( const nsString &path )
{
	FILE *file = fopen( NS_ConvertUTF16toUTF8( path ).get(), "rb" );
	if( !file ){
		return false;
	}

	char buf[30];
	int read;

	fseek( file, 0, SEEK_END);
	int fileSize = ftell( file );
	rewind( file );
	// seek before the last 128 bytes
	fseek( file, fileSize - 128, SEEK_CUR );
	//printf( "%lu\n", ftell( file ) );

	// see if we have a tag
	read = fread( buf, 1, 3, file );
	//printf( "read: %d, buf: %s\n", read, buf );
	if( read != 3 || strncmp( buf, "TAG", 3 ) ){
		//printf( "\n\nno TAG found\n" );
		fclose( file );
		return false;
	}
	bool readTitle = false;
	// check for a title
	read = fread( buf, 1, 29, file );
	if( read > 0 ){
		//printf( "title: %s\n", buf );
		if( isStringVisible( buf, read ) ){
			buf[read] = '\0';
			this->title = NS_ConvertUTF8toUTF16( buf );
			readTitle = true;
		}
	}

	// check fo the artist
	read = fread( buf, 1, 29, file );
	if( read > 0 ){
		if( isStringVisible( buf, read ) ){
			buf[read] = '\0';
			this->artist = NS_ConvertUTF8toUTF16( buf );
			// update the title
			/*if( readTitle )
				this->title = this->artist + NS_LITERAL_STRING( " - " ) + this->title;
				*/
		}
	}

	fclose( file );
	return true;
}

/* [noscript] void initTrack (in nsIURL url); */
NS_IMETHODIMP GstTrack::InitTrack(nsIURL *url)
{
	if( this->needInit ){
		nsCString spec;
		nsCOMPtr<nsIFile> file;

		url->GetSpec( spec );

		this->urlSpec = NS_ConvertUTF8toUTF16( spec );

		NS_GetFileFromURLSpec( spec, getter_AddRefs(file) );
		if( !file )
			return NS_ERROR_FAILURE;

		return this->initTrack( file );
	}
  return NS_OK;
}

/* [noscript] void play (); */
NS_IMETHODIMP GstTrack::Play()
{
	if( this->playState != PLAYING ){
		if( this->needInit || !this->play ){
			this->initTrack( this->file );
		}

		printf( "setting volume: %f\n", this->volume );
		this->SetVolume(this->volume);
		printf( "volume set play is : %x\n", (int)this->play );

		if( gst_element_set_state( this->play, GST_STATE_PLAYING ) 
					== GST_STATE_CHANGE_FAILURE ){
			printf( "failed\n" );
			return NS_ERROR_FAILURE;
		}
		printf( "playing\n" );
		this->playState = PLAYING;
	}
	return NS_OK;
}

/* [noscript] void pause (); */
NS_IMETHODIMP GstTrack::Pause()
{
	if( !this->needInit && this->playState != PAUSED ){
		if( gst_element_set_state( this->play, GST_STATE_PAUSED ) 
					== GST_STATE_CHANGE_FAILURE ){
			return NS_ERROR_FAILURE;
		}
		this->playState = PAUSED;
	}
	return NS_OK;
}

/* [noscript] void stop (); */
NS_IMETHODIMP GstTrack::Stop()
{
	if( !this->needInit && this->playState != STOPPED ){
		this->playState = STOPPED;
		gst_element_set_state( play, GST_STATE_NULL );
		gst_object_unref( GST_OBJECT( play ) );
		this->needInit = true;
	}

	return NS_OK;
}

/* [noscript] void setVolume (in float value); */
NS_IMETHODIMP GstTrack::SetVolume(float value)
{
	if( !this->needInit ){
		double val = value;
		g_object_set( play, "volume", val, NULL );
	}
	this->volume = value;
	return NS_OK;
}

/* [noscript] float getVolume (); */
NS_IMETHODIMP GstTrack::GetVolume(float *_retval)
{
	if( !this->needInit ){
		double val = *_retval;
		g_object_get( play, "volume", &val, NULL );
	}
	return NS_OK;
}
