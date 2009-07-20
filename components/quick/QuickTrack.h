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
#ifndef QUICK_TRACK_H
#define QUICK_TRACK_H

#include "xmITrack.h"
#include "xmIPlayer.h"
#include <nsCOMPtr.h>
#include <nsString.h>
#include <Movies.h>

/* Header file */
class QuickTrack : public xmITrack
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_XMITRACK

  QuickTrack();

private:
  ~QuickTrack();

protected:

	enum PlayState{
		STOPPED=0,
		PAUSED=1,
		PLAYING=2,
	};

	PlayState state;
	Movie movie;
	nsString title, artist, album, urlSpec;
};

#endif
