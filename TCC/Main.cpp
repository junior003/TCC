#include "Problem.h"
#include "Client.h"
#include "Input.h"
#include "KmeansS.h"
#include "KmeansST.h"
#include "Cluster.h"
#include "ISolution.h"
#include "Methods.h"
#include "Population.h"
#include "NSGAII.h"
#include "SMSEMOA.h"
#include "HiperVolume.h"
#include "VNS.h"
#include <iomanip>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>

using namespace std;
using namespace std::chrono;



int main(int argc, char*argv[])
{
	int input_file = 1, output_file1 = 2,prob_cross = 3,prob_mut=4,tam_pop=5,num_gen = 6;
	FILE *arq1;
	FILE *arq2;
	FILE *arq3;

	remove(argv[output_file1]);
	

	srand(time(NULL));
	float pc = 0,pm = 0;
	int tamp =0, ngen =0;
	//Problem *p;
	vector<ISolution> sols;
	Population pop(1);
	//p = new Problem();
	
	

	float aux1 = 0.0, aux2 = 0.0, AR = 0;
	int tam = 0;

	if ((arq2 = fopen("TEST.bin", "wb")) == NULL)
	{
		cout << "erro ao abrir o arquivo";
		return 0;
	}
	tam = 2;
	AR = 75;
	fwrite(&AR, sizeof(float), 1, arq2);
	fwrite(&tam, sizeof(int), 1, arq2);
	for (int i = 0; i < tam; i++)
	{
		aux1 = 5;
		aux2 = 10;
		fwrite(&aux1, sizeof(float), 1, arq2);
		fwrite(&aux2, sizeof(float), 1, arq2);
		aux1 = 10;
		aux2 = 5;
		fwrite(&aux1, sizeof(float), 1, arq2);
		fwrite(&aux2, sizeof(float), 1, arq2);
	}

	fclose(arq2);
		
	
	/*

	if ((arq2 = fopen("TEST.bin", "rb")) == NULL)
	{
		cout << "erro ao abrir o arquivo TEST";
		return 0;
	}
	fread(&AR, sizeof(float), 1, arq2);
	fread(&tam, sizeof(int), 1, arq2);
	cout << "Teste Smetric" << endl;
	cout << "Area: " << AR << endl;
	cout << "Solucoes" << tam << endl;
	for (int i = 0; i < tam; i++)
	{
		fread(&aux1, sizeof(float), 1, arq2);
		fread(&aux2, sizeof(float), 1, arq2);
		cout << "Obj1: " << aux1 << endl;
		cout << "Obj2: " << aux2 << endl<<endl;
	}

	*/

	//K-means utilizando distancia euclidiana
	//KmeansS kmeans(p->get_num_vehicle(), p->get_num_clients(), 2);
	//kmeans.execute_kmeans_S(p);
	//sols = build_solutions_space_or_timewindow_sort_S(p, &kmeans, 1);

	

	//printaSolucao(*p, sols.at(0));
	//K-means utilizando distancia espaco-temporal
	//KmeansST kmeans_ST(*p, p->get_num_vehicle(), 0.5, 0.5, 1, 1.5, 2);
	//kmeans_ST.Calculate_ALL_Spatial_Distance(*p);
	//kmeans_ST.Calculate_ALL_Temporal_Distance(*p);
	//kmeans_ST.Calculate_ALL_Spatial_Temporal_Distance(*p);
	//kmeans_ST.execute_kmeans_ST(p);
	//sols = build_solutions_space_or_timewindow_sort_ST(p, &kmeans_ST, 1);
	
	
	pc = ((float) atoi(argv[prob_cross])) / 100.0;
	pm = ((float)atoi(argv[prob_mut])) / 100.0;
	tamp = atoi(argv[tam_pop]);
	ngen = atoi(argv[num_gen]);
	//cout << "1" << endl;

	high_resolution_clock::time_point t1;
	high_resolution_clock::time_point t2;
	duration<double> time_span;
	duration<double> time_span2;
	inicia_teste();
	int num_teste = 0;

	cout << "r101" << endl;
	if (!ja_rodou(num_teste++))
	{
		t1 = high_resolution_clock::now();
		char in1[9] = "r101.txt";
		char f1[9] = "r101.bin";
		if ((arq1 = fopen(f1, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p1 = new Problem();

		input_data(in1, *p1);

		//SMSemoa sms1(tamp, ngen, pc, pm);
		//sms1.execute_SMSEMOA(*p1, arq1);

		NSGAII nsga1(tamp, ngen, pc, pm);
		nsga1.execute_NSGAII(*p1, arq1, arq2, arq3);


		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;
		teste_completo(num_teste-1);

	}
	
	cout << "r102" << endl;
	if (!ja_rodou(num_teste++))
	{
		t1 = high_resolution_clock::now();
		fclose(arq1);
		char in2[9] = "r102.txt";
		char f2[9] = "r102.bin";
		if ((arq1 = fopen(f2, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p2 = new Problem();

		input_data(in2, *p2);


		NSGAII nsga2(tamp, ngen, pc, pm);
		nsga2.execute_NSGAII(*p2, arq1, arq2, arq3);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;
		teste_completo(num_teste - 1);
	}
	cout << "r103" << endl;
	if (!ja_rodou(num_teste++))
	{

		fclose(arq1);
		t1 = high_resolution_clock::now();
		char in3[9] = "r103.txt";
		char f3[9] = "r103.bin";
		if ((arq1 = fopen(f3, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p3 = new Problem();

		input_data(in3, *p3);


		NSGAII nsga3(tamp, ngen, pc, pm);
		nsga3.execute_NSGAII(*p3, arq1, arq2, arq3);

		//SMSemoa sms3(tamp, ngen, pc, pm);
		//sms3.execute_SMSEMOA(*p3, arq1);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;
		teste_completo(num_teste - 1);
	}
	cout << "r104" << endl;
	if (!ja_rodou(num_teste++))
	{

		fclose(arq1);
		t1 = high_resolution_clock::now();
		char in4[9] = "r104.txt";
		char f4[9] = "r104.bin";
		if ((arq1 = fopen(f4, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p4 = new Problem();

		input_data(in4, *p4);



		//SMSemoa sms4(tamp, ngen, pc, pm);
		//sms4.execute_SMSEMOA(*p4, arq1);

		NSGAII nsga4(tamp, ngen, pc, pm);
		nsga4.execute_NSGAII(*p4, arq1, arq2, arq3);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;
		teste_completo(num_teste - 1);
	}
	cout << "r105" << endl;
	
	if (!ja_rodou(num_teste++))
	{

		fclose(arq1);

		t1 = high_resolution_clock::now();
		char in5[9] = "r105.txt";
		char f5[9] = "r105.bin";
		if ((arq1 = fopen(f5, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p5 = new Problem();

		input_data(in5, *p5);

		//SMSemoa sms5(tamp, ngen, pc, pm);
		//sms5.execute_SMSEMOA(*p5, arq1);

		NSGAII nsga5(tamp, ngen, pc, pm);
		nsga5.execute_NSGAII(*p5, arq1, arq2, arq3);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;

		
	}
	cout << "r106" << endl;
	fclose(arq1);
	t1 = high_resolution_clock::now();
	char in6[9] = "r106.txt";
	char f6[9] = "r106.bin";
	if ((arq1 = fopen(f6, "ab")) == NULL)
	{

		cout << "erro ao abrir o arquivo";
		return 0;

	}

	Problem *p6 = new Problem();

	input_data(in6, *p6);


	NSGAII nsga6(tamp, ngen, pc, pm);
	nsga6.execute_NSGAII(*p6, arq1, arq2, arq3);
	//SMSemoa sms6(tamp, ngen, pc, pm);
	//sms6.execute_SMSEMOA(*p6, arq1);

	t2 = high_resolution_clock::now();
	time_span = duration_cast<duration<double>>(t2 - t1);
	cout << "Tempo: " << time_span.count() << endl;
	teste_completo(num_teste - 1);
	cout << "r107" << endl;
	if (!ja_rodou(num_teste++))
	{
		fclose(arq1);
		t1 = high_resolution_clock::now();

		char in7[9] = "r107.txt";
		char f7[9] = "r107.bin";
		if ((arq1 = fopen(f7, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p7 = new Problem();

		input_data(in7, *p7);

		//SMSemoa sms7(tamp, ngen, pc, pm);
		//sms7.execute_SMSEMOA(*p7, arq1); 

		NSGAII nsga7(tamp, ngen, pc, pm);
		nsga7.execute_NSGAII(*p7, arq1, arq2, arq3);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;


		teste_completo(num_teste - 1);
	}
	cout << "r108" << endl;
	if (!ja_rodou(num_teste++))
	{

		fclose(arq1);
		t1 = high_resolution_clock::now();
		char in8[9] = "r108.txt";
		char f8[9] = "r108.bin";
		if ((arq1 = fopen(f8, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p8 = new Problem();

		input_data(in8, *p8);

		//SMSemoa sms8(tamp, ngen, pc, pm);
		//sms8.execute_SMSEMOA(*p8, arq1);

		NSGAII nsga8(tamp, ngen, pc, pm);
		nsga8.execute_NSGAII(*p8, arq1, arq2, arq3);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;


		teste_completo(num_teste - 1);
	}

	cout << "r109" << endl;
	if (!ja_rodou(num_teste++))
	{
		fclose(arq1);
		t1 = high_resolution_clock::now();
		char in9[9] = "r109.txt";
		char f9[9] = "r109.bin";
		if ((arq1 = fopen(f9, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p9 = new Problem();

		input_data(in9, *p9);

		//SMSemoa sms9(tamp, ngen, pc, pm);
		//sms9.execute_SMSEMOA(*p9, arq1);

		NSGAII nsga9(tamp, ngen, pc, pm);
		nsga9.execute_NSGAII(*p9, arq1, arq2, arq3);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;

		teste_completo(num_teste - 1);
	}
	cout << "r110" << endl;
	if (!ja_rodou(num_teste++))
	{

		fclose(arq1);
		t1 = high_resolution_clock::now();
		char in10[9] = "r110.txt";
		char f10[9] = "r110.bin";
		if ((arq1 = fopen(f10, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p10 = new Problem();

		input_data(in10, *p10);

		//SMSemoa sms10(tamp, ngen, pc, pm);
		//sms10.execute_SMSEMOA(*p10, arq1);
		NSGAII nsga10(tamp, ngen, pc, pm);
		nsga10.execute_NSGAII(*p10, arq1, arq2, arq3);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;

		teste_completo(num_teste - 1);
	}
	cout << "r111" << endl;
	if (!ja_rodou(num_teste++))
	{

		fclose(arq1);
		t1 = high_resolution_clock::now();
		char in11[9] = "r111.txt";
		char f11[9] = "r111.bin";
		if ((arq1 = fopen(f11, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p11 = new Problem();

		input_data(in11, *p11);


		NSGAII nsga11(tamp, ngen, pc, pm);
		nsga11.execute_NSGAII(*p11, arq1, arq2, arq3);

		//SMSemoa sms11(tamp, ngen, pc, pm);
		//sms11.execute_SMSEMOA(*p11, arq1);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;
		teste_completo(num_teste - 1);
	}
	cout << "r112" << endl;
	if (!ja_rodou(num_teste++))
	{
		fclose(arq1);

		t1 = high_resolution_clock::now();
		char in12[9] = "r112.txt";
		char f12[9] = "r112.bin";
		if ((arq1 = fopen(f12, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p12 = new Problem();

		input_data(in12, *p12);


		NSGAII nsga12(tamp, ngen, pc, pm);
		nsga12.execute_NSGAII(*p12, arq1, arq2, arq3);

		//SMSemoa sms11(tamp, ngen, pc, pm);
		//sms11.execute_SMSEMOA(*p11, arq1);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;

		teste_completo(num_teste - 1);
	}
	
	cout << "r201" << endl;
	if (!ja_rodou(num_teste++))
	{


		fclose(arq1);



		t1 = high_resolution_clock::now();
		char in201[9] = "r201.txt";
		char f201[9] = "r201.bin";
		if ((arq1 = fopen(f201, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p201 = new Problem();

		input_data(in201, *p201);


		NSGAII nsga201(tamp, ngen, pc, pm);
		nsga201.execute_NSGAII(*p201, arq1, arq2, arq3);

		//SMSemoa sms11(tamp, ngen, pc, pm);
		//sms11.execute_SMSEMOA(*p11, arq1);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;

		teste_completo(num_teste - 1);
	}
	cout << "r202" << endl;
	if (!ja_rodou(num_teste++))
	{
		fclose(arq1);




		t1 = high_resolution_clock::now();
		char in202[9] = "r202.txt";
		char f202[9] = "r202.bin";
		if ((arq1 = fopen(f202, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p202 = new Problem();

		input_data(in202, *p202);


		NSGAII nsga202(tamp, ngen, pc, pm);
		nsga202.execute_NSGAII(*p202, arq1, arq2, arq3);

		//SMSemoa sms11(tamp, ngen, pc, pm);
		//sms11.execute_SMSEMOA(*p11, arq1);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;

		teste_completo(num_teste - 1);
	}
	cout << "r203" << endl;
	if (!ja_rodou(num_teste++))
	{
		fclose(arq1);



		t1 = high_resolution_clock::now();
		char in203[9] = "r203.txt";
		char f203[9] = "r203.bin";
		if ((arq1 = fopen(f203, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p203 = new Problem();

		input_data(in203, *p203);


		NSGAII nsga203(tamp, ngen, pc, pm);
		nsga203.execute_NSGAII(*p203, arq1, arq2, arq3);

		//SMSemoa sms11(tamp, ngen, pc, pm);
		//sms11.execute_SMSEMOA(*p11, arq1);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;

		teste_completo(num_teste - 1);
	}
	cout << "r204" << endl;
	if (!ja_rodou(num_teste++))
	{
		fclose(arq1);



		t1 = high_resolution_clock::now();
		char in204[9] = "r204.txt";
		char f204[9] = "r204.bin";
		if ((arq1 = fopen(f204, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p204 = new Problem();

		input_data(in204, *p204);


		NSGAII nsga204(tamp, ngen, pc, pm);
		nsga204.execute_NSGAII(*p204, arq1, arq2, arq3);

		//SMSemoa sms11(tamp, ngen, pc, pm);
		//sms11.execute_SMSEMOA(*p11, arq1);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;

		teste_completo(num_teste - 1);
	}
	cout << "r205" << endl;
	if (!ja_rodou(num_teste++))
	{
		fclose(arq1);



		t1 = high_resolution_clock::now();
		char in205[9] = "r205.txt";
		char f205[9] = "r205.bin";
		if ((arq1 = fopen(f205, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p205 = new Problem();

		input_data(in205, *p205);


		NSGAII nsga205(tamp, ngen, pc, pm);
		nsga205.execute_NSGAII(*p205, arq1, arq2, arq3);

		//SMSemoa sms11(tamp, ngen, pc, pm);
		//sms11.execute_SMSEMOA(*p11, arq1);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;

		teste_completo(num_teste - 1);
	}
	cout << "r206" << endl;
	if (!ja_rodou(num_teste++))
	{

		fclose(arq1);




		t1 = high_resolution_clock::now();
		char in206[9] = "r206.txt";
		char f206[9] = "r206.bin";
		if ((arq1 = fopen(f206, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p206 = new Problem();

		input_data(in206, *p206);


		NSGAII nsga206(tamp, ngen, pc, pm);
		nsga206.execute_NSGAII(*p206, arq1, arq2, arq3);

		//SMSemoa sms11(tamp, ngen, pc, pm);
		//sms11.execute_SMSEMOA(*p11, arq1);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;

		teste_completo(num_teste - 1);
	}
	cout << "r207" << endl;
	if (!ja_rodou(num_teste++))
	{

		fclose(arq1);




		t1 = high_resolution_clock::now();
		char in207[9] = "r207.txt";
		char f207[9] = "r207.bin";
		if ((arq1 = fopen(f207, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p207 = new Problem();

		input_data(in207, *p207);


		NSGAII nsga207(tamp, ngen, pc, pm);
		nsga207.execute_NSGAII(*p207, arq1, arq2, arq3);

		//SMSemoa sms11(tamp, ngen, pc, pm);
		//sms11.execute_SMSEMOA(*p11, arq1);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;

		teste_completo(num_teste - 1);
	}
	cout << "r208" << endl;
	if (!ja_rodou(num_teste++))
	{


		fclose(arq1);


		t1 = high_resolution_clock::now();
		char in208[9] = "r208.txt";
		char f208[9] = "r208.bin";
		if ((arq1 = fopen(f208, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p208 = new Problem();

		input_data(in208, *p208);


		NSGAII nsga208(tamp, ngen, pc, pm);
		nsga208.execute_NSGAII(*p208, arq1, arq2, arq3);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;

		teste_completo(num_teste - 1);
	}
	cout << "r209" << endl;
	if (!ja_rodou(num_teste++))
	{

		fclose(arq1);

		t1 = high_resolution_clock::now();
		char in209[9] = "r209.txt";
		char f209[9] = "r209.bin";
		if ((arq1 = fopen(f209, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p209 = new Problem();

		input_data(in209, *p209);


		NSGAII nsga209(tamp, ngen, pc, pm);
		nsga209.execute_NSGAII(*p209, arq1, arq2, arq3);

		//SMSemoa sms11(tamp, ngen, pc, pm);
		//sms11.execute_SMSEMOA(*p11, arq1);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;

		teste_completo(num_teste - 1);
	}
	cout << "r210" << endl;
	if (!ja_rodou(num_teste++))
	{

		fclose(arq1);


		t1 = high_resolution_clock::now();
		char in210[9] = "r210.txt";
		char f210[9] = "r210.bin";
		if ((arq1 = fopen(f210, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p210 = new Problem();

		input_data(in210, *p210);


		NSGAII nsga210(tamp, ngen, pc, pm);
		nsga210.execute_NSGAII(*p210, arq1, arq2, arq3);

		//SMSemoa sms11(tamp, ngen, pc, pm);
		//sms11.execute_SMSEMOA(*p11, arq1);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;

		teste_completo(num_teste - 1);
	}
	cout << "r211" << endl;
	if (!ja_rodou(num_teste++))
	{



		fclose(arq1);



		t1 = high_resolution_clock::now();
		char in211[9] = "r211.txt";
		char f211[9] = "r211.bin";
		if ((arq1 = fopen(f211, "ab")) == NULL)
		{

			cout << "erro ao abrir o arquivo";
			return 0;

		}

		Problem *p211 = new Problem();

		input_data(in211, *p211);


		NSGAII nsga211(tamp, ngen, pc, pm);
		nsga211.execute_NSGAII(*p211, arq1, arq2, arq3);

		//SMSemoa sms11(tamp, ngen, pc, pm);
		//sms11.execute_SMSEMOA(*p11, arq1);

		t2 = high_resolution_clock::now();
		time_span = duration_cast<duration<double>>(t2 - t1);
		cout << "Tempo: " << time_span.count() << endl;
		fclose(arq1);

		teste_completo(num_teste - 1);
	}





	return 0;
}
