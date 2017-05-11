#include <math.h>
#include <stdio.h>

class PSO{
	double wmax;  //maximum value of w
	double wmin;  //minimum value of w
	double cp;    //parameter cp
	double cg;    //parameter cg
	double cl;    //parameter cl
	double cn;    //parameter cn
	int Iter;     //number of iteration
	int nPar; 	  //number of particle 
	int nDim;     //number of dimension
	int NB;       //number of neighbors

	Swarm sSwarm*;

	PSO(int nIter, int nNB, double dwmax, double dwmin, double dcp, double dcg, double dcl, double dcn){
		Iter = nIter;
		NB = nNB;
		wmax = dwmax;
		wmin = dwmin;
		cp = dcp;
		cg = dcg;
		cl = dcl;
		cn = dcn;
	}
	
	~PSO(){
	
	}
	
	void SetDimension(int par, int dim)
	{	//set swarm members and particle dimensions
		nPar = par;
		nDim = dim;
	}
	
	virtual void InitSwarm(){	
		//swarm initialization
	}

	void Run(bool debug){
		//PSO algorithm
		double w = wmax;
		double decr = (wmax - wmin) / Iter;

		sSwarm = new Swarm(nPar, nDim);
		InitSwarm();
		Evaluate();
		sSwarm->UpdateBest(NB);

		if (debug)
		{
			sSwarm->EvalDispersion();
			sSwarm->EvalStatObj();
			printf("%d \t %lf \t %lf  \t %lf  \t %lf  \t %lf  \t %lf", i, sSwarm->posBest, sSwarm->pParticle[sSwarm->posBest]->ObjectiveP, sSwarm->Dispersion, sSwarm->AvgObj, sSwarm->MinObj, sSwarm->MaxObj);
		}

		double u1** = new double[sSwarm->Member];
		double u2** = new double[sSwarm->Member];
		double u3** = new double[sSwarm->Member];
		double u4** = new double[sSwarm->Member];
		for(int i = 0; i < sSwarm->Member; i++){
			u1[i] = new double[sSwarm->pParticle[0]->Dimension];
			u2[i] = new double[sSwarm->pParticle[0]->Dimension];
			u3[i] = new double[sSwarm->pParticle[0]->Dimension];
			u4[i] = new double[sSwarm->pParticle[0]->Dimension];
		}

		for (int i = 1; i < Iter; i++)
		{
			//generate random number for the iteration process
			for (int j = 0; j < sSwarm->Member; j++)
			{
				for (int k = 0; k < sSwarm->pParticle[0].Dimension; k++)
				{
					u1[j][k] = rand();
					u2[j][k] = rand();
					u3[j][k] = rand();
					u4[j][k] = rand();
				}
			}

			sSwarm->Move(w, cp, cg, cl, cn, u1, u2, u3, u4);
			Evaluate();
			sSwarm->UpdateBest(NB);
			if (debug)
			{
				sSwarm->EvalDispersion();
				sSwarm->EvalStatObj();
				printf("%d \t %lf \t %lf  \t %lf  \t %lf  \t %lf  \t %lf", i, sSwarm->posBest, sSwarm->pParticle[sSwarm->posBest]->ObjectiveP, sSwarm->Dispersion, sSwarm->AvgObj, sSwarm->MinObj, sSwarm->MaxObj);
			}
			w -= decr;
		}
	}

	void Evaluate(){
		//evaluate objective function value of each swarm member
		for (int j = 0; j < sSwarm->Member; j++)
			sSwarm->pParticle[j]->Objective = Objective(sSwarm->pParticle[j]);
	}

	virtual double Objective(Particle p){
		//empty function to be override in the problem specific definition
		//to calculate objective function of a particle
		return 0;
	}

	virtual void DisplayResult(){
		//empty function to be override in the problem specific definition
		//to display the result
	}
};