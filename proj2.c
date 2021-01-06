#include <stdio.h>
#include <stdlib.h>
//#include <errno.h>
#include <sys/utsname.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include <stdbool.h>




struct fileData
{
    char proc[20][20];
    char t[20];
    int arrival[20];
    int burst[20];
};

void calcFinish(int, int, float, float, float, float, float);


int main(int argc, char *argv[])
{

FILE *infile1;
FILE *infile2;

char filename[100];

int lineCount = 0, lineCount2 = 0, size = 0, temp = 0, q = 0, fr = 0, back = 0;
int Quantum, burst, x, y, z, j, n, time;
//int arrFlag[size], endFlag[size];

float avgWait = 0, waitSum = 0, tempSum = 0, avgTurn = 0, sumArrivals = 0;
float avgCpu = 0, avgResponse = 0, resTime = 0, cpuUsage = 0;


char file[20], Algorithm[20];
char c;

struct fileData input;

bool completed = false;
bool procFlag[size];


int arrFlag[size], endFlag[size];			// For size variable (after file call)
int Q[size], newBurst[size], timeFlag[size], turnA[size], waitTime[size];
bool waitFlag[size];

int RRburst[20], RRarrival[20];
char RRproc[20];



		
		sscanf(argv[1],"%s", filename);
		sscanf(argv[2],"%s", Algorithm);



	/**********************************************/

	    infile1 = fopen(filename, "r");

	    if (infile1 == NULL)
	    {
		fprintf(stderr, "\nError opening file\n");
		exit (1);
	    }
	    
  	     //for (c = getc(fp); c != EOF; c = getc(fp)) 
	    // while ((c = getc(infile1)) != EOF)
  	     for (c = getc(infile1); c != EOF; c = getc(infile1)) 
		{

		if (c == '\n') 						// Get file line size(lineCount)
		    lineCount = lineCount + 1;
		}

		fclose (infile1);

		size = lineCount;
		//printf("size: %d", lineCount);

 	/****************************************************************************/

		infile2 = fopen(filename, "r");

		for (int x = 0; x < lineCount; x++)			// Copy file data into arrays
		{
			fscanf(infile2," %s %d %d ", input.proc[x], &input.arrival[x], &input.burst[x]);
		}

    		fclose (infile2);

	/****************************************************************************************/


		for (int i = 0; i < size; i++)			// CPU Usage Calc
		   {
		       cpuUsage = cpuUsage + input.burst[i];
		   }

	


	//printf("file: %s Algo: %s Quantum: %d \n", filename, Algorithm, Quantum);


			if(strcmp(Algorithm,"RR") == 0)
			{

				Quantum = atoi(argv[3]);

				printf("Round Robin selected.... \n");

				for (x = 0; x < size; x++)
				   {
					   
					sumArrivals = sumArrivals + input.arrival[x];
				   }
			
				   int waitTime[size];
			      

				   for (x = 0; x < size; x++)
				   {
				       procFlag[x] = false;
				       timeFlag[x] = -1;
				       Q[x] = -1;
				       waitFlag[x] = false;
				       waitTime[x] = 0;
   				   }
			   
				   printf("Total of %d tasks are read from %s. Press ENTER to start...", size, filename);
				   getchar();
				   printf("===============================================================\n");
					

				   completed = false;
				   time = 0;


				  do
				   {


				       if (input.burst[Q[fr]] == 0)
				       {
					   printf("<System Time\t%d> Process: %s\t...is finished\n", time, input.proc[Q[fr]]);
					   procFlag[Q[fr]] = true;


					   waitSum = waitSum + time;
					   tempSum = time;
					   

				       }


				       check_queue: for (x = 0; x < size; x++)
				       {
					   if ((input.arrival[x] <= time) && (procFlag[x] != true) && (input.burst != 0))
					   {
					       waitFlag[x] = false;
					       for (y = 0; y < size; y++)
					       {
						   if (Q[y] == x)
						   {
						       waitFlag[x] = false;
						       break;
						   }
						   else
						   {
						       waitFlag[x] = true;
						   }
					       }
					   }

					   if (waitFlag[x] == true)
					   {
					       back = (back + 1) % size;
					       Q[back] = x;
					   }
				       }

				       if ((q >= Quantum) || (procFlag[Q[fr]] == true))
				       {
					   Q[fr] = -1;
					   fr = (fr + 1) % size;
					   if (q == Quantum)
					   {
					       q = 0;
					       goto check_queue;
					   }
					   q = 0;
				       }
	

				       if (Q[fr] != -1)
				       {
					   q += 1;
					   input.burst[Q[fr]] -= 1;

					   if (timeFlag[Q[fr]] == -1)
					   {
					       timeFlag[Q[fr]] = time - input.arrival[Q[fr]];
					   }
				       }

				       if (Q[fr] != -1)
				       {
					   printf("<System Time\t%d> Process: %s\t...is running \n", time, input.proc[Q[fr]]);
				       }

				       completed = true;

					       for (x = 0; x < size; x++)
					       {
						   if ((input.arrival[x] <= time) && (input.burst[x] != 0))
						   {
						       if (Q[fr] != 1)			//!!!!
						       {
							   waitTime[x] += 1;
						       }
						   }
						   if (procFlag[x] != true)
						   {
						       completed = false;
						       time += 1;
						       break;
						   }
				       }



				   }while(!completed);

				   printf("<System Time\t%d> All processes have finished\n", time);
   				   printf("==================================================================\n");


			  
			   
					for (x = 0; x < size; ++x)
					{
						procFlag[x] = false;
					}

					 for (x = 0; x < size; ++x)
					   {
					       resTime = resTime + timeFlag[x];
		   			   }


			calcFinish(size, time, cpuUsage, sumArrivals, tempSum, resTime, waitSum); 

			}

			/*******************************************************************************/
			time = 0;

			if(strcmp(Algorithm,"FCFS") == 0)
			{

			for(int i=0; i < size; i++)					// Sort Arrival Times
				{
				for(int j=0; j < size; j++)
					{
					if(input.arrival[i]<input.arrival[j])
						{

						temp=input.arrival[i];
						input.arrival[i]=input.arrival[j];
						input.arrival[j]=temp;

						temp=input.burst[i];
						input.burst[i]=input.burst[j];
						input.burst[j]=temp;

						strcpy(input.t,input.proc[i]);
						strcpy(input.proc[i],input.proc[j]);
						strcpy(input.proc[j],input.t);
						}
		 
					}
				}


				printf("First Come First Serve.... \n");

				for (x = 0; x < size; x++)
				   {

					sumArrivals = sumArrivals + input.arrival[x];
				   }

		   int arrFlagFCFS[size];

		   printf("Total of %d tasks are read from %s. Press ENTER to start.....", size, filename);
		   getchar();
		   printf("==================================================================\n");

				for (x = 0; x < size; ++x)
					{

					procFlag[x] = false;
					arrFlagFCFS[x] = -1;
					endFlag[x] = 0;

					}

					time = 0;

				do
				{
					burst = -1;
					n = 500;


					for (x = 0; x < size; x++)
					{
						if ((input.burst[x] != 0) && (input.arrival[x] <= time))
						{
							if ((input.arrival[x] < n))
							{

								burst = x;
								n = input.arrival[x];

							}
						}

						else if ((procFlag[x] != true) && (input.burst[x] == 0))
						{

						printf("<System Time:\t %d> Process:\t %s is finished......  \n", time, input.proc[x]);
						procFlag[x] = true;

						 waitSum = waitSum + time;
						 tempSum = time;

						}
					}

					if (burst != -1)
					{

						if (arrFlagFCFS[burst] == -1)
						{
						arrFlagFCFS[burst] = time - input.arrival[burst];
						}

						printf("<System Time:\t %d> Process:\t %s is running.....\n", time, input.proc[burst]);
						input.burst[burst] -= 1;
					}

					completed = true;
					for (x = 0; x < size; x++)
					{
						if ((input.arrival[x] <= time) && (input.burst[x]))
						{
							if (burst != x)
							{
							endFlag[x] += 1;
							}
						}


						if (procFlag[x] != true)
						{
							completed = false;
							time += 1;
							break;
						}
					}



				}while(!completed);

					printf("<System Time\t%d> Processes have finished \n", time);
					printf("====================================================\n");
			

					   for (x = 0; x < size; x++)
					   {
					       procFlag[x] = false;
					   }

					   for (x = 0; x < size; x++)
					   {
					       resTime = resTime + arrFlagFCFS[x];
				   	   }

			calcFinish(size, time, cpuUsage, sumArrivals, tempSum, resTime, waitSum); 


			}

		/******************************************************************************************/
		
		time = 0;				//!!!! clear time Variable

		if(strcmp(Algorithm,"SRTF") == 0)
			{


			
			for(int i=0; i < size; i++)					// Sort Arrival Times
				{
				for(int j=0; j < size; j++)
					{
					if(input.arrival[i]<input.arrival[j])
						{

						temp=input.arrival[i];
						input.arrival[i]=input.arrival[j];
						input.arrival[j]=temp;

						temp=input.burst[i];
						input.burst[i]=input.burst[j];
						input.burst[j]=temp;

						strcpy(input.t,input.proc[i]);
						strcpy(input.proc[i],input.proc[j]);
						strcpy(input.proc[j],input.t);
						}
		 
					}
				}



			printf("Shortest Remaining Time First..... \n");

			for (x = 0; x < size; x++)
	   			{

  					sumArrivals = sumArrivals + input.arrival[x];
	   			}


		   int arrFlagSRTF[size];

		   printf("Total of %d tasks are read from %s. Press ENTER to start...", size, filename);
		   getchar();
		   printf("==================================================================\n");

			   for (x = 0; x < size; x++)
			   {
			       procFlag[x] = false;				
			       arrFlagSRTF[x] = -1;											
			       endFlag[x] = 0;
			   }


			  do
			   {
			        y = -1;

				z = 1000;
			       for (x = 0; x < size; x++)
			       {
				   if ((input.arrival[x] <= time) && (input.burst[x] != 0))
				   {
				       if (input.burst[x] < z)
				       {
					   z = input.burst[x];
					   y = x;
				       }
				   }
				   else if ((input.burst[x] == 0) && (procFlag[x] != true))
				   {
				       printf("<System Time\t%d> Process: %s \t...is finished\n", time, input.proc[x]);
				       procFlag[x] = true;

					// printf("Time: %d\n", time);

					 waitSum = waitSum + time;
					 tempSum = time;

				   }
			       }
			       if (y != -1)
			       {
				   printf("<System Time\t%d> Process: %s \t...is running\n", time, input.proc[y]);
				   input.burst[y] -= 1;


				   if (arrFlagSRTF[y] == -1)
				   {
				       arrFlagSRTF[y] = time - input.arrival[y];
				   }
			       }

			       completed = true;


			       for (x = 0; x < size; x++)
			       {
				   if ((input.arrival[x] <= time) && (input.burst[x] != 0))
				   {
				       if (y != x)
				       {
					   endFlag[x] += 1;
				       }
				   }


				   if ( procFlag[x] != true)
				   {
				       completed = false;
				       time += 1;
				       break;
				   }
			       }


			   }while(!completed);

			   printf("<System Time\t%d> ...All processes finished\n", time);
	   		   printf("==================================================================\n");

				//printf("resTime: %f\n", resTime);

			  for (x = 0; x < size; x++)
			   {
				 resTime = resTime + arrFlagSRTF[x];
		   	   }

				//printf("resTime: %f", resTime);


			calcFinish(size, time, cpuUsage, sumArrivals, tempSum, resTime, waitSum); 

		}



return 0;


}


void calcFinish(int size, int time, float cpuUsage, float sumArrivals, float tempSum, float resTime, float waitSum) 
{

	float avgCpu = 0, avgResponse = 0, avgWait = 0, avgTurn = 0;


				   avgCpu = 100.0 * (cpuUsage/(float)time);

						avgTurn = (waitSum - sumArrivals) / size;
						avgResponse = resTime / size;
						avgWait = (waitSum - tempSum - sumArrivals) / size;



						printf("Average Cpu Usage: %f \n", avgCpu);
						printf("Average Response Time: %f \n", avgResponse);
						printf("Average Waiting Time: %f \n", avgWait);
						printf("Average Turnaround Time: %f \n", avgTurn);
					printf("====================================================\n");

}
