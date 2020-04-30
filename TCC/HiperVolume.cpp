#include "HiperVolume.h"
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <stdlib.h>
#include <string.h>
#include "Methods.h"

Hipervolume::Hipervolume()
{
	Ponto_ref.ob2 = 1.0;
	Ponto_ref.ob1 = 6000;
}

Hipervolume::Hipervolume(float, float)
{
	Ponto_ref.ob2 = 2.0;
	Ponto_ref.ob1 = 6000;
}



float Hipervolume::Calcula_Area(vector<Tsol> *sols)
{
	float Area_PR;
	float A1 = 0;
	float A2 = 0;
	float A_inter = 0;
	float A_total = 0;

	
	sort(sols->begin(), sols->end(), compara);
	
	

	//maneira correta
	Ponto_ref.ob2 = 2.0;
	Ponto_ref.ob1 = 6000;

	Area_PR = Ponto_ref.ob1*Ponto_ref.ob2;

	vector<Tsol> inter;

	for (int i = 1; i < sols->size(); i++)
	{
		Tsol in;
		in.ob1 = sols->at(i).ob1;
		in.ob2 = sols->at(i - 1).ob2;
		inter.push_back(in);
	}
	for (int i = 0; i < sols->size(); i++)
	{
		A_total += (Ponto_ref.ob1 - sols->at(i).ob1) * (Ponto_ref.ob2 - sols->at(i).ob2);
	}
	for (int i = 0; i < inter.size(); i++)
	{
		A_total -= (Ponto_ref.ob1 - inter.at(i).ob1)*(Ponto_ref.ob2 - inter.at(i).ob2);
	}


	return A_total;
}

void Hipervolume::VerificaRepeticao(vector<Tsol>* sols)
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

void Hipervolume::VerificaDominancia(vector<Tsol> *sols)
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

float Hipervolume::Smetric_per_front(Problem p, vector<ISolution> front,FILE* a1,int num_gen,int actual_gen)
{

	vector<Tsol> sols_R;

	float ob1 = 0;
	float ob2 = 0;
	Tsol sol_aux;
	int pos = 0;
	float Area_referencia, Area_instancia;

	
	for (int i = 0; i < front.size(); i++)
	{
		sol_aux.ob1 = front.at(i).get_obj1_cost();
		
		sol_aux.ob2 = front.at(i).get_obj2_freshness();
		sols_R.push_back(sol_aux);
	}
	
	VerificaRepeticao(&sols_R);

	VerificaDominancia(&sols_R);

	Area_referencia = Calcula_Area(&sols_R);


	
	hv_all_generations.push_back(Area_referencia);
	

	if (actual_gen == num_gen)
	{
	
		int tam = sols_R.size();

		fwrite(&tam, sizeof(int), 1, a1);
	
		for (int i = 0; i < sols_R.size(); i++)
		{
			
			float ob1 = sols_R.at(i).ob1;
			float ob2 = sols_R.at(i).ob2;
			
			fwrite(&ob1, sizeof(float), 1, a1);
			fwrite(&ob2, sizeof(float), 1, a1);
		}
	}
		

	return Area_referencia;
}


float Hipervolume::Area_unique_Solution(Tsol s)
{
	float Area_S = (Ponto_ref.ob1 - s.ob1)*(Ponto_ref.ob2 - s.ob2);

	return Area_S;
}