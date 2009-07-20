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

OBJECTS=GstTrack.o GstPlayer.o GstModule.o
TARGET=../$(LIB_PREFIX)Player$(LIB_SUFFIX)
EXTRA=

all:$(TARGET)

$(TARGET):$(OBJECTS)
	$(AR)

GstTrack.o:GstTrack.cpp GstTrack.h GstPlayer.h #$(HDR)
	$(COMP)

GstPlayer.o:GstPlayer.cpp GstTrack.h GstPlayer.h #$(HDR)
	$(COMP)

GstModule.o:GstModule.cpp GstTrack.h GstPlayer.h #$(HDR)
	$(COMP)

clean:
	rm -f $(OBJECTS) $(TARGET) $(EXTRA)
