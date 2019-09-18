
#ifndef CLUSTER_H //evita que erros na compilação
#define CLUSTER_H

#include <vector>
#include "Client.h"
//Inclui o codigo presente  nos arquivos Header.h e clientes.h
using namespace std;


class Cluster
{  



public:
	Cluster(int, Client, bool); // Contrutor da classe através de um ID e de uma clientes é iniciado um novo cluster
	void add_Client(Client); // metodo de adicionar uma nova clientes no cluster
	bool remove_Client(int);//remover uma clientes do cluster
	double get_att_center(int); //retorna uma variavel do centro por exemplo se passado 4 por parametro sera retornado o valor do custo medio do cluster 
	void set_att_center(int, double); // adiciona uma média ao vetor de variaveis do centro
	Client get_Client(int); //retorna uma clientes atraves do ID
	int get_Num_Clients(); // retorna o numero total de clientes no cluster
	int get_id_Cluster(); // retorna o id do cluster
private:
	int idCluster; //identificacao
	vector<double> attributes_center; //vetor com os valores do centro do cluster
	vector<Client> clients; //Vetor das clientes que farão parte do cluster

};

#endif 