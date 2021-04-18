/* @Author
Student Name: <Muhammet Derviş Kopuz>
Student ID : <504201531>
Date: <06/04/2021> 
Please compile the project using -std=c++11 flag
<g++ sourceCode.cpp -o hw1 -std=c++11>
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <list>
#include <map>

using namespace std;
#define INF 0x3f3f3f3f

class Building {
	private:
		string name;
		int number;

	public:
		Building(string name, int number);
		Building();
		string getname();
		int getnumber();
};

Building::Building(string name, int number) {
	this->name = name;
	this->number = number;
}

Building::Building() {
	
}

string Building::getname() {
	return name;
}

int Building::getnumber() {
	return number;
}

class Edge {
	private:
		Building source;
		Building dest;
		int weight;

	public:
		Edge(Building source, Building dest, int weight);
		Building getSource();
		Building getDest();
		int getWeight();

};

Edge::Edge(Building source, Building dest, int weight) {
	this->source = source;
	this->dest = dest;
	this->weight = weight;
}

Building Edge::getSource() {return source;}

Building Edge::getDest() {return dest;}

int Edge::getWeight() {return weight;}

class Graph {
	private:
		int numberOfVertices;
		// In a weighted graph, we need to store vertex
    // and weight pair for every edge
		list< pair<Building, int> > *adj;
		
	public:
		// Graph specific methods
		Graph(int numberOfVertices);
		void AddEdge(Building source, Building destination, int weight);
		void FindPrimMST(Building startVertex, vector<Building>* buildings, vector<Edge>* edges);
		
};

// Constructor only sets the number of vertices
Graph::Graph(int numberOfVertices) {
  this->numberOfVertices = numberOfVertices;
  adj = new list< pair<Building, int> >[numberOfVertices];
}


// Adds an edge to the graph
void Graph::AddEdge(Building source, Building destination, int weight) {
	// Converts the letter of a vertice to a numeric representation: (source - 'A')
	//source ve dest için int koyulması lazım
  adj[source.getnumber()].push_back(make_pair(destination, weight));
	adj[destination.getnumber()].push_back(make_pair(source, weight));
}


string getBuilding(int buildingNumber, vector<Building>* buildings) {
	vector<Building>::iterator it;
	for (it = buildings->begin(); it != buildings->end(); ++it) {
		if((*it).getnumber() == buildingNumber) {
			return (*it).getname();
		}
	}
	return NULL;
}

Building getBuildingByName(string name, vector<Building>* buildings) {
	vector<Building>::iterator it;
	for (it = buildings->begin(); it != buildings->end(); ++it) {
		if((*it).getname() == name) {
			return (*it);
		}
	}
	return *it;
}

bool enemiesCloseBy(int destNumber, list< pair<Building, int> > *adj) {
	list< pair<Building, int> >::iterator i;
	// 'i' is used to get all adjacent vertices of a vertex
	//list< pair<Building, int> >::iterator i;
	for (i = adj[destNumber].begin(); i != adj[destNumber].end(); ++i){
		if (((*i).first.getname().compare(0,1,"E") == 0) && (*i).second < 5) {
			return true;
		}
	}
	return false;
}

bool isEnemyTerritory(Building destination) {
	if (destination.getname().compare(0,1,"E") == 0) {
		return true;
	}
	return false;
}

void Graph::FindPrimMST(Building startvertex, vector<Building>* buildings, vector<Edge>* edges) {
	priority_queue< pair<int, int>, vector< pair<int, int> >, greater< pair<int, int> > > pq;

	Building src = startvertex;

	// Create a vector for keys and initialize all
    // keys as infinite (INF)
	vector<int> key(numberOfVertices, INF);
	/*
	vector<int>::iterator i;
	int iCounter = 0;
	for (i = key.begin(); i != key.end(); ++i){
		cout << iCounter << ": " << *i <<endl;
		iCounter++;
	}*/

	vector<int> parent(numberOfVertices, -1);
	/*
	vector<int>::iterator is;
	iCounter = 0;
	for (is = parent.begin(); is != parent.end(); ++is){
		cout << iCounter << ": " << *is <<endl;
		iCounter++;
	}*/

	// Insert source itself in priority queue and initialize
  // its key as 0.
	//uzaklık, vertexNum 
  pq.push(make_pair(0, startvertex.getnumber()));
  key[src.getnumber()] = 0;

	/* Looping till priority queue becomes empty */
  while (!pq.empty()) {
		// The first vertex in pair is the minimum key
		// vertex, extract it from priority queue.
		// vertex label is stored in second of pair (it
		// has to be done this way to keep the vertices
		// sorted key (key must be first item
		// in pair)
		int sourceNumber = pq.top().second;
		string sourceName = getBuilding(sourceNumber, buildings);
		pq.pop();


		list< pair<Building, int> >::iterator i;
		// 'i' is used to get all adjacent vertices of a vertex
		//list< pair<Building, int> >::iterator i;
		for (i = adj[sourceNumber].begin(); i != adj[sourceNumber].end(); ++i)
		{
				// Get vertex label and weight of current adjacent
				// of u.
				Building destination = (*i).first;
				int destinationWeight = (*i).second;

				//Rule 3
				if(isEnemyTerritory(destination)) {
					continue;
				}

				//  If v is not in MST and weight of (u,v) is smaller
				// than current key of v
				if (key[destination.getnumber()] > key[sourceNumber] + destinationWeight)
				{
					if (enemiesCloseBy(destination.getnumber(), adj)) {
						continue;
					}
					// Updating key of v
					key[destination.getnumber()] = key[sourceNumber] + destinationWeight;
					/*
					vector<int>::iterator it;
					int iCounter = 0;
					cout << "-------key-------" << endl;
					for (it = key.begin(); it != key.end(); ++it){
						cout << iCounter << ": " << *it <<endl;
						iCounter++;
					}
					*/

					pq.push(make_pair(key[destination.getnumber()], destination.getnumber()));
					parent[destination.getnumber()] = sourceNumber;
					/*
					iCounter = 0;
					cout << "-------parent-------" << endl;
					for (is = parent.begin(); is != parent.end(); ++is){
						cout << iCounter << ": " << *is <<endl;
						iCounter++;
					}
					*/
				}
		}
	}

	// Print edges of MST using parent array
	/*
	for (int i = 0; i < numberOfVertices; ++i) {
		int buildingNum = i;
		cout << getBuilding(buildingNum,buildings) << ": " << key[i] <<endl;
	}*/

	//print path
	Building lastBuilding = getBuildingByName("Mo",buildings);
	int destNumber = lastBuilding.getnumber();
	vector<int> shortestPath;
	int sourceNumber = 0;
	int totalLength = key[getBuildingByName("Mo",buildings).getnumber()];
	//while destination not equals to start
	while (destNumber != sourceNumber) {
		shortestPath.push_back(destNumber);
		destNumber = parent[destNumber];
	}
	shortestPath.push_back(sourceNumber);
	reverse(shortestPath.begin(), shortestPath.end());
	vector<int>::iterator it;
	for (it = shortestPath.begin(); it != shortestPath.end(); ++it) {
		cout << getBuilding(*it,buildings) << " "; 
	}
	cout << totalLength << "\n";


}


int main() {

  string fname;
  fname = "path_info_1.txt";
  //cin >> fname;
  ifstream city_plan(fname);

  string source;
  string dest;
  string weight;
  string line;

	int buildingCounter = 0;
	vector<Building>* buildings = new vector<Building>;
	vector<Edge>* edges = new vector<Edge>;
	Building startVertex; 

  while (getline(city_plan, line)) {
    stringstream ss(line);
    getline(ss, source, ',');
    getline(ss, dest, ',');
    getline(ss, weight, ',');

		Building* newBuildingS;
		Building* newBuildingD;


		if(buildingCounter == 0) {
			newBuildingS = new Building(source, buildingCounter);
			startVertex = *newBuildingS;
			buildings->push_back(*newBuildingS);
			buildingCounter++;
			newBuildingD = new Building(dest, buildingCounter);
			buildings->push_back(*newBuildingD);
			buildingCounter++;

			Edge* newEdge = new Edge(*newBuildingS, *newBuildingD, stoi(weight));
			edges->push_back(*newEdge);
			//g.AddEdge(*newBuildingS, *newBuildingD, stoi(weight));
		} else {
			vector<Building>::iterator buildIt;
			bool sourcePresent = false;
			bool destPresent = false;
			for (buildIt = buildings->begin(); buildIt != buildings->end(); ++buildIt ) {
				if ((*buildIt).getname() == source) {
					sourcePresent = true;
					*newBuildingS = *buildIt;
				}
				if ((*buildIt).getname() == dest) {
					destPresent = true;
					*newBuildingD = *buildIt;
				}
			}

			if (!sourcePresent){
				newBuildingS = new Building(source, buildingCounter);
				buildings->push_back(*newBuildingS);
				buildingCounter++;
			}
			if (!destPresent) {
				newBuildingD = new Building(dest, buildingCounter);
				buildings->push_back(*newBuildingD);
				buildingCounter++;
			}

			Edge* newEdge = new Edge(*newBuildingS, *newBuildingD, stoi(weight));
			edges->push_back(*newEdge);
		}


    //std::cout << source << " " << dest << " " << weight <<endl;
  }
	// Create the graph with the number of vertices that it will contain
	/*
	vector<Building>::iterator it;
	for (it = buildings->begin(); it != buildings->end(); ++it ) {
		std::cout << (*it).getname() << endl;

	}*/

	Graph g(buildings->size());

	vector<Edge>::iterator ite;
	for (ite = edges->begin(); ite != edges->end(); ++ite ) {
		//std::cout << (*ite).getSource().getname() << " " << (*ite).getDest().getname() << " " << (*ite).getWeight() << endl;
		g.AddEdge((*ite).getSource(), (*ite).getDest(), (*ite).getWeight());
	}
	
	/*
  Building* startVertex = new Building("GP",0);
  Building* hipp = new Building("hipp",1);
  Building* ch1 = new Building("ch1",2);
  Building* ch2 = new Building("ch2",3);
  Building* hp1 = new Building("hp1",4);
  Building* hp2 = new Building("hp2",5);

	buildings->push_back(*startVertex);
	buildings->push_back(*hipp);
	buildings->push_back(*ch1);
	buildings->push_back(*ch2);
	buildings->push_back(*hp1);
	buildings->push_back(*hp2);
  //Building* startVertex = new Building("GP",0);
  //Building* startVertex = new Building("GP",0);
	// Add all of the edges to the graph
	g.AddEdge(*startVertex,*hipp, 1);
	g.AddEdge(*startVertex, *ch1, 9);
	g.AddEdge(*startVertex, *ch2, 10);
	g.AddEdge(*startVertex, *hp1, 1);
	g.AddEdge(*ch1, *hp1, 1);
	g.AddEdge(*ch1, *hp2, 1);
	g.AddEdge(*ch2, *hp1, 100);
	g.AddEdge(*ch2, *hp2, 100);
	*/
	// Print all the edges in the graph
	g.FindPrimMST(startVertex, buildings, edges);
	
	return 0;
}