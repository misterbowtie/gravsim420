// ARCgenes.h

#include <time.h>
#include <iostream>

#define MUTATION  .30
#define CROSSOVER  .50  

//double randf(){ return ((double)rand())/(RAND_MAX+1);}

class genes
{
public:
	genes(){gsize=0; data = 0; fit=-1;}
		
	~genes(){if(data!=0 && 0)delete[] data; }
	
	void init(int size)
	{	fit=-1;
		gsize =size; 
		data = new double[size];
		for(int i=0; i<gsize; i++)
		{	
			data[i] = rand(); 
		}
	}
	void init(int size, double value[])
	{	fit=-1;
		gsize =size; 
		data = new double[size];
		for(int i=0; i<gsize; i++)
		{	
			data[i] = value[i]; 
		}
	}
	void make(genes &father, genes &mother);
	
	genes & operator=(const genes& b)
	{	if(data!=NULL) delete[] data; 
		init(b.gsize, b.data); 
		fit = b.fit;
		return *this;
	}
	
	bool operator<(const genes& b)
	{	return (fit>b.fit);
	}
	
	double get(int i){return data[i];}

	void print()
	{
		cout<<"\ngene:";
		for(int i=0; i<gsize; i++)
			cout<<"\t"<<data[i];
		cout<<"\t fit: "<<fit;
	}

	double getFit(){return fit;}
	void setFit(double value){fit=value;}
		
private:
	
	double* data;
	int gsize, val;
	double fit;

};

void genes::make(genes & father, genes &mother)
{
	//clone father
	init(father.gsize, father.data);
	
//CROSSOVER
	if(randf() < CROSSOVER)
		for(int i=0; i<gsize; i++)
		{	if(randf() < .40) // 40% CHANCE
				data[i]= mother.data[i];
			else if(randf() < .33) // 60%*33% = 20% chance
				data[i]= (data[i] + mother.data[i])/2;
		}

//MUTATE
	//if(randf()<MUTATION)
	else
	{	int r = randf()*gsize;
		data[r]+= data[r]*(randf()+randf()-1);
	}
}
