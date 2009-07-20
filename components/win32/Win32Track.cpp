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
#include <prprf.h>
#include <nsIFile.h>
#include <nsMemory.h>
#include <nsIURL.h>
#include <nsNetUtil.h>
#include "Win32Track.h"

/* Implementation file */
NS_IMPL_ISUPPORTS1_CI(Win32Track, xmITrack)

Win32Track::Win32Track()
	:pGraph(0),pControl(0),
	 pSeeking(0),pEvent(0),pVolume(0),
	 playState(STOPPED),
	 needInit(true)
{
  /* member initializers and constructor code */
}

Win32Track::~Win32Track()
{
  /* destructor code */
	if( pControl )
		pControl->Release();
	if( pEvent )
		pEvent->Release();
	if( pSeeking )
		pSeeking->Release();
	if( pGraph )
		pGraph->Release();
}

/* AString getTitle (); */
NS_IMETHODIMP Win32Track::GetTitle(nsAString & _retval)
{
	_retval = this->title;
	return NS_OK;
}

/* AString getArtist (); */
NS_IMETHODIMP Win32Track::GetArtist(nsAString & _retval)
{
	_retval = this->artist;
	return NS_OK;
}

/* AString getURLSpec (); */
NS_IMETHODIMP Win32Track::GetURLSpec(nsAString & _retval)
{
	_retval = this->urlSpec;
	return NS_OK;
}

/* void setPosition (in long position); */
NS_IMETHODIMP Win32Track::SetPosition(PRInt32 position)
{
	if( !this->needInit && pSeeking ){
		PRInt64 loc = (PRInt64)((PRInt64)position * 10000000);
		HRESULT hr;
		PRInt64 duration;
		hr = pSeeking->GetDuration( &duration );
		if( FAILED(hr) ){
			return NS_ERROR_FAILURE;
		}

		if( loc > duration )
			loc = duration;

		if( loc < 0 )
			loc = 0;

		hr = pSeeking->SetPositions( &loc, AM_SEEKING_AbsolutePositioning, 0, AM_SEEKING_NoPositioning );
		if( FAILED(hr) ){
			return NS_ERROR_FAILURE;
		}
	}
	return NS_OK;
}

/* long getPosition (); */
NS_IMETHODIMP Win32Track::GetPosition(PRInt32 *_retval)
{
	if( !this->needInit && pSeeking ){
		HRESULT hr;
		PRInt64 position;
		hr = pSeeking->GetCurrentPosition( &position );
		if( FAILED(hr) ){
			return NS_ERROR_FAILURE;
		}
		// compute seconds
		*_retval = (PRInt32)(position/10000000);
	}
	else
		*_retval = 0;
	return NS_OK;
}

/* long getDuration (); */
NS_IMETHODIMP Win32Track::GetDuration(PRInt32 *_retval)
{
	if( !this->needInit && pSeeking ){
		HRESULT hr;
		PRInt64 duration;
		hr = pSeeking->GetDuration( &duration );
		if( FAILED(hr) ){
			return NS_ERROR_FAILURE;
		}
		// compute seconds
		*_retval = (PRInt32)(duration/10000000);
	}
	else
		*_retval = 0; // XXX: not sure this is always the correct thing to do...
	return NS_OK;
}

/* void initTrack (in nsIURL url); */
NS_IMETHODIMP Win32Track::InitTrack(nsIURL *url)
{
	HRESULT hr;
	nsString path;
	nsCString spec;
	nsCOMPtr<nsIFile> file;

	url->GetSpec( spec );

	this->urlSpec = NS_ConvertUTF8toUTF16( spec );

	NS_GetFileFromURLSpec( spec, getter_AddRefs(file) );
	if( !file )
		return NS_ERROR_FAILURE;

	// Create the filter graph manager and query for interfaces.
	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
											IID_IGraphBuilder, (void **)&pGraph);
	if( FAILED(hr) ){
		printf("ERROR - Could not create the Filter Graph Manager.");
		return NS_ERROR_FAILURE;
	}

	hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
	if( FAILED(hr) ){
		return NS_ERROR_FAILURE;
	}
	hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);
	if( FAILED(hr) ){
		return NS_ERROR_FAILURE;
	}
	hr = pGraph->QueryInterface(IID_IMediaSeeking, (void **)&pSeeking);
	if( FAILED(hr) ){
		return NS_ERROR_FAILURE;
	}
	hr = pGraph->QueryInterface(IID_IBasicAudio, (void **)&pVolume);
	if( FAILED(hr) ){
		return NS_ERROR_FAILURE;
	}

	file->GetPath( path );
	hr = pGraph->RenderFile( path.get(), NULL );
	if( FAILED(hr) ){
		return NS_ERROR_FAILURE;
	}

	// set title to the file leaf name to start
	file->GetLeafName( this->title );
	this->needInit = false;

	// scan for id3tags
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

bool Win32Track::getID3Tags( const nsString &path )
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
	read = fread( buf, 1, 30, file );
	if( read > 0 ){
		//printf( "title: %s\n", buf );
		if( isStringVisible( buf, read ) ){
			this->title = NS_ConvertUTF8toUTF16( buf );
			readTitle = true;
		}
	}

	// check fo the artist
	read = fread( buf, 1, 30, file );
	if( read > 0 ){
		if( isStringVisible( buf, read ) ){
			this->artist = NS_ConvertUTF8toUTF16( buf );
			// update the title
			if( readTitle )
				this->title = this->artist + NS_LITERAL_STRING( " - " ) + this->title;
		}
	}

	fclose( file );
	return true;
}

/* void play (); */
NS_IMETHODIMP Win32Track::Play()
{
	if( !this->needInit && this->playState != PLAYING ){
		HRESULT hr = pControl->Run();
		if( FAILED(hr) ){
			return NS_ERROR_FAILURE;
		}
		this->playState = PLAYING;
	}
	return NS_OK;
}

/* void pause (); */
NS_IMETHODIMP Win32Track::Pause()
{
	if( !this->needInit && this->playState != PAUSED ){
		HRESULT hr = pControl->Pause();
		if( FAILED(hr) ){
			return NS_ERROR_FAILURE;
		}
		this->playState = PAUSED;
	}
	return NS_OK;
}

/* void stop (); */
NS_IMETHODIMP Win32Track::Stop()
{
	if( !this->needInit && this->playState != STOPPED ){
		HRESULT hr = pControl->Stop();
		if( FAILED(hr) ){
			return NS_ERROR_FAILURE;
		}
		LONGLONG llPos = 0;
		pSeeking->SetPositions(&llPos, AM_SEEKING_AbsolutePositioning,
													 &llPos, AM_SEEKING_NoPositioning);
		this->playState = STOPPED;
	}
	return NS_OK;
}

/* short getState (); */
NS_IMETHODIMP Win32Track::GetState(PRInt16 *_retval)
{
	*_retval = (PRInt16)this->playState;
	return NS_OK;
}

/* [noscript] void setVolume (in float value); */
NS_IMETHODIMP Win32Track::SetVolume(float value)
{
	value *= 100;
	value -= 10000;
	HRESULT hr = pVolume->put_Volume( (long)value );
	if( hr != S_OK )
		return NS_ERROR_FAILURE;
	return NS_OK;
}

/* [noscript] float getVolume (); */
NS_IMETHODIMP Win32Track::GetVolume(float *_retval)
{
	pVolume->get_Volume( (long*)_retval );
	*_retval /= 100;
	*_retval += 100;
	return NS_OK;
}
