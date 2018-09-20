#include <iostream>
#include <stdlib.h>
#include <stdio.h>
using namespace std;


int numberOfBins;
int binMaxWeight;
int populationSize; 
int numberOfObjects;
int generations;

struct object {
	int value;
	int weight;
	int knapsack;
};

object ** population;

int fitness(object *chromosome)
{
	// cout << "Testing Fitness" << endl;
	
	int fitnessScore = 0;
	for (int j = 0; j < numberOfObjects; j++)
	{
		// cout << "Current Object is " << j << ", in knapsack #";
		// cout << chromosome[j].knapsack << endl;
		if (chromosome[j].knapsack != 0)
			fitnessScore += chromosome[j].value;
	}
	
	// cout << "Fitness is " << fitnessScore << endl;

	return fitnessScore;
}

// make a new chromosome from chromosome1 and chromosome2 by taking a random number of the objects from each parent chromosome
object * crossover(object *chromosome1, object *chromosome2)
{
	// Make a new chromosome
	object * offspring = new object[numberOfObjects];

	// pick a random divide
	int divide = rand() % numberOfObjects;
	int i;

	// Before the divide, copy from the 1st chromosome
	for (i = 0; i < divide; i++)
		offspring[i] = chromosome1[i];

	// After the divide, copy from the 2nd chromosome
	for (i; i < numberOfObjects; i++)
		offspring[i] = chromosome2[i];

	return offspring;
}

// Make a valid chromosome from a potentially invalid chromosome
object *  fixChromosome(object *chromosome)
{
	int *currentWeights = new int[numberOfBins];
	
	object * fixedChromosome = new object[numberOfObjects];

	// For each bin in, make sure the chromosome is not over weight
	for (int i = 0; i < numberOfObjects; i++)
	{
		// Add the weight to the knapsack if it's in a knapsack
		currentWeights[chromosome[i].knapsack] += chromosome[i].weight;
		fixedChromosome[i] = chromosome[i];

		// If a knapsack exceeds capacity
		if (currentWeights[chromosome[i].knapsack] > binMaxWeight && chromosome[i].knapsack != 0)
		{
			// remove the object from the knapsack
			currentWeights[chromosome[i].knapsack] -= chromosome[i].weight;
			fixedChromosome[i].knapsack = 0;
		}
	}
	
	delete chromosome;
	
	return fixedChromosome;
}

// make a copy of the chromosome but mutate 1 position
object * mutation(object *chromosome)
{
	// Make a new chromosome
	object * offspring = new object[numberOfObjects];

	for (int i = 0; i < numberOfObjects; i++)
		offspring[i] = chromosome[i];

	// pick a random divide
	int change = (rand() % numberOfObjects) + 1;
	
	offspring[change].knapsack = rand() % numberOfBins;
	
	// Make sure the chromosome it valid
	offspring = fixChromosome(offspring);

	return offspring;
}

void runGeneration()
{
	object * parent = NULL;

	// generate a blank new generation
	object ** newGeneration = new object *[populationSize];
	int newGenPopSize = 0;

	// Do tournament selection to generate a new population
	for (int i = 0; i < (2 * populationSize) - 1; i++)
	{
		// randomly get a challenger
		int challenger = (rand() % populationSize);
		int challenger2 = (rand() % populationSize);
		
		object * chromosome1 = population[challenger];
		object * chromosome2 = population[challenger2];
		
		// if there was a previous winner
		if (parent != NULL)
		{
			// crossover and mutate the child
			newGeneration[newGenPopSize] = 
				mutation(crossover(parent, ((fitness(chromosome1) > fitness(chromosome2)) ? chromosome1 : chromosome2)));

			// increase population size and remove previous winner
			newGenPopSize++;
			parent = NULL;
		}
		else // set the winner as previous winner
			parent = (fitness(chromosome1) > fitness(chromosome2)) ? chromosome1 : chromosome2;
	}

	// take the best chromosome from last generation
	int bestChromosome = 0;
	int bestFitness = fitness(population[0]);
	for (int i = 0; i < populationSize; i++)
	{
		int newFitness = fitness(population[i]);
		if (newFitness > bestFitness)
		{
			bestChromosome = i;
			bestFitness = newFitness;
		}
	}

	// Output Information about generation
	newGeneration[newGenPopSize] = population[bestChromosome];
	cout << "Best Chromosome from Last Generation: " << endl;
	object * best = population[bestChromosome];
	for (int i = 0; i < numberOfObjects; i++)
	{
		cout << "Knapsack: "<< best[i].knapsack << " Value: "<< best[i].value << " Weight: "<< best[i].weight << endl;
	}
	
	cout << "Fitness: " << bestFitness << endl;
	
	population = newGeneration;
}

// make a new randomized chromosome
object * generateRandomChromosome(int * value, int * weight)
{
	object * chromosome = new object[numberOfObjects];

	for (int i = 0; i < numberOfObjects; i++)
	{
		chromosome[i].knapsack = rand() % numberOfBins;
		chromosome[i].value = value[i];
		chromosome[i].weight = weight[i];
		// cout << chromosome[i].knapsack << ", " << chromosome[i].value << ", " << chromosome[i].weight << " | ";
	}
	// cout << endl;

	return chromosome;
}


int main()
{
	// Get the instance
	cout << "Enter a Population Size: ";
	cin >> populationSize;
	
	cout << "Enter number of generations: ";
	cin >> generations;
	
	cout << "Enter the number of objects: ";
	cin >> numberOfObjects;
	
	cout << "Enter the number of knapsacks: ";
	cin >> numberOfBins;
	
	cout << "Enter the maximum weight of a knapsack: ";
	cin >> binMaxWeight;
	
	int seed = 0;
	
	srand(seed);
	
	int *values = new int[numberOfObjects];
	int *weights = new int[numberOfObjects];
	
	char inputValues;
	
	cout << "Input values and weights? (y/n) ";
	cin >> inputValues;
	

	for (int i = 0; i < numberOfObjects; i++)
	{
		if(inputValues = 'y')
		{
			cout << "Enter a value for object " << i << ": ";
			cin >> values[i];
			
			cout << "Enter a weight for object " << i << ": ";
			cin >> weights[i];
		}
		else 
		{
			values[i] = rand() % 10;
			weights[i] = rand() % binMaxWeight;
		}
	}

	// generate a random starting population
	population = new object * [populationSize];

	for (int i = 0; i < populationSize; i++)
	{
		population[i]  = generateRandomChromosome(values, weights);
		population[i]  = fixChromosome(population[i]);
	}

	// run the generations
	for(int i = 0; i < generations; i++)
	{
		cout << "Running generation " << i << endl;
		runGeneration();
	}

    return 0;
}
