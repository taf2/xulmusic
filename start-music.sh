#!/bin/bash
#set -x
scriptdir=`dirname $0`
scriptdir=`readlink -f $scriptdir`

xulrunner=`cat $scriptdir/config/xulrunner.txt`
#xulrunner=`readlink -f $scriptdir/$xulrunner`

appini=$scriptdir/application.ini

export XPCOM_DEBUG_BREAK=warn

if ( test `uname | grep -ic Cygwin` -gt 0 )
then
	appini=`cygpath -w $appini`
fi

if ( test `uname | grep -ic Darwin` -gt 0 )
then
	open $scriptdir/XULMusic.app
else
	$xulrunner $appini
fi
