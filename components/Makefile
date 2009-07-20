include ../config/makeconf

TARGET=$(LIB_PREFIX)xmMusic$(DLL_SUFFIX)
OBJECTS=xmPlaylist$(OBJ_SUFFIX) #id3tag$(OBJ_SUFFIX)
LIBS+=$(LIB_PREFIX)Player$(LIB_SUFFIX)
EXTRA+=
CPPFLAGS+=-I$(PATHDIST)/include/storage

all: $(HDR) $(XPT) $(LIB_PREFIX)Player$(LIB_SUFFIX) $(OBJECTS) $(TARGET)
	$(MAKE) -C `cat ../config/buildtarget`
	$(MAKE) -C playlist

$(TARGET):$(OBJECTS) $(LIB_PREFIX)Player$(LIB_SUFFIX)
	$(LINK)

$(LIB_PREFIX)Player$(LIB_SUFFIX):
	$(MAKE) -C `cat ../config/buildtarget`

xmPlaylist$(OBJ_SUFFIX): xmPlaylist.cpp xmPlaylist.h
	$(COMP)

#id3tag$(OBJ_SUFFIX): id3tag.cpp id3tag.h
#	$(COMP)

clean:
	rm -f $(HDR) $(XPT) $(TARGET) $(EXTRA) $(OBJECTS)
	$(MAKE) -C `cat ../config/buildtarget` clean
	$(MAKE) -C playlist clean
