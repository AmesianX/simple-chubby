PAXOS_SRC := paxos/paxos_replica.cc

PAXOS_OBJ := $(PAXOS_SRC:.cc=.o)

paxos/%.o: paxos/%.cc
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

paxos/paxos_replica: $(PAXOS_OBJ)
	$(LD) -o $@ $(LDFLAGS) $(PAXOS_OBJ) $(LIBS)
