Goal:
------------

	Write a Music Player with a XUL Interface.

	Get it to work in Win32, Linux and Mac OSX.

	Define an XPCOM interface in IDL.

	Implement the interface for each platform.
	Win32: DirectShow
	Linux: gstreamer
	Macosx: Quicktime

	Write the GUI once for all platforms.

	Manage playlists using javascript.


Configuring:
------------
	To configure all you need to do is run the config.sh script
	passing it the location of the xulrunner dist folder.  The
	dist folder layout should contain the following:
	
		dist/idl/
		dist/bin/xulrunner.exe  <-- without the exe on mac and linux
		dist/bin/xpidl.exe  <-- without the exe on mac and linux
		dist/include/xpcom/
		dist/include/necko/
		dist/include/string/
		dist/include/nspr/
		dist/lib/

	Example (Linux/Win32):
		svn co https://updates.simosoftware.com/repos/xulmusic
		cd xulmusic
		./config.sh ../mozilla/debug/dist/
		make
		./start-music.sh


Build Requirements:
-------------------

	ALL:
		* Xulrunner with headers, idl files, and .lib files.
		* A working C++ compiler.
		* Subversion - for getting the source

	Win32:
		* DirectX -
				I used DirectShow and have only tested this using DirectX 9 on a WinXP
				SP2 machine.
		* MSVC compiler

	Linux:
		* GStreamer -
			I've tested this with fedora core 3 using gstreamer 0.8.x  
		* GCC
			I used gcc 4.0 on a fedora core 3 system, gcc 3.4 should work too.

	Mac:
		Currently, no implementation, I'm assuming it can be done using
		Quicktime...

TODO:
----

	* Improve styles, and create our own graphics
	* Implement playback support on the Mac. (Have to build xulrunner first)
	* Add support for streaming audio (GstPlayer should already support it)
		- need to do some work on the interface first.
		  xMPlayer.newTrack should probably take an AString url or nsIURL of some sort...
	* Work on making a better slider control the scrollbar is kinda clunky (XBL)
	* Rewrite playlist so it uses a tree instead of a listbox.
