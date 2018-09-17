#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <random>
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
	int fitnessScore = 0;
	for (int i = 0; i < sizeof(chromosome); i++)
	{
		if (chromosome[i].knapsack != 0)
			fitnessScore += chromosome[i].value;
	}

	return fitnessScore;
}

// make a new chromosome from chromosome1 and chromosome2 by taking a random number of the objets from each parent chromosome
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

// make a copy of the chromosome but mutate 1 position
object * mutation(object *chromosome)
{
	// Make a new chromosome
	object * offspring = new object[numberOfObjects];

	for (int i = 0; i < sizeof(chromosome); i++)
		offspring[i] = chromosome[i];

	// pick a random divide
	int change = rand() % numberOfObjects;
	
	offspring[change] = rand() % numberOfBins;

	return offspring;
}

object *  fixChromosome(object *chromosome)
{
	int *currentWeights = new int[numberOfBins];

	// For each bin in, make sure the chromosome is not over weight
	for (int i = 0; i < sizeof(chromosome); i++)
	{
		// Add the weight to the knapsack if it's in a knapsack
		currentWeights[chromosome[i].knapsack] += chromosome[i].weight;

		// If a knapsack exceeds capacity
		if (currentWeights[chromosome[i].knapsack] > binMaxWeight && chromosome[i].knapsack != 0)
		{
			// remove the object from the knapsack
			currentWeights[chromosome[i].knapsack] -= chromosome[i].weight;
			chromosome[i].knapsack = 0;
		}
	}
}

void runGeneration()
{
	object * parent = NULL;

	// generate a blank new generation
	object ** newGeneration = new object *[populationSize];
	int newGenPopSize = 0;

	// Do tournament selection to generate a new population
	for (int i = 0; i < 2 * populationSize - 1; i++)
	{
		// randomly get challengers
		int challenger = (rand() % populationSize);
		int challenger2 = (rand() % populationSize);

		// if there was a previous winner
		if (parent != NULL)
		{
			// crossover and mutate the child
			newGeneration[newGenPopSize] = 
				mutation(crossover(parent, ((population[challenger2] > population[challenger]) ? population[challenger2] : population[challenger])));

			// increase population size and remove previous winner
			newGenPopSize++;
			parent = NULL;
		}
		else // set the winner as previous winner
			parent = (population[challenger2] > population[challenger]) ? population[challenger2] : population[challenger];
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

	newGeneration[newGenPopSize] = population[bestChromosome];
	
	// TODO: display info about generation
}

object * generateRandomChromosome()
{
	object * chromosome = new object[numberOfObjects];

	for (int i = 0; i < numberOfObjects; i++)
	{
		chromosome[i] = rand() % numberOfBins;
	}

	return chromosome;
}


int main()
{
	// generate a random starting population
	population = new object * [populationSize];

	for (int i = 0; i < populationSize; i++)
	{
		population[i] = generateRandomChromosome();
		population[i] = fixChromosome(population[i]);
	}

	// run the generations
	for(int i = 0; i < generations; i++)
	{
		runGeneration();
	}

    return 0;
}
