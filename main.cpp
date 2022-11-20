// David W Vaughan, Arjun Ramsinghani, Arnob Roy
// CS 3383 Theory of Automata
// Project 1.
// Due 11:59pm Nov 6.
#include <algorithm>
#include <array>
#include <cstring>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using namespace std;
const int MAX = 100; // maximum number of states allowed
const int MAXCHAR = 50; // maximum number of alphabets allowed
vector<string> fileReadInt(vector<string> &file, string filename);
vector<string> delimiter(string line, vector<string> input, char deliminator);
vector<string> smallDelimiter(string my_str);
void printVector(vector<string> input);
void removespace(string s);

class AdjMatrix {
public:
  string graph[MAX][MAX]; // adjacent matrix to represent graph 
  string spot[MAX]; // an array representing states q0, q1, q2
  string inp; // input string from user
  // Initialize the matrix to zero
  int final_state = 0;
  int start_state = 0;
  // construct adjacency matrix , fill empty locations with * until filled
  AdjMatrix() {
    for (int i = 0; i < MAX; i++) {
      for (int j = 0; j < MAX; j++) {
        // * means that is empty, no connection
        graph[i][j] = "*"; 
      }
    }
  }
  // Allow user to enter strings to test wether they are accepted by the states
  void mainMenu() {
    string inp;
    cout << "\nType quit to stop entering strings" << endl;
    while (inp != "quit") {
      bool found = false;
      int x = 0;
      cout << "Type your string: "; // prompting user input from the keyboard
      cin >> inp; // read the input to variable input
      found = traversalRecursion(inp, start_state, 0, final_state);
      
      if (found == true) { // if all values are found valid in the automata
        cout << "Accepted" << endl;
      }     
      else {
        cout << "Rejected" << endl;
      }
    }
    // if user does not want to continue
    cout << "Ending Program" << endl;
  }
//display Graph
  void displayMatrix(int states) {
    // Display the states of the matrix
    cout << "Graph is " << endl;
    for (int j = 0; j < states; j++) {
      cout << spot[j];
    }
    // Display the  adjacent list matrix (conections between the nodes) graph itself
    for (int i = 0; i < MAX; i++) {
      for (int j = 0; j < MAX; j++) {
        cout << graph[i][j] << "||";
      }
      cout << endl;
    }
  }
  /* function to add edge into the matrix. */
  /* Gets rid of empty *, adds alphabet that moves to next state to the adjacency matrix. use "," to deliniate different alphabet */
  void addEdge(int start, int end, string edge) {
    string temp = graph[start][end];
    temp.erase(remove(temp.begin(), temp.end(), '*'), temp.end()); // remove * from string
    graph[start][end] = temp + edge + ","; // add a comma after the edge
  }
  /* goes through spot array (containing q0,q1) and find the int location of the node */
  int findMatrixLocation(string node) {
    for (int j = 0; j < MAX; j++) {
      if (spot[j] == node) { // returning the location that contains the node(alphabet)
        return j;
      }
    }
    return -1;
  }
  // Main traversal of the graph
//Using DFS and Recursion.
// For the Start node we go through all of it's connections. If a connection matches the input string use recursion to move to the matching connection node and move through the input string by 1 char. Then we do the same thing at the next node.
  bool traversalRecursion(string input, int node_location, int char_location, int final_state) {
    // recursively processing all the adjacent vertices of the node
    bool stop = false; // when the automata reaches q2 final it will turn to true
    // Go through all the nodes connections on the graph
    for (int i = MAX - 1; i >= 0; i--) {
      int j = 0;
      bool connection = false; // check if state has connection to another state
      string hold;
      string temp;
      vector<string> comp;
      //
      //node connections in Graph
      temp = graph[node_location][i];
      // small demlimantor makes sure you go throught each connections example 0,1 0 then 1
      comp = smallDelimiter(temp); // get the line of the file delimited by the comma and place it in comp
      do {
        hold = comp[j]; // get string of comp[j]; example hold = 0;
        removespace(hold); // remove the space from hold
        // if char from node i connection to node j matches input string
        char c = hold[0]; // convert string of comp[j] (hold) to its character, example string 0
        if (c == input[char_location]) { // compare character of first part of the state to the charachter of
          if (i == final_state) { // if i reaches the final state
            return true;
          }  
          else {
            connection = true; // this should have a connection to another state
            //move to next node but up move up input string 1
            stop = traversalRecursionUtil(input, i, char_location + 1, final_state);
            if (stop == true) { // if automata reaches q2 (or other final state)
              return true;
            }
          }
        } // if c
        j += 1; // increment j by 1
      } // do
      while (j < comp.size() && connection == false);
    } // for i
    return false;

  }
// Main traversal of the graph
//Useing DFS
  bool traversalRecursionUtil(string input, int node_location, int char_location, int final_state) {
    bool stop = false; // state variable
    // Go through all the nodes connections on the graph
    for (int i = MAX - 1; i >= 0; i--) {
      int j = 0;
      bool connection = false;
      string hold;
      string temp;
      vector<string> comp;
      
      temp = graph[node_location][i]; // node connections in Graph
      // small demlimantor makes sure you go throught each connections example 0,1 0 then 1
      comp = smallDelimiter(temp); // get the string delimited by the comma and place it in comp
      do {
        hold = comp[j]; // get string of comp[j]; example hold = 0;
        removespace(hold); // remove the space from hold
        char c = hold[0]; // convert string of comp[j] (hold) to its character, example string 0
        // if char from node i connection to node j matches input string
        if (c == input[char_location]) { // compare charachter of first part of the state to the charachter of
          if (i == final_state) { // if i reaches the final state
            return true;
          }
          else {
            connection = true;
            //move to next node but up move up input string 1
            stop = traversalRecursionUtil(input, i, char_location + 1, final_state);
            if (stop == true) {
              return true;
            }
          }
        } // if c
        j+=1;
      } // do
      while (j < comp.size() && connection == false);
    }
    return false;
  } // function
}; // end of object


int main() {
  AdjMatrix wa; // of adjacency matrix type (object)
  string arr_state[MAX]; // arr_state will have 100 states, where each index is a state
  string alphabet[MAXCHAR]; // alphabet will have 50 states, where each index is a state
  // Read from file
  string fileinp;
  cout<<"Enter file name with extension: "; // ask the user to input the file name
  cin >> fileinp; // prompting user input from the keyboard
  vector<string> file; // keeps the string of the file of the machine
  file = fileReadInt(file, fileinp); // sends the file to the appropriate function to be read
  printVector(file);
  vector<string> aplpha; // keeps the alphabet of the machine
  aplpha = delimiter(file[2], aplpha, ','); // line 3, alphabet of the machine, comma
  printVector(aplpha); // printing that file
  if(aplpha.size() > 50){
    cout<<"The number of alphabets are greater than 50, please reduce the number of alphabets."<< endl;
  }
  vector<string> states; // keeps the states of the machine
  states = delimiter(file[3], states, ','); // line 4, states of the machine, comma
  printVector(states);
  if(states.size() > 100){
    cout<<"The number of states are greater than 100, please reduce the number of states."<< endl;
  }
  // KEEPS EVERYTHING SEPARATED
  vector<string> start; // keeps the start of the machine
  start = delimiter(file[4], start, ',');
  printVector(start);
  vector<string> end; // keeps the end of the machine
  end = delimiter(file[5], end, ',');
  printVector(end); 
  vector<string> connections; // keeps the connections of the states in the machine
  connections = delimiter(file[6], connections, ',');
  printVector(connections);
  vector<string> inputs; // keeps the input of the file
  inputs = delimiter(file[8], inputs, ',');
  printVector(inputs);
  
  AdjMatrix M; // MAKE GRAPH
//Know where final state is
  bool stop = false;
  int q = 0;
  string s1, s2;
  s1 = end[0];
  while (q < states.size() && stop == false) {
    s2 = states[q];
    if (s1 == s2) {
      stop = true;
      M.final_state = q;
    }
    
    else {
      q++;
    }
  }
//Knowing where the start state is
  bool stop2 = false;
  int qw = 0;
  string s3, s4;
  s3 = start[0];
  while (qw < states.size() && stop2 == false) {
    s4 = states[qw];
    if (s3 == s4) {
      stop2 = true;
      M.start_state = qw;
    }
    
    else {
      qw++;
    }
  }
  // add states to array
  for (int i = 0; i < states.size(); i++) {
    M.spot[i] = states[i]; // the index of states is represented by the object M
  }
  string temp1, temp2, temp3;
  int x, y, z;

  // Add all conections to the graph
  for (int k = 0; k < connections.size(); k += 3) {
    temp1 = connections[k]; // connecting to the next node
    x = M.findMatrixLocation(temp1);
    temp2 = connections[k + 1]; // connecting to the 2nd next node
    temp3 = connections[k + 2]; // connecting to the 3rd next node
    y = M.findMatrixLocation(temp3); // object
    M.addEdge(x, y, temp2); // object
    }
  bool found;
  // Represents the inputs given in the text file
  // see if input from text file is valid
  for (int k = 0; k < inputs.size(); k ++) {
    cout<<"input for file is:"<<inputs[k]<<endl;
    found = M.traversalRecursion(inputs[k], M.start_state, 0, M.final_state);
    if (found == true) {
        cout << "Accepted" << endl;
    } 
    else {
        cout << "Rejected" << endl;
    }
  }
  if(inputs.size() == 0){ // if empty string pull the main menu
    M.mainMenu();
  }
  M.mainMenu(); // Alows user to enter ther own string. Assume that user enters valid charachter
}

// Print the Vector
void printVector(vector<string> input) {
  for (int i = 0; i < input.size(); i++) {
    string c = input[i];
  }
}

// read file and return it its string components
vector<string> fileReadInt(vector<string> &file, string filename) {
  ifstream myfile; // creating a file object to be read
  myfile.open(filename); // reading the file
  string myline; // reads the entire line
  int i = 0; // creates an integer counter that starts at 0
  if (myfile.is_open()) { // as long as myfile is open execute this
    while (myfile) { // my file is being read execute this loop
      std::getline(myfile, myline); // get all the contents of the file
      file.push_back(myline); // pushing myLine to file
      i++; // increment index i by 1
    }
  } else { // if the file cannot be read output this to console
    std::cout << "Couldn't open file" << endl;
  }
  return file;
}

/* deliminter should get rid of unecessary characters. Example: (0, 1) should return string 0 and string 1 and (q0, q1, q2), should return string q0 and string q1 and string q2
*/

//returns string
vector<string> delimiter(string line, vector<string> input, char deliminator) {
  string my_str = line; // assign my_str to line
  char char_array[MAXCHAR]; // charachters of alphabet
  //get rid of unecessarry parts of the string (), and spaces
  my_str.erase(remove(my_str.begin(), my_str.end(), '('), my_str.end()); // remove ( from string
  my_str.erase(remove(my_str.begin(), my_str.end(), ')'), my_str.end()); // remove ) from string
  my_str.erase(remove(my_str.begin(), my_str.end(), ' '), my_str.end()); // remove whitespace from string
  my_str.erase(remove(my_str.end() - 2, my_str.end(), ','), my_str.end()); // remove , from end of string
  char delim = deliminator; // assign delim to deliminator
  stringstream s_stream(my_str); // create stream of strings (s_stream) from the string my_str
  int i = 0;
  while (s_stream.good()) { // checking if the stream is good and errorrless to continue
    string substr;
    getline(s_stream, substr, delim); // get first string delimited by comma
    input.push_back(substr); // pushing substr to input
    i++; // increment i by 1
  }
  return input; // return to function
}

//get rid of unecessarry parts of the comma
//returns a vector of strings input string 0f 0,1 would return string 0 and string 1 in a vector string
vector<string> smallDelimiter(string my_str) {
  my_str.erase(remove(my_str.end() - 2, my_str.end(), ','), my_str.end()); // remove , from end of string
  vector<string> result; // result is a string vector for the purpose of storage
  stringstream s_stream(my_str); // create stream of strings (s_stream) from the string my_str
  while (s_stream.good()) { // checking if the stream is good and errorrless to continue
    string substr;
    getline(s_stream, substr, ','); // get first string delimited by comma
    result.push_back(substr); // push result to the end of stack
  }
  return result; // return the result
}

//removes spaces
void removespace(string s) {
  // traversing the string
  for (int i = 0; i < s.length(); i++) {
    if (s[i] == ' ') {
      // using in-built function to erase element
      s.erase(s.begin() + i); // removing the whitespace
      i--; // decrement i by 1 (character)
    }
  }
}