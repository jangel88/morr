# ---------------------------------------------------------------------- 
CC      = mpicc
CXX     = mpiCC
CFLAGS	= 

# ---------------------------------------------------------------------- 
.SUFFIXES:	.o .c

.c.o:
	$(CC) $(CFLAGS) -c $<
 
.C.o:
	$(CXX) $(CFLAGS) -c $<
 
# ---------------------------------------------------------------------- 
#DEFAULT: node_info_test.exe
DEFAULT: node_info_test.exe individual_test.exe population_test.exe

node_info_test.exe:	node_info_test.o node_info.o 
	$(CXX) $(CFLAGS) -o $@ $^

individual_test.exe:	individual_test.o individual.o cart_space.o node_info.o
	$(CXX) $(CFLAGS) -o $@ $^

population_test.exe:    population_test.o population.o cart_space.o individual.o node_info.o
	$(CXX) $(CFLAGS) -o $@ $^
# ---------------------------------------------------------------------- 

clean:
	find . -name "*.o" -exec rm {} \; 

