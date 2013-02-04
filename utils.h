


int* mutate_swap(int elite[], int N, int mutant[]);
void create_pop(int N, int nodes[], int PopSize, int initPop[],int id);
double tournament(int N, int* Pop[], int PopSize, int xcoors[], int ycoors[], int zcoors[],int topology[]);
void randperm(int a, int b, int r[],int id);
double norm2(int cost[], int N);

int compare(const void * a, const void * b)

double avg_cost(int cost[], int N);



double ran3(long *idum);
void computeCost(int cost[], int assignment[], int topology[], int xcoors[],
                 int ycoors[], int zcoors[],  int N);

void get_subset(FILE *fp, int subset_nodes[], int node_count);

int count_lines(FILE *fp);

void get_coords(FILE *fp, int xcoors[], int ycoors[], int zcoors[], int nid[]);


void get_indices(int id, int indices[], int n, int m, int p);

int get_id(int i, int j, int k, int n, int m, int p);

void topomat3d(int topology[], int n, int m, int p);
