PAXOS_SRC := paxos/paxos_replica.cc paxos/paxos_impl.cc paxos/replica_client_set.cc

PAXOS_OBJ := $(PAXOS_SRC:.cc=.o)

paxos/%.o: paxos/%.cc paxos/paxos.hh
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

paxos/paxos_replica: $(PAXOS_OBJ)
	$(LD) -o $@ $(LDFLAGS) $(PAXOS_OBJ) $(LIBS)

# Generate initial files to be edited
scaffold-paxos: paxos/paxos.x xdrpp
	$(XDRC) -serverhh -o paxos/paxos_impl.hh $<
	$(XDRC) -servercc -o paxos/paxos_impl.cc $<
