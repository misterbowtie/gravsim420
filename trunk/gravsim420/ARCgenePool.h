// ARCgenePool.h

#include <time.h>
#include <algorithm>
#include <iostream>
#include "ARCgenes.h"

using namespace std;



float SURVIVAL_RATE = .80f; 

int geneCompare(const void * a, const void * b)
{	if(*(genes*)a<*(genes*)b)
		return -1;
	return 1;
}

class genePool
{
public:
	genePool(int poolSize, double *variables[], int geneSize)
	{	time_t seconds; time(&seconds); 
		srand((unsigned int) seconds); 
		
		max=poolSize;
		size=geneSize;

		var = variables;
		
		genes original;
		double* par = new double[size];
		for(int i=0; i<size; i++)
			par[i]=*var[i];
		original.init(size,par);
		
		gs = new genes[max];
		for(int i=0; i<max; i++)
			gs[i].make(original,original);

		delete[] par;
		index=0;
	}
	~genePool()
	{}

	void nextTest()
	{
		index++;
		if(index==max)
		{
			repopulate();
			index=0;
		}
		for(int i=0;i<size; i++)
		{
			*(var[i]) = gs[index].get(i);
		}

		
	}
	
	void printBest()
	{
		best.print();
	}

	void score(double geneFit)
	{
		gs[index].setFit(geneFit);
	}
	void print()
	{
		cout<<"\n----------------";
		for(int i=0; i<max; i++) 
		{	gs[i].print();}
	}

private:
	void repopulate();

	int max;	//# of genes
	int size;  //# of nums in genes
	int index;
	genes *gs;
	
	genes best;
	double** var;
};


void genePool::repopulate()
{	
	qsort(gs,max,sizeof(genes),geneCompare);
		//(gs, gs+max);

	cout<<"\n AFTER SORT";
	print();

	int mother, father;

	genes* oldgs = gs;

	gs = new genes[max];

	for(int i=0; i<max; i++)
	{
		father= i*SURVIVAL_RATE;
		mother= randf()*randf()*max;
		gs[i].make(oldgs[father],oldgs[mother]);
	}
	if(best.getFit()<oldgs[0].getFit())
		best = oldgs[0];
	delete[] oldgs;
	
}

