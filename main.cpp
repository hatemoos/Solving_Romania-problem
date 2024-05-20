#include <bits/stdc++.h>
using namespace std;
const long long OO = 1e18;
int numberOfCities, numberOfEdges;
string source, destination;
vector<vector<pair<int,int>>>adjacent;
map<string, int>fromNameToId;
vector<string>fromIdToName;
int sourceId, destinationId;
vector<pair<int, long long>>previous;
set<string> cities;
map<pair<string, string>, int>edges;


void buildGraph()
{
    int id = 1;
    fromIdToName = vector<string>(int(cities.size()) + 1);
    for (auto city : cities)
    {
        fromNameToId[city] = id;
        fromIdToName[id] = city;
        id++;
    }
    sourceId = fromNameToId[source];
    destinationId = fromNameToId[destination];
    for (auto it : edges)
    {
        int city1 = fromNameToId[it.first.first];
        int city2 = fromNameToId[it.first.second];
        int cost = it.second;
        adjacent[city1].push_back({ city2,cost });
        adjacent[city2].push_back({ city1,cost });
    }
}


void inputGraph()
{
    cout << "Enter numberOfCities:";
    cin >> numberOfCities;
    cout << "Enter numberOfEdges:";
    cin >> numberOfEdges;
    adjacent = vector<vector<pair<int,int>>>(numberOfCities + 1);
    for (int i = 1;i <= numberOfCities;i++)
    {
        cout<<"Enter city number " << i << " :";
        string c;
        cin >> c;
        if (cities.count(c))
        {
            cout << "You should input different cities\n";
            i--;
            continue;
        }
        cities.insert(c);
    }
    for (int i = 1;i <= numberOfEdges;i++)
    {
        string firstCity, secondCity;
        int cost;
        cout << "Edge number "<<i<<" ...\n";
        cout << "Enter first city:";
        cin >> firstCity;
        cout << "Enter second city:";
        cin >> secondCity;
        cout << "Enter the Cost of the path between them:";
        cin >> cost;
        if (!cities.count(firstCity) || !cities.count(secondCity))
        {
            cout << "please enter cities from the provided set\n";
            i--;
            continue;
        }
        if (cost < 0 || cost>1e9)
        {
            cout << "cost must be between 0 and billion\n";
            i--;
            continue;
        }
        if(firstCity>secondCity)swap(firstCity,secondCity);
        if (edges.count({ firstCity,secondCity }))edges[{firstCity, secondCity}] = min(edges[{firstCity, secondCity}], cost);
        else edges[{firstCity, secondCity}] = cost;
    }
    while (true)
    {
        cout << "Enter source city:";
        cin >> source;
        cout << "Enter destination city:";
        cin >> destination;
        if (cities.count(source) && cities.count(destination))break;
        cout << "please enter cities from the provided set\n";
    }
}


long long calculateCost()
{
    previous = vector<pair<int, long long>>(numberOfCities + 1);
    vector<long long>optimalCost(numberOfCities + 1,OO);
    priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<>>cur;
    optimalCost[sourceId] = 0;
    cur.push({ 0,sourceId });
    while (cur.size())
    {
        long long cost = cur.top().first;
        int city = cur.top().second;
        cur.pop();

        if (optimalCost[city] < cost)continue;

        for (auto child : adjacent[city])
        {
            if (optimalCost[child.first] > child.second + cost)
            {
                previous[child.first] = { city, child.second };
                optimalCost[child.first] = cost + child.second;
                cur.push({ optimalCost[child.first],child.first });
            }
        }
    }
    return optimalCost[destinationId];
}
vector<pair<int, int>>path;


void buildPath()
{
    int cur = destinationId;
    while (cur != sourceId)
    {
        path.push_back( previous[cur] );
        cur = previous[cur].first;
    }
    reverse(path.begin(), path.end());
}


void printPath()
{
    for (auto it : path)
    {
        cout << fromIdToName[it.first]<<"---" << it.second << "--->";
    }
    cout << destination;
}


int main()
{
    inputGraph();
    buildGraph();
    long long shortestPath = calculateCost();
    cout << endl;
    if (shortestPath == OO)
        cout << "Can't reach the destination, there is no path between them\n";
    else
    {
        cout << "The cost of the shortest path between source and destination equals to:" << shortestPath << endl;
        cout << endl;
        buildPath();
        cout << "The shortest path is:\n";
        printPath();
        cout << endl;
    }
}
