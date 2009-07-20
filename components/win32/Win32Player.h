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
#ifndef XM_PLAYER_H
#define XM_PLAYER_H

#include <nsCOMPtr.h>
#include "xmIPlayer.h"
#include <windows.h>

/* Header file */
class Win32Player : public xmIPlayer
{
public:
  NS_DECL_ISUPPORTS
  NS_DECL_XMIPLAYER

  Win32Player();

private:
  ~Win32Player();

protected:
	bool comInit; // true if we've initialized COM
	PRInt16 volume; // between 100 and 0
	nsCOMPtr<xmITrack> activeTrack;
};
#endif
