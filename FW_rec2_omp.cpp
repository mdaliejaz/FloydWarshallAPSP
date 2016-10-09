#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>
#include<limits.h>
#include<algorithm>
#include<omp.h>
//#include<cilkview.h>
using namespace std;

/* maxVertices represents maximum number of vertices that can be present in the graph. */
#ifndef maxVertices
#define maxVertices   8192
#endif
#define INF           INT_MAX-1
#define OMP_NUM_THREADS	16

int vertices;
int m;
int dist[maxVertices][maxVertices];

void print_matrix()
{
	for(int i = 0 ; i < vertices; i++ ) 
	{
		cout << "\n";
		for(int j = 0 ; j< vertices ;j++ )
			cout << dist[i][j] << " " ;
	}
}

void init(int n)
{
	////#pragma omp parallel for
	for(int i=0;i<n;i++)
	{
		////#pragma omp parallel for
		for(int j=0;j<n;j++)
		{
			if(i==j)
			{
				dist[i][j] = 0;
			}
			else
			{
				dist[i][j] = INF;
			}
		}
	}
}

void floyd_warshall_iter(int xRowStart, int xColStart,
										int uRowStart, int uColStart,
										int vRowStart, int vColStart,
										int size)
{	
	////#pragma omp parallel for
	for(int k = vRowStart; k < (vRowStart + size);k++)
	{
		////#pragma omp parallel for
		for(int i = xRowStart; i < (xRowStart + size); i++)
		{
			////#pragma omp parallel for
			for(int j = xColStart; j < (xColStart + size); j++)
			{
				if(i != j && j != k && i != k) 
					dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
			}
		}
	}
}

void CFW(int xRowStart, int xColStart,
				int uRowStart, int uColStart,
				int vRowStart, int vColStart,
				int currSize)
{
	if (currSize == m)
		floyd_warshall_iter(xRowStart,xColStart,
				 uRowStart,uColStart,
				 vRowStart, vColStart,currSize);
	else
	{
		//1. CFW (X11, U11, V11) 
                //Assume currSize = 4. So x11 = first quadrant = rows 0 and 1 which is 0 and currSize/2 - 1
                //#pragma omp parallel sections
               // {
			//#pragma omp section
			 #pragma omp task firstprivate(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart, currSize)
			 {
	                	CFW(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart, currSize/2);
	                 }
        	        //2. CFW (X21, U21, V11)
        	        //#pragma omp section      
			 #pragma omp task firstprivate(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart,currSize)
			 {                                                 
                		CFW(xRowStart + currSize/2,xColStart, uRowStart + currSize/2,uColStart, vRowStart,vColStart, currSize/2);
                	}
		//}
                #pragma omp taskwait
		
		//3. CFW (X12, U11, V12)
                //#pragma omp parallel sections
                //{
			//#pragma omp section
 			#pragma omp task firstprivate(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart,currSize)
 			{ 
				CFW(xRowStart,xColStart + currSize/2, uRowStart,uColStart, vRowStart,vColStart + currSize/2, currSize/2);
			}
        	        //4. CFW (X22, U21, V12)
        	        //#pragma omp section
 			#pragma omp task firstprivate(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart,currSize) 
 			{
	                	CFW(xRowStart + currSize/2,xColStart + currSize/2, uRowStart + currSize/2,uColStart, vRowStart,vColStart + currSize/2, currSize/2);
	                }
		//}
		#pragma omp taskwait                

                //5. (X12, U12, V22)
                //#pragma omp parallel sections
                //{
			//#pragma omp section
			 #pragma omp task firstprivate(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart,currSize) 
			 {
	                	CFW(xRowStart,xColStart + currSize/2, uRowStart,uColStart + currSize/2,  vRowStart + currSize/2,vColStart + currSize/2, currSize/2);
	               		}
               		//6. CFW (X22, U22, V22)
               		//#pragma omp section
               		 #pragma omp task firstprivate(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart,currSize) 
               		 {
	                	CFW(xRowStart + currSize/2,xColStart + currSize/2, uRowStart + currSize/2,uColStart + currSize/2, vRowStart + currSize/2,vColStart + currSize/2, currSize/2);
	                }
		//}	
                #pragma omp taskwait

                //7. CFW (X11, U12, V21)
                //#pragma omp parallel sections
                //{       
			//#pragma omp section
			 #pragma omp task firstprivate(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart,currSize) 
			 {         
	                	CFW(xRowStart,xColStart, uRowStart,uColStart + currSize/2, vRowStart + currSize/2,vColStart, currSize/2);
	                }
			//8. CFW (X21, U22, V12)
			//#pragma omp section
			 #pragma omp task firstprivate(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart,currSize) 
			 {
	     		        CFW(xRowStart + currSize/2,xColStart, uRowStart + currSize/2,uColStart + currSize/2, vRowStart,vColStart + currSize/2, currSize/2);
	     		       }
		//}	
		#pragma omp taskwait
	}
}

void BFW(int xRowStart, int xColStart,
				int uRowStart, int uColStart,
				int vRowStart, int vColStart,
				int currSize)
{
	if (currSize == m)
		floyd_warshall_iter(xRowStart,xColStart,
				 uRowStart,uColStart,
				 vRowStart, vColStart,currSize);
	else
	{
		//1. BFW (X11, U11, V11) 
		//Assume currSize = 4. So x11 = first quadrant = rows 0 and 1 which is 0 and currSize/2 - 1
		//#pragma omp parallel sections
                //{
			//#pragma omp section
			 #pragma omp task firstprivate(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart,currSize) 
			 {
				BFW(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart, currSize/2);
			}
			//2. BFW (X12, U11, V12)
			//#pragma omp section
			 #pragma omp task firstprivate(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart,currSize) 
			 {
				BFW(xRowStart,xColStart + currSize/2, uRowStart,uColStart, vRowStart,vColStart + currSize/2, currSize/2);			
			}
		//}
		#pragma omp taskwait
								
		//3. BFW (X21, U21, V11)							
		//#pragma omp parallel sections
                //{		
			//#pragma omp section
			 #pragma omp task firstprivate(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart,currSize) 
			 {
				BFW(xRowStart + currSize/2,xColStart, uRowStart + currSize/2,uColStart, vRowStart,vColStart, currSize/2);				
			}		
			//4. BFW (X22, U21, V12)			
			//#pragma omp section
			 #pragma omp task firstprivate(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart,currSize) 
			 {
				BFW(xRowStart + currSize/2,xColStart + currSize/2, uRowStart + currSize/2,uColStart, vRowStart,vColStart + currSize/2, currSize/2);
			}	
		//}
		#pragma omp taskwait

		//5. BFW (X21, U22, V21)
		//#pragma omp parallel sections
                //{
			//#pragma omp section
			 #pragma omp task firstprivate(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart,currSize) 
			 {
				BFW(xRowStart + currSize/2,xColStart, uRowStart + currSize/2,uColStart + currSize/2, vRowStart + currSize/2,vColStart, currSize/2);
			}
			//6. BFW (X22, U22, V22)
			//#pragma omp section
			 #pragma omp task firstprivate(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart,currSize) 
			 {
				BFW(xRowStart + currSize/2,xColStart + currSize/2, uRowStart + currSize/2,uColStart + currSize/2, vRowStart + currSize/2,vColStart + currSize/2, currSize/2);
			}
		//}
		#pragma omp taskwait

		//7. BFW (X11, U12, V21)                                
		//#pragma omp parallel sections
                //{
			//#pragma omp section
			 #pragma omp task firstprivate(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart,currSize) 
			 {
				BFW(xRowStart,xColStart, uRowStart,uColStart + currSize/2, vRowStart + currSize/2,vColStart, currSize/2);
			}
			//8. (X12, U12, V22)
			//#pragma omp section
			 #pragma omp task firstprivate(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart,currSize) 
			 {
				BFW(xRowStart,xColStart + currSize/2, uRowStart,uColStart + currSize/2,  vRowStart + currSize/2,vColStart + currSize/2, currSize/2);
			}
		//}
		#pragma omp taskwait
	}
}

void AFW(int xRowStart, int xColStart,
										int uRowStart, int uColStart,
										int vRowStart, int vColStart,
										int currSize)
{
	if (currSize == m)
		floyd_warshall_iter(xRowStart,xColStart,
										 uRowStart,uColStart,
										 vRowStart, vColStart,currSize);
	else
	{
		//2. AFW (X11, U11, V11) 
		//Assume currSize = 4. So x11 = first quadrant = rows 0 and 1 which is 0 and currSize/2 - 1
		AFW(xRowStart,xColStart, uRowStart,uColStart, vRowStart, vColStart, currSize/2);
		
		//3. parallel: BFW (X12, U11, V12)
		//#pragma omp parallel sections
                
			//#pragma omp section
			 #pragma omp task firstprivate(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart,currSize) 
			 {
				BFW(xRowStart,xColStart + currSize/2, uRowStart,uColStart, vRowStart, vColStart + currSize/2, currSize/2);		
			}				
			//3. CFW (X21, U21, V11)							
			//#pragma omp section
			 #pragma omp task firstprivate(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart,currSize) 
			 {
				CFW(xRowStart + currSize/2,xColStart, uRowStart + currSize/2,uColStart, vRowStart, vColStart, currSize/2);	
			}
		#pragma omp taskwait
		
		//4. AFW (X22, U21, V12)			
		AFW(xRowStart + currSize/2,xColStart + currSize/2, uRowStart + currSize/2,uColStart, vRowStart, vColStart + currSize/2, currSize/2);	
		
		//5. AFW (X22, U22, V22)
		AFW(xRowStart + currSize/2,xColStart + currSize/2, uRowStart + currSize/2,uColStart + currSize/2, vRowStart + currSize/2, vColStart + currSize/2, currSize/2);

		//6. parallel: BFW (X21, U22, V21), CFW (X12, U12, V22)
		//#pragma omp parallel sections
                //{
			//#pragma omp section
			 #pragma omp task firstprivate(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart,currSize) 
			 {
				BFW(xRowStart + currSize/2,xColStart, uRowStart + currSize/2,uColStart + currSize/2, vRowStart + currSize/2, vColStart, currSize/2);
			}
			//#pragma omp section
			 #pragma omp task firstprivate(xRowStart,xColStart, uRowStart,uColStart, vRowStart,vColStart,currSize) 
			 {
				CFW(xRowStart,xColStart + currSize/2, uRowStart,uColStart + currSize/2,  vRowStart + currSize/2, vColStart + currSize/2, currSize/2);
			}
		//}
		#pragma omp taskwait
		//7. AFW (X11, U12, V21)				
		AFW(xRowStart,xColStart, uRowStart,uColStart + currSize/2, vRowStart + currSize/2, vColStart, currSize/2);
	}
}


int main(int argc, char *argv[])
{
	vertices=atoi(argv[1]);
	m = atoi(argv[2]);	
//vertices=maxVertices;
	init(vertices);
	for(int i = 0 ; i< vertices; i++ )
        {
                for(int j = 0 ; j< vertices; j++ )
                {
                        if( i == j )
                                dist[i][j] = 0;
                        else {
                                dist[i][j] = ((i+j)%5)?(i+j):(i+j)%7;
                        }
                }
        }
	
	omp_set_dynamic(0);
    omp_set_num_threads(omp_get_num_procs());

	double start_time = omp_get_wtime();
        #pragma omp parallel
	{
		#pragma omp single
		{
        		AFW(0,0,0,0,0,0,vertices);
		}
	}
        double end_time = omp_get_wtime();
        cout <<endl<<endl<< end_time-start_time <<endl<<endl;

	//print_matrix();

	return 0;
}
