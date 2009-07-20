# toplevel makefile used for:
# - building in components
# - packaging 

include config/version
include config/dso

DIST=xulmusic-$(VERSION)
DISTPKG=$(DIST).tar.gz

all:
	$(MAKE) -C components/

clean:
	$(MAKE) -C components/ clean

dist:
	rm -rf $(DIST)
	mkdir -p $(DIST)/components/
	mkdir -p $(DIST)/chrome/content/
	svn export https://updates.simosoftware.com/repos/xulmusic/chrome/ $(DIST)/chrome/content/xulmusic/
	svn export https://updates.simosoftware.com/repos/xulmusic/defaults/ $(DIST)/defaults/
	(cd $(DIST)/chrome/ && zip xulmusic.jar content/* content/xulmusic/*)
	rm -rf $(DIST)/chrome/content/
	cp components/`cat config/buildtarget`/*.$(DSO) $(DIST)/components/
	cp components/*.xpt $(DIST)/components/
	cp application.ini $(DIST)/
	echo "content xulmusic jar:xulmusic.jar!/content/xulmusic/" > $(DIST)/chrome/chrome.manifest
	tar -zcf $(DISTPKG) $(DIST)
	rm -rf $(DIST)
