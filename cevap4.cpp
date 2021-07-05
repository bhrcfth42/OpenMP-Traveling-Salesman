#include <iostream>
#include <fstream>
#include <sstream>
#include <climits>
#include <omp.h>

using namespace std;

#define V 13 //Düðüm sayýsý
#define Ev 0 //Baþlangýç noktasý

int minDistance(int dist[], bool sptSet[]) //Minimum mesafe mi kontrolü yapan fonksiyon
{
    int min = INT_MAX, min_index; 
  
    for (int v = 0; v < V; v++) 
        if (sptSet[v] == false && dist[v] <= min)//Minimum deðer kontrolüne bakýyor
            min = dist[v], min_index = v;
  
    return min_index;//minimum mesafeyi döndürüyor
} 

void printSolution(int dist[])// En kýsa mesafe uzaklýklarýnýn çýktýsýný almak için fonksiyon
{ 
    printf("Dugum \t\t Kaynaga Uzaklik\n");
    
    #pragma omp parallel for
    for (int i = 0; i < V; i++){
    	printf("%d \t\t %d\n", i, dist[i]); 
	}
	#pragma omp barrier  
} 

void dijkstra(int graph[V][V], int src) 
{ 
    int dist[V];
    bool sptSet[V];
    
    #pragma omp parallel for
    for (int i = 0; i < V; i++){
    	#pragma omp critical
    	{
    		dist[i] = INT_MAX, sptSet[i] = false;
		}    	
	}        
    #pragma omp barrier
    
    dist[src] = 0;
    
    #pragma omp parallel for
    for (int count = 0; count < V - 1; count++) {
    	#pragma omp critical
		{
			int u = minDistance(dist, sptSet);
        	sptSet[u] = true;
        	#pragma omp parallel for
	        for (int v = 0; v < V; v++)
	            if (!sptSet[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]){
	                	dist[v] = dist[u] + graph[u][v];             	
				}
		}
    }
    #pragma omp barrier
    
    printSolution(dist); 
}

void dosyaOkuma(int graph[V][V]){
	ifstream dosya("mesafe.txt");
	int loop1=0;
	string satir="";
	if(dosya.is_open()){
		while(getline(dosya,satir)){
			int loop2=0;
			#pragma omp parallel for
			for(int loop=0;loop<satir.length();loop++){
				if(satir[loop]==','){
					#pragma omp critical
					{
						satir[loop]=' ';
					}					
				}
				else if(satir[loop]=='-'){
					#pragma omp critical
					{
						satir[loop]='0';
					}
				}					
			}
			#pragma omp barrier
			
			stringstream degerler(satir);
			int n;
			while (degerler >> n) {
				graph[loop1][loop2]=n;
     			loop2++;
			}
			loop1++;
		}
		dosya.close();
	}
}

int main(){
	
	#pragma omp master
	{
		int procsNum=omp_get_num_procs();
	    int tmax=omp_get_max_threads();
	    printf("Cekirdek Sayisi %d \t En fazla calisabilecek is parcacigi %d\n",procsNum,tmax);
	}
	#pragma omp barrier

	int graph[V][V];
	
	dosyaOkuma(graph);
	
	dijkstra(graph, Ev);
    
  
    return 0; 
} 
