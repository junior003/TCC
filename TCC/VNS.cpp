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

ISolution VNS::execute_VNS(Problem p, ISolution s)
{
	ISolution s_star(100,30);
	Tsol sol_a,sol_d;
	bool melhorou;
	int k,iter = 0;

	//eh necessario sobreescrever o  operador igual pois preciso armazenar a melhor solucao
	while (iter < iter_max)
	{
		k = 1;
		iter++;

		while (k <= 2)//numero de vizinhanças
		{
			switch (k)
			{
			case 1:
				sol_a.ob1 = s.get_obj1_cost();
				sol_a.ob2 = s.get_obj2_freshness();
				
				movement_inter_route(p, &s);
				
				s.set_obj1_cost(p.obj1(&s));
				s.set_obj2_freshness(p.obj2(&s, s.get_dist_travel()));
				sol_d.ob1 = s.get_obj1_cost();
				sol_d.ob2 = s.get_obj2_freshness();
				
				cout << "(1)(" << sol_a.ob1 << "|"<<sol_d.ob1 << ")";
				cout << "[" << sol_a.ob2 << "|" << sol_d.ob2 << "]" << endl;
				break;
			case 2:
				sol_a.ob1 = s.get_obj1_cost();
				sol_a.ob2 = s.get_obj2_freshness();
				
				movement_intra_route(p, &s);
				
				s.set_obj1_cost(p.obj1(&s));
				s.set_obj2_freshness(p.obj2(&s, s.get_dist_travel()));
				sol_d.ob1 = s.get_obj1_cost();
				sol_d.ob2 = s.get_obj2_freshness();
				cout << "(2)(" << sol_a.ob1 << "|" << sol_d.ob1 << ")";
				cout << "[" << sol_a.ob2 << "|" << sol_d.ob2 << "]" << endl;

				break;
			default:
				break;
			}
			
			s_star = VND(p, &s); //método de busca local

			cout<<"(F)"<<s_star.get_obj1_cost()<<"|";
			cout<<s_star.get_obj2_freshness()<<endl;

			system("PAUSE");
			if (s_star.get_obj1_cost() < s.get_obj1_cost())
			{
				
				//atualiza s
				k = 1;
			}
			else {
				
				k++;
			}
		}
	}

	return s;

}

ISolution VNS::VND(Problem p, ISolution *s)
{

	Tsol sol_a, sol_d;
	bool continuar=false;

	while (!continuar)
	{
		
		sol_a.ob1 = s->get_obj1_cost();
		sol_a.ob2 = s->get_obj2_freshness();

		movement_intra_route(p, s);
	
		s->set_obj1_cost(p.obj1(s));
		s->set_obj2_freshness(p.obj2(s, s->get_dist_travel()));
		sol_d.ob1 = s->get_obj1_cost();
		sol_d.ob2 = s->get_obj2_freshness();
		cout << "(D)(" << sol_a.ob1 << "|" << sol_d.ob1 << ")";
		cout << "[" << sol_a.ob2 << "|" << sol_d.ob2 << "]" << endl;

		if (sol_d.ob1<sol_a.ob1 || sol_d.ob2>sol_a.ob2)
		{
		
			continuar = true;
		}
		else
		{
		
			continuar = false;
		}
		
	}
	//armazenar a melhor e retornar ela pois vai haver uma alteracao a mais
	return *s;
}
