# ---------------------------------------------------------------------- 
CC      = cc
CXX     = CC
CFLAGS	= -O2 
LDFLAGS = -lboost_mpi -lboost_serialization $(BOOST_FLAGS)


# ---------------------------------------------------------------------- 
.SUFFIXES:	.o .c

.c.o:
	$(CC) $(CFLAGS) -c $< $(LDFLAGS)
 
.C.o:
	$(CXX) $(CFLAGS) -c $< $(LDFLAGS)
 
# ---------------------------------------------------------------------- 
DEFAULT: node_info_test.exe cart_space_test.exe individual_test.exe
#DEFAULT: node_info_test.exe individual_test.exe population_test.exe dist_test.exe query_nodes.exe

node_info_test.exe:	node_info_test.o node_info.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

cart_space_test.exe:	cart_space_test.o cart_space.o node_info.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

individual_test.exe:	individual_test.o individual.o domain.o node_info.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

population_test.exe:    population_test.o population.o domain.o individual.o node_info.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

query_nodes.exe: query_nodes.o population.o individual.o torus_node_info.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

dist_test.exe: dist_test.o population.o domain.o individual.o torus_node_info.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)
# ---------------------------------------------------------------------- 

clean:
	find . -name "*.o" -exec rm {} \; 

