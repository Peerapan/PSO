#include "particle.h"
#include "pso.h"
#include "swarm.h"

#include <time.h>
#include <math.h>

class spPSO : public PSO{
	// this part is the problem specific code
	// Minimize f(x) = 0.001x^2 + 2*Sin(x), -100<=x<=100
public:
	spPSO(int nPar, int nIter, int nNB, double dwmax, double dwmin, double dcp, double dcg, double dcl, double dcn): PSO(nIter, nNB, dwmax, dwmin, dcp, dcg, dcl, dcn)
	{
		SetDimension(nPar, 1);
		//set the swarm size and dimension of each particle
	}
	
	void DisplayResult()
	{
		printf("Result:\n");
		printf("-------\n");
		//write the position of the best particle
		for(int i=0; i<this->sSwarm->pParticle[this->sSwarm->posBest]->Dimension; i++)
			printf("x(%d) = %lf\n", i, this->sSwarm->pParticle[this->sSwarm->posBest]->BestP[i]);
		printf("f(x) = %lf\n", this->sSwarm->pParticle[this->sSwarm->posBest]->ObjectiveP);
	}
	
	double Objective(Particle*& P)
	{
		double obj = 0;
		//evaluate the objective value corresponding to the position of the Particle
		for (int i = 0; i < P->Dimension; i++)
			obj += 0.001 * pow(P->Position[i], 2) + 2 * sin(P->Position[i]);
		return obj;
	}
	
	void InitSwarm()
	{
		for(int i=0; i<sSwarm->Member; i++)
		{
			for (int j = 0; j < sSwarm->pParticle[i]->Dimension; j++)
			{
				sSwarm->pParticle[i]->Position[j] = -100 + 200 * rand(); //generate random number from 0 to 1
				sSwarm->pParticle[i]->Velocity[j] = 0;
				sSwarm->pParticle[i]->BestP[j] = sSwarm->pParticle[i]->Position[j];
				sSwarm->pParticle[i]->PosMin[j] = -100;  //set the lower bound of a position
				sSwarm->pParticle[i]->PosMax[j] =  100;  //set the upper bound of a position
			}
			sSwarm->pParticle[i]->ObjectiveP = 1.7E308;   //initialize the personal best objective value
		}
		sSwarm->posBest = 0;
	}
};

int main(int argc, char** argv){
	//set the PSO's parameters
	int noPar = 10;
	int noIter = 200;
	int noNB = 5;
	double wMax = 0.9;
	double wMin = 0.4;
	double cP = 2;
	double cG = 2;
	double cL = 0;
	double cN = 0;
	int noRep = 30; //number of replications
	
	// opening output file
	printf("Number of Particle  \t %d \n", noPar);
	printf("Number of Iteration \t %d \n", noIter);
	printf("Number of Neighbor  \t %d \n", noNB);
	printf("Parameter wmax      \t %lf \n", wMax);
	printf("Parameter wmin      \t %lf \n", wMin);
	printf("Parameter cp        \t %lf \n", cP);
	printf("Parameter cg        \t %lf \n", cG);
	printf("Parameter cl        \t %lf \n", cL);
	printf("Parameter cn        \t %lf \n", cN);
	printf("");
	
	for(int i=0; i<noRep; i++)
	{
		printf("Replication %d\n", i+1);
		printf("Iteration \t global best index \t best objective value \t dispersion index \t average obj \t min obj \t max obj\n");
		
		PSO* myPSO = new spPSO(noPar, noIter, noNB, wMax, wMin, cP, cG, cL, cN);
		myPSO->Run(false);
		myPSO->DisplayResult();
		
		delete myPSO;
	}
	
	return 0;
}