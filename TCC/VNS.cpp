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



	int *routesizes = new int[s->get_num_vehicles_in_S()];
	vector<vector<int>> s_path;
	s_path = s->sol_path;

	for (int k = 0; k < s->get_num_vehicles_in_S(); k++)
	{
		routesizes[k] = s->get_route_size(k);
	}

	vector<vector<int>> s_star_path;



	float obj1a = s->get_obj1_cost();
	float obj2a = s->get_obj2_freshness();
	float obj1d,obj2d;

	ISolution s_star;

	//ISolution s_a = *s;
	//ISolution s_super;
	Tsol sol_a,sol_d;
	bool melhorou = false;
	int k,iter = 0;

	int cont = 0;

	//apagar depois


	


	//printaSolucao(p,s_a);
	//s_super = *s;
	//s_star = *s;
	//cout << "Antes:" << s->get_obj1_cost() << "|" << s->get_obj2_freshness() << endl;
	while (iter < iter_max)
	{
		k = 1;
		iter++;
		//cout << "1" << endl;
		//printaSolucao(p, s_super);
		//printaSolucao(p, s_super);
		
		//cout << "2" << endl;
		//system("PAUSE");
		while (k <= 2)//numero de vizinhanças
		{
			switch (k)
			{
			case 1:
				sol_a.ob1 = s->get_obj1_cost();
				sol_a.ob2 = s->get_obj2_freshness();
				//cout << "3" << endl;
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
				//cout << "5" << endl;
				//printaSolucao(p, s);
				//movement_inter_route(p, s);
				movement_intra_route(p, s);
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
			
			//printaSolucao(p, s_super);
			//cout << "8" << endl;
			//system("PAUSE");
			//printaSolucao(p, s);
			VND(p, s); //método de busca local
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


			if (sol_d.ob1<sol_a.ob1 && sol_d.ob2>sol_a.ob2)
			{
				s_star_path.clear();
				s_star_path = s->sol_path;
				//s_star = *s;
				k = 1;
			}else{
				k++;
			}


			/*
			if (s_star.get_obj1_cost() > s->get_obj1_cost() && s_star.get_obj2_freshness()< s->get_obj2_freshness())
			{
				s_star = *s;
				//*s = s_star;
				k = 1;
			}
			else {
				
				k++;
			}
			*/
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

	s->sol_path = s_star_path;

	
	s->set_obj1_cost(p.obj1(s));
	s->set_obj2_freshness(p.obj2(s, s->get_dist_travel()));

	obj1d = s->get_obj1_cost();
	obj2d = s->get_obj2_freshness();

	//cout << "(" << obj1a << "|" << obj1d << ")(" << obj2a << "||" << obj2d << ")" << endl;

	if (obj1d>obj1a && obj2d<obj2a)
	{
		
		s->sol_path = s_path;
		s->set_obj1_cost(p.obj1(s));
		s->set_obj2_freshness(p.obj2(s, s->get_dist_travel()));
	}


	//cout << "Depois:" << s->get_obj1_cost() << "|" << s->get_obj2_freshness() << endl;
	/*
	cout << "Melhorou? " << melhorou;
	
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
	*/

}

void VNS::VND(Problem p, ISolution *s)
{


	int *routesizes = new int[s->get_num_vehicles_in_S()];
	vector<vector<int>> s_path;
	s_path = s->sol_path;

	for (int k = 0; k < s->get_num_vehicles_in_S(); k++)
	{
		routesizes[k] = s->get_route_size(k);
	}

	vector<vector<int>> s_star_path;



	float obj1a = s->get_obj1_cost();
	float obj2a = s->get_obj2_freshness();
	float obj1d, obj2d;



	ISolution s_star;
	Tsol sol_a, sol_d;
	bool continuar=false;
	
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
			s_star_path.clear();
			s_star_path = s->sol_path;
			continuar = false;
			//s_star = *s;
		}
		else
		{
			//cout << "piorou" << endl;
			continuar = true;
		}
		
	}

	
	if (s_star_path.size() > 0)
	{
		s->sol_path = s_star_path;
	}
	
	

	s->set_obj1_cost(p.obj1(s));
	s->set_obj2_freshness(p.obj2(s, s->get_dist_travel()));

	obj1d = s->get_obj1_cost();
	obj2d = s->get_obj2_freshness();

	//cout << "(" << obj1a << "|" << obj1d << ")(" << obj2a << "||" << obj2d << ")" << endl;

	if (obj1d > obj1a && obj2d < obj2a)
	{

		s->sol_path = s_path;
		s->set_obj1_cost(p.obj1(s));
		s->set_obj2_freshness(p.obj2(s, s->get_dist_travel()));
	}

	
	//armazenar a melhor e retornar ela pois vai haver uma alteracao a mais

}
