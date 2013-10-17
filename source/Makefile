# ---------------------------------------------------------------------- 
CC      = cc
CXX     = CC
CFLAGS	= -O3 $(BOOST_FLAGS) -DNDEBUG 
#CFLAGS	= -g $(BOOST_FLAGS)

LIBS = -lboost_mpi -lboost_serialization
LDFLAGS = $(LIBS)

# ---------------------------------------------------------------------- 
.SUFFIXES:	.o .c

.c.o:
	$(CC) $(CFLAGS) -c $<
 
.C.o:
	$(CXX) $(CFLAGS) -c $<
 
# ---------------------------------------------------------------------- 
DEFAULT: CORRECTNESS 

CORRECTNESS: chester_node_info node_info_test.exe cart_space_test.exe individual_test.exe population_test.exe
PERFORMANCE: titan_node_info population_test.exe
REORDER: titan_node_info node_reorder.exe

chester_node_info:
	ln -s -f chester_node_info.h node_info.h

titan_node_info:
	ln -s -f titan_node_info.h node_info.h

node_info_test.exe:	node_info_test.o node_info.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

cart_space_test.exe: cart_space_test.o cart_space.o node_info.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

individual_test.exe: individual_test.o individual.o domain.o node_info.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

population_test.exe: population_test.o population.o individual.o domain.o node_info.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

node_reorder.exe: node_reorder_3D.o population.o individual.o domain.o node_info.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# ---------------------------------------------------------------------- 

clean:
	find . -name "*.o" -exec rm {} \; 

new: clean DEFAULT

