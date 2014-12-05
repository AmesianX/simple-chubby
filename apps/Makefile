
apps/%.o: apps/%.cc apps/common.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

apps/%: apps/%.o libclient/libclient.a
	$(LD) -o $@ $(LDFLAGS) $< $(LIBS)

