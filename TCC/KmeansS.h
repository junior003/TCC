
#ifndef KMEANSS_H
#define KMEANSS_H

#include <iostream>
#include <stdlib.h>
#include "Client.h"
#include "Cluster.h"
#include "Problem.h"
#include <algorithm>

class KmeansS
{
public:

	KmeansS(int, int, int);//construtor recebe os valores que serao atribuidos inicialmente para num_atts Num_Clients e para K 

	void execute_kmeans_S(Problem*); //Executa todo o processo de calculo e geracao de clusters, principal metodo 
	void show();// mostra na tela (ou em um arquivo) a saida do programa.


	Cluster* get_cluster(int);
private:
	int K; // numero de clusters 
	int num_atts, Num_Clients;//Numero de variaveis de cada clientes, Numero total de clientes 
	vector<Cluster> clusters; // vetor com todos os clusters 
	int get_id_nearest_center(Client); // retorna o cluster mais proximo da clientes que for passada por parametro 

};

#endif // !HEADER_H