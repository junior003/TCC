#include<iostream>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iomanip>
#include "SMSEMOA.h"
#include "Methods.h"

using namespace std;

void SMSemoa::VerificaRepeticao(vector<Tsol>* sols)
{
	vector<Tsol> unique;
	vector<int> used;
	bool is_unique = true;
	for (int i = 0; i < sols->size(); i++)
	{
		is_unique = true;
		for (int j = i; j < sols->size(); j++)
		{
			if (i != j)
			{
				if (sols->at(i).ob1 == sols->at(j).ob1 && sols->at(i).ob2 == sols->at(j).ob2)
				{
					is_unique = false;
					sols->erase(sols->begin() + j);
					unique.push_back(sols->at(i));
					break;
				}
			}

		}
		if (is_unique)
		{
			unique.push_back(sols->at(i));
		}


	}
	sols->clear();
	sols->assign(unique.begin(), unique.end());

	unique.clear();
	for (int i = 0; i < sols->size(); i++)
	{
		is_unique = true;
		for (int j = i; j < sols->size(); j++)
		{
			if (i != j)
			{
				if (sols->at(i).ob1 == sols->at(j).ob1 && sols->at(i).ob2 == sols->at(j).ob2)
				{
					is_unique = false;
					sols->erase(sols->begin() + j);
					unique.push_back(sols->at(i));
					break;
				}
			}

		}
		if (is_unique)
		{
			unique.push_back(sols->at(i));
		}


	}
	sols->clear();
	sols->assign(unique.begin(), unique.end());
}

void SMSemoa::VerificaDominancia(vector<Tsol> *sols)
{
	int i, j;
	vector<Tsol> Vdominadas;
	bool dominada = false;
	for (i = 0; i < sols->size(); i++)
	{
		for (j = 0; j < sols->size(); j++)
		{

			if (sols->at(i).ob1 < sols->at(j).ob1 && sols->at(i).ob2 < sols->at(j).ob2)
			{
				dominada = true;


				break;
			}
			else {

				dominada = false;
			}

		}
		if (dominada)
		{
			dominada = false;
			sols->erase(sols->begin() + j);
			i = -1;
		}
	}

}




bool compara(Tsol a, Tsol b)
{
	return (a.ob1 < b.ob1);
}
bool compara_IS(ISolution a, ISolution b)
{
	return (a.get_obj1_cost() < b.get_obj1_cost());
}



float SMSemoa::Calcula_Area(vector<Tsol> *sols)
{
	float Area_PR;
	float A1 = 0;
	float A2 = 0;
	float A_inter = 0;
	float A_total = 0;

	sort(sols->begin(), sols->end(), compara);
	vector<Tsol> inter;
	Area_PR = Ponto_ref.ob1*Ponto_ref.ob2;
	for (int i = 1; i < sols->size(); i++)
	{
		Tsol in;
		in.ob1 = sols->at(i).ob1;
		in.ob2 = sols->at(i - 1).ob2;
		inter.push_back(in);
	}
	for (int i = 0; i < sols->size(); i++)
	{
		A_total += sols->at(i).ob1*sols->at(i).ob2;
	}
	for (int i = 0; i < inter.size(); i++)
	{
		A_total -= inter.at(i).ob1*inter.at(i).ob2;
	}



	A_total = Area_PR - A_total;

	return A_total;
}

float SMSemoa::Calcula_Area_R(vector<Tsol> *sols)
{
	float Area_PR;
	float A1 = 0;
	float A2 = 0;
	float A_inter = 0;
	float A_total = 0;
	sort(sols->begin(), sols->end(), compara);
	Ponto_ref.ob2 = sols->at(0).ob2;
	Ponto_ref.ob1 = sols->at(sols->size() - 1).ob1;
	//cout << "ponto de referencia:" << Ponto_ref.ob1 << " X " << Ponto_ref.ob2 << endl;
	vector<Tsol> inter;
	Area_PR = Ponto_ref.ob1*Ponto_ref.ob2;
	for (int i = 1; i < sols->size(); i++)
	{
		Tsol in;
		in.ob1 = sols->at(i).ob1;
		in.ob2 = sols->at(i - 1).ob2;
		inter.push_back(in);
		//cout << in.ob1 << "|" << in.ob2 << endl;
	}
	
	for (int i = 0; i < sols->size(); i++)
	{
		A_total += sols->at(i).ob1*sols->at(i).ob2;
		//cout << " ** "<<A_total << endl;
	}
	for (int i = 0; i < inter.size(); i++)
	{
		A_total -= inter.at(i).ob1*inter.at(i).ob2;
		//cout << " ** " << A_total << endl;
	}
	//cout << "A_total = Area_PR - A_total" << endl;
	//cout << A_total << "=" << Area_PR << "-" << A_total << endl;
	A_total = Area_PR - A_total;
	
	return A_total;
}


void SMSemoa::LerInstancia()
{
	FILE* arq;

	if ((arq = fopen("INS.bin", "rb")) == NULL)
	{
		cout << "erro ao abrir o arquivo";
		return;
	}
	Tsol sol_aux;
	float ob1, ob2;
	while (!feof(arq))
	{
		fread(&ob1, sizeof(float), 1, arq);
		fread(&ob2, sizeof(float), 1, arq);
		sol_aux.ob1 = ob1;
		sol_aux.ob2 = ob2;
		instancia.push_back(sol_aux);
		//cout<<"Solucao I(";
		//cout<<ob1<<" , ";
		//cout<<ob2<<")"<<endl;
	}
	fclose(arq);
}

void SMSemoa::hipervolume(float ar)
{

	FILE* arq;
	if (ar < 0) { ar = ar * (-1); }

	cout << "Area do conjunto da instancia: " << ar << endl;
	if ((arq = fopen("Smetric.bin", "ab")) == NULL)
	{
		cout << "erro ao abrir o arquivo";
		return;
	}
	if (ar > 0)
	{
		fwrite(&ar, sizeof(float), 1, arq);
	}

	fclose(arq);
}



float SMSemoa::Smetric(Problem p){
	FILE* arq;

	if ((arq = fopen("FRONT.bin", "rb")) == NULL)
	{
		cout << "erro ao abrir o arquivo";
		return 0;
	}
	vector<Tsol> sols_R;

	float ob1 = 0;
	float ob2 = 0;
	Tsol sol_aux;
	int pos = 0;
	float Area_referencia, Area_instancia;
	while (!feof(arq))
	{
		fread(&ob1, sizeof(float), 1, arq);
		fread(&ob2, sizeof(float), 1, arq);
		sol_aux.ob1 = ob1;
		sol_aux.ob2 = ob2;
		
		sols_R.push_back(sol_aux);

	}
	remove("FRONT.bin");

	VerificaRepeticao(&sols_R);

	VerificaDominancia(&sols_R);

	Area_referencia = Calcula_Area_R(&sols_R);

	FILE* arq2;

	if ((arq2 = fopen("HV.bin", "wb")) == NULL)
	{
		cout << "erro ao abrir o arquivo";
		return 0;
	}
	int tam = sols_R.size();
	//fwrite(&Area_referencia, sizeof(float), 1, arq2);
	//fwrite(&tam, sizeof(int), 1, arq2);
	//cout << "Area: " << Area_referencia<<endl;
	if (hv_all_generations.size() == 0)
	{
		hv_all_generations.push_back(Area_referencia);
	}
	else {
		if (Area_referencia < hv_all_generations.at(hv_all_generations.size() - 1))
		{
			Area_referencia = hv_all_generations.at(hv_all_generations.size() - 1);
			hv_all_generations.push_back(Area_referencia);
		}
		else
		{
			hv_all_generations.push_back(Area_referencia);
		}
	
	}
	
	//hv_all_generations.push_back(Area_referencia);
	
	//cout << hv_all_generations.at(hv_all_generations.size() - 1) << endl;
	//cout << "Num Solucoes: " << tam;
	for (int i = 0; i < sols_R.size(); i++)
	{
		//fwrite(&sols_R.at(i).ob1, sizeof(float), 1, arq2);
		//fwrite(&sols_R.at(i).ob2, sizeof(float), 1, arq2);
		//cout << endl;
		//cout << "sols_R.at(i).ob1: " << sols_R.at(i).ob1 << endl;
		//cout << "sols_R.at(i).o21: " << sols_R.at(i).ob2 << endl;
	}

	return Area_referencia;
}

//recebe o front 0 e retorna o ponto de referencia para os demais fronts.
Tsol SMSemoa::Create_ref_point(vector<ISolution> front)
{
	sort(front.begin(), front.end(), compara_IS);
	Ponto_ref.ob2 = front.at(0).get_obj2_freshness();
	Ponto_ref.ob1 = front.at(front.size() - 1).get_obj1_cost();
}

//trocar o tipo do vector para Isolution
float SMSemoa::Smetric_per_front(Problem p, vector<ISolution> front)
{

	vector<Tsol> sols_R;

	float ob1 = 0;
	float ob2 = 0;
	Tsol sol_aux;
	int pos = 0;
	float Area_referencia, Area_instancia;

	cout << "Front:" << endl << endl;
	for (int i = 0; i < front.size(); i++)
	{
		sol_aux.ob1 = front.at(i).get_obj1_cost();
		sol_aux.ob2 = front.at(i).get_obj2_freshness();
		cout << sol_aux.ob1 << endl;
		cout << sol_aux.ob2 << endl;
		sols_R.push_back(sol_aux);
	}

	VerificaRepeticao(&sols_R);

	VerificaDominancia(&sols_R);

	Area_referencia = Calcula_Area_R(&sols_R);

	
	//cout << "Area: " << Area_referencia<<endl;
	//hv_all_generations.push_back(Area_referencia);
	//cout << hv_all_generations.at(hv_all_generations.size() - 1) << endl;
	//cout << "Num Solucoes: " << tam;

	return Area_referencia;
}

float SMSemoa::Area_unique_Solution(Problem p, Tsol s)
{
	float Area_S = (Ponto_ref.ob1-s.ob1)*(Ponto_ref.ob2-s.ob2);

	return Area_S;
}

int SMSemoa::Minor_HV_per_front(Problem p, vector<ISolution> front)
{
	Tsol aux;
	float minor=999999,aux_area;
	int pos_minor;
	for (int i = 0; i < front.size(); i++)
	{
		aux.ob1 = front.at(i).get_obj1_cost();
		aux.ob2 = front.at(i).get_obj2_freshness();
		aux_area = Area_unique_Solution(p, aux);
		if (minor < aux_area)
		{
			minor = aux_area;
			pos_minor = i;
		}
		
	}
	return pos_minor;
}

void SMSemoa::remove_minor_HV(Problem p,int id_front)
{
	smetric_actual_front = Smetric_per_front(p, fronts.at(id_front));
	int minor_HV=-1;
	minor_HV = Minor_HV_per_front(p, fronts.at(id_front));
	fronts.at(id_front).erase(fronts.at(id_front).begin() + minor_HV);
}
void SMSemoa::add_new_ind_per_front(Problem p, int id_front)
{
	float new_smetric_front=0;
	ISolution *S;
	cout << "Smetric original:" << smetric_actual_front;
	while (new_smetric_front <= smetric_actual_front)
	{
		fronts.at(id_front).push_back(generate_individual_S(&p));
		new_smetric_front = Smetric_per_front(p, fronts.at(id_front));
		cout << "new: " << new_smetric_front<<endl;
	}
}

ISolution SMSemoa::generate_individual_S(Problem *p)
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
	int minor_dst = 0;
	vector<int> not_used_IDs;
	
	KmeansST kmeans_ST(*p, p->get_num_vehicle(), 0.5, 0.5, 1, 1.5, 2);
	kmeans_ST.Calculate_ALL_Spatial_Distance(*p);
	kmeans_ST.Calculate_ALL_Temporal_Distance(*p);
	kmeans_ST.Calculate_ALL_Spatial_Temporal_Distance(*p);
	//cout << "criando ind | ";
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
		//50% de chance de ser aleatório e 50% de chance de ser guloso

	//metodo guloso
		if (randomic(0, 1) < 0.0 && greedy)
		{

			not_used_IDs.clear();

			for (int j = 1; j < p->get_num_clients() + 1; j++)
			{
				not_used_IDs.push_back(j);


			}

			not_used_IDs = calculate_diference(&not_used_IDs, &used_IDs);

			minor_dst = find_minor_DST(*p, temp_sol[i][temp_sol[i].size() - 1], &not_used_IDs, &kmeans_ST);

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

			//cout << "não pode entrar aqui" << endl;
		}
		else //metodo aleatorio
		{
		


			do
			{
				int new_ID = (rand() % p->get_num_clients() + 1);//cria valores aleatorios


				not_used_IDs.clear();
				//cout << "Todos:" << endl;
				for (int j = 1; j < p->get_num_clients() + 1; j++)
				{
					not_used_IDs.push_back(j);
					//cout << not_used_IDs.at(j - 1) << ",";

				}

				not_used_IDs = calculate_diference(&not_used_IDs, &used_IDs);
				//cout << endl << "not_used:" << endl;
				//for (int j = 0; j < not_used_IDs.size(); j++)
				//{
				//	cout << not_used_IDs[j] << ",";
				//}
				//cout << endl;

				if (used_IDs.size() == p->get_num_clients() - 1 || not_used_IDs.size() == 0)
				{
					//	cout << "encheu" << endl;
					valid = true;
					greedy = false;
					break;
				}
				if (find(used_IDs.begin(), used_IDs.end(), new_ID) == used_IDs.end() && new_ID != 0)
				{
					//verificar capacidade
					//cout << "eh inedito" << endl;
					if (acum_demand + p->get_client(new_ID)->get_demand() > p->get_capacity())
					{
						//	cout << "demanda notok "<< endl;
						cont_choosen_random_cl++;
						//procura por até 10 clientes para adicionar, caso nao consiga achar
						//um válido passa para o próximo veículo.
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

	//cout << "saiu e ficou assim" << endl;

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
					//procura por até três clientes para adicionar, caso nao consiga achar
					//um válido passa para o próximo veículo.
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
	S = new ISolution(p->get_num_clients(), actual_vehicle + 1);

	for (int i = 0; i < actual_vehicle + 1; i++)
	{

		S->set_route_size(i, temp_sol[i].size());
		temp_route_aux = new int[temp_sol[i].size()];

		for (int k = 0; k < temp_sol[i].size(); k++)
		{

			temp_route_aux[k] = temp_sol[i][k];

		}

		S->set_route(i, temp_route_aux, temp_sol[i].size());
		delete[] temp_route_aux;
	}
	//printaSolucao(*p,*S);
	//system("PAUSE");
	//cout << p->obj1(S) << endl;
	//cout << p->obj2(S, S->get_dist_travel()) << endl;
	if (!p->constraint_vehicle_capacity(*S))
	{
		cout << "Capacidade excedida" << endl;
		system("PAUSE");
	}

	S->set_obj1_cost(p->obj1(S));
	S->set_obj2_freshness(p->obj2(S, S->get_dist_travel()));

	return *S;
}

int SMSemoa::find_minor_DST(Problem p, int last_added, vector<int>* not_used_IDs, KmeansST * kmeans_ST)
{

		float minor = 10;
		float m1 = 0, m2 = 0;
		int pos_minor = 0;


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

//METODOS DO NSGA
SMSemoa::SMSemoa(int num_i, int num_gen, float prob_cross, float prob_mut)
{
	num_ind = num_i;
	num_generation = num_gen;
	prob_crossover = prob_cross;
	prob_mutation = prob_mut;
	pop_1.set_max_size(num_i);
	pop_2.set_max_size(num_i);
	surv_pop.set_max_size(num_i);
}

void SMSemoa::generate_Population(Problem p, int nind)
{
	bool valid = false;
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

void SMSemoa::evaluate_population(Problem p, Population *pop)
{

	for (int i = 0; i < pop->get_max_size(); i++)
	{
		pop->get_individual(i)->set_obj1_cost(p.obj1(pop->get_individual(i)));
		pop->get_individual(i)->set_obj2_freshness(p.obj2(pop->get_individual(i),
			pop->get_individual(i)->get_dist_travel()));
	
	}

}
//metodo que verifica se a primeira solucao passada por parametro domina a segunda
bool SMSemoa::verify_domination(ISolution s1, ISolution s2)
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

void SMSemoa::binary_tournament(Problem p, Population pop, int * father1, int * father2, int ngen)
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
	obj2_f2 = pop.get_individual(f2)->get_obj2_freshness();

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
	} while (f1 == f2 || f1 == *father1 || f2 == *father1);
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

void SMSemoa::crossover_OX(Problem p, ISolution father1, ISolution father2, Population * pop2)
{
	int pos_cut1 = 0, pos_cut2 = 0;
	int *Seq_f1 = new int[p.get_num_clients()];
	int *Seq_f2 = new int[p.get_num_clients()];
	int *Seq_s1 = new int[p.get_num_clients()];
	int *Seq_s2 = new int[p.get_num_clients()];
	vector<int> alocated_cl_s1, alocated_cl_s2;

	//converter os pais para um vetor
	int added = 0, added_s1 = 0, added_s2 = 0;
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
	for (int i = 0; i < father2.get_num_vehicles_in_S(); i++) {

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

	for (int i = pos_cut1, j = 0; i < pos_cut2 + 1; i++, j++)
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



	for (int i = 0; i < pos_cut2 + 1; i++)
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
	for (int i = 0; i < pos_cut2 + 1; i++)
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
	cont_route_sizes = NULL;
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

void SMSemoa::mutation(Problem p, Population *pop)
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

void SMSemoa::crowding_distance_sort(Problem p, Population *pop)
{

	for (int i = 0; i < fronts.size(); i++)
	{

		float major_obj1 = 0, major_obj2 = 0;
		float minor_obj1 = 10000, minor_obj2 = 10000;
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

		order_matrix_CD(dist_matrix_per_front, i);



		dist_matrix_per_front[0][2] = 10000;
		dist_matrix_per_front[0][4] = 10000;
		dist_matrix_per_front[fronts.at(i).size() - 1][2] = 10000;
		dist_matrix_per_front[fronts.at(i).size() - 1][4] = 10000;

		for (int j = 1; j < fronts.at(i).size() - 1; j++)
		{
			dist_matrix_per_front[j][2] = (dist_matrix_per_front[j - 1][1] - dist_matrix_per_front[j + 1][1])
				/ (major_obj1 - minor_obj1);


			dist_matrix_per_front[j][4] = (dist_matrix_per_front[j - 1][3] - dist_matrix_per_front[j + 1][3]) / (major_obj2 - minor_obj2);


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
						fronts.at(i).at(j).set_CD_obj1(dist_matrix_per_front[k][2]);
						fronts.at(i).at(j).set_CD_obj2(dist_matrix_per_front[k][4]);
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

void SMSemoa::crowding_distance_selection(Problem p, int F, int ind_surv)
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


	for (int i = 0; i < fronts.at(F).size(); i++)
	{
		for (int j = 0; j < fronts.at(F).size(); j++)
		{
			if (fronts.at(F).at(j).get_id() == dist_matrix_per_front[i][0] && ind_surv < num_ind)
			{
				surv_pop.add_individual(&fronts.at(F).at(j));
				ind_surv++;
			}
		}
	}

}


void SMSemoa::order_matrix_CD_selection(float ** M, int F)
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


void SMSemoa::order_matrix_CD(float ** M, int F)
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


void SMSemoa::non_dominated_sort(Problem p, Population *p1)
{
	fronts.clear();
	ind_front = new int[num_ind * 2];
	//vetor onde cada posição possui quantos dominam o individuo i
	bool** dominate = new bool*[p1->get_max_size() * 2];
	//vetor que possui em cada posicao um vetor dos individuos em que i dominam
	int pos_dominate = 0;
	vector <int> dom_aux;
	vector<ISolution> sol_front_aux;
	int actual_front = 0;
	int actual_ind_dominate = 0;

	for (int i = 0; i < p1->get_max_size() * 2; i++)
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
		ind_front[i] = 0;
		for (int j = 0; j < p1->get_max_size() * 2; j++)
		{
			if (i != j)
			{
				if (verify_domination(*p1->get_individual(i), *p1->get_individual(j)))
				{
					dominate[i][j] = true;
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

void SMSemoa::selection(Problem p, Population *p1, Population *p2)
{
	FILE *a1, *a2;
	int ind_surv = 0;
	//unificar populacoes

	p1->uni_pops(p2);

	p1->reset_ident();

	non_dominated_sort(p, p1);

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
			if (((int)fronts.at(i).size() - (num_ind - ind_surv)) > 0 && num_ind - ind_surv != 0)
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



void SMSemoa::execute_SMSEMOA(Problem p,FILE* a1)
{
	int actual_gen = 0;
	actual_num_sons = 0;
	generate_Population(p, num_ind);

	while (actual_gen <= num_generation)
	{
		int f1 = 0, f2 = 0;
		while (actual_num_sons < num_ind)
		{
			binary_tournament(p, pop_1, &f1, &f2, num_generation);

			if (randomic(0, 1) < prob_crossover)
			{
				crossover_OX(p, *pop_1.get_individual(f1), *pop_1.get_individual(f2), &pop_2);
				actual_num_sons += 2;
			}
		}

		if (randomic(0, 1) < prob_mutation)
		{
			mutation(p, &pop_2);
		}
	
		evaluate_population(p, &pop_1);
	
		evaluate_population(p, &pop_2);
	
		selection(p, &pop_1, &pop_2);
	
		for (int i = 0; fronts.size(); i++)
		{
		
			
		   remove_minor_HV(p, i);
		   cout << "removeu" << endl;
			add_new_ind_per_front(p, i);
			cout << "adicionou" << endl;
		}
		
		for (int i = 0; i < fronts.at(0).size(); i++)
		{

			float ob1 = fronts.at(0).at(i).get_obj1_cost();
			float ob2 = fronts.at(0).at(i).get_inv_obj2_freshness();
			fwrite(&ob1, sizeof(float), 1, a1);
			fwrite(&ob2, sizeof(float), 1, a1);
		}
		Smetric(p);
		actual_num_sons = 0;
		pop_1.clear_pop();

		pop_2.clear_pop();


		pop_1.transfer_pop(&surv_pop);

		surv_pop.clear_pop();

		actual_gen++;
	}
}
