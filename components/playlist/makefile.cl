# "The contents of this file are subject to the Mozilla Public License
# Version 1.1 (the "License"); you may not use this file except in
# compliance with the License. You may obtain a copy of the License at
# http://www.mozilla.org/MPL/
#
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
# License for the specific language governing rights and limitations
# under the License.
#
# The Original Code is Todd A. Fisher code.
#
# The Initial Developer of the Original Code is Todd A. Fisher.
# Portions created by the Initial Developer are Copyright (C) 2005
# the Initial Developer. All Rights Reserved.
#
# Contributor(s): 

OBJ=shell.obj
TARGET=shell.exe xmplaylist.db
EXTRA=

# DIST is an absolute path but might not work for win32 so
# don't use it for compiler flags...
include ../../config/distmake
include ../../config/pathmake

CC=cl /nologo
AR=lib /nologo
CPPFLAGS= -I. /DWIN32 -I$(PATHDIST)/include/sqlite3/

CFLAGS=/W3 -MDd /Od /Zi /GS

LIBS=
COMP=echo $@; $(CC) -c -Fo"`cygpath -w $@`" $(CPPFLAGS) $(CFLAGS) $<

# override rules for c files
%.obj: %.c
	$(COMP)

all:$(TARGET)

shell.exe: shell.obj
	echo $@; $(CC) -o $@ $(PATHDIST)/lib/sqlite3_s.lib shell.obj

xmplaylist.db: shell.exe
	echo $@; ./shell.exe xmplaylist.db < playlist.sql

clean:
	rm -f vc70.pdb $(OBJ) $(TARGET) $(EXTRA)
