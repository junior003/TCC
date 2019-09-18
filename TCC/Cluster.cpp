#include "Cluster.h"
using namespace std;
Cluster::Cluster(int id, Client E,bool st) //construtor
{
	this->idCluster = id; // atribui o id inserido ao cluster
	int NumVar = E.get_num_attr(); //recebe o total de variaveis da clientes inserida para a criação de um centro
	
	if (st)
	{
		NumVar = 4;
	}
	for (int i = 0; i < NumVar; i++) // Cada variavel da clientes torna-se o centro ja que a clientes e a unica no cluster
	{

		attributes_center.push_back(E.get_attr(i));//adiciona as variaveis no vetor de variaveis do centro
	}
	
	clients.push_back(E);// adiciona a clientes no cluster 
}

void Cluster::add_Client(Client E)// adiciona clientes no cluster
{
	clients.push_back(E);// adiciona no vetor de clientes do cluster
}

bool Cluster::remove_Client(int idEmp)//remove  cliente do cluster
{
	
	int NumEmps = clients.size();// pega-se o tamanho do vetor de clientes do cluster
	for (int i = 0; i < NumEmps; i++)//percorre o vetor procurando clientes a ser removido
	{
		if (clients[i].get_id() == idEmp)// se encontrar
		{
			clients.erase(clients.begin() + i);// apaga ele do cluste
			return true;
		}
	}
	return false;//se nao retorna false
}

double Cluster::get_att_center(int indice)// retorna uma variavel do centro
{
	return attributes_center[indice];
}

void Cluster::set_att_center(int indice, double var)//atribui uma variavel ao centro
{
	attributes_center[indice] = var;
}

Client Cluster::get_Client(int indice)// retorna um clientes do cluster
{
	return clients[indice];
}


int Cluster::get_Num_Clients()//retorna o numero de clientes
{
	return clients.size();
}

int Cluster::get_id_Cluster()// retorna o id do cluster
{
	return this->idCluster;
}

