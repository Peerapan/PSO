#ifndef PARTICLE_H
#define PARTICLE_H

class Particle{
public:
	int Dimension;     //particle dimension
	int localBest;     //index of local best position
	double* Position;   //current particle position 
	double* Velocity;   //current particle velocity
	double* Neighbor;   //near neighbor best position
	double* BestP;      //previous best particle position
	double Objective;     //current particle objective function
	double ObjectiveP;    //previous best particle objective function
	double* PosMax;     //maximum position of particle
	double* PosMin;     //minimum position of particle 

	Particle(int nDim){
		// construct a particle with nDim dimension
		Dimension = nDim;
		Position = new double[Dimension];
		Velocity = new double[Dimension];
		BestP = new double[Dimension];
		Neighbor = new double[Dimension];
		PosMax = new double[Dimension];
		PosMin = new double[Dimension];
	}
	
	~Particle(){
		delete Position;
		delete Velocity;
		delete BestP;
		delete Neighbor;
		delete PosMax;
		delete PosMin;
	}
};

#endif