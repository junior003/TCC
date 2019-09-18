
#ifndef NSGAII_H
#define NSGAII_H

#include "Population.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
class NSGAII
{
public:
	NSGAII();
	NSGAII(int , int , float , float );
	
	void generate_Population(Problem p,int);
	void evaluate_population(Problem p, Population* pop);
	bool verify_domination(ISolution,ISolution);

	void binary_tournament(Problem p,Population pop,int *father1, int *father2, int ngen);
	void crossover_OX(Problem p,ISolution father1, ISolution father2, Population *pop2);
	void mutation(Problem p,Population* pop);
	void crowding_distance_sort(Problem p,Population *pop);
	void crowding_distance_selection(Problem p, int,int);
	void order_matrix_CD(float **dist_matrix_per_front,int F);
	void order_matrix_CD_selection(float ** M, int F);

	void non_dominated_sort(Problem p,Population *p1);
	
	void selection(Problem p,Population *p1, Population *p2);
	
	void execute_NSGAII(Problem p,FILE*,FILE*,FILE*);
private:
	int num_ind; //tamanho da população de pais, que é igual ao tamanho da população de filhos
	int num_generation; //numero de geracoes maximas
	int actual_num_sons; //numero de filhos ja gerados
	Population pop_1;
	Population pop_2;
	Population surv_pop; //fronteira de pareto?
	float prob_crossover; 
	float prob_mutation;
	int* ind_front;//indica em qual front o individuo sera classificado
	vector< vector<ISolution> > fronts;
};

#endif // !NSGAII_H