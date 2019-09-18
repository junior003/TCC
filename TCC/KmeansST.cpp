#include "KmeansST.h"
#include <cmath>
#include <limits.h>
#include "Methods.h"

KmeansST::KmeansST(Problem p, int k, float alpha1, float alpha2, float k1, float k2, float k3)
{
	dist_Spa_Temp_Matrix = new float*[p.get_num_clients()+1];
	dist_Spatial_Matrix = new float*[p.get_num_clients()+1];
	dist_Temporal_Matrix = new float*[p.get_num_clients()+1];

	for (int i = 0; i < p.get_num_clients()+1; i++)
	{
		dist_Spa_Temp_Matrix[i] = new float[p.get_num_clients()+1];
		dist_Spatial_Matrix[i] = new float[p.get_num_clients()+1];
		dist_Temporal_Matrix[i] = new float[p.get_num_clients()+1];
	
		for (int j = 0; j < p.get_num_clients()+1; j++)
		{
			dist_Spa_Temp_Matrix[i][j] = 0.0;
			dist_Spatial_Matrix[i][j] = 0.0;
			dist_Temporal_Matrix[i][j]=0.0;
		}
	}
	largest_tw_width = 0;
	k_1 = k1;
	k_2 = k2;
	k_3 = k3;
	alpha_1 = alpha1;
	alpha_2 = alpha2;
	
	min_spatial_dist = INT_MAX;
	max_spatial_dist = 0;
	min_temporal_dist = INT_MAX;
	max_temporal_dist = 0;


	this->K = k;// atribui os parametros para os atributos 
	this->Num_Clients = p.get_num_clients();
	this->num_atts = 4;//x, y,ini_T,due_T
}

void KmeansST::Calculate_largest_tw(Client c)
{
	if (c.get_due_T() - c.get_initial_T() > largest_tw_width)
	{
		largest_tw_width = c.get_due_T() - c.get_initial_T();
	}
}
void KmeansST::Temporal_Projection(Problem p , int i, int j, float *a1, float* b1)
{
	float dist_temp = p.euclidian_distance(p.get_client(i)->get_X(),
									 p.get_client(i)->get_Y(),
									 p.get_client(j)->get_X(),
									 p.get_client(j)->get_Y());
	*a1 = p.get_client(i)->get_initial_T() + p.get_client(i)->get_service_T() + ((dist_temp) / (30 / 3.6));
	*b1 = p.get_client(i)->get_due_T() + p.get_client(i)->get_service_T() + ((dist_temp) / (30 / 3.6));
}
float KmeansST::Integral_early(Problem p,int i,int j, float a1, float b1)
{
	float t_1 = 0.0, inte_early=0.0;
	//cout << endl;
	t_1 = mini(b1, p.get_client(j)->get_initial_T());
	//cout << "Integral UP " << t_1<<endl;
	//cout << "[" << k_2 << "*((" << t_1 << "*" << t_1 << ")/2)+" << k_1 << "*" << p.get_client(j)->get_due_T() << "-(" << k_1 << "+" << k_2 << ")*" << p.get_client(j)->get_initial_T() << "]"<<endl;
	inte_early = k_2 * ((t_1*t_1) / 2) + k_1 * p.get_client(j)->get_due_T()*t_1 - (((k_1 + k_2)*p.get_client(j)->get_initial_T())*t_1);
	t_1 = mini(a1, p.get_client(j)->get_initial_T());
	//cout << "Integral Down " << t_1 << endl;
	//cout << "- [" << k_2 << "*((" << t_1 << "*" << t_1 << ")/2)+" << k_1 << "*" << p.get_client(j)->get_due_T() << "-(" << k_1 << "+" << k_2 << ")*" << p.get_client(j)->get_initial_T() << "]" << endl;

	inte_early -= k_2 * ((t_1*t_1) / 2) + k_1 * p.get_client(j)->get_due_T()*t_1 - (((k_1 + k_2)*p.get_client(j)->get_initial_T())*t_1);
	//cout << "Result:" << inte_early << endl;
	return inte_early;
}
float KmeansST::Integral_normal(Problem p , int i, int j, float a1, float b1)
{
	float t_1 = 0.0, inte_normal = 0.0;
	//cout << endl;
	t_1 = maxi(mini(b1, p.get_client(j)->get_due_T()), p.get_client(j)->get_initial_T());
	//cout << "Integral UP " << t_1 << endl;
	inte_normal = -k_1 * ((t_1*t_1) / 2) + k_1 * p.get_client(j)->get_due_T()*t_1;
	//cout << "[-" << k_1 << "*((" << t_1 << "*" << t_1 << ")/2)+" << k_1 << "*" << p.get_client(j)->get_due_T() << "*" << t_1 << endl;
	t_1 = mini(maxi(a1, p.get_client(j)->get_initial_T()), p.get_client(j)->get_due_T());

	//cout << "Integral Down " << t_1 << endl;
	inte_normal -= -k_1 *((t_1*t_1) / 2) + k_1 * p.get_client(j)->get_due_T()*t_1;

	//cout << "- [-" << k_1 << "*((" << t_1 << "*" << t_1 << ")/2)+" << k_1 << "*" << p.get_client(j)->get_due_T() << "*" << t_1 << endl;
	//cout << "Result:" << inte_normal << endl;
	return inte_normal;
}
float KmeansST::Integral_due(Problem p , int i, int j, float a1, float b1)
{
	float t_1 = 0.0, inte_due = 0.0;
	//cout << endl;
	t_1 = mini(b1, p.get_client(j)->get_due_T());
	//cout << "Integral UP " << t_1 << endl;
	//cout << "[-" << k_3 << "*((" << t_1 << "*" << t_1 << ")/2+" << k_3 << "*" << p.get_client(j)->get_due_T() << "*" << t_1 << endl;
	inte_due = -k_3 * ((t_1*t_1) / 2) + k_3 * p.get_client(j)->get_due_T()*t_1;
	t_1 = maxi(a1, p.get_client(j)->get_due_T());
	//cout << "Integral Down " << t_1 << endl;
	inte_due -= -k_3 * ((t_1*t_1) / 2) + k_3 * p.get_client(j)->get_due_T()*t_1;
	//cout << "-[-" << k_3 << "*((" << t_1 << "*" << t_1 << ")/2+" << k_3 << "*" << p.get_client(j)->get_due_T() << "*" << t_1 << endl;
	//cout << "result:" << inte_due << endl;
	return inte_due;
}
//funcao para calcular a spatial
void KmeansST::Calculate_ALL_Spatial_Distance(Problem p) {
	int i = 0, j = 0;
	for (i =1; i < p.get_num_clients()+1; i++)
	{
			Calculate_largest_tw(*p.get_client(i));
		
		for (j = 1; j < p.get_num_clients()+1; j++)
		{
			
			if (i != j)
			{
				dist_Spatial_Matrix[i][j] =
					p.euclidian_distance(
						p.get_client(i)->get_X(),
						p.get_client(i)->get_Y(),
						p.get_client(j)->get_X(),
						p.get_client(j)->get_Y()
					);
				//cout << "(" << i << "," << j << ")=" << dist_Spatial_Matrix[i][j] << endl;

				if (dist_Spatial_Matrix[i][j] > max_spatial_dist)
				{
					max_spatial_dist = dist_Spatial_Matrix[i][j];
				}
				else if (dist_Spatial_Matrix[i][j] < min_spatial_dist)
				{
					min_spatial_dist = dist_Spatial_Matrix[i][j];
				}
			}
				
		}
	}

	
}

//funcao para calcular a temporal
void KmeansST::Calculate_ALL_Temporal_Distance(Problem p) {
	int i = 0, j = 0;
	float a_1 = 0.0,b_1 = 0.0;

	
	
	
	for (i = 0; i < p.get_num_clients()+1; i++)
	{
		
		for (j = 0; j < p.get_num_clients()+1; j++)
		{
			
			if (i != j && i!=0 && j!=0)
			{
				
				Temporal_Projection(p, i, j, &a_1, &b_1);
				/*
				cout << "INICIO TESTE" << endl;
				float dist_temp = p.euclidian_distance(p.get_client(i)->get_X(),
					p.get_client(i)->get_Y(),
					p.get_client(j)->get_X(),
					p.get_client(j)->get_Y());
				cout << "dist:" << dist_temp<<endl;
				cout << "calculo do a_1:" << endl;
				cout << "a_1=" << p.get_client(i)->get_initial_T() << "+" << p.get_client(i)->get_service_T() << "+" << ((dist_temp) / (30 / 3.6))<<endl;
				a_1 = p.get_client(i)->get_initial_T() + p.get_client(i)->get_service_T() + ((dist_temp) / (30 / 3.6));
				b_1 = p.get_client(i)->get_due_T() + p.get_client(i)->get_service_T() + ((dist_temp) / (30 / 3.6));
				cout << "b_1=" << p.get_client(i)->get_due_T() << "+" << p.get_client(i)->get_service_T() << "+" << ((dist_temp) / (30 / 3.6)) << endl;
				cout << "FIMTESTE" << endl;
				
				cout << "<" << p.get_client(i)->get_initial_T() << "," << p.get_client(i)->get_service_T() << "," <<p.get_client(i)->get_due_T() << ">" << endl;
				cout << "<" << p.get_client(j)->get_initial_T() << "," << p.get_client(j)->get_service_T() << "," << p.get_client(j)->get_due_T() << ">" << endl;

				cout << "a_1:" << a_1 << endl;
				cout << "b_1:" << b_1 << endl;
				cout << "ltww:" << largest_tw_width << endl;*/
				dist_Temporal_Matrix[i][j] = k_1 * largest_tw_width +
					(
						+Integral_early(p, i, j, a_1, b_1)
						+ Integral_normal(p, i, j, a_1, b_1)
						+ Integral_due(p, i, j, a_1, b_1)
						) / (b_1 - a_1);
				/*
				cout << "I_early:" << Integral_early(p, i, j, a_1, b_1) << endl;
				cout << "I_normal:" << Integral_normal(p, i, j, a_1, b_1) << endl;
				cout << "I_due:" << Integral_due(p, i, j, a_1, b_1) << endl;
				cout << "Dst ij = " << dist_Temporal_Matrix[i][j] << endl;
				cout << "INVERSO" << endl;
				*/
				Temporal_Projection(p, j, i, &a_1, &b_1);
				/*cout << "<" << p.get_client(j)->get_initial_T() << "," << p.get_client(j)->get_service_T() << "," << p.get_client(j)->get_due_T() << ">" << endl;
				cout << "<" << p.get_client(i)->get_initial_T() << "," << p.get_client(i)->get_service_T() << "," << p.get_client(i)->get_due_T() << ">" << endl;
				
				cout << "a_1:" << a_1 << endl;
				cout << "b_1:" << b_1 << endl;
				cout << "I_early:" << Integral_early(p, j, i, a_1, b_1) << endl;
				cout << "I_normal:" << Integral_normal(p, j, i, a_1, b_1) << endl;
				cout << "I_due:" << Integral_due(p, j, i, a_1, b_1) << endl;
				*/
				dist_Temporal_Matrix[j][i] = k_1 * largest_tw_width +
					(
						+Integral_early(p, j, i, a_1, b_1)
						+ Integral_normal(p, j, i, a_1, b_1)
						+ Integral_due(p, j, i, a_1, b_1)
						) / (b_1 - a_1);
				
				//cout << "Dst ji = " << dist_Temporal_Matrix[j][i] << endl;
				/*if (dist_Temporal_Matrix[i][j] < 0)
				{
					dist_Temporal_Matrix[i][j] = -1 * dist_Temporal_Matrix[i][j];
				}
				*/
				//cout << dist_Temporal_Matrix[i][j] << "|";


				//Garantindo que o maior valor sera mantido tanto de i para j  quanto de j para i:
				
				if (dist_Temporal_Matrix[i][j] > dist_Temporal_Matrix[j][i])
				{
					dist_Temporal_Matrix[j][i] = dist_Temporal_Matrix[i][j];
				}
				else
				{
					dist_Temporal_Matrix[i][j] = dist_Temporal_Matrix[j][i];
				}

				//procurando o menor e o maior valor da matriz
				if (dist_Temporal_Matrix[i][j] > max_temporal_dist)
				{
					max_temporal_dist = dist_Temporal_Matrix[i][j];
				}
				else if (dist_Temporal_Matrix[i][j] < min_temporal_dist)
				{
					min_temporal_dist = dist_Temporal_Matrix[i][j];
				}

				if (dist_Temporal_Matrix[j][i] > max_temporal_dist)
				{
					max_temporal_dist = dist_Temporal_Matrix[j][i];
				}
				else if (dist_Temporal_Matrix[j][i] < min_temporal_dist)
				{
					min_temporal_dist = dist_Temporal_Matrix[j][i];
				}
				//cout << "passou ifs" << endl;
			
				
			}
			
		}

	}

}

void KmeansST::Calculate_ALL_Spatial_Temporal_Distance(Problem p)
{
	//alocar matriz
	//cout << "final" << endl;

	/*
	for (int i = 0; i < p.get_num_clients() + 1; i++)
	{
		cout << endl;
		for (int j = 0; j < p.get_num_clients() + 1; j++)
		{
			cout << " " << i << "," << j << " ";
			cout << "  " << dist_Spatial_Matrix[i][j] << "|" << dist_Temporal_Matrix[i][j] << "|" << dist_Spa_Temp_Matrix[i][j];
		}
	}
	system("PAUSE");
	*/

	//laco para cada posicao da matriz
	for (int i = 0; i < p.get_num_clients() + 1; i++)
	{
		for (int j = 0; j < p.get_num_clients() + 1; j++)
		{
			/*cout << "alfa1 e alfa2:" << alpha_1 << "," << alpha_2 << endl;
			cout << "dS[" << i << "][" << j << "]:" << dist_Spatial_Matrix[i][j] << endl;
			cout << "dT[" << i << "][" << j << "]:" << dist_Temporal_Matrix[i][j] << endl;
			cout << "min_spatial_dist:" << min_spatial_dist << endl;
			cout << "max_spatial_dist:" << max_spatial_dist << endl;
			cout << "min_temporal_dist:" << min_temporal_dist << endl;
			cout << "max_temporal_dist:" << max_temporal_dist << endl;*/
			if (i != j && i != 0 && j != 0)
			{
				dist_Spa_Temp_Matrix[i][j] =
					alpha_1 * ((dist_Spatial_Matrix[i][j] - min_spatial_dist) / (max_spatial_dist - min_spatial_dist))
					+ alpha_2 * ((dist_Temporal_Matrix[i][j] - min_temporal_dist) / (max_temporal_dist - min_temporal_dist));
				//cout << "dS[" << i << "][" << j << "]:" << dist_Spatial_Matrix[i][j] << endl;
				//cout << "dT[" << i << "][" << j << "]:" << dist_Temporal_Matrix[i][j] << endl;
		
					
				
				
				
				//cout << "................................................................." << endl;
			}


		}
	}


}

float KmeansST::Calculate_Temporal_Distance_Pair(Problem p,Client C1, Client C2)
{
	int i = 0, j = 0;
	float a_1 = 0.0, b_1 = 0.0;
	float temporal_dist1, temporal_dist2;
	Temporal_Projection(p, C1.get_id(), C2.get_id(), &a_1, &b_1);
	temporal_dist1 = k_1 * largest_tw_width +
		(
			+Integral_early(p, C1.get_id() , C2.get_id(), a_1, b_1)
			+ Integral_normal(p, C1.get_id(), C2.get_id(), a_1, b_1)
			+ Integral_due(p, C1.get_id(), C2.get_id(), a_1, b_1)
			) / (b_1 - a_1);

	Temporal_Projection(p, C2.get_id(), C1.get_id(), &a_1, &b_1);
	temporal_dist2 = k_1 * largest_tw_width +
		(
			+Integral_early(p, C2.get_id(), C1.get_id(), a_1, b_1)
			+ Integral_normal(p, C2.get_id(), C1.get_id(), a_1, b_1)
			+ Integral_due(p, C2.get_id(), C1.get_id(), a_1, b_1)
			) / (b_1 - a_1);

	if (temporal_dist1 > temporal_dist2)
	{
		return temporal_dist1;
	}
	else
	{
		return temporal_dist2;
	}

}

float KmeansST::Calculate_Spatial_Temporal_Distance_Pair(Problem p,Client c1, Client c2)
{
	float spatial_dist = p.euclidian_distance(c1.get_X(), c1.get_Y(), c2.get_X(), c2.get_Y());
	float temporal_dist = Calculate_Temporal_Distance_Pair(p, c1, c2);
	if (spatial_dist < min_spatial_dist)
	{
		min_spatial_dist = spatial_dist;
	}
	if (temporal_dist < min_temporal_dist)
	{
		min_temporal_dist = temporal_dist;
	}
	
	float Spa_Temp_dist =
		alpha_1 * ((spatial_dist - min_spatial_dist) / (max_spatial_dist - min_spatial_dist))
		+ alpha_2 * ((Calculate_Temporal_Distance_Pair(p,c1,c2) - min_temporal_dist) / (max_temporal_dist - min_temporal_dist));
	//cout << "dST = " << alpha_1 << "*((" << spatial_dist << "-" << min_spatial_dist << ")/(" << max_spatial_dist << "-" << min_spatial_dist << "))" << endl;
	//cout << "+" << alpha_2 << "*((" << Calculate_Temporal_Distance_Pair(p, c1, c2) << "-" << min_temporal_dist << ")/(" << max_temporal_dist << "-" << min_temporal_dist << "))";
	
	
	return Spa_Temp_dist;
}

int KmeansST::get_id_nearest_center(Problem p, Client E)//retorna o centro mais proximo da clientes 
{
	double soma = 0.0;
	double distMinor = 9999;//soma recebera o somatorio das diferencas ao quadrado e distMenor servira para comparar as distancias
	int id_Cluster_center = 0;// recebera id do cluster mais proximo
	double distance = 0;
	//cout << "REALOCACAO" << endl;
	for (int i = 0; i < K; i++)//para cada cluster o processo se repete
	{
		//cout << "(" << i << "," << E.get_id()<<")";
		Client Ctemp(i, clusters[i].get_att_center(0),
			clusters[i].get_att_center(1),0,
			clusters[i].get_att_center(2),
			clusters[i].get_att_center(3), 0.0, 0.0);

			distance = Calculate_Spatial_Temporal_Distance_Pair(p, Ctemp, E);
	
			//cout << "dST:" << distance << endl;
			
		if (distance < distMinor) //se a distancia atual for a menor distancia 
		{
			distMinor = distance;// a distancia menor passa a ser a distancia atual
			id_Cluster_center = i;// o id do cluster mais proximo é o cluster identificado pelo valor contido em i 

		}
		distance = 0;
	}
	return id_Cluster_center;//retorna o id 
}

void KmeansST::execute_kmeans_ST(Problem * P)
{
	
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

			int IndiceNovo = (rand() % Num_Clients) + 1;//cria valores aleatorios


			if (find(IndicesUsados.begin(), IndicesUsados.end(), IndiceNovo) == IndicesUsados.end())//verifica se o valor ja foi usado
			{
				//caso nao seja e criado um novo cluster
				IndicesUsados.push_back(IndiceNovo);// adiciona o incice no vetor de indices usados


				P->get_client(IndiceNovo)->setCluster(i); //indica a  uma clientes um cluster


				Cluster cluster(i, *P->get_client(IndiceNovo),true); //cria um cluster e adiciona-se uma clientes

				clusters.push_back(cluster);//adiciona o cluster no vetor de clusters


				break;// quebra o loop
			}
		}
	}

	while (true)
	{

		bool TERMINOU = true;// variavel que indica se alguma clientes mudou de cluster, o programa continua se no fim do laco ela se encontrar como false

		for (int i = 1; i < Num_Clients+1; i++)//associa cada clientes ao centro do cluster mais proximo
		{

			int idClusterAntigo = P->get_client(i)->getCluster();//pega o id do cluster antigo da clientes
			int idCentroProx = get_id_nearest_center(*P,*P->get_client(i));// calcula a distancia da emprea ao cluster mais proximo
			if (idClusterAntigo != idCentroProx) // verifica se o cluster antigo ainda é o mais proximo 
			{
				if (idClusterAntigo != -1  )//se a clientes ja estiver em um cluster entra no if se nao ele é ignorado
				{
					if (clusters[idClusterAntigo].get_Num_Clients() == 1)
					{
						continue;
					}
					clusters[idClusterAntigo].remove_Client(P->get_client(i)->get_id());//remove-se a clientes do cluster antigo
				}

				P->get_client(i)->setCluster(idCentroProx);//indica a clientes qual o cluster mais proximo 
				clusters[idCentroProx].add_Client(*P->get_client(i));//adiciona a clientes no cluster mais proximo

				TERMINOU = false;// como houve mudanca de um cluster para o outro o programa precisa continuar rodando entao nao terminou
			}
		}

		for (int i = 0; i < K; i++)//recalcula o centro de cada cluster
		{
			int Num_clients_in_Cluster = clusters[i].get_Num_Clients(); //pega o numero de clientes no cluster 
			
			for (int j = 0; j < num_atts; j++)// repete para cada variavel
			{
				
				double soma = 0.0;
				if (Num_clients_in_Cluster == 0 && TERMINOU == true)
				{
					cout << "A";
					
				}
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
			//cout << "Agoritmo KMEANS-ST - FIM" << endl;
			//cout << K << endl;
			//show(); // Mostra os clientes pertencentes a cada cluster

			break;//quebra o loop
		}

	}
}



void KmeansST::show() //mostra os dados na tela
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
			cout << " ID:" << clusters[i].get_Client(j).get_id() + 1 << " |";//se nao for mostra o nome
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


Cluster * KmeansST::get_cluster(int i )
{
	return &clusters.at(i);
}