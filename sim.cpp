//Tyler Yamashiro
//File: sim.cpp
//1/25/18
//This program simulates scheduling algorithms by taking input from
//a file and using the appropriate schedule specified by the user
#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <cstdlib>
#include <math.h>
#include <iomanip>
#include <sstream>
using namespace std;
const int FILENAME_INDEX = 1;//index of the fileName
const int SCHEDULE_INDEX = 2; //index of the schedule algorithm
const int ARGUMENTS = 3;//numer of arguments from argv


struct process{ //struct to hold the info of the process
  int ID;
  float arrive;
  float burst;
  float OGburst;//origonal burst time
  float priority;
  float termination;
  float turnaround;
  float wait;
};

list<process> createArrivallist(string fileName);
//creates a list of type process to hold processes not yet arrived
//from a file parses the information and returns the list
void SJF(list<process> arrival);
//Passes in the arrival list and uses it to simulate shortest job
//first algorithm
void NP(list<process> arrival);
//Passes in the arrival list and uses it to simulate Nonpreemptive Piority
//algorithm
void PP(list<process> arrival);
//Passes in the arrival list and uses it to simulate Preemptive Piority
//algorithm
void SRTF(list<process> arrival);
//Passes in the arrival list and uses it to simulate Shortest remaining
//time first algorithm
bool compareBurst(process a, process b){//compares burst times if they are
  if(a.burst == b.burst){              //equal compare IDs instead
	   return(a.ID < b.ID);
  }
  else{
    return (a.burst < b.burst);
  }
}

bool comparePriority(process a, process b){//Compares priority but it they
  if(a.priority != b.priority){             //are equal compare IDs instead
	   return(a.priority < b.priority);
   }
   else{
     return(a.ID < b.ID);
   }
}

bool compareArrival(process a, process b){//compare arrival times
  return(a.arrive < b.arrive);
}


int main(int argc, char *argv[]) {
  if(argc != ARGUMENTS){
    //Error and exit if number of arguments are wrong
    cerr << endl << "Error Invalid command line. " << endl
    << "Exiting Program" << endl << endl;
    return 0;
  }
  string fileNames = argv[FILENAME_INDEX];
  //Index to get the filename
  string scheduleAlg = argv[SCHEDULE_INDEX];
  //Index to get schedule algorithm

  list<process> arrival = createArrivallist(fileNames);
  //store a list from the filename using createArrivallist and stores it
  //in a list called arrival
  if( scheduleAlg == "SJF"){
    SJF(arrival);
  }
  else if(scheduleAlg == "SRTF"){
    SRTF(arrival);
  }
  else if(scheduleAlg == "NP"){
    NP(arrival);
  }
  else if (scheduleAlg == "PP"){
    PP(arrival);
  }
  else{
    cout << "Error: Algorithm Parameter Invalid" << endl
    << "Now Exiting." << endl;
    //If the wrong code for the scheduling algorithm is entered exit and
    //provide an erroe
    return 0;
  }
  return 0;

}

list<process> createArrivallist(string fileName){
  ifstream inFile;
  inFile.open(fileName.c_str());
  if(inFile.fail()){
    cerr << "Failed to open file: now closing" << endl;
    exit(EXIT_FAILURE);
  }
  //If the file fails to open then print error to screen and exit
  list<process> arrive;
  //List named arrive to return at end of function
  string line;
  float x;
  //temporarliy store values for the Process
  int ProcessNum = 0;
  process temp;
  string procName;
  while (getline(inFile, line)){
    int slot = 0;
    istringstream is(line);
    while(is >> x){
      if(slot == 0){
        temp.ID = x;
        slot++;
      }
      else if(slot == 1){
        temp.arrive = x;
        slot++;
      }
      else if(slot == 2){
        temp.burst = x;
        slot++;
      }
      else{
        temp.priority = x;
      }
    }
    temp.OGburst = temp.burst;
    ProcessNum++;
    arrive.push_back(temp);
  }
  //Iterates through the file store the information in an Process
  //Pushes each process into the list named arrive do this until file ends
  return arrive;
}




void SJF(list<process> arrival){
  list<process> ready;
  //ready queue to hold arrived processes
  process done;
  //keep track of just finished processes
  process current;
  //the current process that is running
  current.burst = 0;
  //set a initial value for current.burst
  current.termination = 0;
  //set a initial value for current.termiantion
  int idleCount = 0;
  //initialize amount of time idle
  bool idle = false;
  //set to false since not idle immediately
  float maxWait = 0;
  //initialize max wait time variable
  float avgWait = 0;
  //initialize avg wait time variable
  float numProcesses = arrival.size();
  //get number of processes from the size of the arrival list
  float timer = 0;
  //initialize the timer
  arrival.sort(compareArrival);
  //sort the arrival list by arrival time
  while(!arrival.empty() || !ready.empty()){

    while(timer == arrival.front().arrive){
      ready.push_back(arrival.front());
      arrival.pop_front();
      ready.sort(compareBurst);
    }
    //Push elements from the arrival list to the ready list when
    //it is its arrival time and sort them by burst time

    if(current.termination == timer || (idle == true)){
      done = current;
      //if the current thing running is done or the cpu is idle
      if(!ready.empty()){
        current = ready.front();
        ready.pop_front();
        //if current process is done and ready list is not empty
        //update current
        current.termination = timer + current.burst;
        current.turnaround = current.termination - current.arrive;
        current.wait = current.turnaround - current.burst;
        //updates currents information
        idle = false;
        cout << "Time " + to_string(int(timer)) +
             " Process " + to_string(current.ID);
        cout << endl;
        //print the process to the screen
        if(current.wait > maxWait){
          maxWait = current.wait;
        }
        avgWait = avgWait + current.wait;
        //update maximum wait time and average wait time
      }
      else if(idle == false){
        cout << "Time " + to_string(int(timer)) + " Idle";
        cout << endl;
        idle = true;
      }
      //if the ready list is empty and the cpu is not idle print that it is
      //idle to the screen and set idle to true, also if current processes
      //is done
    }
    if(idle){
      idleCount++;
    }

    timer++;

  }
  avgWait = (avgWait / numProcesses);
  //calculate average wait time
  cout << "CPU Utilization: "
      << 100 - int((idleCount / current.termination) * 100) << "%" << endl;
  cout << "Average waiting time: ";
  cout << fixed << setprecision(2) << avgWait;
  cout << endl;
  cout << "Worst-case waiting time: " + to_string(int(maxWait)) << endl;
  //print out statistics
}


void NP(list<process> arrival){
  list<process> ready;
  process done;
  process current;
  current.burst = 0;
  current.termination = 0;
  int idleCount = 0;
  bool idle = false;
  float maxWait = 0;
  float avgWait = 0;
  float numProcesses = arrival.size();
  float timer = 0;
  arrival.sort(compareArrival);
  //Initialize the same variables as the SJF function
  while(!arrival.empty() || !ready.empty()){
    while(timer == arrival.front().arrive){
      ready.push_back(arrival.front());
      arrival.pop_front();
      ready.sort(comparePriority);
    }
    //as the time matches arrival time push into the ready List
    //sort by priority
    if(current.termination == timer || (idle == true)){
      done = current;
      if(!ready.empty()){
        current = ready.front();
        ready.pop_front();
        //if the current process is done or idle and ready List
        //is not empty then update current
        current.termination = timer + current.burst;
        current.turnaround = current.termination - current.arrive;
        current.wait = current.turnaround - current.burst;
        //update the current information
        idle = false;
        cout << "Time " + to_string(int(timer))
            + " Process " + to_string(current.ID);
        cout << endl;
        //print process to the screen
        if(current.wait > maxWait){
          maxWait = current.wait;
        }
        avgWait = avgWait + current.wait;
      }
      //calculate average and current wait times
      else if(idle == false){
        cout << "Time " + to_string(int(timer)) + " Idle";
        cout << endl;
        idle = true;
      }
      //if current process is done and ready list is empty set to idle
    }
    if(idle){
      idleCount++;
    }

    timer++;

  }
  avgWait = (avgWait / numProcesses);
  cout << "CPU Utilization: "
      << 100 - int((idleCount / current.termination) * 100) << "%" << endl;
  cout << "Average waiting time: ";
  cout << fixed << setprecision(2) << avgWait;
  cout << endl;
  cout << "Worst-case waiting time: " + to_string(int(maxWait)) << endl;
  //print out the statistics of the algorithm
}


void PP(list<process> arrival){
  list<process> ready;
  process done;
  process current;
  current.ID = -1;
  current.burst = 0;
  current.termination = 0;
  current.wait = 0;
  int idleCount = 0;
  bool idle = false;
  float maxWait = 0;
  float avgWait = 0;
  float numProcesses = arrival.size();
  float timer = 0;
  arrival.sort(compareArrival);
  //initialize variables in current to zero
  //Id set to -1 to represent no processes
  //sort arrival list by arrival time
  while(!arrival.empty() || !ready.empty()){

    while(timer == arrival.front().arrive){
      ready.push_back(arrival.front());
      arrival.pop_front();
      ready.sort(comparePriority);
    }
    //push processses into the ready list and sort
    //by priority

    if( comparePriority(ready.front(),current) || (idle == true)
        || current.termination == timer ){
    //if priority number is lower or process is idle or current processes
    //is over continue
      if(!ready.empty()){
        done = current;
        current = ready.front();
        ready.pop_front();
    //update the current process
        current.termination = timer + current.burst;
        current.turnaround = current.termination - current.arrive;
        current.wait = current.turnaround - current.OGburst;
    //update the information in the current process
        idle = false;
        cout << "Time " + to_string(int(timer))
          + " Process " + to_string(current.ID);
      //Print out the the process to the screen
        cout << endl;
        if(current.wait > maxWait){
          maxWait = current.wait;
        }
      //update the max wait or current
      }
      else if(!idle && current.termination == timer){
        done = current;
        cout << "Time " + to_string(int(timer)) + " Idle";
        cout << endl;
        idle = true;
      }
      //if current process is done and it is not idle the make idle
      if(done.termination != 0 && timer < done.termination){
        done.burst = done.termination - timer;
        ready.push_back(done);
        ready.sort(comparePriority);
        done.termination = 0;
      }
      //if process is kicked out due to priority adjust burst time
      // and push back into the ready queue and sort the ready list
      else if(done.ID >-1){
        avgWait = avgWait + done.wait;
        done.termination = 0;
        done.ID = -1;
      }
      //if process is done for good then update avg wait and mark as done
      // using -1
    }
    if(idle){
      idleCount++;
    }

  timer++;

  }
  avgWait = avgWait + current.wait;
  avgWait = (avgWait / numProcesses);
  cout << "CPU Utilization: "
  << 100 - int((idleCount / current.termination) * 100) << "%" << endl;
  cout << "Average waiting time: ";
  cout << fixed << setprecision(2) << avgWait;
  cout << endl;
  cout << "Worst-case waiting time: " + to_string(int(maxWait)) << endl;
  //Print out statistics to the screen
}



void SRTF(list<process> arrival){
  list<process> ready;
  process done;
  process current;
  current.burst = 0;
  current.termination = 0;
  string test;
  int idleCount = 0;
  bool idle = false;
  float maxWait = 0;
  float avgWait = 0;
  float numProcesses = arrival.size();
  float timer = 0;
  arrival.sort(compareArrival);
  //initiate variables like in the PP function
  while(!arrival.empty() || !ready.empty()){
    while(timer == arrival.front().arrive){
      ready.push_back(arrival.front());
      arrival.pop_front();
      ready.sort(compareBurst);
    }
    //when arrival time move from arrival queue to ready queue and sort
    //by burst

    if( compareBurst(ready.front(),current)|| (idle == true)
        || current.termination == timer ){
      if(!ready.empty()){
        done = current;
        current = ready.front();
        ready.pop_front();
        //if burst is smaller, idle is true,  or current process is over
        current.termination = timer + current.burst;
        current.turnaround = current.termination - current.arrive;
        current.wait = current.turnaround - current.OGburst;
        //update current information
        idle = false;
        cout << "Time " + to_string(int(timer))
        + " Process " + to_string(current.ID);
        cout << endl;
        //print out process to screen
        if(current.wait > maxWait){
          maxWait = current.wait;
        }
        //update max wait

      }
      else if(!idle && current.termination == timer){
        done = current;
        cout << "Time " + to_string(int(timer)) + " Idle";
        cout << endl;
        idle = true;
      }
      //if not idle and process is done and ready list is empty
      //set idle to true
      if(done.termination != 0 && timer < done.termination){
        done.burst = done.termination - timer;
        ready.push_back(done);
        ready.sort(compareBurst);
        done.termination = 0;
      }
      //if process is kicked off and not done update burst time and
      //push back into ready list
      else if(done.ID >-1){
        avgWait = avgWait + done.wait;
        done.termination = 0;
        done.ID = -1;
    }
    //if actaully done set to -1 to signify and update the avg wait time
    if(idle){
      idleCount++;
    }

    for(unsigned i = 0; i < ready.size() ; i++){
      process update;
      update = ready.front();
      ready.pop_front();
      update.wait ++;
      ready.push_back(update);
    }
    //update the wait time in the the ready list
  }

    timer++;

  }
  avgWait = avgWait + current.wait;
  avgWait = (avgWait / numProcesses);
  cout << "CPU Utilization: "
  << 100 - int((idleCount / current.termination) * 100) << "%" << endl;
  cout << "Average waiting time: ";
  cout << fixed << setprecision(2) << avgWait;
  cout << endl;
  cout << "Worst-case waiting time: " + to_string(int(maxWait)) << endl;
  //Print out the statistics to the screen
}
