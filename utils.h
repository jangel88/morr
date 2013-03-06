
typedef int nodeid; 
int rand_int_inclusive(int min, int max);
int* copy_inject(int elite[], int N, int mutant[],int start, int stop, int z, int mirr_flag);
int* mutate(int elite[], int N, int mutant[],int id);
int* mutate_inject(int elite[], int N, int mutant[],int x, int y,int z);
int* mutate_mirror(int elite[], int N, int mutant[], int z);
int* mutate_swap(int elite[], int N, int mutant[]);
void create_pop(int node_count, int nodes[], int pop_size, int* pop[],int id);
double tournament(int N, int* pop[], int pop_size,const int xcoors[],const int ycoors[],const int zcoors[],int topology[],int elites[], int num_of_elites);
void randperm(int a, int b, int r[],int id);
double norm2(float cost[], int N);

int compare(const void * a, const void * b);

double avg_cost(float cost[], int N);




void computeCost(float cost[], int assignment[], int topology[],const int xcoors[],
                 const int ycoors[],const int zcoors[],  int N);

void get_subset(FILE *fp, int subset_nodes[], int node_count);

int count_lines(FILE *fp);

//void get_coords(FILE *fp,const int xcoors[],const int ycoors[],const int zcoors[], int nid[]);


void get_indices(int id, int indices[], int n, int m, int p);

int get_id(int i, int j, int k, int n, int m, int p);

//void topomat3d(int topology[], int n, int m, int p); 
// Using topomat3d from source/struct_cart_domain.c
