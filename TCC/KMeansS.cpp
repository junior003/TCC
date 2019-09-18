#include "KmeansS.h"
#include <limits.h>

int KmeansS::get_id_nearest_center(Client E)//retorna o centro mais proximo da clientes 
{
	double soma = 0.0;
	double distMinor = 9999;//soma recebera o somatorio das diferencas ao quadrado e distMenor servira para comparar as distancias
	int id_Cluster_center = 0;// recebera id do cluster mais proximo
	double distance = 0;
	for (int i = 0; i < K; i++)//para cada cluster o processo se repete
	{

		soma = 0.0;
		for (int j = 0; j < num_atts; j++)// para cada variavel o processo se repete
		{
			//distancia euclidiana
			soma += pow(clusters[i].get_att_center(j) - E.get_attr(j), 2.0);// realiza o somatorio das diferencas ao quadrado de cada variavel
		}
		distance = sqrt(soma);// distancia recebe a raiz da soma 
		if (distance < distMinor) //se a distancia atual for a menor distancia 
		{
			distMinor = distance;// a distancia menor passa a ser a distancia atual
			id_Cluster_center = i;// o id do cluster mais proximo é o cluster identificado pelo valor contido em i 

		}
		distance = 0;
	}
	return id_Cluster_center;//retorna o id 
}

KmeansS::KmeansS(int k, int Num_Clients, int num_atts)//construtor da classe kmeans 
{
	this->K = k;// atribui os parametros para os atributos 
	this->Num_Clients = Num_Clients;
	this->num_atts = num_atts;


}


void KmeansS::execute_kmeans_S(Problem* P) //principal funcao
{
	//cout << "K-means espaco iniciado" << endl;
	if (K > Num_Clients) //caso o numero de clientes seja menor do que K sera mostrado uma mensagen
	{
		cout << "ERRO - K maior que o numero total de clientes!" << endl;
		return;//encerra o programa
	}
	vector<int> IndicesUsados; //Cria um vetor para receber os centros dos clusters e nao podem repetir valores
	for (int i = 0; i < K; i++)//para cada cluster e criado valores aleatorios para os centros
	{
		while (true)// loop infinito
		{

			int IndiceNovo = (rand() % Num_Clients)+1;//cria valores aleatorios


			if (find(IndicesUsados.begin(), IndicesUsados.end(), IndiceNovo) == IndicesUsados.end())//verifica se o valor ja foi usado
			{
				//caso nao seja e criado um novo cluster
				IndicesUsados.push_back(IndiceNovo);// adiciona o incice no vetor de indices usados


				P->get_client(IndiceNovo)->setCluster(i); //indica a  uma clientes um cluster


				Cluster cluster(i, *P->get_client(IndiceNovo),false); //cria um cluster e adiciona-se uma clientes
				
				clusters.push_back(cluster);//adiciona o cluster no vetor de clusters


				break;// quebra o loop
			}
		}
	}
	int contador_iter = 0;
	while (true)
	{
		contador_iter++;
		bool TERMINOU = true;// variavel que indica se alguma clientes mudou de cluster, o programa continua se no fim do laco ela se encontrar como false

		for (int i = 1; i < Num_Clients+1; i++)//associa cada clientes ao centro do cluster mais proximo
		{

			int idClusterAntigo = P->get_client(i)->getCluster();//pega o id do cluster antigo da clientes
			int idCentroProx = get_id_nearest_center(*P->get_client(i));// calcula a distancia da emprea ao cluster mais proximo
			if (idClusterAntigo != idCentroProx) // verifica se o cluster antigo ainda é o mais proximo 
			{
				if (idClusterAntigo != -1)//se a clientes ja estiver em um cluster entra no if se nao ele é ignorado
				{
					
					clusters[idClusterAntigo].remove_Client(P->get_client(i)->get_id());//remove-se a clientes do cluster antigo
				}
				
				P->get_client(i)->setCluster(idCentroProx);//indica a clientes qual o cluster mais proximo 
				clusters[idCentroProx].add_Client(*P->get_client(i));//adiciona a clientes no cluster mais proximo
				
				TERMINOU = false;// como houve mudanca de um cluster para o outro o programa precisa continuar rodando entao nao terminou
			}
		}

		for (int i = 0; i < K; i++)//recalcula o centro de cada cluster
		{

			for (int j = 0; j < num_atts; j++)// repete para cada variavel
			{
				int Num_clients_in_Cluster = clusters[i].get_Num_Clients(); //pega o numero de clientes no cluster 
				double soma = 0.0;

				if (Num_clients_in_Cluster > 0)//se houver uma clientes 
				{
					for (int p = 0; p < Num_clients_in_Cluster; p++)// realiza a media das variaveis das clientes do cluter
					{
						soma += clusters[i].get_Client(p).get_attr(j);
					}
					clusters[i].set_att_center(j, soma / Num_clients_in_Cluster);//muda os valores do centro 
				}
			}
		}
		if (TERMINOU == true)//se tiver terminado mostra a saida
		{
			cout << "contador do kmeans: " << contador_iter << endl;
			//cout << "Agoritmo KMEANS - FIM" << endl;

			//show(); // Mostra os clientes pertencentes a cada cluster

			break;//quebra o loop
		}

	}

}

void KmeansS::show() //mostra os dados na tela
{
	for (int i = 0; i < K; i++)//para cada cluster o processo se repete
	{
		int Num_clients_in_Cluster = clusters[i].get_Num_Clients();//recebe o numero de clientes do cluster
		cout << "CLUSTER " << clusters[i].get_id_Cluster() + 1 << ":" << endl; //mostra o id do cluster
		cout << "clientes pertencentes:" << endl << "| ";
		for (int j = 0; j < Num_clients_in_Cluster; j++)//para cada clientes o processo se repete
		{
			for (int r = 0; r < num_atts; r++)//para cada variavel o processo se repete
			{
				cout << clusters[i].get_Client(j).get_attr(r) << " | ";//mostra todas as variaveis da clientes do cluster
			}
			//string NomeEmp = clusters[i].get_Client(j).getNome(); //recebe o nome
			cout << " ID:" << clusters[i].get_Client(j).get_id()+1 << " |";//se nao for mostra o nome
			cout << endl;
			if (j != Num_clients_in_Cluster - 1)//se estiver no fim da linha coloca um separador
			{
				cout << "| ";
			}
		}
		cout << endl;
		cout << " Valores do centro do Cluster: | ";
		for (int j = 0; j < num_atts; j++)//mostra os valores dos centro do cluster
		{
			cout << clusters[i].get_att_center(j) << " | ";
		}
		cout << endl << endl;
	}
}

Cluster * KmeansS::get_cluster(int i)
{

	return &clusters.at(i);
}
