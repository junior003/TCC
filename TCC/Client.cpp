#include "Client.h"


Client::Client()
{
}

Client::Client(int i, int x, int y, int d, int it, int dt, int st, int mf)
{
	id = i;
	set_demand(d);
	set_due_T(dt);
	set_initial_T(it);
	set_minimum_f(mf);
	set_service_T(st);
	set_X(x);
	set_Y(y);
	setCluster(-1);
	
	
}

int Client::get_X()
{
	return coord_X;
}

int Client::get_Y()
{
	return coord_Y;
}

int Client::get_demand()
{
	return demand;
}

int Client::get_initial_T()
{
	return initial_T;
}

int Client::get_due_T()
{
	return due_T;
}

int Client::get_service_T()
{
	return service_T;
}

int Client::get_minimum_f()
{
	return minimum_freshness;
}

void Client::set_X(int x)
{
	coord_X = x;
}

void Client::set_Y(int y)
{
	coord_Y = y;
}

void Client::set_demand(int d)
{
	demand = d;
}

void Client::set_initial_T(int i)
{
	initial_T = i;
}

void Client::set_due_T(int d)
{
	due_T = d;
}

void Client::set_service_T(int s)
{
	service_T = s;
}

void Client::set_minimum_f(int m)
{
	minimum_freshness = m;
}

void Client::setCluster(int c)
{
	id_cluster = c;
}

int Client::getCluster()
{
	return id_cluster;
}

int Client::get_attr(int a) 
{
	
	if (a == 0)
	{
		return coord_X;
	}
	else if(a==1)
	{
		return coord_Y;
	}
	else if (a == 2)
	{
		return initial_T;
	}
	else if (a == 3)
	{
		return due_T;
	}
	//pode ter mais
}

int Client::get_num_attr()
{
	return 2;
}

int Client::get_id()
{
	return id;
}


