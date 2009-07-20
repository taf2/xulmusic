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
#ifndef XM_WIN32_TRACK_H
#define XM_WIN32_TRACK_H

#include <nsCOMPtr.h>
#include <nsString.h>
#include "xmITrack.h"
#include "xmIPlayer.h"
#include <dshow.h> // included for IGraphBuilder IMediaControl etc..

/* Header file */
class Win32Track : public xmITrack
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_XMITRACK

  Win32Track();

	static void EventLoop( void *track );
private:
  ~Win32Track();

protected:
	IGraphBuilder *pGraph;
	IMediaControl *pControl;
	IMediaSeeking *pSeeking;
	IMediaEvent   *pEvent;
	IBasicAudio   *pVolume;

	enum PlayState{
		STOPPED=0,
		PAUSED=1,
		PLAYING=2,
	};

	PlayState playState;
	bool needInit;
	nsString urlSpec;
	nsString title;
	nsString artist;

	bool getID3Tags( const nsString &path );
};

#endif
