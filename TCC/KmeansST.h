
#ifndef KMEANSST_H
#define KMEANSST_H

#include <iostream>
#include <stdlib.h>
#include "Client.h"
#include "Cluster.h"
#include "Problem.h"
#include <algorithm>
#include "KmeansS.h"

class KmeansST
{


public:

	KmeansST(Problem p, int K, float alpha1, float alpha2, float k_1, float k_2, float k_3);//construtor recebe os valores que serao atribuidos inicialmente

	//calcula a distancia espaco tempora entre todos os clientes
	void Calculate_ALL_Spatial_Temporal_Distance(Problem p);
	//calcula a DT para um par de clientes
	float Calculate_Temporal_Distance_Pair(Problem, Client C1, Client C2);
	//Calcula a DS para um par de clientes
	float Calculate_Spatial_Temporal_Distance_Pair(Problem, Client, Client);
	//Calcula todas as DS
	void Calculate_ALL_Spatial_Distance(Problem p);
	//Calcula toda as DT
	void Calculate_ALL_Temporal_Distance(Problem p);
	//Calcula a maior janela de tempo
	void Calculate_largest_tw(Client c);
	//Calcula a projecao da janela de tempo
	void Temporal_Projection(Problem, int, int, float*, float*);

	//Calcula a DST em relacao a projecao
	float Integral_early(Problem, int, int, float, float);
	float Integral_normal(Problem, int, int, float, float);
	float Integral_due(Problem, int, int, float, float);


	void execute_kmeans_ST(Problem*); //Executa todo o processo de calculo e geracao de clusters, principal metodo 


	void show();
	Cluster* get_cluster(int);

private:
	int K; // numero de clusters 
	int Num_Clients;
	int num_atts;
	float alpha_1, alpha_2;

	//Matriz que armazena a distancia espaco-temporal dos clientes
	float **dist_Spa_Temp_Matrix;

	//Matriz que armazena a distancia espacial dos clientes
	float **dist_Spatial_Matrix;

	//Matriz que armazena a distancia temporal dos clientes
	float **dist_Temporal_Matrix;
	
	//minimos e maximos das matrizes de distancia espacial e temporal
	float min_spatial_dist, max_spatial_dist;
	float min_temporal_dist, max_temporal_dist;

	//atributos referentes a distancia temporal
	float k_1, k_2, k_3;
	float largest_tw_width;

	//atributos referentes ao processo de agrupamento
	vector<Cluster> clusters; // vetor com todos os clusters 
	
	int get_id_nearest_center(Problem,Client); // retorna o cluster mais proximo do cliente que for passado por parametro 
	
};

#endif