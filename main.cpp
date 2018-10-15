#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <math.h>
#include <fstream>
using namespace std;

#define GRAPH_VERTICES 10
#define MAX_WEIGHT 20

// each vertex tracks which vertices it's connected to, the weight of each corresponding edge, and the number of connected vertices
struct vertex
{
	vertex * connected_vertices[GRAPH_VERTICES]; // vertices connected to this one
	int connected_vertices_weights[GRAPH_VERTICES]; // corresponding weights for vertex connections
	int connected_vertices_count; // number of connections
	int id; // ID number for vertex
};

class totalGraph
{
public:

	vertex * graph[GRAPH_VERTICES]; // an aray of vertices for the whole graph
	totalGraph(); // constructor to make a random graph

	bool isGraphConnected(); // check if the graph is fully connected
	void connected(bool *found_vertices); // used by isGraphConnected to check what vertices are connected to the partition given by found_vertices array

};

totalGraph::totalGraph()
{

	// initialize the vertices in the graph
	for (int i = 0; i < GRAPH_VERTICES; i++)
	{
		graph[i] = new vertex; // make a new vertex
		graph[i]->connected_vertices_count = 0; // has 0 connections
		graph[i]->id = i; // assign id
	}

	while (!isGraphConnected())
	{
		for (int i = 0; i < GRAPH_VERTICES; i++)
		{

			int connections = rand() % (GRAPH_VERTICES - 1) + 1; // determine number of connected vertices
			vertex * current_vertex = graph[i]; // for easier naming/assignment

			cout << "Vertex " << i << " wll have " << connections << " connections." << endl;

			// for all connections not made
			for (int j = current_vertex->connected_vertices_count; j < connections; j++)
			{
				vertex * connected_vertex = graph[rand() % GRAPH_VERTICES]; // a vertex to connect to

				bool found = false;
				// while vertex is already connected to or is itself
				while (!found || connected_vertex->id == current_vertex->id)
				{
					found = true; // assume a valid connected vertex is found
					connected_vertex = graph[rand() % GRAPH_VERTICES];
					for (int k = 0; k < current_vertex->connected_vertices_count; k++)
						if (connected_vertex->id == current_vertex->connected_vertices[k]->id)
							found = false;
				}

				// Output the connection
				cout << "Connecting vertex " << i << " and " << connected_vertex->id << endl;

				// connect the vertices together since this is a undirected graph
				current_vertex->connected_vertices[current_vertex->connected_vertices_count] = connected_vertex;
				connected_vertex->connected_vertices[connected_vertex->connected_vertices_count] = current_vertex;

				// add weight to the edge
				int weight = rand() % MAX_WEIGHT + 1;
				current_vertex->connected_vertices_weights[current_vertex->connected_vertices_count++] = weight;
				connected_vertex->connected_vertices_weights[connected_vertex->connected_vertices_count++] = weight;
			}
		}
	}
	// Check if the graph is connected
	if (isGraphConnected())
		cout << "Graph is connected" << endl;
}

void totalGraph::connected(bool *found_vertices)
{
	for (int i = 0; i < GRAPH_VERTICES; i++)
		// if the vertex is already found
		if (found_vertices[i])
			for (int j = 0; j < graph[i]->connected_vertices_count; j++)
				// if the vertex has not already been reached, mark it as reached
				if (!found_vertices[graph[i]->connected_vertices[j]->id])
					found_vertices[graph[i]->connected_vertices[j]->id] = true;
}



bool totalGraph::isGraphConnected()
{
	// Start with 1 found vertex
	bool found_vertices[GRAPH_VERTICES];
	for (int i = 0; i < GRAPH_VERTICES; i++)
		found_vertices[i] = false;

	found_vertices[0] = true;

	// connect all vertices that are connected to vertices already found
	// this only needs to run GRAPH_VERTICES times as its impossible a vertex to be farther away and still connected
	for (int i = 0; i < GRAPH_VERTICES; i++)
		connected(found_vertices);

	// Check if any vertex was not found
	for (int i = 0; i < GRAPH_VERTICES; i++)
		if (!found_vertices[i])
			return false;

	return true;
}

int main()
{
	ofstream out_file;
	out_file.open("results.txt");

	// This is all you need torandomly seed based on time
	srand(time(NULL));

	totalGraph base;
	out_file.close();
	//the following lines are needed to keep the terminal open until you want the program to end when running from visual studio
	// cout << "exiting...";
	// cin >> exit_value;
	return 0;
}


