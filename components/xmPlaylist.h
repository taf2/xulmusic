#ifndef XM_PLAYLIST_H
#define XM_PLAYLIST_H

#include <nsCOMPtr.h>
#include "xmIPlaylist.h"
#include <mozIStorageConnection.h>

#define XM_PLAYLIST_CONTRACTID "@XulMusic.org;PlayList;1"
// 50ce5e62-60d9-422f-b518-67e2e6928df3
#define XM_PLAYLIST_CID { 0x50ce5e62, 0x60d9, 0x422f, \
  { 0xb5, 0x18, 0x67, 0xe2, 0xe6, 0x92, 0x8d, 0xf3 } }

/* Header file */
class xmPlaylist : public xmIPlaylist
{
public:
  NS_DECL_ISUPPORTS
	NS_DECL_NSITREEVIEW
  NS_DECL_XMIPLAYLIST

  xmPlaylist();

private:
  ~xmPlaylist();

protected:
	nsCOMPtr<mozIStorageConnection> conn;
};


#endif
