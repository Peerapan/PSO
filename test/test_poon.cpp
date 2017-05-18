#include "particle.h"
#include "pso.h"
#include "swarm.h"

#include <string>
#include <iostream>
#include <time.h>
#include <math.h>

// 1 - seaside
// 2 - landside

class spPSO : public PSO{
	// this part is the problem specific code
	// Minimize f(x) = 0.001x^2 + 2*Sin(x), -100<=x<=100
	int side_of_working, import_container, free_area, export_container, movement;
	double **time_side_to_a;
	double **time_side_to_c;
	double **time_a_to_c;
public:
	spPSO(int nPar, int nIter, int nNB, double dwmax, double dwmin, double dcp, double dcg, double dcl, double dcn): PSO(nIter, nNB, dwmax, dwmin, dcp, dcg, dcl, dcn)
	{
		time_side_to_a = time_side_to_c = time_side_to_c = NULL;
	}
	
	~spPSO(){
		for(int i=0; i<side_of_working; i++){
			free(time_side_to_a[i]);
		}
		free(time_side_to_a);
		for(int i=0; i<side_of_working; i++){
			free(time_side_to_c[i]);
		}
		free(time_side_to_c);
		for(int i=0; i<free_area; i++){
			free(time_a_to_c[i]);
		}
		free(time_side_to_c);
	}
	
	void extract(int dim, int& w, int& i, int& a, int& c, int& m){
		m = dim % movement;
		dim /= movement;
		c = dim % export_container;
		dim /= export_container;
		a = dim % free_area;
		dim /= free_area;
		i = dim % import_container;
		dim /= import_container;
		w = dim % side_of_working;
	}
	
	int read_data(const char* file){
		FILE *ptr = NULL;
		ptr = fopen(file,"r");
		if(ptr){
			fscanf(ptr, "%d %d %d %d %d", &side_of_working, &import_container, &free_area, &export_container, &movement );
			time_side_to_a = (double**) malloc(sizeof(double*) * side_of_working);
			for(int i=0; i<side_of_working; i++){
				time_side_to_a[i] = (double*) malloc(sizeof(double) * free_area);
				for(int j=0; j<free_area; j++){
					fscanf(ptr, "%lf", &time_side_to_a[i][j]);
					printf("%lf ", time_side_to_a[i][j]);
				}
				printf("\n");
			}
			time_side_to_c = (double**) malloc(sizeof(double*) * side_of_working);
			for(int i=0; i<side_of_working; i++){
				time_side_to_c[i] = (double*) malloc(sizeof(double) * export_container);
				for(int j=0; j<export_container; j++){
					fscanf(ptr, "%lf", &time_side_to_c[i][j]);
					printf("%lf\n", time_side_to_c[i][j]);
				}
			}
			time_a_to_c = (double**) malloc(sizeof(double*) * free_area);
			for(int i=0; i<free_area; i++){
				time_a_to_c[i] = (double*) malloc(sizeof(double) * export_container);
				for(int j=0; j<export_container; j++){
					fscanf(ptr, "%lf", &time_a_to_c[i][j]);
					printf("%lf ", time_a_to_c[i][j]);
				}
				printf("\n");
			}
		}
		fclose(ptr);
		return side_of_working*import_container*free_area*export_container*movement;
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
		for (int dim = 0; dim < P->Dimension; dim++){
			bool decision = (P->Position[dim] >= 0 ? true : false);
			if( decision ){
				int w, i, a, c, m;
				extract(dim, w, i, a, c, m);
				obj += (time_a_to_c[a][c] + time_side_to_a[w][a] + time_side_to_c[w][c]);
			}
		}
		return obj;
	}
	
	void InitSwarm()
	{
		for(int i=0; i<sSwarm->Member; i++)
		{
			for (int j = 0; j < sSwarm->pParticle[i]->Dimension; j++){
				sSwarm->pParticle[i]->Position[j] = -1 + 2 * ((double) rand() / (RAND_MAX)); //generate random number from 0 to 1
				sSwarm->pParticle[i]->Velocity[j] = 0.5;
				sSwarm->pParticle[i]->BestP[j] = sSwarm->pParticle[i]->Position[j];
				sSwarm->pParticle[i]->PosMin[j] = -1;  //set the lower bound of a position
				sSwarm->pParticle[i]->PosMax[j] =  1;  //set the upper bound of a position
			}
			sSwarm->pParticle[i]->ObjectiveP = 1.7E308;   //initialize the personal best objective value
		}
		sSwarm->posBest = 0;
	}
};

int main(int argc, char** argv){
	srand(time(0));
	
	//set the PSO's parameters
	int noPar = 10;
	int noIter = 50000;
	int noNB = 1;
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
		
		spPSO* myPSO = new spPSO(noPar, noIter, noNB, wMax, wMin, cP, cG, cL, cN);
		int dimSize = myPSO->read_data("data_10_1.txt");
		myPSO->SetDimension(noPar, dimSize);
		
		/*
		for(int dim=0; dim<dimSize; dim+=1){
			int i, w, a, c, m;
			myPSO->extract(dim, w, i, a, c, m);
			printf("%d : %d %d %d %d %d\n", dim, w, i, a, c, m);
		}*/
		
		myPSO->Run(true);
		myPSO->DisplayResult();
		delete myPSO;
	}
	
	return 0;
}