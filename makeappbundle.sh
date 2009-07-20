#!/bin/bash
# this assumes xulrunner is on the users system and installed
# as a framework.
# for testing purposes the path to the bundle and
# the path to xulrunner framework are hardcoded.
# to start the only things this customizes is the bundle
# name, version, vender, and copyright.

scriptdir=`dirname $0`
scriptdir=`readlink -f $scriptdir`

PACKAGE=$scriptdir
BUNDLE=$PACKAGE/XULMusic.app
RESOURCE=$BUNDLE/Contents/Resources
XULRUNNER=`cat $PACKAGE/config/distmake |  sed s/DIST=//`
APPINI=$PACKAGE/application.ini

# get the package details
VENDOR=`grep ^Vendor= $APPINI | sed s/Vendor=//`
NAME=`grep ^Name= $APPINI | sed s/Name=//`
VERSION=`grep ^Version= $APPINI | sed s/Version=//`
COPYRIGHT=`grep ^Copyright= $APPINI | sed s/Copyright=//`

# create the resources folder
#if [ ! -e $RESOURCE ]; then
	mkdir -p $RESOURCE
	# copy files from source package to resources folder
	ln -s $PACKAGE/chrome $RESOURCE/chrome
	ln -s $PACKAGE/components $RESOURCE/components
	ln -s $APPINI $RESOURCE/application.ini
	ln -s document.icns $RESOURCE/document.icns
#fi

#gcc -Wall -o appstarter appstarter.c
# create the symbolic link from xulrunner to MacOS
#if [ ! -e $BUNDLE/Contents/MacOS ]; then
	#ln -s $XULRUNNER/XUL.framework/Versions/Current/ $BUNDLE/Contents/MacOS
	mkdir -p $BUNDLE/Contents/MacOS

	#if [ ! -e $PACKAGE/appstarter ]; then
		gcc -Wall -o appstarter appstarter.c
		mv appstarter  $BUNDLE/Contents/MacOS/appstarter
		#ln -s $PACKAGE/appstarter  $BUNDLE/Contents/MacOS/appstarter
	#fi
#fi

# copy defaults folder
#if [ ! -e $RESOURCE/defaults/preferences ]; then
	ln -s $PACKAGE/defaults $RESOURCE/defaults
#fi

# copy the xul framework 
mkdir -p $BUNDLE/Contents/Frameworks
ln -s $XULRUNNER/XUL.framework $BUNDLE/Contents/Frameworks/XUL.framework

if [ ! -e  $BUNDLE/Contents/Info.plist ]; then
# create the Info.plist
cat > $BUNDLE/Contents/Info.plist << EOF
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple Computer//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
	<key>CFBundleDevelopmentRegion</key>
	<string>English</string>
	<key>CFBundleExecutable</key>
	<string>appstarter</string>
	<key>CFBundleGetInfoString</key>
	<string>$NAME $VERSION $COPYRIGHT</string>
	<key>CFBundleIdentifier</key>
	<string>org.$VENDOR.$NAME</string>
	<key>CFBundleInfoDictionaryVersion</key>
	<string>$VERSION</string>
	<key>CFBundleDisplayName</key>
	<string>$NAME</string>
	<key>CFBundleName</key>
	<string>$NAME</string>
	<key>CFBundlePackageType</key>
	<string>APPL</string>
	<key>CFBundleShortVersionString</key>
	<string>$VERSION</string>
	<key>CFBundleSignature</key>
	<string>MOZB</string>
	<key>CFBundleIconFile</key>
	<string>document.icns</string>
	<key>CFBundleVersion</key>
	<string>$VERSION</string>
	<key>NSAppleScriptEnabled</key>
	<true/>
</dict>
</plist>
EOF

fi
