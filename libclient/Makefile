
LIBCLIENT_SRC := libclient/client.cc

LIBCLIENT_OBJ := $(LIBCLIENT_SRC:.cc=.o)
$(LIBCLIENT_OBJ): include/server.hh

libclient/%.o: libclient/%.cc
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

libclient/libclient.a: $(LIBCLIENT_OBJ)
	$(AR) -r $@ $(LIBCLIENT_OBJ)

