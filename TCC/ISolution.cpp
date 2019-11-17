#include "ISolution.h"
#include <iostream>
int ident = 0;

ISolution::ISolution()
{
	this->route_sizes = nullptr;
	this->num_vehicles_in_S = 0;
}

ISolution::~ISolution()
{
	
}

ISolution::ISolution(int nc, int nv)
{
	
	id = ident++;
	set_num_vehicles_in_S(nv);
	route_sizes = new int[nv];
	distance_traveled = 0;
}

void ISolution::operator=(ISolution s)
{
	int i = 0;
	this->id = s.id;
	
	
	vector<int> rout_aux;

	if (this->num_vehicles_in_S == 0)
	{
		this->num_vehicles_in_S = s.num_vehicles_in_S;
		this->route_sizes = new int[this->num_vehicles_in_S];
		for (i = 0; i < this->num_vehicles_in_S; i++)
		{
			this->route_sizes[i] = s.route_sizes[i];
		}
	}
	else
	{
		this->num_vehicles_in_S = s.num_vehicles_in_S;
		delete[] this->route_sizes;
		this->route_sizes = new int[this->num_vehicles_in_S];
		for (i = 0; i < this->num_vehicles_in_S; i++)
		{
			this->route_sizes[i] = s.route_sizes[i];
		}
	}

	this->sol_path = s.sol_path;
	/*for (i = 0; i < this->sol_path.size(); i++)
	{

		this->sol_path.at(i).clear();
	}
	this->sol_path.clear();

	for (i = 0; i < s.sol_path.size(); i++)
	{
		this->sol_path.push_back(s.sol_path.at(i));
	}
	*/
	
	this->obj1_cost = s.obj1_cost;
	this->obj2_freshness = s.obj2_freshness;
	this->inv_obj2_freshness = s.inv_obj2_freshness;
	this->distance_traveled = s.distance_traveled;
	this->crownding_dist_obj1 = s.crownding_dist_obj1;
	this->crownding_dist_obj2 = s.crownding_dist_obj2;
}




//adiciona uma rota por vez, recebe qual o veiculo, a rota e o numero de clientes

void ISolution::set_num_vehicles_in_S(int numv)
{
	num_vehicles_in_S = numv;
}

void ISolution::set_route(int posv,int*rv,int nc)
{
	vector<int> rt;
	
	for (int i = 0; i < nc; i++)
	{
		rt.push_back(rv[i]);
	}
	sol_path.push_back(rt);
}

void ISolution::set_obj1_cost(float c)
{
	obj1_cost = c;
}

void ISolution::set_obj2_freshness(float f)
{
	obj2_freshness = f;
}

void ISolution::set_inv_obj2_freshness()
{
	inv_obj2_freshness = 1.0 - obj2_freshness;
}

void ISolution::set_dist_travel(float d)
{
	distance_traveled = d;
}

void ISolution::set_CD_obj1(float x)
{
	crownding_dist_obj1 = x;
}

void ISolution::set_CD_obj2(float x)
{
	crownding_dist_obj2 = x;
}

int ISolution::get_num_vehicles_in_S()
{
	return num_vehicles_in_S;
}

int ISolution::get_client_id_on_route(int v, int pos)
{
	return sol_path[v][pos];
}

void ISolution::substitute_route(int v,int*route,int tam)
{
	for (int i = 0; i < tam; i++)
	{
		sol_path[v][i] = route[i];
	}
	
}

//atribui ao vetor de rotas quantos clientes cada veiculo vai atender, este numero é compativel com o numero de clientes por cluster inicialmente
void ISolution::set_route_size(int vpos, int rsize)
{

	route_sizes[vpos] = rsize;

}



//retorna uma sequencia de clientes atendidos pelo veiculo v
int* ISolution::get_path_Sol(int v,int rsize)
{
	int *rtemp;
	rtemp = new int[rsize];
	for (int i = 0; i < rsize; i++)
	{
		rtemp[i] = sol_path[v].at(i);
	}
	return rtemp;
}



int ISolution::get_route_size(int vpos)
{
	return route_sizes[vpos];
}

float ISolution::get_obj1_cost()
{
	return obj1_cost;
}

float ISolution::get_obj2_freshness()
{
	return obj2_freshness;
}

float ISolution::get_inv_obj2_freshness()
{
	return inv_obj2_freshness;
}

int ISolution::get_id()
{
	return id;
}

float ISolution::get_dist_travel()
{
	return distance_traveled;
}

float ISolution::get_CD_obj1()
{
	return crownding_dist_obj1;
}

float ISolution::get_CD_obj2()
{
	return crownding_dist_obj2;
}

void ISolution::set_ident(int x)
{
	id = x;
}

void ISolution::reset_ident()
{
	ident = 0;
}

void ISolution::recreate_route_sizes(int newtam)
{
	delete[] route_sizes;

	route_sizes = new int[newtam];

}

void ISolution::recreate_path_sol()
{
	sol_path.clear();
}

