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
#include <nsMemory.h>
#include <nsString.h>
#include <nsIURL.h>
#include <nsNetUtil.h>
#include "QuickTrack.h"

#define QUICK_CHECK( exp )\
	if( (exp) != noErr ) return NS_ERROR_FAILURE;

/* Implementation file */
NS_IMPL_ISUPPORTS1_CI(QuickTrack, xmITrack)

QuickTrack::QuickTrack()
	:state(STOPPED),movie(0)
{
  /* member initializers and constructor code */
}

QuickTrack::~QuickTrack()
{
  /* destructor code */
	if( this->movie ){
		// free resource
		DisposeMovie( this->movie );
	}
}

/* AString getTitle (); */
NS_IMETHODIMP QuickTrack::GetTitle(nsAString & _retval)
{
	_retval = this->title;
	return NS_OK;;
}

/* AString getArtist (); */
NS_IMETHODIMP QuickTrack::GetArtist(nsAString & _retval)
{
	_retval = this->artist;
	return NS_OK;;
}

/* AString getURLSpec (); */
NS_IMETHODIMP QuickTrack::GetURLSpec(nsAString & _retval)
{
	_retval = this->urlSpec;
	return NS_OK;
}

/* void setPosition (in long position); */
NS_IMETHODIMP QuickTrack::SetPosition(PRInt32 position)
{
	if( this->movie ){
		//TimeValue duration = GetMovieDuration( this->movie );
		TimeValue timeval = position * 600;
		SetMovieTimeValue( this->movie, timeval );
	}
	return NS_OK;
}

/* long getPosition (); */
NS_IMETHODIMP QuickTrack::GetPosition(PRInt32 *_retval)
{
	if( this->movie ){
		TimeValue timeval = GetMovieTime( this->movie, NULL ) / 600;
		*_retval = timeval;// / duration;
	}
  return NS_OK;
}

/* long getDuration (); */
NS_IMETHODIMP QuickTrack::GetDuration(PRInt32 *_retval)
{
	if( this->movie ){
		*_retval = (((int)GetMovieDuration( this->movie )) / 60) / 10;
	}
	return NS_OK;
}

/* short getState (); */
NS_IMETHODIMP QuickTrack::GetState(PRInt16 *_retval)
{
	*_retval = this->state;
	return NS_OK;
}

#if 0
static bool 
IsStringVisible( const char *buf, int len )
{	
	int i;
	int visible = 0;

	for( i = 0; i < len; ++i ){
		visible += isprint( buf[i] );
		visible -= isspace( buf[i] );
	}
	return visible != 0;
}
#endif

// see: http://developer.apple.com/qa/qa2001/qa1135.html
const UInt8 kUserDataIsText = 0xA9; // the copyright symbol
static bool
GetID3Tags( Movie movie, nsString &title, nsString &artist, nsString &album )
{
	OSErr err = noErr;
	OSType udType = 0;
	int dataCount = 0;
	UserData trackData = GetMovieUserData( movie );
	Handle textData = NewHandle(0);

	if( !trackData ){
		printf( "no tags!\n" );
		return false;
	}

	printf( "processing ID3Tags\n" );
	//int chk = 0;

	while( (udType = GetNextUserDataType( trackData, udType ) ) ){
		printf( "tag type: %d\n", (int)udType );
		if( (udType>>24) == kUserDataIsText ){ //|| udType == 1835365473 ){
			dataCount = CountUserDataType( trackData, udType );
			printf( "data count: %d\n", dataCount );
			for( int i = 1; i <= dataCount; ++i ){
				// FIXME: support for other languages??
				err = GetUserDataText( trackData, textData, udType, i, langEnglish );
				PtrAndHand(0, textData, 1);
				HLock( textData );
				if( (udType>>16) == 'n' && (udType>>8) == 'a' && udType == 'm' ){
					title = NS_ConvertUTF8toUTF16(*textData);
				}
				printf("\n%c%c%c%c: %s\n", (char)(udType>>24),
							(char)(udType>>16),
							(char)(udType>>8),
							(char)udType, *textData);
				HUnlock( textData );
			}
		}
	}
	printf( "processed ID3Tags\n" );
	DisposeHandle( textData );
	return true;
}


/* [noscript] void initTrack (in nsIURL url); */
NS_IMETHODIMP QuickTrack::InitTrack(nsIURL *url)
{
	printf( "Initializing track\n" );
	if( this->movie ){
		// free resource
		DisposeMovie( this->movie );
	}

	// get a native file path from the url
	nsString path;
	nsCString spec;
	nsCOMPtr<nsIFile> file;

	url->GetSpec( spec );
	this->urlSpec = NS_ConvertUTF8toUTF16( spec );
	NS_GetFileFromURLSpec( spec, getter_AddRefs(file) );
	if( !file )
		return NS_ERROR_FAILURE;

	short	movieRefNum;
	short resID = 0;
	Str255 name;
	Boolean changed;
	FSRef fsref;
	FSSpec movieFileSpec;

	// get the path string
	file->GetPath( path );

	// do some quick time stuff to get a handle for the movie
	QUICK_CHECK( FSPathMakeRef( (const UInt8*)NS_ConvertUTF16toUTF8( path ).get(), &fsref, 0 ) );
	QUICK_CHECK( FSGetCatalogInfo( &fsref, kFSCatInfoVolume, NULL, NULL, &movieFileSpec, NULL ) );
	QUICK_CHECK( OpenMovieFile( &movieFileSpec, &movieRefNum, fsRdPerm ) );
	QUICK_CHECK( NewMovieFromFile( &(this->movie), movieRefNum, &resID, name, newMovieActive, &changed ) );

	// parse the id3tags from the file
	// XXX: this is mp3 only! we need an abstract way of accessing music tags
	GetID3Tags( this->movie, this->title, this->artist, this->album );

	if( this->title.Length() == 0 ){
		printf( "couldn't get id3 tags for the title\n" );
		// if we couldn't a title string use the file name
		file->GetLeafName( this->title );
	}

	QUICK_CHECK( CloseMovieFile( movieRefNum ) );

	return NS_OK;
}

/* [noscript] void play (); */
NS_IMETHODIMP QuickTrack::Play()
{
	if( this->movie && this->state != PLAYING ){
		this->state = PLAYING;
		StartMovie( this->movie );
	}
	return NS_OK;
}

/* [noscript] void pause (); */
NS_IMETHODIMP QuickTrack::Pause()
{
	if( this->movie && this->state == PLAYING ){
		this->state = PAUSED;
		StopMovie( this->movie );
	}
	return NS_OK;
}

/* [noscript] void stop (); */
NS_IMETHODIMP QuickTrack::Stop()
{
	if( this->movie && this->state != STOPPED ){
		this->state = STOPPED;
		StopMovie( this->movie );
		GoToBeginningOfMovie( this->movie );
	}
	return NS_OK;
}

/* [noscript] void setVolume (in float value); */
NS_IMETHODIMP QuickTrack::SetVolume(float value)
{
	if( this->movie ){
		// do some bit operations to convert float value to short
		short volume = 0;
		float percent = value / 100;
		float bitson = 16 * percent;
		
		for( int i = 0; i < bitson; ++i )
			volume &= ~(1 <<i);

		SetMovieVolume( this->movie, (short)value );
	}
	return NS_OK;
}

/* [noscript] float getVolume (); */
NS_IMETHODIMP QuickTrack::GetVolume(float *_retval)
{
	// FIXME: this won't return a proper value between 0 and 100
	if( this->movie )
		*_retval = (float)GetMovieVolume( this->movie );
	return NS_OK;
}
