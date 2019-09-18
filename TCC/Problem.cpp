#include "Problem.h"
#include "ISolution.h"
#include "Methods.h"
#include <math.h>




float Problem::obj2(ISolution* s,float d)//trocar distancia para distancia por veiculo e descobrir o valor correto
{
	float SUM_freshness = 0;
	float distance =0;
	float arrive_time=0;
	float Total_demand = 0;
	int i, j,k;
	for (i = 0; i < s->get_num_vehicles_in_S(); i++)
	{
		//cout <<endl << "veiculo " << i << ":"<<endl;
		distance = euclidian_distance(
			origin.get_X(),
			origin.get_Y(),
			cons.at(s->get_client_id_on_route(i, 0)).get_X(),
			cons.at(s->get_client_id_on_route(i, 0)).get_Y()
		);
		Total_demand += cons.at(s->get_client_id_on_route(i, 0)).get_demand();

		arrive_time = cons.at(s->get_client_id_on_route(i, 0)).get_initial_T();
		SUM_freshness += calc_beta(arrive_time) *cons.at(s->get_client_id_on_route(i, 0)).get_demand();
		
		for ( j = 0, k = 1; j < s->get_route_size(i) - 1; j++, k++)
		{
			distance = euclidian_distance(
				cons.at(s->get_client_id_on_route(i, j)).get_X(),
				cons.at(s->get_client_id_on_route(i, j)).get_Y(),
				cons.at(s->get_client_id_on_route(i, k)).get_X(),
				cons.at(s->get_client_id_on_route(i, k)).get_Y()
			);
			arrive_time = calc_arrival_time_of_vehicle(arrive_time, 
				cons.at(s->get_client_id_on_route(i, j)).get_id(), distance);
			
			//analisar esta parte
			//if (arrive_time < cons.at(s->get_client_id_on_route(i, k)).get_initial_T())
			//{
				//espera o cliente abrir
				//arrive_time += cons.at(s->get_client_id_on_route(i, k)).get_initial_T() - arrive_time;
			//}
			Total_demand += cons.at(s->get_client_id_on_route(i, k)).get_demand();
			SUM_freshness += calc_beta(arrive_time) *cons.at(s->get_client_id_on_route(i, k)).get_demand(); ;
		}
		Total_demand += cons.at(s->get_client_id_on_route(i, j)).get_demand();
		distance = euclidian_distance(origin.get_X(), origin.get_Y(),
			cons.at(s->get_client_id_on_route(i, j)).get_X(),
			cons.at(s->get_client_id_on_route(i, j)).get_Y());

		arrive_time = calc_arrival_time_of_vehicle(arrive_time,
			cons.at(s->get_client_id_on_route(i, j)).get_id(), distance);

		SUM_freshness += calc_beta(arrive_time) *cons.at(s->get_client_id_on_route(i, j)).get_demand(); ;

	}
	//cout << "SOMA FRESH:" << SUM_freshness << endl;
	//cout << "Total_demand:" << Total_demand;
	s->set_obj2_freshness(SUM_freshness / Total_demand);
	s->set_inv_obj2_freshness();
	return SUM_freshness/Total_demand;
}

bool Problem::constraint_vehicle_capacity(ISolution s)
{
	float SUM_demand=0;
	for (int i = 0; i < num_vehicle; i++)
	{
		SUM_demand = 0;
		for (int j = 0; j < s.get_route_size(i); j++)
		{
			//cout << "demand for client:" << cons.at(s.get_client_id_on_route(i, j)).get_demand();
			SUM_demand += cons.at(s.get_client_id_on_route(i,j)).get_demand();
		}
		
		//cout << "[" << SUM_demand << "." << capacity << "]";
		if (SUM_demand <= capacity)
		{
			
			continue;
		}
		else
		{
			return false;
		}
	}
	
	
	return true;
}

bool Problem::constraint_curr_freshness(ISolution s)
{
	float SUM_freshness = 1;
	for (int i = 0; i < num_vehicle; i++)
	{
		for (int j = 0; j < s.get_route_size(i); j++)//ta errado
		{
			SUM_freshness -= 0.0001;
		}
		if (SUM_freshness >= product_freshness)
		{
			continue;
		}
		else
		{
			break;
		}
	}
	if (SUM_freshness >= product_freshness)
	{
		return true;
	}
	return false;
}

float Problem::euclidian_distance(int x1, int y1, int x2, int y2)
{
	return sqrt(pow(x2-x1,2)+pow(y2-y1,2));
}

float Problem::calc_arrival_time_of_vehicle(float past_time,int c, float dist)
{
	//casos seja o primeiro cliente nao ha tempo de servico nem tempo de chegada
	if (past_time == 0)
	{
		return dist/velocity_vehicle ;
	}
	else
	{
		
		return past_time + cons.at(c).get_service_T() + ((dist)/(velocity_vehicle/3.6)) ;
	}
	
}

float Problem::calc_transport_cost(float dist)
{
	return (dist * transport_cost_per_KM);
}

Problem::Problem()
{
}

Problem::Problem(int num_v, int cap, int num_c, int cost_v, float prod_fresh)
{
	set_num_vehicle(num_v);
	set_capacity(cap);
	set_num_clients(num_c);
	set_fixed_cost_vehicle(cost_v);
	set_product_freshness(prod_fresh);
}

void Problem::set_capacity(int c)
{
	capacity = c;
}

void Problem::set_num_clients(int c)
{
	num_consumers = c;
}

void Problem::add_client(Client * c)
{
	cons.push_back(*c);
}

void Problem::set_product_freshness(float pf )
{
	product_freshness = pf;
}

int Problem::get_capacity()
{
	return capacity;
}

int Problem::get_num_clients()
{
	return num_consumers;
}

Client * Problem::get_client(int pos)
{
	return &cons.at(pos);
}

float Problem::get_fixed_cost_vehicle()
{
	return cost_vehicle;
}

float Problem::get_velocity_vehicle()
{
	return velocity_vehicle;
}

float Problem::get_transportation_cost_per_KM()
{
	return transport_cost_per_KM;
}

float Problem::get_product_freshness()
{
	return product_freshness;
}

void Problem::set_num_vehicle(int v)
{
	num_vehicle = v;
}

void Problem::set_weight(int w1, int w2, int w3)
{
	w_1 = w1;
	w_2 = w2;
	w_3 = w3;
}

void Problem::set_fixed_cost_vehicle(float c)
{
	cost_vehicle = c;
}

void Problem::set_velocity_vehicle(float v)
{
	velocity_vehicle = v;
}

void Problem::set_transportation_cost_per_KM(float tcpk)
{
	transport_cost_per_KM = tcpk;
}

int Problem::get_num_vehicle()
{
	return num_vehicle;
}


float Problem::obj1(ISolution* s)
{
	
	int i = 0, j = 0, k = 0;
	float SUM_time_vehicle_cost = 0;
	float SUM_cost_vehicle=0;
	float SUM_cost_early_arrive = 0;
	float SUM_cost_due_arrive = 0;
	float SUM_lost_product = 0;
	float Total_distance_traveled=0;
	float distance_by_vehicle = 0;
	float arrive_time=0;
	int early_pen=0;
	int due_pen = 0;
	int dist_temp = 0;

	for (i = 0; i < s->get_num_vehicles_in_S(); i++)
	{
		SUM_cost_vehicle += cost_vehicle;
	}
	
	for (i = 0; i < s->get_num_vehicles_in_S(); i++)
	{
		//distancia de origem ate 1o cliente
		distance_by_vehicle += euclidian_distance(origin.get_X(), origin.get_Y(),
			cons.at(s->get_client_id_on_route(i, 0)).get_X(),
			cons.at(s->get_client_id_on_route(i, 0)).get_Y());
		
		//Abordagens de calculo do tempo de chegada no primeiro cliente:
		//1 - O primeiro veiculo chega no tempo inicial do primeiro cliente
		arrive_time = cons.at(s->get_client_id_on_route(i, 0)).get_initial_T();
		dist_temp = euclidian_distance(origin.get_X(), origin.get_Y(), cons.at(s->get_client_id_on_route(i, 0)).get_X(), cons.at(s->get_client_id_on_route(i, 0)).get_Y());
		
		//2 - calculo normal como nos outros clientes
		//arrive_time += calc_time_vehicle(0, cons.at(s->get_client_id_on_route(i, 0)).get_id(), dist_temp);
		
		
		//cout << "<" << cons.at(s->get_client_id_on_route(i, 0)).get_initial_T() << "'" << arrive_time << "'" << cons.at(s->get_client_id_on_route(i, 0)).get_due_T() << ">";
		
		if (arrive_time < cons.at(s->get_client_id_on_route(i, 0)).get_initial_T())
		{
			early_pen++;

			//SUM_cost_early_arrive += w_1 * (cons.at(s->get_client_id_on_route(i, 0)).get_initial_T() - arrive_time);//****
		
			arrive_time += (cons.at(s->get_client_id_on_route(i, 0)).get_initial_T() - arrive_time);
		}
		else if (arrive_time > cons.at(s->get_client_id_on_route(i, 0)).get_due_T())
		{
			due_pen++;
			
			
			//Abordagem similar ao early:
			//SUM_cost_due_arrive += w_2 * (arrive_time -cons.at(s->get_client_id_on_route(i, 0)).get_due_T());

			arrive_time += (cons.at(s->get_client_id_on_route(i, 0)).get_initial_T() - arrive_time);

		}
		SUM_lost_product += calc_phi(distance_by_vehicle / velocity_vehicle)*cons.at(s->get_client_id_on_route(i, 0)).get_demand();
		
		//Demais clientes
		for (j = 0, k = 1; j < s->get_route_size(i)-1; j++, k++)
		{
			
			distance_by_vehicle += euclidian_distance(
				cons.at(s->get_client_id_on_route(i, j)).get_X(),
				cons.at(s->get_client_id_on_route(i, j)).get_Y(),
				cons.at(s->get_client_id_on_route(i, k)).get_X(),
				cons.at(s->get_client_id_on_route(i, k)).get_Y()
			);
			
			dist_temp = euclidian_distance(cons.at(s->get_client_id_on_route(i, j)).get_X(), 
				cons.at(s->get_client_id_on_route(i, j)).get_Y(), 
				cons.at(s->get_client_id_on_route(i, k)).get_X(), 
				cons.at(s->get_client_id_on_route(i, k)).get_Y());

			//Janela de tempo
			arrive_time = calc_arrival_time_of_vehicle(arrive_time,cons.at(s->get_client_id_on_route(i, j)).get_id(),dist_temp);
			
			//cout << "<" << cons.at(s->get_client_id_on_route(i, k)).get_initial_T() << "'" << arrive_time << "'" << cons.at(s->get_client_id_on_route(i, k)).get_due_T() << ">";
			
			if (arrive_time < cons.at(s->get_client_id_on_route(i, k)).get_initial_T())
			{
				early_pen++;
				
				//int dist_temp = euclidian_distance(origin.get_X(), origin.get_Y(), cons.at(s->get_client_id_on_route(i, k)).get_X(), cons.at(s->get_client_id_on_route(i, k)).get_Y());
				//SUM_cost_early_arrive += w_1 * (cons.at(s->get_client_id_on_route(i, k)).get_initial_T() - arrive_time);//****																																										 //arrive_time += (cons.at(s->get_client_id_on_route(i, 0)).get_initial_T() - arrive_time);
				arrive_time += (cons.at(s->get_client_id_on_route(i, k)).get_initial_T() - arrive_time);
			}
			else if (arrive_time > cons.at(s->get_client_id_on_route(i, k)).get_due_T())
			{
				due_pen++;
				//SUM_cost_due_arrive += w_2 * (arrive_time - cons.at(s->get_client_id_on_route(i, k)).get_due_T());//****
			}

			SUM_lost_product += calc_phi(distance_by_vehicle / velocity_vehicle)*cons.at(s->get_client_id_on_route(i, j)).get_demand();
		}
		
		
		//calculo do ultimo de volta a origem
		distance_by_vehicle += euclidian_distance(origin.get_X(), origin.get_Y(),
			cons.at(s->get_client_id_on_route(i, j)).get_X(),
			cons.at(s->get_client_id_on_route(i, j)).get_Y());
		
		//calculo do custo do transporte e tempo percorrido
		SUM_time_vehicle_cost +=  distance_by_vehicle*transport_cost_per_KM;
		

		
		Total_distance_traveled += distance_by_vehicle;

		SUM_lost_product += calc_phi(distance_by_vehicle / velocity_vehicle)*cons.at(s->get_client_id_on_route(i, j)).get_demand();
		distance_by_vehicle = 0;


	}

	
	s->set_obj1_cost(SUM_time_vehicle_cost + early_pen * w_1 + due_pen * w_2 + SUM_cost_vehicle + SUM_lost_product * w_3);
	
	s->set_dist_travel(Total_distance_traveled);

	//SUM_cost_early_arrive + SUM_cost_due_arrive
	return SUM_time_vehicle_cost + early_pen*w_1 + due_pen*w_2 +SUM_cost_vehicle + SUM_lost_product*w_3;
}


