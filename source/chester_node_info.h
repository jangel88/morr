#include <stdbool.h>

// These are the definitions that should be available across all node architectures
typedef int nodeid; 
typedef enum nodeid_validity{
  VALID, INVALID 
} nodeid_validity; 

int init_node_info(void);
nodeid query_nodeid();
bool valid_nodeid(nodeid); 
bool valid_nodeid_list(int, nodeid*); 

float distance_between_nodes(nodeid, nodeid); 


// These are the definitions and data that are unique to Titan and should not be used directly elsewhere
#define COMPUTE_NODE_COUNT 96
#define XDIM 25 // Dimensions for Titan's interconnect
#define YDIM 16
#define ZDIM 24

// start chester compute_node_coords declaration
static const int compute_node_count=COMPUTE_NODE_COUNT; 
static const int compute_node_coords[COMPUTE_NODE_COUNT][3]= 
{
{0,  0,  0,},//      0
{0,  0,  0,},//      1
{0,  0,  1,},//      2
{0,  0,  1,},//      3
{0,  0,  2,},//      4
{0,  0,  2,},//      5
{0,  0,  3,},//      6
{0,  0,  3,},//      7
{0,  0,  4,},//      8
{0,  0,  4,},//      9
{0,  0,  5,},//     10
{0,  0,  5,},//     11
{0,  0,  6,},//     12
{0,  0,  6,},//     13
{0,  0,  7,},//     14
{0,  0,  7,},//     15
{0,  1,  7,},//     16
{0,  1,  7,},//     17
{0,  1,  6,},//     18
{0,  1,  6,},//     19
{0,  1,  5,},//     20
{0,  1,  5,},//     21
{0,  1,  4,},//     22
{0,  1,  4,},//     23
{0,  1,  3,},//     24
{0,  1,  3,},//     25
{0,  1,  2,},//     26
{0,  1,  2,},//     27
{0,  1,  1,},//     28
{0,  1,  1,},//     29
{0,  1,  0,},//     30
{0,  1,  0,},//     31
{1,  1,  0,},//     32
{1,  1,  0,},//     33
{1,  1,  1,},//     34
{1,  1,  1,},//     35
{1,  1,  2,},//     36
{1,  1,  2,},//     37
{1,  1,  3,},//     38
{1,  1,  3,},//     39
{1,  1,  4,},//     40
{1,  1,  4,},//     41
{1,  1,  5,},//     42
{1,  1,  5,},//     43
{1,  1,  6,},//     44
{1,  1,  6,},//     45
{1,  1,  7,},//     46
{1,  1,  7,},//     47
{1,  0,  7,},//     48
{1,  0,  7,},//     49
{1,  0,  6,},//     50
{1,  0,  6,},//     51
{1,  0,  5,},//     52
{1,  0,  5,},//     53
{1,  0,  4,},//     54
{1,  0,  4,},//     55
{1,  0,  3,},//     56
{1,  0,  3,},//     57
{1,  0,  2,},//     58
{1,  0,  2,},//     59
{1,  0,  1,},//     60
{1,  0,  1,},//     61
{1,  0,  0,},//     62
{1,  0,  0,},//     63
{2,  0,  0,},//     64
{2,  0,  0,},//     65
{2,  0,  1,},//     66
{2,  0,  1,},//     67
{2,  0,  2,},//     68
{2,  0,  2,},//     69
{2,  0,  3,},//     70
{2,  0,  3,},//     71
{2,  0,  4,},//     72
{2,  0,  4,},//     73
{2,  0,  5,},//     74
{2,  0,  5,},//     75
{2,  0,  6,},//     76
{2,  0,  6,},//     77
{2,  0,  7,},//     78
{2,  0,  7,},//     79
{2,  1,  7,},//     80
{2,  1,  7,},//     81
{2,  1,  6,},//     82
{2,  1,  6,},//     83
{2,  1,  5,},//     84
{2,  1,  5,},//     85
{2,  1,  4,},//     86
{2,  1,  4,},//     87
{2,  1,  3,},//     88
{2,  1,  3,},//     89
{2,  1,  2,},//     90
{2,  1,  2,},//     91
{2,  1,  1,},//     92
{2,  1,  1,},//     93
{2,  1,  0,},//     94
{2,  1,  0,},//     95
}; 
// end chester compute_node_coords declaration


