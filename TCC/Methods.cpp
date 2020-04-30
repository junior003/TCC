#include "Methods.h"
#include <cmath>
#include <limits.h>
#include <algorithm>

#include <iomanip>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>




//Calcula a Diferenca entre dois conjuntos e retorna o resultado no primeiro
vector<int> calculate_diference(vector<int>*P, vector<int>*X)
{

	vector<int> R;
	bool valid = true;
	if (P->size() == 0)
	{
		return R;
	}
	for (int i = 0; i<P->size(); i++)
	{
		valid = true;
		for (int j = 0; j<X->size(); j++)
		{
			
			if (P->at(i) == X->at(j))
			{

				valid = false;
				
			}
			else if (j == X->size() - 1 && valid)
			{
				
				R.push_back(P->at(i));
			}
		}
	}
	return R;
}



float calc_phi(float time)
{

	return exp((log(2) / 720)*time) - 1;
}

float calc_beta(float time)
{

	return 1 - calc_phi(time);
}
void sort_by_early_time(int * route_temp,int  n,Problem *p)
{
	int aux = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (p->get_client(route_temp[i])->get_initial_T() < p->get_client(route_temp[j])->get_initial_T())
			{
				aux = route_temp[i];
				route_temp[i] = route_temp[j];
				route_temp[j] = aux;
			}

		}
	}
}

void sort_by_spatial_distance(int * route_temp, int  n, Problem *p)
{
	int * final_route = new int[n];
	float menordist = INT_MAX;
	float dist_atual = 0;
	

	int pivox = 0;
	int pivoy = 0;
	bool repeated = false;
	pivox = p->get_client(0)->get_X();
	pivoy = p->get_client(0)->get_Y();
	cout << "PIVO:0" << endl;
	for (int i = 0,j=0; i < n; i++)
	{
		cout << "i:" << i<<endl;
		cout << "j:" << j<<endl;
		cout << "posicao no final_route:" << j;
		dist_atual = p->euclidian_distance(pivox,pivoy,
			p->get_client(route_temp[i])->get_X(), p->get_client(route_temp[i])->get_Y());
		cout << "dist_atual:" << dist_atual;
		if (dist_atual == 0)
		{
			continue;
		}
		else if (dist_atual < menordist)
		{
			for (int w = 0; w < j; w++)
			{
				if (route_temp[i] == final_route[w])
				{
					repeated = true;
				}
			}
			if (repeated)
			{
				repeated = false;
				continue;
			}
			menordist = dist_atual;
			cout << "a menor distancia e dist_atual" << endl;
			cout << "final route recebe:" << route_temp[i] << endl;
			final_route[j] = route_temp[i];
		}
		if (i == n - 1)
		{
			cout << "terminou uma rodada, muda pivo" << endl;
			pivox = p->get_client(final_route[j])->get_X();
			pivoy = p->get_client(final_route[j])->get_Y();
			j++;
			i = -1;
			if (j == n)
			{
				cout << "terminou a escolha dos pivos" << endl;
				break;
			}
			menordist = INT_MAX;
		}

		
		
	}

	for (int k = 0; k < n; k++)
	{
		cout << "rt[k]= " << final_route[k] << endl;
		route_temp[k] = final_route[k];
	}
	
	
}
void switch_clients(int*route_temp,int size)
{
	if (size == 1)
	{
		return;
	}
	int i, j,aux;
	do {
		i = rand() % size;
		j = rand() % size;
	} while (i == j);
	
	aux = route_temp[i];
	route_temp[i] = route_temp[j];
	route_temp[j] = aux;
}

vector<ISolution> build_solutions_space_or_timewindow_sort_S(Problem *p,KmeansS * K, int num_max_s)
{
	
	vector<ISolution> solutions;
	ISolution *s;
	int* Seq_kmeans_sol = new int[p->get_num_clients()];
	int* Seq_v_allocated_kmeans_sol = new int[p->get_num_clients()];
	int pos_added_Seq = 0;

	
	int *Seq_backup = new int[p->get_num_clients()];
	int  *uma99 = new int[p->get_num_clients()];

	for (int i = 0; i < p->get_num_clients(); i++)
	{
		uma99[i] = i+1;
	
	}

	for (int i = 0; i < p->get_num_vehicle(); i++)
	{
		for (int j = 0; j < K->get_cluster(i)->get_Num_Clients(); j++)
		{
			
			
			Seq_kmeans_sol[pos_added_Seq++] = K->get_cluster(i)->get_Client(j).get_id();
			Seq_backup[pos_added_Seq - 1] = Seq_kmeans_sol[pos_added_Seq - 1];
		}
	}

	for (int i = 0; i < p->get_num_clients(); i++)
	{
		for (int j = 0; j < p->get_num_clients(); j++)
		{
			if (Seq_backup[i] < 100)
			{
				if (Seq_backup[i] == uma99[j])
				{
					uma99[j] = -1;
				}
			}
		}
	}

	for (int i = 0; i < p->get_num_clients(); i++)
	{
		if (Seq_backup[i] >= 100 || Seq_backup[i] <= 0)
		{
			for (int j = 0; j < p->get_num_clients(); j++)
			{
				if (uma99[j] != -1)
				{
					Seq_backup[i] = uma99[j];
					uma99[j] = -1;
					break;
				}
			}
		}
	}

	for (int i = 0; i < p->get_num_clients(); i++)
	{
		Seq_kmeans_sol[i] = Seq_backup[i];
	}
	
	int acum_demand=0;
	int cont_num_v = 1;
	int actual_vehicle = 0;
	int* cont_route_sizes=0;
	
	for (int i = 0; i < p->get_num_clients(); i++)
	{
		
		if (acum_demand + p->get_client(Seq_kmeans_sol[i])->get_demand() > p->get_capacity())
		{
			cont_num_v++;
			acum_demand = p->get_client(Seq_kmeans_sol[i])->get_demand();
			
		}
		else
		{
			acum_demand += p->get_client(Seq_kmeans_sol[i])->get_demand();
		}
	}
	
	cont_route_sizes = new int[cont_num_v];
	acum_demand = 0;
	cont_route_sizes[actual_vehicle] = 0;
	
	for (int i = 0; i < p->get_num_clients(); i++)
	{
		if (acum_demand + p->get_client(Seq_kmeans_sol[i])->get_demand() > p->get_capacity())
		{
			
			cont_route_sizes[++actual_vehicle] = 0;
			cont_route_sizes[actual_vehicle]++;
			acum_demand = p->get_client(Seq_kmeans_sol[i])->get_demand();
			Seq_v_allocated_kmeans_sol[i] = actual_vehicle;
			
		}
		else
		{
			Seq_v_allocated_kmeans_sol[i] = actual_vehicle;
			
			cont_route_sizes[actual_vehicle]++;
			acum_demand += p->get_client(Seq_kmeans_sol[i])->get_demand();
		}
	}
	int actual_client = -1;


	s = new ISolution(p->get_num_clients(), cont_num_v);
	
	for (int i = 0; i < cont_num_v; i++)
	{
		int j = 0;
		int* route_temp = new int[cont_route_sizes[i]];
		
		do 
		{
			
			route_temp[j] = Seq_kmeans_sol[++actual_client];
			
			j++;
			
		} while (Seq_v_allocated_kmeans_sol[actual_client] == Seq_v_allocated_kmeans_sol[actual_client + 1]);
	
		sort_by_early_time(route_temp, cont_route_sizes[i], p);
	
		s->set_route_size(i, cont_route_sizes[i]);
		s->set_route(i, route_temp, cont_route_sizes[i]);

		delete[] route_temp;
	}
	delete[] Seq_kmeans_sol;
//	cout << "6" << endl;
	solutions.push_back(*s);

	float obj1j;
	float obj2j;

	bool better = false;
	int conta = 0;

		//Possivel melhoria pode ser aplicada na construcao da solucao, basta retirar o comentario abaixo
	/*for (int i = 0; i < p->get_num_clients() / 5; i++)
	{
	
		movement_inter_route(*p, &solutions.at(0));
		movement_intra_route(*p, &solutions.at(0));
	}
	*/
	/*do {
		obj1j = p->obj1(S);
		obj2j = p->obj2(S, S->get_dist_travel());
		realoc_inter_route(*p, S);
  	    
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

	
	return solutions;
}

vector<ISolution> build_solutions_space_or_timewindow_sort_ST(Problem *p, KmeansST * K, int num_max_s)
{
	vector<ISolution> solutions;
	ISolution *s;
	int* Seq_kmeans_sol = new int[p->get_num_clients()];
	int* Seq_v_allocated_kmeans_sol = new int[p->get_num_clients()];
	int pos_added_Seq = 0;

	for (int i = 0; i < p->get_num_vehicle(); i++)
	{
		for (int j = 0; j < K->get_cluster(i)->get_Num_Clients(); j++)
		{
			Seq_kmeans_sol[pos_added_Seq++] = K->get_cluster(i)->get_Client(j).get_id();
			
		}
	}

	int acum_demand = 0;
	int cont_num_v = 1;
	int actual_vehicle = 0;
	int* cont_route_sizes;


	for (int i = 0; i < p->get_num_clients(); i++)
	{
		if (acum_demand + p->get_client(Seq_kmeans_sol[i])->get_demand() > p->get_capacity())
		{
			cont_num_v++;
			acum_demand = p->get_client(Seq_kmeans_sol[i])->get_demand();
		}
		else
		{
			acum_demand += p->get_client(Seq_kmeans_sol[i])->get_demand();
		}
	}
	cont_route_sizes = new int[cont_num_v];
	acum_demand = 0;
	cont_route_sizes[actual_vehicle] = 0;

	for (int i = 0; i < p->get_num_clients(); i++)
	{
		if (acum_demand + p->get_client(Seq_kmeans_sol[i])->get_demand() > p->get_capacity())
		{

			cont_route_sizes[++actual_vehicle] = 0;
			cont_route_sizes[actual_vehicle]++;
			acum_demand = p->get_client(Seq_kmeans_sol[i])->get_demand();
			Seq_v_allocated_kmeans_sol[i] = actual_vehicle;

		}
		else
		{
			Seq_v_allocated_kmeans_sol[i] = actual_vehicle;

			cont_route_sizes[actual_vehicle]++;
			acum_demand += p->get_client(Seq_kmeans_sol[i])->get_demand();
		}
	}
	int actual_client = -1;

	s = new ISolution(p->get_num_clients(), cont_num_v);

	for (int i = 0; i < cont_num_v; i++)
	{
		int j = 0;
		int* route_temp = new int[cont_route_sizes[i]];
		//cout << "valores sendo repassados de seq para route:" << endl;
		do
		{

			route_temp[j] = Seq_kmeans_sol[++actual_client];
			
			j++;

		} while (Seq_v_allocated_kmeans_sol[actual_client] == Seq_v_allocated_kmeans_sol[actual_client + 1]);
		sort_by_early_time(route_temp, cont_route_sizes[i], p);

		s->set_route_size(i, cont_route_sizes[i]);
		s->set_route(i, route_temp, cont_route_sizes[i]);

		delete[] route_temp;
	}
	solutions.push_back(*s);
	delete[] Seq_kmeans_sol;
	delete[] Seq_v_allocated_kmeans_sol;



	return solutions;
}

float maxi(float a, float b)
{
	if (a > b)
	{
		return a;
	}
	else {
		return b;
	}
}

float mini(float a, float b)
{
	if (a < b)
	{
		return a;
	}
	else {
		return b;
	}
	
}


void movement_intra_route(Problem p, ISolution* S)//trocas dentro da mesma rota
{
	int pi = 0,pj=0;
	int *path;
	int aux;
	for (int i = 0; i < S->get_num_vehicles_in_S(); i++)
	{
		
		//somente em 50% das vezes havera troca 
		if (randomic(0, 1) > 0.5)
		{
			path = NULL;
			if (S->get_route_size(i) > 1)
			{
				do
				{
					
					pi = rand() % S->get_route_size(i);
					pj = rand() % S->get_route_size(i);
				} while (pi == pj);

				

				path = S->get_path_Sol(i, S->get_route_size(i));
				
				aux = path[pi];
				
				path[pi] = path[pj];
				
				path[pj] = aux;
			
				sort_by_early_time(path, S->get_route_size(i), &p);
				S->substitute_route(i, path, S->get_route_size(i));

				
			}
		}
				

			
			
		
	}

}

//troca entre de dois clientes de rotas diferentes
void movement_inter_route(Problem p, ISolution *S)
{
	int ri = 0, rj = 0, pi = 0, pj = 0;
	int *pathi,*pathj;
	int aux;

		for (int k = 0; k < 1; k++)
		{

			do
			{
				//cout << "gerando";

				ri = rand() % S->get_num_vehicles_in_S();
				rj = rand() % S->get_num_vehicles_in_S();
				
			} while (ri == rj);

			pathi = NULL;
			pathj = NULL;
			
				
			
				pi = rand() % S->get_route_size(ri);
				pj = rand() % S->get_route_size(rj);
				

				pathi = S->get_path_Sol(ri, S->get_route_size(ri));
				pathj = S->get_path_Sol(rj, S->get_route_size(rj));
				
				aux = pathi[pi];
				
				pathi[pi] = pathj[pj];
				
				pathj[pj] = aux;
				
				sort_by_early_time(pathi, S->get_route_size(ri), &p);
				sort_by_early_time(pathj, S->get_route_size(rj), &p);
				S->substitute_route(ri, pathi, S->get_route_size(ri));
				S->substitute_route(rj, pathj, S->get_route_size(rj));
				
				
			}


	
}


void realoc_inter_route(Problem p, ISolution *S)
{
	int pos_realoc_or = 0, pos_realoc_de = 0;

	//sequencias que representam os pais e filhos sem divis�o de veiculos
	int *Seq_f1 = new int[p.get_num_clients()];
	int *Seq_f1_new = new int[p.get_num_clients()];
	
	vector<int> alocated_cl_s1;
	/
	int added = 0, added_s1 = 0, added_s2 = 0;
	int *route_f1;
	int *route_f2;
	for (int i = 0; i < S->get_num_vehicles_in_S(); i++)
	{
		route_f1 = S->get_path_Sol(i, S->get_route_size(i));
		for (int j = 0; j < S->get_route_size(i); j++)
		{
			Seq_f1[added++] = route_f1[j];

		}
		route_f1 = NULL;
	}





	//definindo pontos de corte e alocando vetores

	int cont_better = 0;
	int acum_demand = 0;
	int cont_num_v = 1;
	int old_num_v = 1;
	int actual_vehicle = 0;
	do {
		do
		{
			pos_realoc_or = rand() % p.get_num_clients();
			pos_realoc_de = rand() % p.get_num_clients();
		} while (pos_realoc_or == pos_realoc_de);


		int realoced_cl = Seq_f1[pos_realoc_or];


		//Realocar e colocar na nova sequencia


		for (int i = 0; i < p.get_num_clients(); i++)
		{
			if (pos_realoc_or < pos_realoc_de)
			{
				if (i < pos_realoc_de && i < pos_realoc_or)
				{
					Seq_f1_new[i] = Seq_f1[i];
				}
				if (i < pos_realoc_de && i >= pos_realoc_or)
				{
					Seq_f1_new[i] = Seq_f1[i + 1];
				}
				if (i == pos_realoc_de)
				{
					Seq_f1_new[i] = Seq_f1[pos_realoc_or];
				}
				if (i > pos_realoc_de)
				{
					Seq_f1_new[i] = Seq_f1[i];
				}

			}
			else
			{
				if (i < pos_realoc_de && i < pos_realoc_or)
				{
					Seq_f1_new[i] = Seq_f1[i];
				}
				else if (i == pos_realoc_de)
				{
					Seq_f1_new[i] = Seq_f1[pos_realoc_or];
				}
				else if (i <= pos_realoc_or && i > pos_realoc_de)
				{
					Seq_f1_new[i] = Seq_f1[i - 1];
				}
				else if (i > pos_realoc_or)
				{
					Seq_f1_new[i] = Seq_f1[i];
				}
			}
		}



		acum_demand = 0;
		cont_num_v = 1;
		actual_vehicle = 0;

		for (int i = 0; i < p.get_num_clients(); i++)
		{
			if (acum_demand + p.get_client(Seq_f1_new[i])->get_demand() > p.get_capacity())
			{
				cont_num_v++;
				acum_demand = p.get_client(Seq_f1_new[i])->get_demand();
			}
			else
			{
				acum_demand += p.get_client(Seq_f1_new[i])->get_demand();
			}
		}
		if (old_num_v == 1)
		{
			old_num_v = cont_num_v;
		}
		cont_better++;
	} while (false);
	

	//cout << old_num_v << " X " << cont_num_v << endl;
	
	int* cont_route_sizes;
	int* Seq_v_allocated_f1 = new int[p.get_num_clients()];
	
	cont_route_sizes = new int[cont_num_v];
	acum_demand = 0;
	cont_route_sizes[actual_vehicle] = 0;
	
	for (int i = 0; i < p.get_num_clients(); i++)
	{
		if (acum_demand + p.get_client(Seq_f1_new[i])->get_demand() > p.get_capacity())
		{

			
			cont_route_sizes[++actual_vehicle] = 0;
			cont_route_sizes[actual_vehicle]++;
			acum_demand = p.get_client(Seq_f1_new[i])->get_demand();
			Seq_v_allocated_f1[i] = actual_vehicle;

		}
		else
		{
			Seq_v_allocated_f1[i] = actual_vehicle;

			cont_route_sizes[actual_vehicle]++;
			acum_demand += p.get_client(Seq_f1_new[i])->get_demand();
		}
	}
	int actual_client = -1;
	

	vector<vector<int>> path_sol_aux;
	vector<int> rt;

	
	int j;
	int* route_temp;
	for (int i = 0; i < cont_num_v; i++)
	{
		j = 0;
		route_temp = new int[cont_route_sizes[i]];

		do
		{

			route_temp[j] = Seq_f1_new[++actual_client];

			
			j++;
			

		} while (Seq_v_allocated_f1[actual_client] == Seq_v_allocated_f1[actual_client + 1]);
	
		
		S->recreate_route_sizes(cont_num_v);
		S->set_num_vehicles_in_S(cont_num_v);
		
		S->set_route_size(i, cont_route_sizes[i]);
	
		S->recreate_path_sol();

	

		for (int w = 0; w < cont_route_sizes[i]; w++)
		{
			rt.push_back(route_temp[w]);
		}
		path_sol_aux.push_back(rt);
		rt.clear();

		S->set_route(i, route_temp, cont_route_sizes[i]);

		delete[] route_temp;
		

	}

	S->sol_path = path_sol_aux;
	for (int i = 0; i < cont_num_v; i++)
	{
		
			S->set_route_size(i, cont_route_sizes[i]);
	

	}
	

	delete[] cont_route_sizes;

	
}


void printaSolucao(Problem p, ISolution s)
{
	cout << "num_v: " << s.get_num_vehicles_in_S() << endl;
	for (int i = 0; i < s.get_num_vehicles_in_S(); i++)
	{
		cout << s.get_route_size(i) << "..";
	}
	cout << endl;

	int cont = 0;
	for (int i = 0; i < s.get_num_vehicles_in_S(); i++)
	{
		cout << "veiculo " << i << ":";
		cout << "[";
		for (int j = 0; j < s.get_route_size(i); j++)
		{
			cout << p.get_client(s.get_client_id_on_route(i, j))->get_id() << ".";
			cont++;
		}
		cout << "]";
	}
	cout << "numero de clientes na solucao:" << cont << endl;


}

bool evalue_solution(Problem *p, vector<ISolution> *solutions)
{
	bool feasible = false;
	for (int i = 0; i < solutions->size(); i++)
	{
		if (p->constraint_curr_freshness(solutions->at(i)))
		{
			feasible = true;
		}
		else
		{
			feasible = false;
		}

	}
}


float randomic(float min, float max)
{
	if (min == max)
	{
		return min;
	}
	return ((float)(rand() % 1000 / 1000.0)*(max - min) + min);
}

//funcao que indica se o teste foi rodado (retornando false por padrao)
bool ja_rodou(int test)
{
	
	return false;
}



void teste_completo(int test)
{
	FILE* arq;

	if ((arq = fopen("state_teste.bin", "r+b")) == NULL)
	{
		cout << "erro ao abrir o arquivo";
		return;
	}
	bool state = true;
	for (int i = 0; i < 23; i++)
	{
		if (i <= test)
		{
			fwrite(&state, sizeof(bool), 1, arq);
		}
	}
	fclose(arq);
}
void inicia_teste()
{
	FILE* arq;
	if ((arq = fopen("state_teste.bin", "r+b")) == NULL)
	{
		cout << "erro ao abrir o arquivo state";
		return;
	}

	bool state = false;
	fread(&state, sizeof(bool), 1, arq);
	cout << "iniciando " << state;
	if (!state)
	{
		for (int i = 0; i < 23; i++)
		{
			fwrite(&state, sizeof(bool), 1, arq);
		}
	}
	
	fclose(arq);
}