// C++ implementation of the approach 
#include <bits/stdc++.h>
#include <omp.h>
using namespace std; 
#define V 13 
  
// Function to find the minimum weight Hamiltonian Cycle 
void hamiltonCycle(int graph[V][V], vector<bool>& boolArr, int currPos, int n, int count, int cost, int& ans){ 

    if (count == n && graph[currPos][0]) { //son düðüme gelindiyse kontrolü
        ans = min(ans, cost + graph[currPos][0]);//minimum olaný uygulamasýný istiyoruz.
        return; 
    }

    #pragma omp parallel for
    for (int i = 0; i < n; i++) { 
        if (!boolArr[i] && graph[currPos][i]){ //Eðer gezilmemiþ düðüm ise gezildi olarak iþaretleyip tekrar donksiyonumuzu çaðýrýyoruz.
        
        	boolArr[i] = true;//Düðüm gezildi olarak belirliyoruz.
            
	        hamiltonCycle(graph, boolArr, i, n, count + 1, cost + graph[currPos][i], ans); 
	
	        boolArr[i] = false;  
        } 
    }
};

void dosyaOkuma(int graph[V][V]){//mesafe dosyasýný okuyoruz
	ifstream dosya("mesafe.txt");
	int loop1=0;
	string satir="";
	if(dosya.is_open()){
		while(getline(dosya,satir)){
			int loop2=0;
			#pragma omp parallel for
			for(int loop=0;loop<satir.length();loop++){// int þekilde algýlamasý için satýrlarda replace özelliði yapýyoz 
				if(satir[loop]==','){//virgülleri tek boþluk yapýyoruz
					#pragma omp critical
					{
						satir[loop]=' ';
					}					
				}
				else if(satir[loop]=='-'){//tireleri 0 olarak deðiþtiriyoruz
					#pragma omp critical
					{
						satir[loop]='0';
					}
				}					
			}
			#pragma omp barrier
			
			stringstream degerler(satir);
			int n;
			while (degerler >> n) {//2 boyutlu diziye aktarýyoruz dosyadan okuduklarýmýzý
				graph[loop1][loop2]=n;
     			loop2++;
			}
			loop1++;
		}
		dosya.close();
	}
}
  
// Driver code 
int main() 
{ 
    int n = 13; //Gezilecek düðüm adet sayýsý
  
    int graph[V][V];//2 boyulu dizimizi oluþturuyoz
    dosyaOkuma(graph);//Dizimi dosyadan okuduðumuz verilerle dolduruyoruz
  
    vector<bool> boolArr(n); //gezildi mi için boolean kontrolü
    for (int i = 0; i < n; i++) //bool dizisinin hepsini false yapýyoruz
        boolArr[i] = false; 
  
    boolArr[0] = true; //Baþlangýcý true yapýyoz
    int ans=INT_MAX;//Minimum aðýrlýklý cycle bulmak için deðiþken belirliyoruz.
  
    hamiltonCycle(graph, boolArr, 0, n, 1, 0, ans); //Minimum aðýrlýk Cycle bulma
  
    cout <<"Minimum yol Agirligi : "<< ans<<endl; //Ans minimum aðýrlýk Cycle deðerimiz
  
    return 0; 
} 
