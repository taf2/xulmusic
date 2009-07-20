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
#ifndef QUICK_PLAYER_H
#define QUICK_PLAYER_H

#include "xmITrack.h"
#include "xmIPlayer.h"
#include <nsCOMPtr.h>

/* Header file */
class QuickPlayer : public xmIPlayer
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_XMIPLAYER

  QuickPlayer();

private:
  ~QuickPlayer();

protected:
	float volume;
	nsCOMPtr<xmITrack> activeTrack;
};

#endif
