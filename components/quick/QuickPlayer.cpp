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
#include <nsComponentManagerUtils.h>
#include <nsServiceManagerUtils.h>
#include <Movies.h>
#include "QuickPlayer.h"

/* Implementation file */
NS_IMPL_ISUPPORTS1_CI(QuickPlayer, xmIPlayer)

QuickPlayer::QuickPlayer()
	: volume(80), activeTrack(0)
{
  /* member initializers and constructor code */
	EnterMovies();
}

QuickPlayer::~QuickPlayer()
{
  /* destructor code */
	ExitMovies();
}

/* xmITrack newTrack (in nsIURL url); */
NS_IMETHODIMP QuickPlayer::NewTrack(nsIURL *url, xmITrack **_retval)
{
	nsCOMPtr<xmITrack> track = do_CreateInstance( XM_TRACK_CONTRACTID );
	nsresult rv = track->InitTrack( url );
	if( NS_FAILED( rv ) )
		return rv;
	track->SetVolume( this->volume );
	NS_ADDREF( *_retval = track );
	return NS_OK;
}

/* attribute xmITrack track; */
NS_IMETHODIMP QuickPlayer::GetTrack(xmITrack * *aTrack)
{
	if( this->activeTrack )
		NS_ADDREF( *aTrack = this->activeTrack );
	return NS_OK;
}
NS_IMETHODIMP QuickPlayer::SetTrack(xmITrack * aTrack)
{
	if( this->activeTrack != aTrack ){
		if( this->activeTrack ){
			this->activeTrack->Stop();
		}
		this->activeTrack = aTrack;
		this->activeTrack->SetVolume( this->volume );
	}
	return NS_OK;
}

/* void play (); */
NS_IMETHODIMP QuickPlayer::Play()
{
	if( !this->activeTrack )
		return NS_ERROR_FAILURE;
	return this->activeTrack->Play();
}

/* void pause (); */
NS_IMETHODIMP QuickPlayer::Pause()
{
	if( !this->activeTrack )
		return NS_OK;
	return this->activeTrack->Pause();
}

/* void stop (); */
NS_IMETHODIMP QuickPlayer::Stop()
{
	if( !this->activeTrack )
		return NS_OK;
	return this->activeTrack->Stop();
}

/* boolean setVolume (in short value); */
NS_IMETHODIMP QuickPlayer::SetVolume(PRInt16 value, PRBool *_retval)
{
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
NS_IMETHODIMP QuickPlayer::GetVolume(PRInt16 *_retval)
{
	*_retval = (PRInt16)this->volume;
	return NS_OK;
}
