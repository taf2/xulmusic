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
#include <nsIFile.h>
#include <nsMemory.h>
#include <nsString.h>
#include <nsIProxyObjectManager.h>
#include <nsIClassInfoImpl.h>
#include "GstPlayer.h"

/* Implementation file */
NS_IMPL_ISUPPORTS1_CI(GstPlayer, xmIPlayer)

GstPlayer::GstPlayer()
	:gstInit(false),volume(1.0),activeTrack(0)
{
  /* member initializers and constructor code */
}

GstPlayer::~GstPlayer()
{
  /* destructor code */
}

/* xmITrack newTrack (in nsIURL url); */
NS_IMETHODIMP GstPlayer::NewTrack(nsIURL *url, xmITrack **_retval)
{
	printf( "new track\n" );
	if( !this->gstInit ){
		gst_init( NULL,NULL);
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
NS_IMETHODIMP GstPlayer::GetTrack(xmITrack * *aTrack)
{
	if( this->activeTrack )
		NS_ADDREF( *aTrack = this->activeTrack );
	return NS_OK;
}
NS_IMETHODIMP GstPlayer::SetTrack(xmITrack * aTrack)
{
	if( !aTrack )
		return NS_ERROR_FAILURE;

	if( this->activeTrack != aTrack ){
		printf( "setting track\n" );
		if( this->activeTrack ){
			printf( "stopping active track\n" );
			this->activeTrack->Stop();
			printf( "stopped active track\n" );
		}
		printf( "setting active track\n" );
		this->activeTrack = aTrack;
		this->activeTrack->SetVolume( this->volume );
		printf( "set active track\n" );
	}
	return NS_OK;
}

/* void play (); */
NS_IMETHODIMP GstPlayer::Play()
{
	printf( "Play %x\n", (int)((xmITrack*)this->activeTrack) );
	if( !this->activeTrack )
		return NS_ERROR_FAILURE;
	return this->activeTrack->Play();
}

/* void pause (); */
NS_IMETHODIMP GstPlayer::Pause()
{
	printf( "Pause\n" );
	if( !this->activeTrack )
		return NS_OK;
	return this->activeTrack->Pause();
}

/* void stop (); */
NS_IMETHODIMP GstPlayer::Stop()
{
	printf( "Stop\n" );
	if( !this->activeTrack )
		return NS_OK;
	return this->activeTrack->Stop();
}

/* boolean setVolume (in short value); */
NS_IMETHODIMP GstPlayer::SetVolume(PRInt16 value, PRBool *_retval)
{
	printf( "setting volume\n" );
	*_retval = true;
	if( value > 100 )
		value = 100;
	else
	if( value < 0 )
		value = 0;

	this->volume = (value / 25.0);
	if( !this->activeTrack )
		return NS_OK;

	printf( "SetVolume %x\n", (int)((xmITrack*)this->activeTrack) );
	printf( "setting volume: %f\n", this->volume );
	this->activeTrack->SetVolume( this->volume );
	return NS_OK;
}

/* short getVolume (); */
NS_IMETHODIMP GstPlayer::GetVolume(PRInt16 *_retval)
{
	*_retval = (PRInt16)(this->volume * 25);
	return NS_OK;
}
