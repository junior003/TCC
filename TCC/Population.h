

#ifndef POPULATION_H
#define POPULATION_H


#include "ISolution.h"
#include "Problem.h"
#include <vector>
#include "KmeansST.h"
using namespace std;

class Population
{

public:
	Population();
	Population(int);
	void set_max_size(int);
	int get_max_size();
	int get_actual_size();
	void add_individual(ISolution*);
	void remove_individual(ISolution*);
	ISolution* get_individual(int);
	void Generate_Random_Individual(Problem *p);

	int find_minor_DST(Problem p, int last_added, vector<int>* not_used_IDs, KmeansST * kmeans_ST);
	void print_population(Problem p, FILE* arq1, FILE *arq2,FILE *arq3);

	void uni_pops(Population*);
	void transfer_pop(Population*);
	void clear_pop();
	void reset_ident();
	void reset_ident_SMSEMOA();
private:
	friend class KmeansST;
	friend class NSGAII;
	int max_size;
	vector<ISolution> pop;
	int actual_size;
	

};

#endif 