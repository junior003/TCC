#include "Problem.h"
#include "Client.h"
#include "Input.h"
#include "KmeansS.h"
#include "KmeansST.h"
#include "Cluster.h"
#include "ISolution.h"
#include "Methods.h"
#include "Population.h"
#include "NSGAII.h"
#include <iomanip>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>

using namespace std;
using namespace std::chrono;



int main(int argc, char*argv[])
{
	int input_file = 1, output_file1 = 2,prob_cross = 3,prob_mut=4,tam_pop=5,num_gen = 6;
	FILE *arq1;
	FILE *arq2;
	FILE *arq3;
	if ((arq1 = fopen(argv[output_file1], "ab")) == NULL)
	{
		cout << "erro ao abrir o arquivo";
		return 0;
	}


	srand(time(NULL));
	float pc = 0,pm = 0;
	int tamp =0, ngen =0;
	Problem *p;
	vector<ISolution> sols;
	Population pop(1);
	p = new Problem();
	input_data(argv[input_file],*p);
	high_resolution_clock::time_point t1;
	duration<double> time_span2;
	t1 = high_resolution_clock::now();
	
	
	//K-means utilizando distancia euclidiana
	KmeansS kmeans(p->get_num_vehicle(), p->get_num_clients(), 2);
	kmeans.execute_kmeans_S(p);
	sols = build_solutions_space_or_timewindow_sort_S(p, &kmeans, 1);


	//K-means utilizando distancia espaco-temporal
	//KmeansST kmeans_ST(*p, p->get_num_vehicle(), 0.5, 0.5, 1, 1.5, 2);
	//kmeans_ST.Calculate_ALL_Spatial_Distance(*p);
	//kmeans_ST.Calculate_ALL_Temporal_Distance(*p);
	//kmeans_ST.Calculate_ALL_Spatial_Temporal_Distance(*p);
	//kmeans_ST.execute_kmeans_ST(p);
	//sols = build_solutions_space_or_timewindow_sort_ST(p, &kmeans_ST, 1);
	
	
	pc = ((float) atoi(argv[prob_cross])) / 100.0;
	pm = ((float)atoi(argv[prob_mut])) / 100.0;
	tamp = atoi(argv[tam_pop]);
	ngen = atoi(argv[num_gen]);
	
	//NSGAII nsga(tamp,ngen, pc , pm);
	//nsga.execute_NSGAII(*p,arq1,arq2,arq3);
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	duration<double> time_span = duration_cast<duration<double> >(t2 - t1);
	cout << "Tempo: " << time_span.count()<<endl;
	fclose(arq1);
	
	return 0;
}
