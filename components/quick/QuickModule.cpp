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
#include "Module.h"
#include "QuickTrack.h"
#include "QuickPlayer.h"
#include "xmPlaylist.h"

GENERIC_MODULE_SPEC(QuickTrack)
GENERIC_MODULE_SPEC(QuickPlayer)
GENERIC_MODULE_SPEC(xmPlaylist)

static const nsModuleComponentInfo components[] =
{
	XM_COMPONENT_SPEC( "Media Track Playback Interface",
										 XM_TRACK_CID,
										 XM_TRACK_CONTRACTID,
										 QuickTrack),
	XM_COMPONENT_SPEC( "Singleton Player Interface",
										 XM_PLAYER_CID,
										 XM_PLAYER_CONTRACTID,
										 QuickPlayer),
	XM_COMPONENT_SPEC( "Singleton Playlist Interface",
										 XM_PLAYLIST_CID,
										 XM_PLAYLIST_CONTRACTID,
										 xmPlaylist)
};

NS_IMPL_NSGETMODULE(xMModule, components)
