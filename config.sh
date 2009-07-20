#!/bin/bash

if [ $# -lt 1 ]; then
	echo "usage $0 <xulrunner/dist>"
	exit 1
fi

xulrunner=`readlink -f $1`
if [ $? -ne 0 ]; then
	echo "check that $1 exists!"
	exit 1
fi
platform=$xulrunner
echo "xulrunner: $xulrunner"
echo "platform: $platform"

#rm -f `pwd`/components/Makefile
scriptdir=`dirname $0`
scriptdir=`readlink -f $scriptdir`

# convert windows paths
if ( test `uname | grep -ic Cygwin` -gt 0 )
then
	platform=`cygpath -m $platform`
	scriptdir=`cygpath -m $scriptdir`
fi
	
# remove makefile semilink

if [ -h  `pwd`/components/playlist/Makefile ]; then
	rm -f `pwd`/components/playlist/Makefile
fi

# output make file configuration
cat > $scriptdir/config/makeconf << EOF
include $scriptdir/config/distmake
include $scriptdir/config/pathmake

CPPFLAGS= -I\$(PATHDIST)/include/xpcom/ \\
          -I\$(PATHDIST)/include/nspr/	\\
          -I\$(PATHDIST)/include/string/ \\
          -I\$(PATHDIST)/include/necko/ \\
          -I\$(PATHDIST)/include/layout/ \\
          -I\$(PATHDIST)/include/dom/ \\
          -I\$(PATHDIST)/include/storage/ \\
          -I$scriptdir/components/
CFLAGS=

EOF

if ( test `uname | grep -ic Cygwin` -gt 0 )
then
	dso=dll
	libprefix=
	echo "Configuring Win32"
	ostarget="Cygwin"
	buildtarget="win32"
	ln -s `pwd`/components/playlist/makefile.cl `pwd`/components/playlist/Makefile
# output win32 specific compiler rules
cat >> $scriptdir/config/makeconf << EOF

CXX=cl
LIB_PREFIX=
LIB_SUFFIX=_s.lib
DLL_SUFFIX=.dll
OBJ_SUFFIX=.obj
EXTRA=vc70.pdb

CPPFLAGS+= -DXP_WIN -DXP_WIN32 \\
					-DWIN32_LEAN_AND_MEAN \\
					-DMOZILLA_INTERNAL_API
CFLAGS+= -nologo -W3 -Zi -MDd -Ot

LIBS=\$(PATHDIST)/lib/xul.lib \\
		 \$(PATHDIST)/lib/xpcom.lib \\
		 \$(PATHDIST)/lib/plc4.lib \\
		 \$(PATHDIST)/lib/plds4.lib \\
		 \$(PATHDIST)/lib/nspr4.lib \\
		 strmiids.lib ole32.lib User32.lib 

COMP=\$(CXX) -Fo\$@ \$(CPPFLAGS) \$(CFLAGS) -c \$<
AR=echo \$@; lib -nologo -out:\$@ \$(OBJECTS)
LINK=link -nologo -DLL -out:\$@ \$(OBJECTS) \$(LIBS)

EOF

elif ( test `uname | grep -ic Darwin` -gt 0 )
then
	dso=dylib
	libprefix=lib
	echo "Configuring MacOSX App Bundle"
	if [ -e $scriptdir/XULMusic.app/Contents/Frameworks/ ]; then 
		rm $scriptdir/XULMusic.app/Contents/Frameworks/XUL.framework
		rm $scriptdir/XULMusic.app/Contents/Resources
		rm $scriptdir/XULMusic.app/Contents/MacOS/xulrunner
		rmdir $scriptdir/XULMusic.app/Contents/Frameworks
	fi
	mkdir -p $scriptdir/XULMusic.app/Contents/Frameworks/
	ln -s $platform/XUL.framework $scriptdir/XULMusic.app/Contents/Frameworks/XUL.framework
	ln -s $scriptdir $scriptdir/XULMusic.app/Contents/Resources
	cp $platform/bin/xulrunner $scriptdir/XULMusic.app/Contents/MacOS/xulrunner
	ostarget="Darwin"
	buildtarget="quick"
	ln -s `pwd`/components/playlist/makefile.gcc `pwd`/components/playlist/Makefile

# output osx specific compiler rules
cat >> $scriptdir/config/makeconf << EOF
# Targeting MacOSX - `uname -a`

CXX=ccache g++
LIB_PREFIX=lib
LIB_SUFFIX=_s.a
DLL_SUFFIX=.dylib
OBJ_SUFFIX=.o

CPPFLAGS+=-I/Developer/Headers/FlatCarbon \\
          -I../ \\
          -DXP_MACOSX=1 -DMOZILLA_INTERNAL_API

CFLAGS+=-Wall \\
        -Wno-ctor-dtor-privacy  \\
        -Wno-non-virtual-dtor \\
        -fPIC

NSPR_LIBS=-L\$(DIST)/lib -lplds4 -lplc4 -lnspr4
XPCOM_LIBS=-L\$(DIST)/lib \$(DIST)/XUL.framework/XUL -lmozjs -framework QuickTime -framework Carbon
LIBS=\$(XPCOM_LIBS) \$(NSPR_LIBS)

COMP=echo \$@; \$(CXX) -o \$@ \$(CPPFLAGS) \$(CFLAGS) -c \$<
AR=echo \$@; ar rs \$@ \$(OBJECTS)
LINK=echo \$@; g++ -bundle -o \$@ \$(OBJECTS) \$(LIBS)

EOF


elif ( test `uname | grep -ic Linux` -gt 0 )
then
	dso=so
	libprefix=lib
	echo "Configuring Linux"
	ostarget="Linux"
	buildtarget="gst"
	ln -s `pwd`/components/playlist/makefile.gcc `pwd`/components/playlist/Makefile

# output linux specific compiler rules
cat >> $scriptdir/config/makeconf << EOF

include $scriptdir/config/distmake
include $scriptdir/config/pathmake

LIB_PREFIX=lib
LIB_SUFFIX=_s.a
DLL_SUFFIX=.so
OBJ_SUFFIX=.o

CPPFLAGS+=-DXP_UNIX -DMOZILLA_INTERNAL_API

CFLAGS+=-Wall \\
        -Wno-ctor-dtor-privacy  \\
        -Wno-non-virtual-dtor \\
        `pkg-config gstreamer-0.10 --cflags` \\
        -fPIC

NSPR_LIBS=-L\$(DIST)/lib  -lplds4 -lplc4 -lnspr4 -lpthread -ldl
XPCOM_LIBS=-L\$(DIST)/bin -lxul  -lxpcom
LIBS=\$(XPCOM_LIBS) \$(NSPR_LIBS) `pkg-config gstreamer-0.10 --libs`

COMP=echo \$@; \$(CXX) -o \$@ \$(CPPFLAGS) \$(CFLAGS) -c \$<
AR=echo \$@; ar rs \$@ \$(OBJECTS)
LINK=echo \$@; g++ -shared -o \$@ \$(OBJECTS) \$(LIBS)

EOF

else
	echo "Your system does not appear to be supported. At this time only Cygwin(win32), Darwin(10.4), and Linux(gstreamer-0.10+) are supported";
	exit 1;
fi

cat >> $scriptdir/config/makeconf << EOF

HDR=xmITrack.h xmIPlayer.h xmIPlaylist.h
XPT=xmITrack.xpt xmIPlayer.xpt xmIPlaylist.xpt
XPIDL=\$(DIST)/bin/xpidl
IDLFLAGS= -I\$(PATHDIST)/idl/ -I/.
TYPELIB=echo \$@; \$(XPIDL) -w -v -e \$@ -m typelib \$(IDLFLAGS) \$<
HEADER=echo \$@; \$(XPIDL) -w -v -e \$@ -m header \$(IDLFLAGS) \$<

%.xpt: %.idl
	\$(TYPELIB)
%.h: %.idl
	\$(HEADER)

EOF

# check to make sure CXX is setup
tmp=$$
test="$tmp.cpp"
cat > $test << EOF
#include <stdio.h>
#include <stdlib.h>
int main()
{
	printf( "looks like it works\n" );
	return 0;
}
EOF

if [ $ostarget == "Cygwin" ]; then
	cl $test -nologo -c
	link $tmp.obj -nologo -out:$tmp.exe
	rm -f $tmp.obj
	if [ ! -e $tmp.exe ]; then
		echo "Failed to compile a valid executable using MSVC compiler cl.exe, are you sure your environment is configured correctly?"
		echo "See: http://severna.homeip.net/cygwin.php for details"
		echo "Failed to compile test file: $test"
		exit 1;
	fi
	rm -f $tmp.exe
else
	g++ $test -o test.out
	if [ ! -e test.out ]; then
		echo "Failed to compile a valid binary using GCC compiler, g++.  Are you sure you have GCC installed correcly?"
		echo "Failed to compile test file: $test"
		exit 1;
	fi
	rm -f test.out
fi

echo "Compiler Test Passed"
rm $test

# check for gstreamer 0.10 > if we're running linux
#if [ $ostarget == "Linux" ]; then
#	if ( test `pkg-config gstreamer-0.10 --modversion` -lt 0.10 ); then
#		echo "Linux requires gstreamer-0.10+";
#		exit 1;
#	fi
#fi

if ( test `uname | grep -ic Darwin` -gt 0 )
then

	echo "outputing config/xulrunner.txt"
	cat > config/xulrunner.txt << EOF
$xulrunner/XUL.framework/xulrunner-bin
EOF

else

	echo "outputing config/xulrunner.txt"
	cat > config/xulrunner.txt << EOF
$xulrunner/bin/xulrunner
EOF

fi

echo "outputing config/distmake"
cat > config/distmake << EOF
DIST=$xulrunner
EOF

echo "outputing config/make"
cat > config/pathmake << EOF
PATHDIST=$platform
EOF

echo "outputing chrome/chrome.manifest"
cat > chrome/chrome.manifest << EOF
content xulmusic file:///$scriptdir/chrome/
EOF

echo "outputing config/version"
cat > config/version << EOF
VERSION=0.2
EOF

echo "outputing config/dso"
cat > config/dso << EOF
DSO=$dso
LIBPREFIX=$libprefix
EOF

echo "outputing config/buildtarget"
cat > config/buildtarget << EOF
$buildtarget
EOF
