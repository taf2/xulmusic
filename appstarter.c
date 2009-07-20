#include <stdio.h>
#include <sys/param.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define XULFramework "/Users/taf2/projects/moztrunk/debug/dist/XUL.framework/"
#define BUNDLE "/Users/taf2/projects/xulmusic/XULMusic.app/"

#define DYLD_LIBRARY_PATH XULFramework"/Versions/1.8b4/"
#define XULRUNNER DYLD_LIBRARY_PATH"/xulrunner-bin"
#define XRE_BINARY_PATH DYLD_LIBRARY_PATH"/xulrunner-bin"
#define ARGV0 BUNDLE"/Contents/MacOS/xulrunner-bin"
#define APPINI BUNDLE"/Contents/Resources/application.ini"

int main()
{
	printf( "setenv: %s %s\n", "DYLD_LIBRARY_PATH",  DYLD_LIBRARY_PATH );
	printf( "setenv: %s %s\n", "XRE_BINARY_PATH", XRE_BINARY_PATH );
	printf( "execute: %s %s, %s\n", XULRUNNER, ARGV0, APPINI );
 
	setenv( "DYLD_LIBRARY_PATH", DYLD_LIBRARY_PATH, 1 );
	setenv( "DYLD_FRAMEWORK_PATH", XULFramework, 1 );
	setenv( "XRE_BINARY_PATH", XRE_BINARY_PATH, 1 );
	execl( XULRUNNER, ARGV0, APPINI, (char*)0 );

	return 0;
}
