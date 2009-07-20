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
#include <nsComponentManagerUtils.h>
#include <nsServiceManagerUtils.h>
#include <nsMemory.h>
#include <nsString.h>
#include "Win32Player.h"
#include "xmITrack.h"
#include "Win32Track.h"
#include <process.h>
#include <windows.h>
#include <dshow.h> // included for IGraphBuilder IMediaControl etc..

static
void ErrorExit(LPTSTR lpszFunction)
	{
	TCHAR szBuf[80];
	LPVOID lpMsgBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMsgBuf,
		0, NULL );

	wsprintf(szBuf,
		"%s failed with error %d: %s",
		lpszFunction, dw, lpMsgBuf);

	MessageBox(NULL, szBuf, "Error", MB_OK);

	LocalFree(lpMsgBuf);
}

/* Implementation file */
NS_IMPL_ISUPPORTS1_CI(Win32Player, xmIPlayer)

Win32Player::Win32Player()
	:comInit(false),volume(50),activeTrack(0)
{
  /* member initializers and constructor code */
}

Win32Player::~Win32Player()
{
  /* destructor code */
	if( this->comInit )
		CoUninitialize();
	this->activeTrack = 0;
}


/* xmITrack newTrack (in nsIURL url); */
NS_IMETHODIMP Win32Player::NewTrack(nsIURL *url, xmITrack **_retval)
{
	if( !this->comInit ){
		HRESULT hr = CoInitialize(NULL);
		if( FAILED(hr) ){
			printf("ERROR - Could not initialize COM library");
			return NS_ERROR_FAILURE;
		}
		this->comInit = true;
	}
	nsCOMPtr<xmITrack> track = do_CreateInstance( XM_TRACK_CONTRACTID );
	nsresult rv = track->InitTrack( url );
	if( NS_FAILED( rv ) ){
		return rv;
	}
	track->SetVolume( this->volume );
	NS_ADDREF( *_retval = track );
	return NS_OK;
}

/* attribute xmITrack track; */
NS_IMETHODIMP Win32Player::GetTrack(xmITrack * *aTrack)
{
	if( this->activeTrack )
		NS_ADDREF( *aTrack = this->activeTrack );
	return NS_OK;
}


NS_IMETHODIMP Win32Player::SetTrack(xmITrack * aTrack)
{
	if( this->activeTrack != aTrack ){
		if( this->activeTrack ){
			this->activeTrack->Stop();
			//gRunEventLoop = 0;
		}
		this->activeTrack = aTrack;
		// sync the volume
		this->activeTrack->SetVolume( this->volume );
	}
	return NS_OK;
}

/* void play (); */
NS_IMETHODIMP Win32Player::Play()
{
	printf( "Play\n" );
	if( !this->activeTrack )
		return NS_ERROR_FAILURE;
	return this->activeTrack->Play();
}

/* void pause (); */
NS_IMETHODIMP Win32Player::Pause()
{
	printf( "Pause\n" );
	if( !this->activeTrack )
		return NS_OK;
	return this->activeTrack->Pause();
}

/* void stop (); */
NS_IMETHODIMP Win32Player::Stop()
{
	printf( "Stop\n" );
	if( !this->activeTrack )
		return NS_OK;
	return this->activeTrack->Stop();
}

/* boolean setVolume (in short value); */
NS_IMETHODIMP Win32Player::SetVolume(PRInt16 value, PRBool *_retval)
{
	*_retval = true;

	if( value > 100 )
		value = 100;
	else if( value < 0 )
		value = 0;

	this->volume = value;

	if( !this->activeTrack ){
		*_retval = false;
		return NS_OK;
	}

	return this->activeTrack->SetVolume( value );
}

/* short getVolume (); */
NS_IMETHODIMP Win32Player::GetVolume(PRInt16 *_retval)
{
	*_retval = this->volume;
	return NS_OK;
}
