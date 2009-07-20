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

include ../../config/makeconf

OBJECTS=Win32Track.o Win32Player.o Win32Module.o
TARGET=../$(LIB_PREFIX)Player$(LIB_SUFFIX)
EXTRA=$(LIB_PREFIX)Player_s.exp vc70.pdb

all:$(TARGET)

$(TARGET):$(OBJECTS)
	$(AR)

Win32Track.o:Win32Track.cpp Win32Track.h Win32Player.h
	$(COMP)

Win32Player.o:Win32Player.cpp Win32Track.h Win32Player.h
	$(COMP)

Win32Module.o:Win32Module.cpp Win32Track.h Win32Player.h
	$(COMP)

clean:
	rm -f $(OBJECTS) $(TARGET) $(EXTRA)
