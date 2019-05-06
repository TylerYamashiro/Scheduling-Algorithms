#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <cstdlib>
#include <list>
#include <math.h>
#include <iomanip>
#include <sstream>
using namespace std;
const int FILENAME_INDEX = 1;
const int SCHEDULE_INDEX = 2;
const int ARGUMENTS = 3;


struct process{
  int ID;
  float arrive;
  float burst;
  float OGburst;
  float priority;
  float termination;
  float turnaround;
  float wait;
};

list<process> createArrivallist(string fileName);
void SJF(list<process> arrival);

void NP(list<process> arrival);

void PP(list<process> arrival);

void SRTF(list<process> arrival);

bool compareBurst(process a, process b){
  if(a.burst == b.burst){
	   return(a.ID < b.ID);
  }
  else{
    return (a.burst < b.burst);
  }
}

bool comparePriority(process a, process b){
  if(a.priority != b.priority){
	   return(a.priority < b.priority);
   }
   else{
     return(a.ID < b.ID);
   }
}

bool compareArrival(process a, process b){
  return(a.arrive < b.arrive);
}


int main(int argc, char *argv[]) {
  if(argc != ARGUMENTS){
    cerr << endl << "Error Invalid command line. " << endl
    << "Exiting Program" << endl << endl;
    return 0;
  }
  string fileNames = argv[FILENAME_INDEX];
  string scheduleAlg = argv[SCHEDULE_INDEX];

  list<process> arrival = createArrivallist(fileNames);
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
    << "Now Exiting.";
    return 0;
  }
  return 0;

}

list<process> createArrivallist(string fileName){
  ifstream inFile;
  inFile.open(fileName.c_str());
  if(inFile.fail()){
    cerr << "Failed to open file now closing";
    exit(EXIT_FAILURE);// maybe return some number -1 or something instead or use cerr
  }
  list<process> arrive;
  string line;
  float x;
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
  return arrive;
}




void SJF(list<process> arrival){
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
  while(!arrival.empty() || !ready.empty()){

    while(timer == arrival.front().arrive){
      ready.push_back(arrival.front());
      arrival.pop_front();
      ready.sort(compareBurst);
    }

    if(current.termination == timer || (idle == true)){
      done = current;
      if(!ready.empty()){
        current = ready.front();
        ready.pop_front();
        current.termination = timer + current.burst;
        current.turnaround = current.termination - current.arrive;
        current.wait = current.turnaround - current.burst;
        idle = false;
        cout << "Time " + to_string(int(timer)) + " Process " + to_string(current.ID);
        cout << endl;
        if(current.wait > maxWait){
          maxWait = current.wait;
        }
        avgWait = avgWait + current.wait;
      }
      else if(idle == false){
        cout << "Time " + to_string(int(timer)) + " Idle";
        cout << endl;
        idle = true;
      }
    }
    if(idle){
      idleCount++;
    }

    timer++;

  }
  avgWait = (avgWait / numProcesses);
  cout << "CPU Utilization: " << 100 - int((idleCount / current.termination) * 100) << "%" << endl;
  cout << "Average waiting time: ";
  cout << fixed << setprecision(2) << avgWait;
  cout << endl;
  cout << "Worst-case waiting time: " + to_string(int(maxWait)) << endl;
  //make cpu percentage thing
}


void NP(list<process> arrival){
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
  while(!arrival.empty() || !ready.empty()){
    while(timer == arrival.front().arrive){
      ready.push_back(arrival.front());
      arrival.pop_front();
      ready.sort(comparePriority);
    }
    if(current.termination == timer || (idle == true)){
      done = current;
      if(!ready.empty()){
        current = ready.front();
        ready.pop_front();
        current.termination = timer + current.burst;
        current.turnaround = current.termination - current.arrive;
        current.wait = current.turnaround - current.burst;
        idle = false;
        cout << "Time " + to_string(int(timer)) + " Process " + to_string(current.ID);
        cout << endl;
        if(current.wait > maxWait){
          maxWait = current.wait;
        }
        avgWait = avgWait + current.wait;
      }
      else if(idle == false){
        cout << "Time " + to_string(int(timer)) + " Idle";
        cout << endl;
        idle = true;
      }
    }
    if(idle){
      idleCount++;
    }

    timer++;

  }
  avgWait = (avgWait / numProcesses);
  cout << "CPU Utilization: " << 100 - int((idleCount / current.termination) * 100) << "%" << endl;
  cout << "Average waiting time: ";
  cout << fixed << setprecision(2) << avgWait;
  cout << endl;
  cout << "Worst-case waiting time: " + to_string(int(maxWait)) << endl;
  //make cpu percentage thing
}


void PP(list<process> arrival){
  list<process> ready;
  process done;
  process current;
  current.ID = -1;
  current.burst = 0;
  current.termination = 0;
  current.wait = 0;
  string test;
  int idleCount = 0;
  bool idle = false;
  float maxWait = 0;
  float avgWait = 0;
  float numProcesses = arrival.size();
  float timer = 0;
  arrival.sort(compareArrival);
  while(!arrival.empty() || !ready.empty()){

    while(timer == arrival.front().arrive){
      ready.push_back(arrival.front());
      arrival.pop_front();
      ready.sort(comparePriority);
    }


    if( comparePriority(ready.front(),current) || (idle == true)
        || current.termination == timer ){
      if(!ready.empty()){
        done = current;
        current = ready.front();
        ready.pop_front();
        current.termination = timer + current.burst;
        current.turnaround = current.termination - current.arrive;
        current.wait = current.turnaround - current.OGburst;
        idle = false;
        cout << "Time " + to_string(int(timer)) + " Process " + to_string(current.ID);
        cout << endl;
        if(current.wait > maxWait){
          maxWait = current.wait;
        }
      }
      else if(!idle && current.termination == timer){
        done = current;
        cout << "Time " + to_string(int(timer)) + " Idle";
        cout << endl;
        idle = true;
      }
      if(done.termination != 0 && timer < done.termination){
        done.burst = done.termination - timer;
        ready.push_back(done);
        ready.sort(comparePriority);
        done.termination = 0;
      }
      else if(done.ID >-1){
        avgWait = avgWait + done.wait;
        done.termination = 0;
        done.ID = -1;
      }
    }
    if(idle){
      idleCount++;
    }

  timer++;

  }
  avgWait = avgWait + current.wait;
  avgWait = (avgWait / numProcesses);
  cout << "CPU Utilization: " << 100 - int((idleCount / current.termination) * 100) << "%" << endl;
  cout << "Average waiting time: ";
  cout << fixed << setprecision(2) << avgWait;
  cout << endl;
  cout << "Worst-case waiting time: " + to_string(int(maxWait)) << endl;
  //make cpu percentage thing
} // might have to fix the wait avg isnt right



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
  while(!arrival.empty() || !ready.empty()){
    while(timer == arrival.front().arrive){
      ready.push_back(arrival.front());
      arrival.pop_front();
      ready.sort(compareBurst);
    }


    if( compareBurst(ready.front(),current)|| (idle == true)
        || current.termination == timer ){
      if(!ready.empty()){
        done = current;
        current = ready.front();
        ready.pop_front();
        current.termination = timer + current.burst;
        current.turnaround = current.termination - current.arrive;
        current.wait = current.turnaround - current.OGburst;
        idle = false;
        cout << "Time " + to_string(int(timer)) + " Process " + to_string(current.ID);
        cout << endl;
        if(current.wait > maxWait){
          maxWait = current.wait;
        }

      }
      else if(!idle && current.termination == timer){
        done = current;
        cout << "Time " + to_string(int(timer)) + " Idle";
        cout << endl;
        idle = true;
      }
      if(done.termination != 0 && timer < done.termination){
        done.burst = done.termination - timer;
        ready.push_back(done);
        ready.sort(compareBurst);
        done.termination = 0;
      }
      else if(done.ID >-1){
        avgWait = avgWait + done.wait;
        done.termination = 0;
        done.ID = -1;
    }
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
  }

    timer++;

  }
  avgWait = avgWait + current.wait;
  avgWait = (avgWait / numProcesses);
  cout << "CPU Utilization: " << 100 - int((idleCount / current.termination) * 100) << "%" << endl;
  cout << "Average waiting time: ";
  cout << fixed << setprecision(2) << avgWait;
  cout << endl;
  cout << "Worst-case waiting time: " + to_string(int(maxWait)) << endl;
  //make cpu percentage thing
}
