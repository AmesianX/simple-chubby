PAXOS_SRC := paxos/paxos_impl.cc \
	     paxos/replica_client_set.cc \
	     paxos/replica_state.cc \
	     paxos/helper.cc \
	     paxos/change_view_engine.cc \
	     paxos/execute_replicate_engine.cc \
             paxos/paxos_lib.cc

PAXOS_HEADER := $(PAXOS_SRC:.cc=.hh) paxos/back_store.hh

PAXOS_LIB := -Lpaxos `$(PKGCONFIG) --libs xdrpp` -ldl -lpaxos

PAXOS_OBJ := $(PAXOS_SRC:.cc=.o)

paxos/%.o: paxos/%.cc $(PAXOS_HEADER)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

paxos/libpaxos.a: $(PAXOS_OBJ)
	$(AR) -r $@ $(PAXOS_OBJ)

paxos/paxos_replica: paxos/paxos_replica.o paxos/libpaxos.a
	$(LD) -o $@ $(LDFLAGS) $< $(PAXOS_LIB)

# Generate initial files to be edited
scaffold-paxos: paxos/paxos.x xdrpp
	$(XDRC) -serverhh -o paxos/paxos_impl.hh $<
	$(XDRC) -servercc -o paxos/paxos_impl.cc $<
