typedef std::pair<float,int> fit_pair;

bool comparator (const fit_pair& i, const fit_pair& j);
float norm2(float cost[], int chromo_length);

void compute_cost(float cost[], std::vector<int> chromosome, std::vector<int> topology, int chromo_length);
