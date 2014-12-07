
-include Makefile.local

PKGCONFIG = PKG_CONFIG_PATH="xdrpp:$${PKG_CONFIG_PATH}" pkg-config

# Seems to fix issues with the latest Xcode
LD := $(CXX)

# Don't use :=, or will break first time
XDRC = $(shell $(PKGCONFIG) --variable=xdrc xdrpp)

CPPFLAGS := `$(PKGCONFIG) --cflags xdrpp` -I.
LIBDIRS := -Llibclient
LIBS := -lclient `$(PKGCONFIG) --libs xdrpp` -ldl

CFLAGS := -ggdb3 -O0 -pthread
CXXFLAGS := -ggdb3 -O0 -pthread -std=c++11 -U__STRICT_ANSI__
LDFLAGS := -g -pthread $(LIBDIRS)

default: all

include server/Makefile
include libclient/Makefile
include shell/Makefile
include apps/Makefile
include paxos/Makefile

.PHONY: all clean xdrpp

all: xdrpp paxos/paxos.hh include/server.hh libclient/libclient.a server/server shell/shell paxos/libpaxos.a paxos/paxos_replica \
    apps/leader_elect apps/double_barrier apps/group_membership

paxos/paxos.hh: paxos/paxos.x
	$(XDRC) -hh -o paxos/paxos.hh $<

include/server.hh: include/server.x
	$(XDRC) -hh -o include/server.hh $<

include/event.hh: include/event.x
	$(XDRC) -hh -o include/event.hh $<

xdrpp:
	cd xdrpp; test -f Makefile || ./configure CXX="$(CXX)" CXXFLAGS="$(CXXFLAGS)"
	$(MAKE) -C xdrpp

clean:
	rm -f server/server
	rm -f server/*.o
	rm -f libclient/*.o
	rm -f libclient/libclient.a
	rm -f shell/*.o
	rm -f shell/shell
	rm -f paxos/*.o
	rm -f paxos/paxos.a
	rm -f paxos/paxos_replica
	! test -f xdrpp/Makefile || cd xdrpp && $(MAKE) clean

README.html: README.md
	-pandoc -o $@ README.md
