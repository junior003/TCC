



#ifndef ISOLUTION_H
#define ISOLUTION_H


#include <math.h>

#include <vector>
using namespace std;
class ISolution
{


public:

	ISolution(int, int);//construtor sem solucao pronta
	//sobreescrever operador igual


	void set_num_vehicles_in_S(int);
	//atribui uma rota a um veiculo
	void set_route(int, int*, int);

	//substitui a rota de um veículo por outra do mesmo tamanho.
	void substitute_route(int, int*, int);
	//atribui tamanho da rota por veiculo no vetor route_sizes
	void set_route_size(int, int);

	//atribui valores associadosa obj1 e obj2
	void set_obj1_cost(float);
	void set_obj2_freshness(float);
	void set_inv_obj2_freshness();

	void set_dist_travel(float);
	void set_CD_obj1(float);
	void set_CD_obj2(float);

	int get_num_vehicles_in_S();

	int get_client_id_on_route(int, int);
	//retorna um vetor q contem uma rota de um veiculo passado por parametro
	int* get_path_Sol(int, int);
	//retorna quantos clientes o veiculo passado por parametro atende
	int get_route_size(int);

	//retorna valores associadosa obj1 e obj2
	float get_obj1_cost();
	float get_obj2_freshness();
	float get_inv_obj2_freshness();
	//retorna o identificador unico da solucao
	int get_id();
	float get_dist_travel();
	float get_CD_obj1();
	float get_CD_obj2();

	void set_ident(int);
	void reset_ident();

private:
	int id;
	int num_vehicles_in_S;
	float distance_traveled;
	vector< vector<int> > sol_path; //vetor que indica para cada veiculo a sequencia de clientes a serem visitado
	int *route_sizes; //tamanho do caminho por veiculo (varia pois o numero de clientes muda)
	float obj1_cost; //custo da solucao associado a funcao obj1
	float obj2_freshness; //qualidade do produto no momento da ultima entrega associado a obj2
	float inv_obj2_freshness;

	float crownding_dist_obj1;
	float crownding_dist_obj2;


};

#endif // !1
