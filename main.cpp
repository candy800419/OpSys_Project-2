// Operating System FALL2011
// Project #2 - Memory Management Simulation System
// Tiffany Milano & Meng Jung Lin
// Finalize Date: 12/09/2011

#include <iostream>
#include <string.h>
#include <time.h>
#include <vector>
#include "mem.h"
using namespace std;

#define ros 80

//Process* create(int size, int tp); //function to create an array of processes

void firstFit(vector<Process> &p, int ptprob, int npprob);
void bestFit(vector<Process> &p, int ptprob, int npprob);	//Best Fit Algorithm
void nextFit(vector<Process> &p, int ptprob, int npprob);	//Next Fit Algorithm
void worstFit(vector<Process> &p, int ptprob, int npprob);	//Worst Fit Algorithm
int defragmentation(vector<Process> &p);

bool enterProbability(int prob);
int nextProcessChar();
int findFit(int length); //function that find the size of the first empty slot
void printMem(); //function to print main memory
int createProcess(vector<Process> &proc, int pt);
int findMatch(vector<Process> &proc, char procName);
void checkTerminate(vector<Process> & proce); //function that check for terminated processes

int charArrLoc = 0; //counter for the ASCII character 
int asciiChar[58]; 
char mainMem[2400]; //1-dimentional character array, main memory
int used = ros;

int main(int argc, char * argv[])
{
	int ptp, npp, i, j;

	if (argc != 4)
	{
		cout << "USAGE: memsim { first | best | next | worst } <process-termination-probability> <new-process-probability>" << endl;
		system("pause");
		return 0;
	}

	ptp = atoi(argv[2]); //process-termination-probability = argv[2]
	npp = atoi(argv[3]); //new-process-probability = argv[3]

	//initialize main memory
	for (i = 0; i < 2400; i++)
	{
		if (i < ros)
			mainMem[i] = '#';
		else
			mainMem[i] = '.';
	}

	srand(time(NULL)); //set seed to get random numbers

	//Process* processes = new Process[58];	//create an array of Process
	vector <Process> processes;
	
	//create and assign 20 processes into memory
	for (int i = 0; i < 20; i++)
	{
		int check = createProcess(processes, ptp);
		if(check == -1) 
		{ 
			// the program is out of memory so it ends
			cout << "1: ERROR: Process could not be created" << endl; 
			return -1; 
		}
		for (j = 0; j < processes.back().getCellRequired(); j++)
		{
			mainMem[processes.back().getStartPos() + j] = processes.back().getProcName();
		}
	}

	//print out main memory
	printMem();

	if (strcmp(argv[1],"first") == 0)
	{
		//running first fit algo
		firstFit(processes, ptp, npp);
	}
	else if (strcmp(argv[1],"best") == 0)
	{
		//running best fit algo
		bestFit(processes, ptp, npp);
	}
	else if (strcmp(argv[1],"next") == 0)
	{
		//running next fit algo
//		nextFit(processes, ptp, npp);
	}
	else if (strcmp(argv[1],"worst") == 0)
	{
		//running worst fit algo
//		worstFit(processes, ptp, npp);
	}
	else
	{
		cout << "Can not identify the algorithm." << endl;
		cout << "USAGE: memsim { first | best | next | worst } <process-termination-probability> <new-process-probability>" << endl;

		system("pause");
		return 0;
	}

	system("pause");
	return 0;
}

void firstFit(vector<Process> &p, int ptprob, int npprob)
{
	cout << "ENTER�@FIRST FIT FUNCTION " << endl;
	char input;
	cin >> input;
	while( input != 'q' )
	{
		if (input == 'c')
		{ 
			checkTerminate(p);

			//attempt to create new process
			if(enterProbability(npprob) == true)
			{ 
				cout << "creating new processes" << endl; 
				int check = createProcess(p, ptprob);
				if(check == -1) 
				{ 
					//cout << "2: ERROR: Process could not be created" << endl;
					int def = defragmentation(p);
					if(def == -1) 
					{
						cout << "5: ERROR: Out of Memory, unable to perform defragmentation" << endl; 
						return; 
					}
				}
				
				for (int i = 0; i < p.back().getCellRequired(); i++)
				{
					mainMem[p.back().getStartPos() + i] = p.back().getProcName();
				}
			}
			printMem();
			cin >> input;
		}
		else
		{
			cout << "INVALID COMMAND: Please enter either c or q to continue or quit" << endl;
			cin >> input;
		}
	} 
	return; 
}

void bestFit(vector<Process> &p, int ptprob, int npprob)
{
	char input;
	int bestFit = 2401, bestFitStartLoc = 2401, i = ros, count = 0;
	bool breakLoop = false; 

	cin >> input;
	while( input != 'q' )
	{
		if (input == 'c')
		{
			checkTerminate(p);
			
			if(enterProbability(npprob) == true)
			{ 
				cout << "CREATING NEW PROCESSES" << endl; 
				int check = createProcess(p, ptprob);
				cout << "CHECK CALLS CREATE PROCESS" << endl; 
				if(check == -1) 
				{ 
					int def = defragmentation(p);
					if(def == -1) 
					{
						cout << "5: ERROR: Out of Memory, unable to perform defragmentation" << endl; 
						return; 
					}
				}

				cout << "NOT OUT OF MEMORY AFTER CREATING NEW PROCESS" << endl; 

				while(mainMem[i] == '.' && i < 2400)
				{
					cout << "MAIN MEM [" << i << "]" << endl; 
					count++;
					i++;

					if (count > bestFit)
					{
						break; 
					}
				}





				if (i == 2399)
				{
					int def = defragmentation(p);
					if(def == -1) 
					{
						cout << "6: ERROR: Out of Memory, unable to perform defragmentation" << endl; 
						return; 
					}
				}
				else
				{
					int pLoc = findMatch(p, mainMem[i]); 
					if(count < bestFit && count == p[pLoc].getCellRequired())
					{
						bestFit = count;
						bestFitStartLoc = i;
						breakLoop = true; 
					}
					else if( count < bestFit && count > p[pLoc].getCellRequired()) 
					{ 
						bestFit = count;
						bestFitStartLoc = i;
					}
					count = 0;				
				}
				if(breakLoop == true)
				{
					break; 
				}
				
				i++;
			}
			if(bestFit != 2401) 
			{
				cout << "BEST FIT CRAP" << endl; 
				p.back().setStartPos(bestFitStartLoc);
				for(int x = bestFitStartLoc; x < p.back().getCellRequired(); x++)
				{
					mainMem[x] = p.back().getProcName(); 
				}

			}


			printMem();
			cin >> input;
		}
		else
		{
			cout << "INVALID COMMAND: Please enter either c or q to continue or quit" << endl;
			cin >> input;
		}
	}
	cout << "BEST FIT END RETURN" << endl;
	return;
}

void nextFit(vector<Process> &p, int ptprob, int npprob)
{
	enterProbability(npprob);
}

void worstFit(vector<Process> &p, int ptprob, int npprob)
{
	enterProbability(npprob);
}

int defragmentation(vector<Process> &p)
{
	int i = ros, empty = 0, tempStart;

	cout << "Performing defragmentation." << endl;
	bool defrag = false; 
	int freeCells = 0, numProc = 0;
	int oldEnd = 0, newEnd = 0;
	double per;

	cout << "oldEnd first set to " << oldEnd << endl << endl;
	while (i < 2400)
	{
		if(mainMem[i] == '.') 
		{
			bool breakLoop = false; 
			for(int x = i; x < 2400; x ++) 
			{
				if (x == 2399)
				{
					i = 2400;
				}
				if(mainMem[x] != '.') 
				{
					defrag = true; 
					// the memory is in use and needs to be shifted 
					int pLoc = findMatch(p, mainMem[x]);
					int tempOldEnd, j, oldStart, oldLen, tempNewEnd; 

					tempOldEnd = p[pLoc].getStartPos() + p[pLoc].getCellRequired();
					if(tempOldEnd > oldEnd)
					{
						oldEnd = tempOldEnd; 
					}

					oldLen = p[pLoc].getStartPos() + p[pLoc].getCellRequired();
					p[pLoc].setStartPos(i); 

					for(j = i; j < i + p[pLoc].getCellRequired(); j ++) 
					{
						// sets from i to the cell required with the mem name
						mainMem[j] = p[pLoc].getProcName();
						tempNewEnd = p[pLoc].getStartPos() + p[pLoc].getCellRequired();
						if(tempNewEnd > newEnd)
						{
							newEnd = tempNewEnd; 
						}
					}

					oldStart = j; 

					for(j = oldStart; j < oldLen; j++) 
					{
						mainMem[j] = '.'; 
						breakLoop = true;
					}
					if(breakLoop == true) 
					{
						numProc++;
						i += p[pLoc].getCellRequired();
						break;
					}
				}
			}
		}
		else
		{
			i++;
		}
	}

	freeCells = oldEnd - newEnd;
	cout << "oldEnd = " << oldEnd << " ; newEnd = " << newEnd << endl;

	per = ((double)freeCells / (double)2400) * (double)100;

	cout << "Defragmentation complete." << endl;
	cout << "Relocated " << numProc << " processes to create free memory block of " << freeCells << " units (" << per << "% of total memory)." << endl;
	if(defrag == true)
	{
		return 1; 
	}
	else
	{
		// unable to complete defragmentation (OUT OF MEMORY)
		return -1; 
	}
}

int findMatch(vector<Process> &proc, char procName) 
{
	int i = 0; 
	for(i; i < proc.size(); i ++) 
	{
		if(proc[i].getProcName() == procName)
		{
			return i;
		}
	}
	return -1; 
}

int createProcess(vector<Process> &proc, int pt)
{
	int k = nextProcessChar();
	if (k == -1)
	{
		// the process could not be created so it sends a -1 flag
		//cout << "3: ERROR: out of memory" << endl;
		return -1;
	}
	int len = rand() % 90 + 10;
	int startLocation = findFit(len); 
	if (startLocation == -1)
	{
		cout << "4: ERROR: Out of Memory in Create Process " << endl; 
		return -1; 
	}
 
	Process newProcess(k, len, startLocation, pt);
	proc.push_back(newProcess);
 
	return 0; 
}

bool enterProbability(int prob){ 
	int random = rand() % 99 + 1; 
	//If the number is within the probability return true
	if(random <= prob) 
	{ 
		return true; 
	}
	else
	{ 
		return false; 
	} 
}

int nextProcessChar(){ 
	int outOfMemory = 0;
	while(outOfMemory < 58)
	{
		if( charArrLoc != 58 ) 
		{ 
			if( charArrLoc == 0 && asciiChar[charArrLoc] != 1)
			{
				asciiChar[charArrLoc] = 1; 
				charArrLoc++;
				return 65;
			}
			else if( asciiChar[charArrLoc] == 0 ) 
			{ 
				int temp; 
				asciiChar[charArrLoc] = 1;
				temp = charArrLoc + 65; 
				charArrLoc++;
				return temp; 
			}
			else
			{
				charArrLoc++; 
			} 
			outOfMemory++;
		}
		else
		{
			charArrLoc = 0;
			outOfMemory = 0; // reset outOfMemory to zero
		}
		cout << "The out of memory number is: "<< outOfMemory << endl; 
		
	}
	return -1; //if -1 is returned the program is out of memory therefore it should terminate 
}

int findFit(int length)
{
	int i = ros; // ros = the begining of memory
	int size = 0; 
 
	for(i; i < 2400; i++)
	{
		size = 0; // reset size 
		while (mainMem[i] == '.')
		{	
			size++;
			i++;
			if(size == length)
			{
				return i-size;  // starting location for memory of this length 
			}
		} 
	}
	//if we get here that means we are out of memory 
	return -1; 
}

void checkTerminate(vector<Process> &p)
{
	vector<Process> pCopy;

	pCopy.clear();

	for (int j = 0; j < p.size(); j++)
			{
				if (enterProbability(p[j].getTermProb()) == true)
				{
					for(int r = 0; r < 58; r++) 
					{
						char temp = r + 65;  
						if(temp == p[j].getProcName())
						{
							asciiChar[r] = 0; 
							break;
						}
					}
					for (int m = 0; m < p[j].getCellRequired(); m++)
					{
						mainMem[p[j].getStartPos() + m] = '.';
					}
				}
				else
				{
					pCopy.push_back(p[j]);
				}
			}
			if(pCopy.size() != p.size())
			{
				p.clear(); 
				p = pCopy; 
			}
}
void printMem()
{
	for (int i = 0; i < 2400; i++)
		cout << mainMem[i];
	cout << endl << endl;
}