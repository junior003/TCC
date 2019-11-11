
#ifndef SMSEMOA_H
#define SMSEMOA_H

#include "Problem.h"
#include "NSGAII.h"
#include "Methods.h"
#include "Population.h"
#include "ISolution.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include "KmeansST.h"
#include "HiperVolume.h"




class SMSemoa
{
public:

	Tsol Ponto_ref;

	vector<Tsol> instancia;
	vector<float> hv_all_generations;

	SMSemoa(int,int,float,float);




	//colocar metodos do nsga aqui assim como atributos
	void generate_Population(Problem p, int);
	void evaluate_population(Problem p, Population* pop);
	bool verify_domination(ISolution, ISolution);

	void binary_tournament(Problem p, Population pop, int *father1, int *father2, int ngen);
	void crossover_OX(Problem p, ISolution father1, ISolution father2, Population *pop1);
	void mutation(Problem p, Population* pop);
	void crowding_distance_sort(Problem p, Population *pop);
	void crowding_distance_selection(Problem p, int, int);
	void order_matrix_CD(float **dist_matrix_per_front, int F);
	void order_matrix_CD_selection(float ** M, int F);

	void non_dominated_sort(Problem p, Population *p1);

	void discard_worst_HV_solution(vector<ISolution> fronts);

	void selection(Problem p, Population *p1);

	void execute_SMSEMOA(Problem p, FILE* a1);

	
	friend class KmeansST;
	friend class Population;

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


	float smetric_actual_front;
};


bool compara(Tsol a, Tsol b);
bool compara_IS(ISolution a, ISolution b);



#endif //! SMSEMOA_H
