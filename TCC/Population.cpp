#include "Population.h"
#include "Methods.h"
#include <iostream>
#include <stdlib.h>
#include <algorithm>
#include <iomanip>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
using namespace std;
Population::Population()
{
}
//recebe como parametro o tamanho da popula��o
Population::Population(int n)
{
	set_max_size(n);
}

void Population::set_max_size(int n)
{
	if (n < 1)
	{
		cout << "Populacao vazia!" << endl;
	}
	else
	{
		max_size = n;
	}
}


int Population::get_max_size()
{
	return max_size;
	
}

int Population::get_actual_size()
{
	return (int)pop.size();
}

void Population::add_individual(ISolution * is)
{
	
	pop.push_back(*is);
}

void Population::remove_individual(ISolution *is)
{
	for (vector<ISolution>::iterator j = pop.begin(); j != pop.end(); j++)
	{
		if (is->get_id() == j->get_id())
		{
			pop.erase(j);
			j--;
			break;
		}
	}
}

ISolution* Population::get_individual(int id)
{
	return &pop.at(id);
}

void Population::Generate_Random_Individual(Problem * p)
{
	ISolution *S;
	vector< vector<int> > temp_sol;
	vector<int> temp_route;
	vector<int> used_IDs;
	int* temp_route_aux;
	bool valid = false;
	int num_clients_added = 0;
	int chosen_vehicle;
	int acum_demand = 0;
	int cont_choosen_random_cl = 0;
	int added = 0;
	int actual_vehicle = 0;
	bool greedy = true;
	//guloso
	int minor_dst=0;
	vector<int> not_used_IDs;
	KmeansST kmeans_ST(*p, p->get_num_vehicle(), 0.5, 0.5, 1, 1.5, 2);
	kmeans_ST.Calculate_ALL_Spatial_Distance(*p);
	kmeans_ST.Calculate_ALL_Temporal_Distance(*p);
	kmeans_ST.Calculate_ALL_Spatial_Temporal_Distance(*p);
	
	//Associa um primeiro cliente aleatorio para cada veiculo
	for (int i = 0; i < p->get_num_vehicle(); i++)
	{
		do
		{
			
			int new_ID = (rand() % p->get_num_clients() + 1);//cria valores aleatorios
			if (find(used_IDs.begin(), used_IDs.end(), new_ID) == used_IDs.end() && new_ID != 0)
			{
				used_IDs.push_back(new_ID);// adiciona o incice no vetor de indices usados
				temp_route.clear();
				temp_route.push_back(new_ID);
				temp_sol.push_back(temp_route);
				num_clients_added++;
				valid = true;
			}
			else
			{
				valid = false;
			}

		} while (!valid);
	}
	valid = false;


		for (int i = 0; i < p->get_num_vehicle(); i++)
		{
			acum_demand = 0;
			for (int k = 0; k < temp_sol[i].size(); k++)
			{
				acum_demand += p->get_client(temp_sol[i][k])->get_demand();
			}
			//50% de chance de ser aleat�rio e 50% de chance de ser guloso
		
		//metodo guloso
			if (randomic(0,1)<0.5 && greedy)
			{
				
				not_used_IDs.clear();
				
				for (int j = 1; j < p->get_num_clients()+1; j++)
				{
					not_used_IDs.push_back(j);
				
					
				}
				
				not_used_IDs = calculate_diference(&not_used_IDs, &used_IDs);

				 minor_dst= find_minor_DST(*p,temp_sol[i][temp_sol[i].size()-1],&not_used_IDs,&kmeans_ST);
				
				 if (acum_demand + p->get_client(minor_dst)->get_demand() > p->get_capacity())
				 {
					// cout << "nao cabe, logo nao pode ser guloso mais" << endl;
						 greedy = false;
						 i--;
						 continue;
				 }
				 else
				 {
					// cout << "cabe" << endl;
					
					 used_IDs.push_back(minor_dst);// adiciona o incice no vetor de indices usados
					 temp_sol[i].push_back(minor_dst);
					 num_clients_added++;
					 i--;
					 continue;


					 //valid = true;
				 }

				//cout << "n�o pode entrar aqui" << endl;
			}
			else //metodo aleatorio
			{
			
				
				do
				{
					int new_ID = (rand() % p->get_num_clients() + 1);//cria valores aleatorios
					
					
					not_used_IDs.clear();
					
					for (int j = 1; j < p->get_num_clients() + 1; j++)
					{
						not_used_IDs.push_back(j);
						

					}

					not_used_IDs = calculate_diference(&not_used_IDs, &used_IDs);
					
					
					if (used_IDs.size() == p->get_num_clients() - 1 || not_used_IDs.size() == 0)
					{
				
						valid = true;
						greedy = false;
						break;
					}
					if (find(used_IDs.begin(), used_IDs.end(), new_ID) == used_IDs.end() && new_ID != 0)
					{
						//verificar capacidade
						
						if (acum_demand + p->get_client(new_ID)->get_demand() > p->get_capacity())
						{
					
							cont_choosen_random_cl++;
							//procura por at� 10 clientes para adicionar, caso nao consiga achar
							//um v�lido passa para o pr�ximo ve�culo.
							if (cont_choosen_random_cl > 10)
							{
								cont_choosen_random_cl = 0;
								valid = false;
								break;
							}
						}
						else
						{
						//	cout << "demanda ok" << endl;
							acum_demand += p->get_client(new_ID)->get_demand();
							used_IDs.push_back(new_ID);// adiciona o incice no vetor de indices usados
							temp_sol[i].push_back(new_ID);
							num_clients_added++;
							i--;
							break;
							//valid = true;
						}
						
					}
					else
					{
						valid = false;
					}
					//cout << "preso while" << endl;
				} while (!valid);
				//cout << "preso for" << endl;
			}
		}
	actual_vehicle = p->get_num_vehicle();

	

			//para os clientes restantes

	while (num_clients_added < p->get_num_clients())//verificar numero de clientes no fim
	{
		cont_choosen_random_cl = 0;

				do
				{
					int new_ID = (rand() % p->get_num_clients() + 1);//cria valores aleatorios
					if (num_clients_added == p->get_num_clients())
					{
						break;
					}
					if (find(used_IDs.begin(), used_IDs.end(), new_ID) == used_IDs.end() && new_ID != 0)
					{
						//verificar capacidade
						if (added == 0)
						{
							
							acum_demand = 0;
						}
						else
						{
							acum_demand = 0;
							for (int k = 0; k < temp_sol[actual_vehicle].size(); k++)
							{
								acum_demand += p->get_client(temp_sol[actual_vehicle][k])->get_demand();
							}
						}
					
						if (acum_demand + p->get_client(new_ID)->get_demand() > p->get_capacity())
						{
							
							cont_choosen_random_cl++;
							
							//um v�lido passa para o pr�ximo ve�culo.
							if (cont_choosen_random_cl > 10)
							{
								actual_vehicle++;
								added = 0;
								valid = false;
								
							}
						}
						else
						{
							if (added == 0)
							{

							
								used_IDs.push_back(new_ID);// adiciona o incice no vetor de indices usados
								temp_route.clear();
								temp_route.push_back(new_ID);
								temp_sol.push_back(temp_route);
								num_clients_added++;
								added++;
								
							}
							else
							{
							
								used_IDs.push_back(new_ID);// adiciona o incice no vetor de indices usados
								temp_sol[actual_vehicle].push_back(new_ID);
								num_clients_added++;
								
							}
							


							
							
							
							
						}

					}
					else
					{
						valid = false;
					}

				} while (!valid);
	}
	S = new ISolution(p->get_num_clients(), actual_vehicle+1);
	
	for (int i = 0; i < actual_vehicle+1; i++)
	{
		
		S->set_route_size(i, temp_sol[i].size());
		temp_route_aux = new int[temp_sol[i].size()];
		
		for (int k = 0; k < temp_sol[i].size(); k++)
		{
			
			temp_route_aux[k] = temp_sol[i][k];
			
		}
		
		S->set_route(i, temp_route_aux, temp_sol[i].size());
		delete [] temp_route_aux;
	}
	
	if (!p->constraint_vehicle_capacity(*S))
	{
		cout << "Capacidade excedida" << endl;
		system("PAUSE");
	}
	
	float obj1j;
	float obj2j;
	//Possivel melhoria pode ser aplicada aqui
	bool better=false;
	int conta = 0;
	/*do {
		obj1j = p->obj1(S);
		obj2j = p->obj2(S, S->get_dist_travel());
		realoc_inter_route(*p, S);
	//	movement_inter_route(*p, S);
	//	movement_intra_route(*p, S);
		S->set_obj1_cost(p->obj1(S));
		S->set_obj2_freshness(p->obj2(S, S->get_dist_travel()));
		//cout << " $ ";

	
		if (obj1j > S->get_obj1_cost() && obj2j < S->get_obj2_freshness())
		{
			//cout << obj1j << "," << obj2j << "," << S->get_obj1_cost() << "," << S->get_obj2_freshness() << endl;
			//cout << "true" << endl;
		
			conta++;
		}
		if (conta > 30)
		{
			better = true;
		}
		
	} while ((!better) && (conta<30));
*/
	add_individual(S);

}

int Population::find_minor_DST(Problem p,int last_added, vector<int>* not_used_IDs, KmeansST * kmeans_ST)
{
	float minor=10;
	float m1 = 0, m2 = 0;
	int pos_minor=0;
	

	for (int i = 0; i < not_used_IDs->size(); i++)
	{
		
		m1 = kmeans_ST->Calculate_Spatial_Temporal_Distance_Pair(p, *p.get_client(last_added), *p.get_client(not_used_IDs->at(i)));
		
			if (m1 < minor)
			{
				minor = m1;
				pos_minor = not_used_IDs->at(i);
			}
	}
	
	return pos_minor;
}

void Population::print_population(Problem p,FILE* arq1,FILE *arq2,FILE*arq3)
{
	for (int i = 0; i < pop.size(); i++)
	{
		//fprintf(arq1, "%.6f\n", pop.at(i).get_obj1_cost());
		//fprintf(arq2, "%.6f\n", pop.at(i).get_inv_obj2_freshness());
		cout << fixed << setprecision(2)<< "ind" << i << "  " << pop.at(i).get_obj1_cost()<<"  "<<
			pop.at(i).get_obj2_freshness()<< " " << pop.at(i).get_CD_obj1()<< " " << pop.at(i).get_CD_obj2()<<" id:"<<pop.at(i).get_id()<<endl;
	
	
	}
}

void Population::uni_pops(Population *p2)
{
	pop.insert(pop.end(), p2->pop.begin(), p2->pop.end());
}

void Population::transfer_pop(Population *p2)
{
	clear_pop();
	pop.assign(p2->pop.begin(), p2->pop.end());
}

void Population::clear_pop()
{
	pop.clear();
}

void Population::reset_ident()
{
	for (int i = 0; i < max_size*2; i++)
	{
		pop.at(i).set_ident(i);
	}
}



void Population::reset_ident_SMSEMOA()
{
	for (int i = 0; i < max_size+1; i++)
	{
		pop.at(i).set_ident(i);
	}
}
