#include <nsMemory.h>
#include "xmPlaylist.h"
#include <nsIClassInfoImpl.h>
#include <mozIStorageService.h>

/* Implementation file */
NS_IMPL_ISUPPORTS2_CI(xmPlaylist, xmIPlaylist, nsITreeView)

xmPlaylist::xmPlaylist()
	:conn(0)
{
  /* member initializers and constructor code */
}

xmPlaylist::~xmPlaylist()
{
  /* destructor code */
}

/* readonly attribute long rowCount; */
NS_IMETHODIMP xmPlaylist::GetRowCount(PRInt32 *aRowCount)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* attribute nsITreeSelection selection; */
NS_IMETHODIMP xmPlaylist::GetSelection(nsITreeSelection * *aSelection)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
NS_IMETHODIMP xmPlaylist::SetSelection(nsITreeSelection * aSelection)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void getRowProperties (in long index, in nsISupportsArray properties); */
NS_IMETHODIMP xmPlaylist::GetRowProperties(PRInt32 index, nsISupportsArray *properties)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void getCellProperties (in long row, in nsITreeColumn col, in nsISupportsArray properties); */
NS_IMETHODIMP xmPlaylist::GetCellProperties(PRInt32 row, nsITreeColumn *col, nsISupportsArray *properties)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void getColumnProperties (in nsITreeColumn col, in nsISupportsArray properties); */
NS_IMETHODIMP xmPlaylist::GetColumnProperties(nsITreeColumn *col, nsISupportsArray *properties)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean isContainer (in long index); */
NS_IMETHODIMP xmPlaylist::IsContainer(PRInt32 index, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean isContainerOpen (in long index); */
NS_IMETHODIMP xmPlaylist::IsContainerOpen(PRInt32 index, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean isContainerEmpty (in long index); */
NS_IMETHODIMP xmPlaylist::IsContainerEmpty(PRInt32 index, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean isSeparator (in long index); */
NS_IMETHODIMP xmPlaylist::IsSeparator(PRInt32 index, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean isSorted (); */
NS_IMETHODIMP xmPlaylist::IsSorted(PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean canDrop (in long index, in long orientation); */
NS_IMETHODIMP xmPlaylist::CanDrop(PRInt32 index, PRInt32 orientation, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void drop (in long row, in long orientation); */
NS_IMETHODIMP xmPlaylist::Drop(PRInt32 row, PRInt32 orientation)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* long getParentIndex (in long rowIndex); */
NS_IMETHODIMP xmPlaylist::GetParentIndex(PRInt32 rowIndex, PRInt32 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean hasNextSibling (in long rowIndex, in long afterIndex); */
NS_IMETHODIMP xmPlaylist::HasNextSibling(PRInt32 rowIndex, PRInt32 afterIndex, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* long getLevel (in long index); */
NS_IMETHODIMP xmPlaylist::GetLevel(PRInt32 index, PRInt32 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString getImageSrc (in long row, in nsITreeColumn col); */
NS_IMETHODIMP xmPlaylist::GetImageSrc(PRInt32 row, nsITreeColumn *col, nsAString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* long getProgressMode (in long row, in nsITreeColumn col); */
NS_IMETHODIMP xmPlaylist::GetProgressMode(PRInt32 row, nsITreeColumn *col, PRInt32 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString getCellValue (in long row, in nsITreeColumn col); */
NS_IMETHODIMP xmPlaylist::GetCellValue(PRInt32 row, nsITreeColumn *col, nsAString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* AString getCellText (in long row, in nsITreeColumn col); */
NS_IMETHODIMP xmPlaylist::GetCellText(PRInt32 row, nsITreeColumn *col, nsAString & _retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setTree (in nsITreeBoxObject tree); */
NS_IMETHODIMP xmPlaylist::SetTree(nsITreeBoxObject *tree)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void toggleOpenState (in long index); */
NS_IMETHODIMP xmPlaylist::ToggleOpenState(PRInt32 index)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void cycleHeader (in nsITreeColumn col); */
NS_IMETHODIMP xmPlaylist::CycleHeader(nsITreeColumn *col)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void selectionChanged (); */
NS_IMETHODIMP xmPlaylist::SelectionChanged()
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void cycleCell (in long row, in nsITreeColumn col); */
NS_IMETHODIMP xmPlaylist::CycleCell(PRInt32 row, nsITreeColumn *col)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* boolean isEditable (in long row, in nsITreeColumn col); */
NS_IMETHODIMP xmPlaylist::IsEditable(PRInt32 row, nsITreeColumn *col, PRBool *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setCellValue (in long row, in nsITreeColumn col, in AString value); */
NS_IMETHODIMP xmPlaylist::SetCellValue(PRInt32 row, nsITreeColumn *col, const nsAString & value)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void setCellText (in long row, in nsITreeColumn col, in AString value); */
NS_IMETHODIMP xmPlaylist::SetCellText(PRInt32 row, nsITreeColumn *col, const nsAString & value)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void performAction (in wstring action); */
NS_IMETHODIMP xmPlaylist::PerformAction(const PRUnichar *action)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void performActionOnRow (in wstring action, in long row); */
NS_IMETHODIMP xmPlaylist::PerformActionOnRow(const PRUnichar *action, PRInt32 row)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void performActionOnCell (in wstring action, in long row, in nsITreeColumn col); */
NS_IMETHODIMP xmPlaylist::PerformActionOnCell(const PRUnichar *action, PRInt32 row, nsITreeColumn *col)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* long addFile (in nsIFile file); */
NS_IMETHODIMP xmPlaylist::AddFile(nsIFile *file, PRInt32 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* long addFeed (in nsIURL url); */
NS_IMETHODIMP xmPlaylist::AddFeed(nsIURL *url, PRInt32 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* long addHttp (in nsIURL url); */
NS_IMETHODIMP xmPlaylist::AddHttp(nsIURL *url, PRInt32 *_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* void removeTrack (in long trackId); */
NS_IMETHODIMP xmPlaylist::RemoveTrack(PRInt32 trackId)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}

/* xmITrack getTrack (in long trackId); */
NS_IMETHODIMP xmPlaylist::GetTrack(PRInt32 trackId, xmITrack **_retval)
{
    return NS_ERROR_NOT_IMPLEMENTED;
}
