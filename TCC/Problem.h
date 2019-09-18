

#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>
#include "Client.h"
#include "ISolution.h"
#include "Cluster.h"
#include <math.h>
#include <iostream>
using namespace std;


class Problem
{
private:
	float transport_cost_per_KM;//**
	int num_vehicle; //quantidade de veiculos utilizado 
	int capacity; //capacidade de cada veiculo
	int num_consumers; //numero de consumidores
	vector<Client> cons; // vetor que armazena dados sobre os consumidores
	float product_freshness; //valor 0 a 1 da qualidade do produto(beta)
	float product_life_cycle; //tempo de validade do produto
	friend class Cluster;
	float velocity_vehicle;//**
	float cost_vehicle;//**
	int w_1, w_2, w_3;//**
public:
	Problem();
	Problem(int num_v, int cap, int num_c, int cost_v, float prod_fresh);

	Client origin;
	//atribui a capacidade maxima do veiculos
	void set_capacity(int);
	//atribui a quantidade de clientes na instancia
	void set_num_clients(int);
	//adiciona um cliente ao problema
	void add_client(Client*);
	//Estabelece a qualidade minima aceita pelos clientes
	void set_product_freshness(float);
	//atribui a quantidade de veiculos do problema
	void set_num_vehicle(int);

	void set_weight(int, int, int);
	void set_fixed_cost_vehicle(float);
	void set_velocity_vehicle(float);
	void set_transportation_cost_per_KM(float);

	float get_product_freshness();
	int get_capacity();
	int get_num_clients();
	Client* get_client(int);
	float get_fixed_cost_vehicle();
	float get_velocity_vehicle();
	float get_transportation_cost_per_KM();


	int get_num_vehicle();
	//OBJ1:
	//dano

	//distance
	float euclidian_distance(int, int, int, int);
	//recebe a distancia percorrida e retornam:
	// tempo da rota
	float calc_arrival_time_of_vehicle(float past_time, int client, float dist);
	//custo por KM
	float calc_transport_cost(float);


	float obj1(ISolution* s);//calcula a distancia percorrida pela solucao(temporaria)


	float obj2(ISolution*, float); // calcula a qualidade do produto no fim do percurso(temporaria)
	//float constr_capacity(ISolution);//restrição de capacidade dos veiculos

	bool constraint_vehicle_capacity(ISolution);
	bool constraint_curr_freshness(ISolution);


	
};
#endif 