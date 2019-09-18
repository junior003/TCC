#ifndef CLIENT_H
#define CLIENT_H


class Client
{
public:
	Client();
	Client(int id,int x, int y, int d, int it, int dt, int st, int mf);
	int get_X();
	int get_Y();
	int get_demand();
	int get_initial_T();
	int get_due_T();
	int get_service_T();
	int get_minimum_f();

	void set_X(int);
	void set_Y(int);
	void set_demand(int);
	void set_initial_T(int);
	void set_due_T(int);
	void set_service_T(int);
	void set_minimum_f(int);
	
	//Kmeans
	void setCluster(int); //define qual cluster o clientes pertence 
	int getCluster(); //retorna qual o cluster o clientes pertence 

	int get_attr(int); // retorna uma varial do clientes 

	int get_num_attr();	// retona o total de variaveis do clientes
	int get_id();
private:
	int id;
	int coord_X;
	int coord_Y;
	int demand;
	int initial_T;
	int due_T;
	int service_T;
	int minimum_freshness;
	int id_cluster;
	bool kmeans_ST;

};

#endif // CONSUMER_H