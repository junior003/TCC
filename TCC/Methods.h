

#ifndef METHODS_H
#define METHODS_H

#include "KmeansS.h"
#include "SMSEMOA.h"
#include "ISolution.h"
#include "KmeansST.h"



vector<int> calculate_diference(vector<int>*P, vector<int>*X);


float calc_phi(float time);
float calc_beta(float time);
void sort_by_early_time(int * route_temp, int  n, Problem *p);
void sort_by_spatial_distance(int * route_temp, int  n, Problem *p);

vector<ISolution> build_solutions_space_or_timewindow_sort_S(Problem*,KmeansS*,int);
vector<ISolution> build_solutions_space_or_timewindow_sort_ST(Problem *p, KmeansST * K, int num_max_s);
float maxi(float, float);
float mini(float, float);

void movement_intra_route(Problem p, ISolution*);
void movement_inter_route(Problem p, ISolution*);
void realoc_inter_route(Problem p, ISolution *S);


void printaSolucao(Problem p, ISolution s);
float randomic(float min, float max);

bool ja_rodou(int test);

void teste_completo(int test);

void inicia_teste();


#endif // !1

