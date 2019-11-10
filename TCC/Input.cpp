#include "Input.h"

void input_data(char* name,Problem &p)
{
	ifstream arq;
	int num_v;
	int id=0,cap;
	int x, y, d, rt, dt, st,beta;
	arq.open(name, ios::in);
	/*
		Leitura dos dados iniciais da instancia	
	*/
	arq >>beta>> num_v >> cap;

	p.set_num_vehicle(35);
	p.set_capacity(50);
	p.set_num_clients(99);
	p.set_fixed_cost_vehicle(50);
	p.set_product_freshness(beta/100.0);
	p.set_weight(10, 10, 20);
	p.set_transportation_cost_per_KM(0.5);
	p.set_velocity_vehicle(30);
	
	
	while (id < p.get_num_clients())
	{
		
		arq >> id>>x>>y>>d>>rt>>dt>>st;
		//cout << "id:" << id;
		if (id == 0)
		{
			p.origin.set_X(x);
			p.origin.set_Y(y);
			p.origin.set_due_T(dt);
			p.origin.set_initial_T(rt);
			p.origin.set_demand(d);
			p.origin.set_service_T(st);
			p.origin.set_minimum_f(beta);
		}
		Client *c = new Client(id, x, y, d, rt, dt, 5, beta);
		p.add_client(c);

		//Verifica
		//cout << "Client " << id<<":"<< p.get_client(id)->get_X()<<" "<< p.get_client(id)->get_Y()<<" "<< p.get_client(id)->get_demand()<<" "
		//<< p.get_client(id)->get_initial_T()<<" "<< p.get_client(id)->get_due_T()<<" "<< p.get_client(id)->get_service_T()<<" "<< p.get_client(id)->get_minimum_f()<<endl;
	}

	//cout << "Clientes na instancia: " << p.get_num_clients() << endl;

	//cout << "Leitura de dados - FIM" << endl;
	arq.close();

}
