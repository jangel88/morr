# ---------------------------------------------------------------------- 
CC      = cc
CXX     = CC
CFLAGS	= 

# ---------------------------------------------------------------------- 
.SUFFIXES:	.o .c

.c.o:
	$(CC) $(CFLAGS) -c $<
 
.C.o:
	$(CXX) $(CFLAGS) -c $<
 
# ---------------------------------------------------------------------- 
DEFAULT = titan_node_info_test.exe

titan_node_info_test.exe:	titan_node_info_test.o titan_node_info.o 
	$(CC) $(CFLAGS) -o $@ $^

# ---------------------------------------------------------------------- 

clean:
	find . -name "*.o" -exec rm {} \; 

