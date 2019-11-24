
#ifndef HIPERVOLUME_H
#define HIPERVOLUME_H

#include "Problem.h"
#include "NSGAII.h"
#include "Methods.h"
#include "Population.h"
#include "ISolution.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include "KmeansST.h"
#include "SMSEMOA.h"


class Hipervolume
{
public:
	Hipervolume();
	Hipervolume(float, float);


	vector<float> hv_all_generations;

	float Calcula_Area(vector<Tsol>* sols);

	void VerificaRepeticao(vector<Tsol>* sols);

	void VerificaDominancia(vector<Tsol>* sols);

	float Smetric_per_front(Problem p, vector<ISolution> front, FILE* a1, int numgen, int actualgen);

	float Area_unique_Solution(Tsol s);


	//funcoes referentes ao armazenamento do valor do hipervolume e dos fronts em arquivos


private:
	Tsol Ponto_ref;


};
#endif