#ifndef SWARM_H
#define SWARM_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "particle.h"

class Swarm{
public:
	int Member;			//number of swarm member
	int posBest;			//index of best particle in the swarm
	double Dispersion;   //dispersion index of the swarm
	double VelIndex;		//velocity index of the swarm
	double MaxObj;		//maximum objective function of the swarm
	double MinObj;		//minimum objective function of the swarm
	double AvgObj;		//average objective function of the swarm

	//definition array of particle
	Particle** pParticle;

	Swarm(int nPar, int nDim){
		// construct a swarm with nPar particles,
		// each particle with nDim dimension
		Member = nPar;
		pParticle = new Particle* [Member];

		for (int i = 0; i < Member; i++){
			//Initialize particles
			pParticle[i] = new Particle(nDim);
		}
	}
	
	~Swarm(){
		for (int i = 0; i < Member; i++){
			delete pParticle[i];
		}
		delete pParticle;
	}

	void Move(double w, double cp, double cg, double cl, double cn, double** r1, double** r2, double** r3, double** r4){
		//moving swarm ...
		for (int i = 0; i < Member; i++)
		{
			for (int j = 0; j < pParticle[i]->Dimension; j++)
			{
				pParticle[i]->Velocity[j] *= w;
				pParticle[i]->Velocity[j] += cp * r1[i][j] * (pParticle[i]->BestP[j] - pParticle[i]->Position[j]);
				pParticle[i]->Velocity[j] += cg * r2[i][j] * (pParticle[posBest]->BestP[j] - pParticle[i]->Position[j]);
				pParticle[i]->Velocity[j] += cl * r3[i][j] * (pParticle[pParticle[i]->localBest]->BestP[j] - pParticle[i]->Position[j]);
				pParticle[i]->Velocity[j] += cn * r4[i][j] * (pParticle[i]->Neighbor[j] - pParticle[i]->Position[j]);

				pParticle[i]->Position[j] += pParticle[i]->Velocity[j];

				if (pParticle[i]->Position[j] > pParticle[i]->PosMax[j])
				{
					pParticle[i]->Position[j] = pParticle[i]->PosMax[j];
					pParticle[i]->Velocity[j] = 0;
				}
				if (pParticle[i]->Position[j] < pParticle[i]->PosMin[j])
				{
					pParticle[i]->Position[j] = pParticle[i]->PosMin[j];
					pParticle[i]->Velocity[j] = 0;
				}
			}
		}
	}

	void UpdateBest(int nbSize)
	{	//updating cognitive and social information 

		int l_temp, n_temp;
		double FDR;
		double FDRBest;

		//update personal best
		for (int i = 0; i < Member; i++)
		{
			if (pParticle[i]->Objective < pParticle[i]->ObjectiveP)
			{
				pParticle[i]->ObjectiveP = pParticle[i]->Objective;
				for (int j = 0; j < pParticle[i]->Dimension; j++)
					pParticle[i]->BestP[j] = pParticle[i]->Position[j];
			}
		}

		//update global best
		for (int i = 0; i < Member; i++)
			if (pParticle[i]->ObjectiveP < pParticle[posBest]->ObjectiveP)
				posBest = i;

		//update local best
		for (int i = 0; i < Member; i++)
		{
			pParticle[i]->localBest = i;

			for (int j = i - nbSize / 2; j <= i + nbSize / 2; j++)
			{
				l_temp = j;
				if (i == j)
					continue;
				if (l_temp < 0)
					l_temp += Member;
				if (l_temp >= Member)
					l_temp -= Member;
				if (pParticle[l_temp]->ObjectiveP < pParticle[pParticle[i]->localBest]->ObjectiveP)
					pParticle[i]->localBest = l_temp;
			}
		}

		//update near neighbor best
		for (int i = 0; i < Member; i++)
		{
			for (int j = 0; j < pParticle[i]->Dimension; j++)
			{
				if (i == 0)
					n_temp = 1;
				else
					n_temp = 0;

				FDRBest = (pParticle[i]->Objective - pParticle[n_temp]->ObjectiveP) / fabs(pParticle[i]->Position[j] - pParticle[n_temp]->BestP[j]);

				for (int k = 0; k < Member; k++)
				{
					if (i == k) continue;
					FDR = (pParticle[i]->Objective - pParticle[k]->ObjectiveP) / fabs(pParticle[i]->Position[j] - pParticle[k]->BestP[j]);
					if (FDR > FDRBest)
					{
						n_temp = k;
						FDRBest = FDR;
					}
				}
				pParticle[i]->Neighbor[j] = pParticle[n_temp]->BestP[j];
			}
		}
	}

	void DisplayBest()
	{	//display the best particle

		printf("\n");
		printf("Best Particle in the Swarm\n");
		printf("--------------------------\n");
		printf("position:\n");
		printf("---------\n");
		for (int j = 0; j < pParticle[posBest]->Dimension; j++)
		{
			printf("dimension %d: %lf\n", j, pParticle[posBest]->Position[j]);
		}
		printf("---------\n");
		printf("objective: %lf\n", pParticle[posBest]->ObjectiveP);

	}

	void EvalDispersion()
	{	//evaluate dispersion index

		double result = 0;
		for (int i = 0; i < this->Member; i++)
		{
			for (int j = 0; j < this->pParticle[i]->Dimension; j++){
				result += fabs(this->pParticle[i]->Position[j] - this->pParticle[this->posBest]->BestP[j]);
			}
		}
		this->Dispersion = result / this->Member / this->pParticle[0]->Dimension;
	}

	void EvalVelIndex()
	{	//evaluate velocity index

		double result = 0;
		for (int i = 0; i < this->Member; i++)
			for (int j = 0; j < this->pParticle[i]->Dimension; j++)
				result += fabs(this->pParticle[i]->Velocity[j]);

		this->VelIndex = result / this->Member / this->pParticle[0]->Dimension;
	}

	void EvalStatObj()
	{	//evaluate swarm's objective function statistic
		MaxObj = -1.7E308;
		MinObj = 1.7E308;
		AvgObj = 0;

		for (int i = 0; i < this->Member; i++)
		{
			if (MaxObj < this->pParticle[i]->Objective)
				MaxObj = this->pParticle[i]->Objective;
			if (MinObj > this->pParticle[i]->Objective)
				MinObj = this->pParticle[i]->Objective;
			AvgObj += this->pParticle[i]->Objective;
		}
		AvgObj /= this->Member;
	}
};

#endif