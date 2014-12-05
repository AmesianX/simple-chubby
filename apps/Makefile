
apps/%.o: apps/%.cc apps/common.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

apps/leader_elect: apps/leader_elect.o libclient/libclient.a
	$(LD) -o $@ $(LDFLAGS) $< $(LIBS)

apps/double_barrier: apps/double_barrier.o libclient/libclient.a
	$(LD) -o $@ $(LDFLAGS) $< $(LIBS)

