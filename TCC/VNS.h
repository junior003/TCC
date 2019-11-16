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
#include <iomanip>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>

#ifndef VNS_H
#define VNS_H



class VNS
{
public:
	VNS(int);
	void execute_VNS(Problem p, ISolution *s);
	void VND(Problem p, ISolution *s);

private:
	int iter_max;


};


#endif // !VNS_H