/* @Author
Student Name: <Muhammet Derviş Kopuz>
Student ID : <504201531>
Date: <20/04/2021> 
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <queue>
#include <list>

using namespace std;
#define INF 0x6FFFFFFF

//Class to hold building names and assigned numbers
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

//constructor
Building::Building(string name, int number) {
	this->name = name;
	this->number = number;
}

//empty constructor
Building::Building() {
	
}

//getter
string Building::getname() {
	return name;
}

//getter
int Building::getnumber() {
	return number;
}

//class to hold every edge
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

//constructor
Edge::Edge(Building source, Building dest, int weight) {
	this->source = source;
	this->dest = dest;
	this->weight = weight;
}

//getter
Building Edge::getSource() {return source;}

//getter
Building Edge::getDest() {return dest;}

//getter
int Edge::getWeight() {return weight;}

class Graph {
	private:
		//number of vertices in the graph
		int numberOfVertices;
		//Every building and its weight is stored in an adjacency list
		list< pair<Building, int> > *adj;
		
	public:
		Graph(int numberOfVertices);
		void AddEdge(Building source, Building destination, int weight);
		void FindPrimMST(Building startVertex, vector<Building>* buildings, vector<Edge>* edges);
		
};

// constructor for the graph
Graph::Graph(int numberOfVertices) {
  this->numberOfVertices = numberOfVertices;
  adj = new list< pair<Building, int> >[numberOfVertices];
}


void Graph::AddEdge(Building source, Building destination, int weight) {
	//adds the corresponding building and weight pair to the adjacency list of both source and destination
  adj[source.getnumber()].push_back(make_pair(destination, weight));
	adj[destination.getnumber()].push_back(make_pair(source, weight));
}

//get building name by assigned building number
string getBuilding(int buildingNumber, vector<Building>* buildings) {
	vector<Building>::iterator it;
	//traverse buildings
	for (it = buildings->begin(); it != buildings->end(); ++it) {
		if((*it).getnumber() == buildingNumber) {
			return (*it).getname();
		}
	}
	return NULL;
}

//get building by building name
Building getBuildingByName(string name, vector<Building>* buildings) {
	vector<Building>::iterator it;
	for (it = buildings->begin(); it != buildings->end(); ++it) {
		if((*it).getname() == name) {
			return (*it);
		}
	}
	return *it;
}

//check if there are enemies close by. Rule 5
bool enemiesCloseBy(int destNumber, list< pair<Building, int> > *adj) {
	list< pair<Building, int> >::iterator i;
	//traverse adjacent buildings of the destination
	for (i = adj[destNumber].begin(); i != adj[destNumber].end(); ++i){
		if (((*i).first.getname().compare(0,1,"E") == 0) && (*i).second < 5) {
			return true;
		}
	}
	return false;
}

//check if destination is enemy territory
bool isEnemyTerritory(Building destination) {
	if (destination.getname().compare(0,1,"E") == 0) {
		return true;
	}
	return false;
}

void Graph::FindPrimMST(Building startvertex, vector<Building>* buildings, vector<Edge>* edges) {
	priority_queue< pair<int, int>, vector< pair<int, int> >, greater< pair<int, int> > > pq;

	Building src = startvertex;

	//keep track of all distances for every vertex, initialize distances as infinite
	vector<int> dist(numberOfVertices, INF);

	//keep a parent array to indicate which vertex is connected to which vertex
	//no parent = -1
	vector<int> parent(numberOfVertices, -1);

	//uzaklık, vertexNum 
	//push source vertex in to priority queue
	//initialize source's distance as 0 since it is the starting point
  pq.push(make_pair(0, startvertex.getnumber()));
	//initialize source's distance as 0 since it is the starting point
  dist[src.getnumber()] = 0;

	//while priority queue is not empty
  while (!pq.empty()) {
		//priority queue holds (distance, building number)
		//it is ordered by the first variable
		//get the second variable for the element which has the smallest distance
		int sourceNumber = pq.top().second;
		//get the name of the source
		string sourceName = getBuilding(sourceNumber, buildings);
		//pop the first element from queue
		pq.pop();


		list< pair<Building, int> >::iterator i;
		//traverse over all adjacent buildings for the source
		for (i = adj[sourceNumber].begin(); i != adj[sourceNumber].end(); ++i)
		{
				//get destination building
				Building destination = (*i).first;
				//get destination distance
				int destinationWeight = (*i).second;

				//Rule 3
				if(isEnemyTerritory(destination)) {
					continue;
				}

				//check if destination is operated on before using the MST vector.
				//if the total distance can be reduced  
				if (dist[destination.getnumber()] > dist[sourceNumber] + destinationWeight)
				{
					//if there is an enemy within 5 blocks check. Rule 5
					if (enemiesCloseBy(destination.getnumber(), adj)) {
						continue;
					}
					//update the distance
					dist[destination.getnumber()] = dist[sourceNumber] + destinationWeight;

					//push the pair consisting of (new distance for destination, destination)
					pq.push(make_pair(dist[destination.getnumber()], destination.getnumber()));
					//assign source as parent to destination / connect destination and source
					parent[destination.getnumber()] = sourceNumber;
				}
		}
	}

	//print path
	//get the Mo building
	Building lastBuilding = getBuildingByName("Mo",buildings);
	int destNumber = lastBuilding.getnumber();
	vector<int> shortestPath;
	int sourceNumber = 0;
	//get Mo's distance
	int totalLength = dist[getBuildingByName("Mo",buildings).getnumber()];
	//while destination not equals to start
	//traverse the parent vector from end to beginning
	while (destNumber != sourceNumber) {
		shortestPath.push_back(destNumber);
		destNumber = parent[destNumber];
	}
	shortestPath.push_back(sourceNumber);
	//reverse the list so Mo goes to the end of the list
	reverse(shortestPath.begin(), shortestPath.end());
	vector<int>::iterator it;
	//traverse the shortest path and print
	for (it = shortestPath.begin(); it != shortestPath.end(); ++it) {
		cout << getBuilding(*it,buildings) << " "; 
	}
	cout << totalLength << "\n";


}


int main() {

  string fname;
  //fname = "path_info_1.txt";
	//get file name
  cin >> fname;
  ifstream city_plan(fname);

  string source;
  string dest;
  string weight;
  string line;

	int buildingCounter = 0;
	//init buildings vector
	vector<Building>* buildings = new vector<Building>;
	//init edges vector
	vector<Edge>* edges = new vector<Edge>;
	Building startVertex; 

	//read file
  while (getline(city_plan, line)) {
    stringstream ss(line);
    getline(ss, source, ',');
    getline(ss, dest, ',');
    getline(ss, weight, ',');

		Building* newBuildingS;
		Building* newBuildingD;

		//if building counter = 0, directly push buildings in to buildings vector.
		if(buildingCounter == 0) {
			newBuildingS = new Building(source, buildingCounter);
			startVertex = *newBuildingS;
			buildings->push_back(*newBuildingS);
			buildingCounter++;
			newBuildingD = new Building(dest, buildingCounter);
			buildings->push_back(*newBuildingD);
			buildingCounter++;

			Edge* newEdge = new Edge(*newBuildingS, *newBuildingD, stoi(weight));
			//push new edge in to edges vector
			edges->push_back(*newEdge);
		} else {
			vector<Building>::iterator buildIt;
			bool sourcePresent = false;
			bool destPresent = false;

			//check the buildings in the new loop. If they are new or existing
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

			//if new building push to buildings vector
			if (!sourcePresent){
				newBuildingS = new Building(source, buildingCounter);
				buildings->push_back(*newBuildingS);
				buildingCounter++;
			}
			//if new building push to buildings vector
			if (!destPresent) {
				newBuildingD = new Building(dest, buildingCounter);
				buildings->push_back(*newBuildingD);
				buildingCounter++;
			}

			//push the new edge to edges vector
			Edge* newEdge = new Edge(*newBuildingS, *newBuildingD, stoi(weight));
			edges->push_back(*newEdge);
		}
  }
	
	//initialize the graph with number of vertices equal to buidings size
	Graph g(buildings->size());

	//add every edge to the graph
	vector<Edge>::iterator ite;
	for (ite = edges->begin(); ite != edges->end(); ++ite ) {
		g.AddEdge((*ite).getSource(), (*ite).getDest(), (*ite).getWeight());
	}
	
	//find the shortest path and print
	g.FindPrimMST(startVertex, buildings, edges);
	
	return 0;
}