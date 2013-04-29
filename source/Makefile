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
#DEFAULT: titan_node_info_test.exe
DEFAULT: titan_node_info_test.exe individual_test.exe

titan_node_info_test.exe:	titan_node_info_test.o titan_node_info.o 
	$(CXX) $(CFLAGS) -o $@ $^

individual_test.exe:	individual_test.o individual.o node_info.o 
	$(CXX) $(CFLAGS) -o $@ $^

# ---------------------------------------------------------------------- 

clean:
	find . -name "*.o" -exec rm {} \; 

