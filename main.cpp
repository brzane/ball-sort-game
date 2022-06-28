#include <bits/stdc++.h>
using namespace std;
using Grid = vector<string>;
//Creating the Grid of the game
Grid makeGrid(string tubes[], int numberOfTubes)
{
	Grid grid;
	for (int i = 0; i < numberOfTubes; i++)
        //pushing strings to the vector
		grid.push_back(tubes[i]);

	return grid;
}
//getting the tallest string size to make as default tubes height
int getTubeHeight(Grid grid)
{   //normal max finding strategy
	int max = 0;
	for (auto tube : grid)
		if (max < tube.size())
			max = tube.size();
	return max;
}
//printing the grid to user in a friendly way
void printGrid(Grid grid, int height){
    for (int j=height-1; j>=0; j--){
        for(int i=0;i<grid.size();i++){
            //if the string is less than the tube height print 0
            grid[i].size()>j?cout<<grid[i][j]:cout<<'-';
            cout<<" ";
    }
    cout<<endl;
}
}
//checking if the grid is solved
bool isSolved(Grid grid, int tubeHeight)
{

	for (auto tube : grid) {
        //for non empty tubes
		if (!tube.size())
			continue;
        //if one tube is less than the tube height return false
		else if (tube.size() < tubeHeight)
			return false;
        //if the number of balls with same color equals the tube height return true else return false
		else if (std::count(tube.begin(),
							tube.end(),
							tube[0])
				!= tubeHeight)
			return false;
	}
	return true;
}
//Game Rules| checking if we can move the ball from one place to another
bool canMove(string tube1,string tube2,int height)
{
	// Can't move from an empty tube  or to a full tube
	if (tube1.size() == 0|| tube2.size() == height)		return false;

	int colors= std::count(tube1.begin(),tube1.end(),tube1[0]);

	// Can't move if the source tube full and  same colored
	if (colors == height)return false;

	if (tube2.size() == 0) return true;
	//only move if the source tube and destination tube are same colored
	return (tube1[tube1.size() - 1]== tube2[tube2.size() - 1]);
}
//Moving the ball on the grid
Grid moveBall( Grid grid ,int tube1,int tube2,int height)
{

    if(canMove(grid[tube1],grid[tube2],height)){
        grid[tube2]+=grid[tube1][grid[tube1].size()-1];
        grid[tube1].pop_back();
    }
       return grid;

}
//Sting Conversion
string canonicalStringConversion(Grid grid)
{
    string finalString;
    for (auto stack : grid) {
        finalString += (stack + ";");
    }
    return finalString;
}
vector<Grid> getAvailabeStates(Grid grid){
    vector<Grid> moves;
    for(int i=0;i<grid.size();i++){
        for (int j = 0 ;j<grid.size(); j++) {
                if(i!=j && canMove(grid[i],grid[j],getTubeHeight(grid)))
                moves.push_back(moveBall(grid,i,j,getTubeHeight(grid)));
        }
    }
    return moves;
}
//playing the game as user
void userGame(Grid grid, int height){
    //counting how many moves did the user make
    int num=0;
    //while the grid is not solved let the user move the balls between tubes
    while(!isSolved(grid,height)){
        system("CLS") ;
        printGrid(grid,height);
        cout<<"Move A Ball From x to y: "<<endl;
        int x,y;
        cin>>x>>y;
        grid=moveBall(grid,x-1,y-1,height);
        num++;
    }
    printGrid(grid,height);
    cout<<"Congratulations You Solved The Puzzle in "<<num<<" Moves"<<endl;
}
//BFS
void bfs(Grid grid,int count1)
{
    unordered_set<string> visited;
    queue<Grid> q;
    int height =getTubeHeight(grid);
    q.push(grid);
    visited.insert(canonicalStringConversion(grid));
    while (!q.empty()) {
    count1++;
    vector<Grid> moves=getAvailabeStates(q.front());
       q.pop();
        // Enqueue all adjacent of f and mark them visited
        for( auto move:moves){
            if(!isSolved(move,height)){
                    if(visited.find(canonicalStringConversion(move))==visited.end()){
                    visited.insert(canonicalStringConversion(move));
                    q.push(move);
                    }
            }
            else{
                printGrid(move,getTubeHeight(move));
                cout<<"PUZZLE SOLVED"<<endl;
                cout<<"BFS : "<<count1<<endl;
                return;
            }

        }
    }
}
//DFS
void dfs(Grid grid,int count1){
    unordered_set<string> visited2;
    stack<Grid> s;
    int height =getTubeHeight(grid);
    s.push(grid);
    visited2.insert(canonicalStringConversion(grid));
    while(!s.empty()){
        count1++;
        vector<Grid> moves=getAvailabeStates(s.top());
        s.pop();
        // Enqueue all adjacent of f and mark them visited
        for( auto move:moves){
            if(!isSolved(move,height)){
                    if(visited2.find(canonicalStringConversion(move))==visited2.end()){
                    visited2.insert(canonicalStringConversion(move));
                    s.push(move);
                    }
            }
            else{
                cout<<"DFS : "<<count1<<endl;
                return;
            }
    }

    }
    }
//compare function
bool compare(pair<Grid ,int> i, pair<Grid, int> j) {
  return i.second < j.second;
}
//get minimum value from map
int getmin(map<Grid,int> mymap){
    pair<Grid, int> min = *min_element(mymap.begin(), mymap.end(), compare);
    return min.second;
}
//UCS

void ucs(Grid grid, int count1)
{   unordered_set<string> visited;
    map<Grid,int> cost;
    queue<Grid> q;
    int height =getTubeHeight(grid);
    q.push(grid);
    visited.insert(canonicalStringConversion(grid));
    while (!q.empty()) {
    count1++;
    grid=q.front();
    vector<Grid> moves=getAvailabeStates(grid);
       q.pop();
       for( auto move:moves){
        cost[move] = 1;
       }
        // Enqueue all adjacent of f and mark them visited
        for( auto move:moves){
            if(!isSolved(move,height)){
                    if(cost[move]<= getmin(cost)){
                    if(visited.find(canonicalStringConversion(move))==visited.end()){
                    cost[move] +=1;
                    visited.insert(canonicalStringConversion(move));
                    q.push(move);
                    }
            }}
            else{
                cout<<"UCS : "<<count1<<endl;
                return;
            }

        }
    }
}

//Calculate max ball color in a tube
int maxOcc(string tube){
  	int max = -1;
  	int freq[256] = {0};
	int len = tube.length();
  	for(int i = 0; i < len; i++)
  	{
  		freq[tube[i]]++;
	}
  	for(int i = 0; i < len; i++)
  	{
		if(max < freq[tube[i]])
		{
			max = freq[tube[i]];
		}
	}
	return max;
}
//Calculate Heuristic
int calcHeuristic(Grid grid){
    int propCost=0;
  	for (auto tube : grid)
    {
        int mainColor= maxOcc(tube);
        if(tube.size()==0)continue;
        if(mainColor==tube.size())continue;
        else propCost+=tube.size()-mainColor;

    }
    return propCost;
}
/*bool checkChilds(vector<Grid> moves){
    for(auto move :moves ){
        if(isSolved(move,getTubeHeight(move)))
            return false;
    }
    return true;
}
*/
//Hill Climbing
void hillClimbing(Grid grid ,int count1){
    map<Grid,int> heuristic;
    unordered_set<string> visited2;
    int height =getTubeHeight(grid);
    visited2.insert(canonicalStringConversion(grid));
    while(!isSolved(grid,height)){
        count1++;
        vector<Grid> moves=getAvailabeStates(grid);
        for( auto move:moves){
        heuristic[move] = calcHeuristic(move);
       }
        // Enqueue all adjacent of f and mark them visited
        for( auto move:moves){
                if(visited2.find(canonicalStringConversion(move))==visited2.end()){
                if(heuristic[move]<=getmin(heuristic)){
                heuristic[move]=99999;
                grid =move;
                visited2.insert(canonicalStringConversion(move));

                    }
                }
                else continue;
        }
    }
        cout<<"Hill Climbing : "<<count1<<endl;
        return;

}
//A Star
void astar(Grid grid, int count1){
    unordered_set<string> visited;
    map<Grid,int> heuristic;
    map<Grid,int> cost;
    map<Grid,int> ftotal;
    queue<Grid> q;
    int height =getTubeHeight(grid);
    q.push(grid);
    visited.insert(canonicalStringConversion(grid));
    while (!q.empty()) {
    count1++;
    grid=q.front();
    vector<Grid> moves=getAvailabeStates(grid);
       q.pop();
       for( auto move:moves){
        cost[move] = 1;
        heuristic[move]=calcHeuristic(move);
        ftotal[move]=cost[move] +heuristic[move];
       }
        // Enqueue all adjacent of f and mark them visited
        for( auto move:moves){
            if(!isSolved(move,height)){
                    if(ftotal[move]<= getmin(ftotal)){
                    if(visited.find(canonicalStringConversion(move))==visited.end()){
                    cost[move] +=1;
                    visited.insert(canonicalStringConversion(move));
                    q.push(move);
                    }
            }}
            else{
                cout<<"A Star : "<<count1<<endl;
                return;
            }

        }
    }
}
//reading level from text files
Grid level(string level){
     string tubes[20];
    ifstream input(level);
    string line;
    int i=0;
    while (getline(input, line)) {
        //each line is a tube(string)
        tubes[i]=line;
        i++;
    }
      Grid grid=makeGrid(tubes,i);
      return grid;
}
//choose level
Grid chooseLevel(){
    int levels;
    cout<<"Choose A level From 1 to 5 To play: "<<endl;
    cin>>levels;
    switch(levels){
      case 1:
       return level("1.txt");
      case 2:
       return level("2.txt");
      case 3:
       return level("3.txt");
      case 4:
       return level("4.txt");
      case 5:
       return level("5.txt");



      default:
          cout<<"Choose A Valid Level";
    }
}
void game(){
    int type;
    cout<<"Welcome Gamer Choose how you Want to play:"<<endl;
    cout<<"Play as a user press 1"<<endl;
    cout<<"Play as a computer press 2"<<endl;
    cin>>type;
    if(type==1){
      Grid grid=  chooseLevel();
      userGame(grid,getTubeHeight(grid));
    }
    else {
        int algo;
        Grid grid = chooseLevel();
        printGrid(grid,getTubeHeight(grid));
        cout<<"Run algorithms to compare press 1"<<endl;
        cin>>algo;
        if(algo==1){
        bfs(grid,0);
        dfs(grid,0);
        ucs(grid,0);
        hillClimbing(grid,0);
        astar(grid,0);
        }
        else
            cout<<"okay good luck:)";

    }
}


// Driver Code
int main(void)
{
    game();
	return 0;
}

