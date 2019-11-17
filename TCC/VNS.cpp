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

	
	
	while (iter < iter_max)
	{
		k = 1;
		iter++;
		//cout << "1" << endl;
		
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
			
			VND(p, s); //método de busca local
			if (sol_d.ob1<sol_a.ob1 && sol_d.ob2>sol_a.ob2)
			{
				s_star_path.clear();
				s_star_path = s->sol_path;
				
				k = 1;
			}else{
				k++;
			}


		}
		

	}
	

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
	
	//enquanto nao for pra continuar
	while (!continuar)
	{
		sol_a.ob1 = s->get_obj1_cost();
		sol_a.ob2 = s->get_obj2_freshness();


		

		//movement_inter_route(p, s);

		movement_intra_route(p, s);

		//Problema com a vizinhança de realocação, funciona na geracao da populacao
		//realoc_inter_route(p, s); 


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





