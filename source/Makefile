# ---------------------------------------------------------------------- 
CC      = mpicc
CXX     = mpiCC
CFLAGS	= -O2
LDFLAGS = -lprofiler -lboost_mpi -lboost_serialization
BOOST_LIB = /usr/lib

# ---------------------------------------------------------------------- 
.SUFFIXES:	.o .c

.c.o:
	$(CC) $(CFLAGS) -c $<
 
.C.o:
	$(CXX) $(CFLAGS) -c $<
 
# ---------------------------------------------------------------------- 
#DEFAULT: node_info_test.exe
DEFAULT: node_info_test.exe individual_test.exe population_test.exe dist_test

node_info_test.exe:	node_info_test.o node_info.o 
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

individual_test.exe:	individual_test.o individual.o cart_space.o node_info.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

population_test.exe:    population_test.o population.o cart_space.o individual.o node_info.o
	$(CXX) $(CFLAGS) -o $@ $^ $(LDFLAGS)

dist_test: dist_test.o population.o cart_space.o individual.o node_info.o 
	$(CXX) $(CFLAGS) -o $@ $^ -L$(BOOST_LIB) -ldl -Wl,-rpath,$(BOOST_LIB) $(LDFLAGS)
# ---------------------------------------------------------------------- 

clean:
	find . -name "*.o" -exec rm {} \; 

