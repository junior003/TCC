#include "NSGAII.h"
#include "Methods.h"
#include <vector>
#include <limits.h>
#include "ISolution.h"
#include "KmeansS.h"
#include "KmeansST.h"
#include "HiperVolume.h"
#include "VNS.h"

VNS::VNS(int iM)
{
	iter_max = iM;
}

void VNS::execute_VNS(Problem p, ISolution *s)
{
	ISolution s_star;
	ISolution s_a = *s;
	ISolution s_super;
	Tsol sol_a,sol_d;
	bool melhorou = false;
	int k,iter = 0;

	int cont = 0;

	//apagar depois
	iter_max = 10;

	int *routesizes = new int[s_a.get_num_vehicles_in_S()];
	vector<vector<int>> path_backup;
	
	path_backup = s_a.sol_path;

	for (int k = 0; k < s_a.get_num_vehicles_in_S(); k++)
	{
		routesizes[k] = s_a.get_route_size(k);
	}

	printaSolucao(p,s_a);
	s_super = *s;
	s_star = *s;
	
	while (iter < iter_max)
	{
		k = 1;
		iter++;
		cout << "1" << endl;
		//printaSolucao(p, s_super);
		//printaSolucao(p, s_super);
		
		cout << "2" << endl;
		//system("PAUSE");
		while (k <= 2)//numero de vizinhanças
		{
			switch (k)
			{
			case 1:
				sol_a.ob1 = s->get_obj1_cost();
				sol_a.ob2 = s->get_obj2_freshness();
				cout << "3" << endl;
				//printaSolucao(p, s);
				movement_inter_route(p, s);
				 //movement_intra_route(p, s);
				//printaSolucao(p, s);
				//cout << "4" << endl;
				//system("PAUSE");

				s->set_obj1_cost(p.obj1(s));
				s->set_obj2_freshness(p.obj2(s, s->get_dist_travel()));
				sol_d.ob1 = s->get_obj1_cost();
				sol_d.ob2 = s->get_obj2_freshness();

				break;
			case 2:
				
				sol_a.ob1 = s->get_obj1_cost();
				sol_a.ob2 = s->get_obj2_freshness();
				cout << "5" << endl;
				//printaSolucao(p, s);
				movement_inter_route(p, s);
				//movement_intra_route(p, s);
				//printaSolucao(p, s);
				//cout << "6" << endl;
				//system("PAUSE");
				s->set_obj1_cost(p.obj1(s));
				s->set_obj2_freshness(p.obj2(s, s->get_dist_travel()));
				sol_d.ob1 = s->get_obj1_cost();
				sol_d.ob2 = s->get_obj2_freshness();
				
				break;
			}
			//cout << "7" << endl;
			//printaSolucao(p, s_super);
			//if (sol_d.ob1<sol_a.ob1 && sol_d.ob2>sol_a.ob2)
			//{
			//	s_star = *s;
			//}
			//printaSolucao(p, s_super);
			//cout << "8" << endl;
			//system("PAUSE");
			//printaSolucao(p, s);
			//s_star = VND(p, &s); //método de busca local
			//printaSolucao(p, s);
			//cout << "9" << endl;
			//system("PAUSE");
			//if (s_star.get_obj1_cost() < s_super.get_obj1_cost() && s_star.get_obj2_freshness() > s_super.get_obj2_freshness())
			//{
			//	s_super = s_star;
			//}
			//printaSolucao(p, s_super);
			//cout << "10" << endl;
			//system("PAUSE");
			if (s_star.get_obj1_cost() > s->get_obj1_cost() && s_star.get_obj2_freshness()< s->get_obj2_freshness())
			{
				s_star = *s;
				//*s = s_star;
				k = 1;
			}
			else {
				
				k++;
			}
			//printaSolucao(p, s);
			//cout << "11" << endl;
			//system("PAUSE");
		}
		

	}
	
	/*
	
	if (s_star.get_obj1_cost() < s_super.get_obj1_cost() && s_star.get_obj2_freshness() > s_super.get_obj2_freshness())
	{
		s_super = s_star;
	}
	
	*/

	if (s_a.get_obj1_cost() > s_star.get_obj1_cost() && s_a.get_obj2_freshness() < s_star.get_obj2_freshness())
	{
		melhorou = true;
	}

	cout << "Melhorou? " << melhorou;
	cout << "Antes:" << s_a.get_obj1_cost() << "|" << s_a.get_obj2_freshness() << endl;
	cout << " Depois:" << s_star.get_obj1_cost() <<"|" << s_star.get_obj2_freshness() << endl;
	for (int k = 0; k < s_a.get_num_vehicles_in_S(); k++)
	{
		s_a.set_route_size(k, routesizes[k]);
	}
	
	s_a.sol_path = path_backup;
	printaSolucao(p, s_a);

	
	if (!melhorou)
	{
		*s = s_a;
	}
	else
	{
		*s = s_star;
	}

}

ISolution VNS::VND(Problem p, ISolution *s)
{
	ISolution s_star;
	Tsol sol_a, sol_d;
	bool continuar=false;
	s_star = *s;
	while (!continuar)
	{
		
		sol_a.ob1 = s->get_obj1_cost();
		sol_a.ob2 = s->get_obj2_freshness();

		movement_inter_route(p, s);
	
		s->set_obj1_cost(p.obj1(s));
		s->set_obj2_freshness(p.obj2(s, s->get_dist_travel()));
		sol_d.ob1 = s->get_obj1_cost();
		sol_d.ob2 = s->get_obj2_freshness();
		//cout << "(D)(" << sol_a.ob1 << "|" << sol_d.ob1 << ")";
		//cout << "[" << sol_a.ob2 << "|" << sol_d.ob2 << "]" << endl;

		if (sol_d.ob1<sol_a.ob1 && sol_d.ob2>sol_a.ob2)
		{
		
			continuar = false;
			s_star = *s;
		}
		else
		{
			//cout << "piorou" << endl;
			continuar = true;
		}
		
	}

	//armazenar a melhor e retornar ela pois vai haver uma alteracao a mais
	return s_star;
}
