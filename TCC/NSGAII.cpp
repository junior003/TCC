#include "NSGAII.h"
#include "Methods.h"
#include <vector>
#include <limits.h>
#include "ISolution.h"
#include "KmeansS.h"
#include "KmeansST.h"
#include "HiperVolume.h"

NSGAII::NSGAII()
{
}

NSGAII::NSGAII(int num_i, int num_gen, float prob_cross, float prob_mut)
{
	num_ind = num_i;
	num_generation = num_gen;
	prob_crossover = prob_cross;
	prob_mutation = prob_mut;
	pop_1.set_max_size(num_i);
	pop_2.set_max_size(num_i);
	surv_pop.set_max_size(num_i);
}

void NSGAII::generate_Population(Problem p, int nind)
{
	bool valid=false;
	for (int i = 0; i < nind; i++)
	{
		/*if (i < 5)
		{
			KmeansS kmeans(p.get_num_vehicle(), p.get_num_clients(), 2);
			kmeans.execute_kmeans_S(&p);
			pop_1.add_individual(&build_solutions_space_or_timewindow_sort_S(&p, &kmeans, 1)[0]);
			cout << "2 ind" << endl;
		}
		else {*/
			//cout << "others ind" << endl;
			
			
			pop_1.Generate_Random_Individual(&p);
	//}
		


		
	}
}

void NSGAII::evaluate_population(Problem p,Population *pop)
{
	
	for (int i = 0; i < pop->get_max_size(); i++)
	{
		pop->get_individual(i)->set_obj1_cost(p.obj1(pop->get_individual(i)));
		pop->get_individual(i)->set_obj2_freshness(p.obj2(pop->get_individual(i),
														 pop->get_individual(i)->get_dist_travel()));
	}
}
//metodo que verifica se a primeira solucao passada por parametro domina a segunda
bool NSGAII::verify_domination(ISolution s1, ISolution s2)
{

	//se s1 dominar s2
	if (s1.get_inv_obj2_freshness() < s2.get_inv_obj2_freshness() && s1.get_obj1_cost() < s2.get_obj1_cost())
	{
		return true;
	}
	else
	{
		return false;
	}
	
}

void NSGAII::binary_tournament(Problem p, Population pop, int * father1, int * father2,int ngen)
{
	float obj1_f1, obj2_f1;
	float obj1_f2, obj2_f2;
	int f1, f2;
	
	do
	{
		f1 = rand() % pop.get_max_size();
		f2 = rand() % pop.get_max_size();
	} while (f1 == f2);
	
	obj1_f1 = pop.get_individual(f1)->get_obj1_cost();
	obj2_f1 = pop.get_individual(f1)->get_obj2_freshness();
	obj1_f2 = pop.get_individual(f2)->get_obj1_cost();
	obj2_f2= pop.get_individual(f2)->get_obj2_freshness();
	
	if (obj1_f1<obj1_f2 && obj2_f1>obj2_f2)
	{
		
		*father1 = f1;

	}
	else if (obj1_f2<obj1_f1 && obj2_f2>obj2_f1)
	{
		//cout << "f2 domina -> escolhida" << endl;
		*father1 = f2;
		
	}
	else if (ngen > 1)
	{
		if (pop.get_individual(f1)->get_CD_obj1() > pop.get_individual(f2)->get_CD_obj1())
		{
			//cout << "ndom = f1"<<endl;
			*father1 = f1;
		}
		else if (pop.get_individual(f1)->get_CD_obj1() < pop.get_individual(f2)->get_CD_obj1()) {
			//cout << "ndom = f2"<<endl;
			*father1 = f2;
		}
		else if (pop.get_individual(f1)->get_CD_obj2() > pop.get_individual(f2)->get_CD_obj2())
		{
			*father1 = f1;
		}
		else
		{
			*father1 = f2;
		}
	}
	else
	{
		if (randomic(0, 1) > 0.5)
		{
			*father1 = f1;
		}
		else
		{
			*father1 = f2;
		}
	}

	do
	{
		f1 = rand() % pop.get_max_size();
		f2 = rand() % pop.get_max_size();
	} while (f1 == f2 || f1==*father1 || f2 == *father1);
	obj1_f1 = pop.get_individual(f1)->get_obj1_cost();
	obj2_f1 = pop.get_individual(f1)->get_obj2_freshness();
	obj1_f2 = pop.get_individual(f2)->get_obj1_cost();
	obj2_f2 = pop.get_individual(f2)->get_obj2_freshness();

	if (obj1_f1<obj1_f2 && obj2_f1>obj2_f2)
	{
		*father2 = f1;
	}
	else if (obj1_f2<obj1_f1 && obj2_f2>obj2_f1)
	{
		*father2 = f2;
	}
	else if (ngen > 1)
	{
		if (pop.get_individual(f1)->get_CD_obj1() > pop.get_individual(f2)->get_CD_obj1())
		{
			//cout << "ndom = f1"<<endl;
			*father2 = f1;
		}
		else if (pop.get_individual(f1)->get_CD_obj1() < pop.get_individual(f2)->get_CD_obj1()) {
			//cout << "ndom = f2"<<endl;
			*father2 = f2;
		}
		else if (pop.get_individual(f1)->get_CD_obj2() > pop.get_individual(f2)->get_CD_obj2())
		{
			*father2 = f1;
		}
		else
		{
			*father2 = f2;
		}
	}
	else
	{
		if (randomic(0, 1) > 0.5)
		{
			*father2 = f1;
		}
		else
		{
			*father2 = f2;
		}
	}
}

void NSGAII::crossover_OX(Problem p,ISolution father1, ISolution father2, Population * pop2)
{
	int pos_cut1=0, pos_cut2=0;
	int *Seq_f1 = new int[p.get_num_clients()];
	int *Seq_f2 = new int[p.get_num_clients()];
	int *Seq_s1 = new int[p.get_num_clients()];
	int *Seq_s2 = new int[p.get_num_clients()];
	vector<int> alocated_cl_s1, alocated_cl_s2;
	
	//converter os pais para um vetor
	int added = 0,added_s1 = 0,added_s2=0;
	int *route_f1;
	int *route_f2;
	for (int i = 0; i < father1.get_num_vehicles_in_S(); i++)
	{
		route_f1 = father1.get_path_Sol(i, father1.get_route_size(i));
		for (int j = 0; j < father1.get_route_size(i); j++)
		{
			Seq_f1[added++] = route_f1[j];
			
		}
		route_f1 = NULL;
	}

	

	added = 0;
	for (int i = 0; i < father2.get_num_vehicles_in_S(); i++){

		route_f2 = father2.get_path_Sol(i, father2.get_route_size(i));
		for (int j = 0; j < father2.get_route_size(i); j++)
		{
			Seq_f2[added++] = route_f2[j];
			
		}
		route_f2 = NULL;
	}
	


	

	//cortar
	
	do
	{
		pos_cut1 = rand() % p.get_num_clients();
		pos_cut2 = rand() % p.get_num_clients();
	} while (pos_cut1 == pos_cut2 || pos_cut1 > pos_cut2);

	int *cutf1 = new int[pos_cut2 - pos_cut1 + 1];
	int *cutf2 = new int[pos_cut2 - pos_cut1 + 1];

	for (int i = pos_cut1,j=0; i < pos_cut2+1; i++,j++)
	{
		cutf1[j] = Seq_f1[i];
		cutf2[j] = Seq_f2[i];
	}

	
	//passar para os filhos
	
	for (int i = pos_cut1, j = 0; i < pos_cut2 + 1; i++, j++)
	{
		Seq_s1[i] = cutf1[j];
		Seq_s2[i] = cutf2[j];
	}
	
	//repasse de genes sem repeticao
	//primeira etapa
	//FILHO 1
	bool repeated = false;
	added_s1 = pos_cut2 + 1;
	for (int i = pos_cut2 + 1; i < p.get_num_clients(); i++)
	{
		for (int j = 0; j < pos_cut2 - pos_cut1 + 1; j++)
		{
			if (Seq_f2[i] == cutf1[j])
			{
				repeated = true;
				break;
			}
		}
		if (repeated)
		{
			repeated = false;
			continue;
		}
		else
		{
			Seq_s1[added_s1++] = Seq_f2[i];
		}
	}



	for (int i = 0; i < pos_cut2+1; i++)
	{
		for (int j = 0; j < pos_cut2 - pos_cut1 + 1; j++)
		{
			if (Seq_f2[i] == cutf1[j])
			{
				repeated = true;
				break;
			}
		}
		if (repeated)
		{
			repeated = false;
			continue;
		}
		else
		{
			if (added_s1 == p.get_num_clients())
			{
				added_s1 = 0;
			}
			Seq_s1[added_s1++] = Seq_f2[i];
		}
	}

	//FILHO 2
	//primeira etapa
	repeated = false;
	added_s2 = pos_cut2 + 1;
	for (int i = pos_cut2 + 1; i < p.get_num_clients(); i++)
	{
		for (int j = 0; j < pos_cut2 - pos_cut1 + 1; j++)
		{
			if (Seq_f1[i] == cutf2[j])
			{
				repeated = true;
				break;
			}
		}
		if (repeated)
		{
			repeated = false;
			continue;
		}
		else
		{
			Seq_s2[added_s2++] = Seq_f1[i];
		}
	}
	for (int i = 0; i < pos_cut2+1; i++)
	{
		for (int j = 0; j < pos_cut2 - pos_cut1 + 1; j++)
		{
			if (Seq_f1[i] == cutf2[j])
			{
				repeated = true;
				break;
			}
		}
		if (repeated)
		{
			repeated = false;
			continue;
		}
		else
		{
			if (added_s2 == p.get_num_clients())
			{
				added_s2 = 0;
			}
			Seq_s2[added_s2++] = Seq_f1[i];
		}
	}


	
	int* Seq_v_allocated_s1 = new int[p.get_num_clients()];
	int acum_demand = 0;
	int cont_num_v = 1;
	int actual_vehicle = 0;
	int* cont_route_sizes;
	for (int i = 0; i < p.get_num_clients(); i++)
	{
		if (acum_demand + p.get_client(Seq_s1[i])->get_demand() > p.get_capacity())
		{
			cont_num_v++;
			acum_demand = p.get_client(Seq_s1[i])->get_demand();
		}
		else
		{
			acum_demand += p.get_client(Seq_s1[i])->get_demand();
		}
	}
	cont_route_sizes = new int[cont_num_v];
	acum_demand = 0;
	cont_route_sizes[actual_vehicle] = 0;

	for (int i = 0; i < p.get_num_clients(); i++)
	{
		if (acum_demand + p.get_client(Seq_s1[i])->get_demand() > p.get_capacity())
		{

			cont_route_sizes[++actual_vehicle] = 0;
			cont_route_sizes[actual_vehicle]++;
			acum_demand = p.get_client(Seq_s1[i])->get_demand();
			Seq_v_allocated_s1[i] = actual_vehicle;

		}
		else
		{
			Seq_v_allocated_s1[i] = actual_vehicle;

			cont_route_sizes[actual_vehicle]++;
			acum_demand += p.get_client(Seq_s1[i])->get_demand();
		}
	}
	int actual_client = -1;

	ISolution *son1 = new ISolution(p.get_num_clients(), cont_num_v);
	son1->reset_ident();
	for (int i = 0; i < cont_num_v; i++)
	{
		int j = 0;
		int* route_temp = new int[cont_route_sizes[i]];
		
		do
		{

			route_temp[j] = Seq_s1[++actual_client];
			
			j++;

		} while (Seq_v_allocated_s1[actual_client] == Seq_v_allocated_s1[actual_client + 1]);
		
		son1->set_route_size(i, cont_route_sizes[i]);
		son1->set_route(i, route_temp, cont_route_sizes[i]);

		delete[] route_temp;
	}
	
	delete[] cont_route_sizes;
	
	int* Seq_v_allocated_s2 = new int[p.get_num_clients()];
	 acum_demand = 0;
	 cont_num_v = 1;
	 actual_vehicle = 0;
	cont_route_sizes=NULL;
	for (int i = 0; i < p.get_num_clients(); i++)
	{
		if (acum_demand + p.get_client(Seq_s2[i])->get_demand() > p.get_capacity())
		{
			cont_num_v++;
			acum_demand = p.get_client(Seq_s2[i])->get_demand();
		}
		else
		{
			acum_demand += p.get_client(Seq_s2[i])->get_demand();
		}
	}
	cont_route_sizes = new int[cont_num_v];
	acum_demand = 0;
	cont_route_sizes[actual_vehicle] = 0;

	for (int i = 0; i < p.get_num_clients(); i++)
	{
		if (acum_demand + p.get_client(Seq_s2[i])->get_demand() > p.get_capacity())
		{

			cont_route_sizes[++actual_vehicle] = 0;
			cont_route_sizes[actual_vehicle]++;
			acum_demand = p.get_client(Seq_s2[i])->get_demand();
			Seq_v_allocated_s2[i] = actual_vehicle;

		}
		else
		{
			Seq_v_allocated_s2[i] = actual_vehicle;

			cont_route_sizes[actual_vehicle]++;
			acum_demand += p.get_client(Seq_s2[i])->get_demand();
		}
	}
	actual_client = -1;

	ISolution *son2 = new ISolution(p.get_num_clients(), cont_num_v);

	for (int i = 0; i < cont_num_v; i++)
	{
		int j = 0;
		int* route_temp = new int[cont_route_sizes[i]];
		
		do
		{

			route_temp[j] = Seq_s2[++actual_client];
		
			j++;

		} while (Seq_v_allocated_s2[actual_client] == Seq_v_allocated_s2[actual_client + 1]);
		
		son2->set_route_size(i, cont_route_sizes[i]);
		son2->set_route(i, route_temp, cont_route_sizes[i]);

		delete[] route_temp;
	}
	

	son1->set_obj1_cost(p.obj1(son1));
	son1->set_obj2_freshness(p.obj2(son1, son1->get_dist_travel()));
	son2->set_obj1_cost(p.obj1(son2));
	son2->set_obj2_freshness(p.obj2(son2, son2->get_dist_travel()));
	pop2->add_individual(son1);
	pop2->add_individual(son2);
}

void NSGAII::mutation(Problem p, Population *pop)
{
	
	
		int ri = 0, rj = 0, pi = 0, pj = 0;
		int *pathi, *pathj;
		int aux;
		ISolution *ISaux;
		for (int i = 0; i < pop->get_max_size(); i++)
		{
			
			

			do
			{
				
				ri = rand() % pop->get_individual(i)->get_num_vehicles_in_S();
				rj = rand() % pop->get_individual(i)->get_num_vehicles_in_S();
				
			} while (ri == rj);

			pathi = NULL;
			pathj = NULL;


			pi = rand() % pop->get_individual(i)->get_route_size(ri);
			pj = rand() % pop->get_individual(i)->get_route_size(rj);
			
			
			pathi = pop->get_individual(i)->get_path_Sol(ri, pop->get_individual(i)->get_route_size(ri));
			
			pathj = pop->get_individual(i)->get_path_Sol(rj, pop->get_individual(i)->get_route_size(rj));
			

		
			
			
			aux = pathi[pi];
			
			pathi[pi] = pathj[pj];
			
			pathj[pj] = aux;
			
			
			pop->get_individual(i)->substitute_route(ri, pathi, pop->get_individual(i)->get_route_size(ri));
			pop->get_individual(i)->substitute_route(rj, pathj, pop->get_individual(i)->get_route_size(rj));
			
		
			

		}

}

void NSGAII::crowding_distance_sort(Problem p, Population *pop)
{

	for (int i = 0; i < fronts.size(); i++)
	{
		
		float major_obj1=0, major_obj2=0;
		float minor_obj1=10000, minor_obj2=10000;
		float **dist_matrix_per_front = new float*[fronts.at(i).size()];
		for (int j = 0; j < fronts.at(i).size(); j++)
		{
			//id - obj1 -CD1 - obj2 -CD2
			dist_matrix_per_front[j] = new float[5]; 
			dist_matrix_per_front[j][0] = fronts.at(i).at(j).get_id();
			dist_matrix_per_front[j][1] = fronts.at(i).at(j).get_obj1_cost();
			dist_matrix_per_front[j][2] = 0;
			dist_matrix_per_front[j][3] = fronts.at(i).at(j).get_obj2_freshness();
			dist_matrix_per_front[j][4] = 0;
			//encontrado min e max de cada obj
			if (fronts.at(i).at(j).get_obj1_cost() > major_obj1)
			{
				major_obj1 = fronts.at(i).at(j).get_obj1_cost();
			}
			if (fronts.at(i).at(j).get_obj1_cost() < minor_obj1)
			{
				minor_obj1 = fronts.at(i).at(j).get_obj1_cost();
			}

			if (fronts.at(i).at(j).get_obj2_freshness() > major_obj2)
			{
				major_obj2 = fronts.at(i).at(j).get_obj2_freshness();
			}
			if (fronts.at(i).at(j).get_obj2_freshness() < minor_obj2)
			{
				minor_obj2 = fronts.at(i).at(j).get_obj2_freshness();
			}
			
		}

		order_matrix_CD(dist_matrix_per_front,i);
	
		
		
		dist_matrix_per_front[0][2] = 10000;
		dist_matrix_per_front[0][4] = 10000;
		dist_matrix_per_front[fronts.at(i).size() - 1][2] = 10000;
		dist_matrix_per_front[fronts.at(i).size() - 1][4] = 10000;

		for (int j = 1; j < fronts.at(i).size()-1; j++)
		{
			dist_matrix_per_front[j][2] = (dist_matrix_per_front[j - 1][1] - dist_matrix_per_front[j + 1][1])
										/ (major_obj1-minor_obj1);

			
			dist_matrix_per_front[j][4] = (dist_matrix_per_front[j - 1][3] - dist_matrix_per_front[j + 1][3])/ (major_obj2 - minor_obj2);

			
		}
		
		
		
		for (int j = 0; j < fronts.at(i).size(); j++)
		{
			for (int k = 0; k < fronts.at(i).size(); k++)
			{
				if (fronts.at(i).at(j).get_id() == dist_matrix_per_front[k][0])
				{
					
					if (dist_matrix_per_front[k][2] < 0)
					{
						fronts.at(i).at(j).set_CD_obj1(dist_matrix_per_front[k][2] * (-1));
						fronts.at(i).at(j).set_CD_obj2(dist_matrix_per_front[k][4] * (-1));
					}
					else
					{
						fronts.at(i).at(j).set_CD_obj1(dist_matrix_per_front[k][2] );
						fronts.at(i).at(j).set_CD_obj2(dist_matrix_per_front[k][4] );
					}
					
				}
			}
		}
		
		for (int j = 0; j < fronts.at(i).size(); j++)
		{
			delete[] dist_matrix_per_front[j];
		}
		delete[] dist_matrix_per_front;

	}
	
}

void NSGAII::crowding_distance_selection(Problem p, int F,int ind_surv)
{
	
	float **dist_matrix_per_front = new float*[fronts.at(F).size()];
	for (int i = 0; i < fronts.at(F).size(); i++)
	{
			
			dist_matrix_per_front[i] = new float[5];
			dist_matrix_per_front[i][0] = fronts.at(F).at(i).get_id();
			dist_matrix_per_front[i][1] = fronts.at(F).at(i).get_obj1_cost();
			dist_matrix_per_front[i][2] = fronts.at(F).at(i).get_CD_obj1();
			dist_matrix_per_front[i][3] = fronts.at(F).at(i).get_obj2_freshness();
			dist_matrix_per_front[i][4] = fronts.at(F).at(i).get_CD_obj2();
		

		
	}

	order_matrix_CD_selection(dist_matrix_per_front, F);
	

	//Procura as soluções, por ordem decrescente de CD no front e coloca na pop sobrevivente
	for (int i = 0; i < fronts.at(F).size(); i++)
	{
		for (int j = 0; j < fronts.at(F).size(); j++)
		{
			if (fronts.at(F).at(j).get_id() == dist_matrix_per_front[i][0] && ind_surv<num_ind)
			{
				surv_pop.add_individual(&fronts.at(F).at(j));
				ind_surv++;
			}
		}
	}
	
}


void NSGAII::order_matrix_CD_selection(float ** M, int F)
{
	int j = 2;
	float a1, a2, a3, a4, a5;
	for (int i = 0; i < fronts.at(F).size(); i++)
	{
		for (int w = 0; w < fronts.at(F).size(); w++)
		{
			if (M[i][j] > M[w][j])
			{
				a1 = M[i][0];
				a2 = M[i][1];
				a3 = M[i][2];
				a4 = M[i][3];
				a5 = M[i][4];

				M[i][0] = M[w][0];
				M[i][1] = M[w][1];
				M[i][2] = M[w][2];
				M[i][3] = M[w][3];
				M[i][4] = M[w][4];

				M[w][0] = a1;
				M[w][1] = a2;
				M[w][2] = a3;
				M[w][3] = a4;
				M[w][4] = a5;
			}
			else if (M[i][j] == M[w][j] || M[i][j + 2] > M[w][j + 2])
			{
				a1 = M[i][0];
				a2 = M[i][1];
				a3 = M[i][2];
				a4 = M[i][3];
				a5 = M[i][4];

				M[i][0] = M[w][0];
				M[i][1] = M[w][1];
				M[i][2] = M[w][2];
				M[i][3] = M[w][3];
				M[i][4] = M[w][4];

				M[w][0] = a1;
				M[w][1] = a2;
				M[w][2] = a3;
				M[w][3] = a4;
				M[w][4] = a5;
			}

		}

	}
}


void NSGAII::order_matrix_CD(float ** M,int F)
{
	int j = 1;
	float a1, a2, a3, a4, a5;
	for (int i = 0; i < fronts.at(F).size(); i++)
	{
			for (int w = 0; w < fronts.at(F).size(); w++)
			{
					if (M[i][j] < M[w][j])
					{
						a1 = M[i][0];
						a2 = M[i][1];
						a3 = M[i][2];
						a4 = M[i][3];
						a5 = M[i][4];

						M[i][0]= M[w][0];
						M[i][1]= M[w][1];
						M[i][2]= M[w][2];
						M[i][3]= M[w][3];
						M[i][4]= M[w][4];

						M[w][0] = a1;
						M[w][1] = a2;
						M[w][2] = a3;
						M[w][3] = a4;
						M[w][4] = a5;
					}
					else if (M[i][j] == M[w][j] || M[i][j + 2] < M[w][j + 2])
					{
						a1 = M[i][0];
						a2 = M[i][1];
						a3 = M[i][2];
						a4 = M[i][3];
						a5 = M[i][4];

						M[i][0] = M[w][0];
						M[i][1] = M[w][1];
						M[i][2] = M[w][2];
						M[i][3] = M[w][3];
						M[i][4] = M[w][4];

						M[w][0] = a1;
						M[w][1] = a2;
						M[w][2] = a3;
						M[w][3] = a4;
						M[w][4] = a5;
					}
				
			}
		
	}
}




void NSGAII::non_dominated_sort(Problem p, Population *p1)
{
	fronts.clear();
	ind_front = new int[num_ind*2]; 
	//vetor onde cada posição possui quantos dominam o individuo i
	bool** dominate = new bool*[p1->get_max_size() * 2]; 
	//vetor que possui em cada posicao um vetor dos individuos em que i dominam
	int pos_dominate = 0;
	vector <int> dom_aux;
	vector<ISolution> sol_front_aux;
	int actual_front = 0;
	int actual_ind_dominate = 0;

	for (int i = 0; i < p1->get_max_size()*2; i++)
	{
		dominate[i] = new bool[p1->get_max_size() * 2];
		for (int j = 0; j < p1->get_max_size() * 2; j++)
		{
			dominate[i][j] = false;
		}
	}
	
	//verificando dominancia entre todos os individuos da populacao
	for (int i = 0; i < p1->get_max_size() * 2; i++)
	{
		ind_front[i]=0;
		for (int j = 0; j < p1->get_max_size() * 2; j++)
		{
			if (i != j)
			{
				if (verify_domination(*p1->get_individual(i), *p1->get_individual(j)))
				{
					dominate[i][j] =true ;
				}//se p1j domina p1i
				else if (verify_domination(*p1->get_individual(j), *p1->get_individual(i)))
				{
					ind_front[i]++;
				}
				
			}
			
			
		}
		if (ind_front[i] == 0)
		{
			if (sol_front_aux.size() == 0)
			{
				
				sol_front_aux.clear();
				sol_front_aux.push_back(*p1->get_individual(i));
				fronts.push_back(sol_front_aux);
			}
			else {
				
				fronts[actual_front].push_back(*p1->get_individual(i));
			}
			
		}
	}
	

	
	
	actual_front++;
	for (int i = 0; i < fronts.size(); i++)
	{
		for (int j = 0; j < fronts.at(i).size(); j++)
		{

			
			sol_front_aux.clear();
			
			for (int k = 0; k < p1->get_max_size() * 2; k++)
			{
				
				
				if (dominate[fronts.at(i).at(j).get_id()][k])
				{
					ind_front[k]--;
					if (ind_front[k] == 0)
					{
						
						if (sol_front_aux.size() == 0)
						{
							
							sol_front_aux.clear();
							sol_front_aux.push_back(*p1->get_individual(k));
							fronts.push_back(sol_front_aux);
						}
						else
						{
							
							fronts[actual_front].push_back(*p1->get_individual(k));
						}
						
						
					}
				}
			}
		}
		
		
		actual_front++;
	}
	

	for (int i = 0; i < p1->get_max_size() * 2; i++)
	{
			delete[] dominate[i];	
	}
	delete[] dominate;
	delete[] ind_front;
}

void NSGAII::selection(Problem p, Population *p1, Population *p2)
{
	FILE *a1, *a2;
	int ind_surv = 0;
	//unificar populacoes
	
	p1->uni_pops(p2);
	
	p1->reset_ident();
	non_dominated_sort(p,p1);
	crowding_distance_sort(p, p1);
	for (int i = 0; i < fronts.size(); i++)
	{
		for (int j = 0; j < fronts.at(i).size(); j++)
		{	
			if (ind_surv < num_ind)
			{
				surv_pop.add_individual(&fronts.at(i).at(j));
				ind_surv++;	
			}
			if (((int)fronts.at(i).size() - (num_ind - ind_surv)) > 0 && num_ind - ind_surv!=0)
			{
				
				crowding_distance_selection(p, i, ind_surv);
				break;
			}
			if (ind_surv >= num_ind)
			{
				break;
			}

		}
		
	}

}

void NSGAII::execute_NSGAII(Problem p,FILE*a1,FILE*a2,FILE*a3)
{
	Tsol aux_BL;


	bool melhorou = false;
	int actual_gen=0;
	actual_num_sons = 0;
	//cout << "GERACAO DA POPULACAO INICIAL" << endl;
	generate_Population(p, num_ind);
	Hipervolume hv;
	while (actual_gen < num_generation)
	{
		actual_gen++;
		int f1=0, f2=0;
		while (actual_num_sons < num_ind)
		{
			//cout << "BT |";
			binary_tournament(p, pop_1, &f1, &f2,num_generation);
			
			if (randomic(0, 1) < prob_crossover)
			{
				//pode haver melhoria aqui
				while (!melhorou)
				{
					cout << "["<<aux_BL.ob1 << "*" << aux_BL.ob2 << "]";
					aux_BL.ob1 = pop_1.get_individual(f1)->get_obj1_cost();
					aux_BL.ob2 = pop_1.get_individual(f1)->get_obj2_freshness();
					movement_intra_route(p, pop_1.get_individual(f1));
					pop_1.get_individual(f1)->set_obj1_cost(p.obj1(pop_1.get_individual(f1)));
					pop_1.get_individual(f1)->set_obj2_freshness(p.obj2(pop_1.get_individual(f1),pop_1.get_individual(f1)->get_dist_travel()));
					
					if (pop_1.get_individual(f1)->get_obj1_cost() < aux_BL.ob1 || pop_1.get_individual(f1)->get_obj2_freshness() > aux_BL.ob2)
					{
						cout <<"["<< pop_1.get_individual(f1)->get_obj1_cost() << "**"
							<< pop_1.get_individual(f1)->get_obj2_freshness() <<"]";
						melhorou = true;
					}
				}
				cout << "melhorou" << endl;
				melhorou = false;




				while (!melhorou)
				{
					cout <<"["<< aux_BL.ob1 << "*" << aux_BL.ob2<<"]";
					aux_BL.ob1 = pop_1.get_individual(f2)->get_obj1_cost();
					aux_BL.ob2 = pop_1.get_individual(f2)->get_obj2_freshness();
					movement_intra_route(p, pop_1.get_individual(f2));
					pop_1.get_individual(f2)->set_obj1_cost(p.obj1(pop_1.get_individual(f2)));
					pop_1.get_individual(f2)->set_obj2_freshness(p.obj2(pop_1.get_individual(f2), pop_1.get_individual(f2)->get_dist_travel()));

					if (pop_1.get_individual(f2)->get_obj1_cost() < aux_BL.ob1 || pop_1.get_individual(f2)->get_obj2_freshness() > aux_BL.ob2)
					{
						cout <<"["<< pop_1.get_individual(f2)->get_obj1_cost() << "**"
							<< pop_1.get_individual(f2)->get_obj2_freshness() <<"]";
						melhorou = true;
					}
				}
				cout << "melhorou" << endl;
				melhorou = false;
				
				//cout << "CROSSOVER" << endl;
				
				//cout << "COOX |";
				crossover_OX(p,*pop_1.get_individual(f1), *pop_1.get_individual(f2), &pop_2);

				
				
				actual_num_sons += 2;
			}
		}
		
		
		if (randomic(0, 1) < prob_mutation)
		{
			//cout << "MT |";
			
			mutation(p, &pop_2);
		}
		//cout << "EVALUATE" << endl;
		//cout << "EV |";
		evaluate_population(p, &pop_1);
		evaluate_population(p, &pop_2);
		


		//cout << "SE |";
		selection(p, &pop_1, &pop_2);
	
			
			/*
			for (int i = 0; i < fronts.at(0).size(); i++)
			{

				float ob1 = fronts.at(0).at(i).get_obj1_cost();
				float ob2 = fronts.at(0).at(i).get_inv_obj2_freshness();

				
				fwrite(&ob1, sizeof(float),1,a1);
				fwrite(&ob2,sizeof(float),1,a1);
			}
			*/

		cout<<"Hipervolume: "<<hv.Smetric_per_front(p, fronts.at(0))<<endl;

		//cout << "GERACAO: " << actual_gen << endl;
		actual_num_sons = 0;
		pop_1.clear_pop();
		
		pop_2.clear_pop();

		pop_1.transfer_pop(&surv_pop);
		
		surv_pop.clear_pop();

		
		// Mostrando todas os hipervolumes de todas as geracoes:
		if (actual_gen == num_generation)
		{
		
			for (int i = 0; i < hv.hv_all_generations.size(); i++)
			{
				cout << hv.hv_all_generations.at(i) <<endl;
			}
		}
		
		
	}
}
