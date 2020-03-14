// ---------------------------------------------------------------------------
// TSP solution using genetic algorithm with mutation
// ---------------------------------------------------------------------------

#include <iostream>
#include <ctime>
#include <time.h>
#include <cmath>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <stdlib.h>

using namespace std;

// structure for input data
struct data
{
	int id;
	int xcord;
	int ycord;
	
};

// structure for population
struct pop
{
	vector<int> route;
	double cost;
	double fitness;
};

// function to swap elements of the list
void swap(struct pop *a, struct pop *b)
{
	pop temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

// function to print the list of cities
void printList(vector<int> list, int size) 
{
	for(int k= 0; k < size; k++)
	{
		cout << list[k] << " ";
	}
	cout << endl;
}

// function to calculate the euclidean distance
double euc(int a, int b, int a1, int b1)
{
	double dist = 0;
	
	dist = pow((a-a1), 2) + pow((b-b1),2);
	dist = sqrt(dist);
	
	return dist;
}

// function to calculate distance using euc()
double calculateDist(struct data *city, int size, vector<int> list)
{
	double dist = 0;
	double total = 0;
	
	for(int j = 0; j < (size-1); j++)
	{
		dist = euc(city[list[j]].xcord, city[list[j+1]].xcord, city[list[j]].ycord, city[list[j+1]].ycord);
		total = total + dist;
		if(j == (size-2))
		{
			dist = euc(city[list[j]].xcord, city[list[0]].xcord, city[list[j]].ycord, city[list[0]].ycord);
			total = total + dist;
		}
	}
	
	return total;
}

// mutation function
void mutate1(vector<int> &list) 
{
	iter_swap(list.begin()+1, list.begin()+2);
}

// mutation function //may need (pop *population)instead
void mutate2(vector<int> &list)
{
	random_shuffle(list.begin() + 1, list.end());
}

// selection sort to sort by fitness
void selectsort(struct pop *population, int size)
{
	int min = 0;
    for(int i = 0; i < size-1; i++)
    {
        //finding smallest element
        min = i;
        for (int j = i+1; j < size; j++)
        {
        	if (population[j].fitness < population[min].fitness)
        	{
        		min = j;
			}
		}
 
        //swapping the smallest with the first
        swap(population[min], population[i]);
    }
}


int main(){
	
	clock_t timePassed = clock(); //for execution time
	ifstream file;
	struct data city[48];
	vector<int> list;
	double dist = 0;
	int lines = 47;
	int travel = 0;	//number of cities to travel to
	int costs[48][48];
	double leastDist = 1000000000;
	float time = 0;
	int gen = 1;
	int populationSize = 20;
	struct pop population[populationSize];
	double fittest = 0;
	double opt[48];
	vector<int> r;
	int numGen = 10; //quit at this number of generations
	struct pop best;
	double b = 0;;
	
	file.open("assign6-in.txt");
		
	file >> travel;
		
	// reading data into struct 
	for(int i = 0; i <= lines; i++)
	{
		file >> city[i].id >> city[i].xcord >> city[i].ycord;
	}
	file.close();
	
	file.open("tsp4-in.txt");
	//put optimal cost from exhaustive search into array 
	for(int n = 0; n <= lines; n++)
    {
        file >> opt[n];
    }
    file.close();
	
	//populating vector with the correct number of cities to travel to
	for(int j = 0; j < travel; j++)
	{
		list.push_back(city[j].id);
	}
	
	//generate population
	for(int k = 0; k < populationSize; k++)
	{
		random_shuffle(list.begin()+1, list.end());
		population[k].route = list;
	}
	
	
	while(gen < numGen)
	{

		//generate c children using randomly chosen mutate fuction (if rand number is even then mutate1(), else mutate2())
		for(int m = 0; m < populationSize; m++)
		{		
			if((rand() % 2) == 0)
			{
				mutate1(population[m].route);
			}
			else
			{
				mutate2(population[m].route);
			}	
		}
		
		for(int w = 0; w < populationSize; w++)
		{
			r = population[w].route;
			population[w].cost = calculateDist(city, travel, r);
		}
	
		
		//evaluate fitness
		//optimal cost from exhaustive search / cost
		for(int p = 0; p < populationSize; p++)
		{
			population[p].fitness = opt[travel] / population[p].cost;
		}
		
		
		//sort all individuals by fitness - selection sort
		selectsort(population, populationSize);
		
		//return best		
		if(b < population[0].fitness)
		{
			cout << "Best so far " << endl;
			cout << "route: ";
			printList(population[0].route, travel);
    		cout << "cost: " << setprecision(1) << fixed << population[0].cost << endl;
    		cout << "fitness: " << setprecision(1) << fixed << population[0].fitness << endl;
    		cout << endl;
    		
    		b = population[0].fitness;
    		
    		best.route = population[0].route;
    		best.cost = population[0].cost;
    		best.fitness = population[0].fitness;
		}
		
		gen = gen + 1;
	}
	
	
	//best overall route found
	cout << "------------------------------------" << endl;
	cout << "Best route " << endl;
	cout << "route: ";
	printList(best.route, travel);
    cout << "cost: " << setprecision(1) << fixed << best.cost << endl;
    cout << "fitness: " << setprecision(1) << fixed << best.fitness << endl;
    cout << endl;
	
	//program execution time
	time = (float)(clock() - timePassed)/CLOCKS_PER_SEC;
	cout << fixed << "Program execution time: " << setprecision(3) << time;

	return 0;
}
