// C++ implementation of the approach 
#include <bits/stdc++.h>
#include <omp.h>
using namespace std; 
#define V 13 
  
// Function to find the minimum weight Hamiltonian Cycle 
void hamiltonCycle(int graph[V][V], vector<bool>& boolArr, int currPos, int n, int count, int cost, int& ans){ 

    if (count == n && graph[currPos][0]) { //son d���me gelindiyse kontrol�
        ans = min(ans, cost + graph[currPos][0]);//minimum olan� uygulamas�n� istiyoruz.
        return; 
    }

    #pragma omp parallel for
    for (int i = 0; i < n; i++) { 
        if (!boolArr[i] && graph[currPos][i]){ //E�er gezilmemi� d���m ise gezildi olarak i�aretleyip tekrar donksiyonumuzu �a��r�yoruz.
        
        	boolArr[i] = true;//D���m gezildi olarak belirliyoruz.
            
	        hamiltonCycle(graph, boolArr, i, n, count + 1, cost + graph[currPos][i], ans); 
	
	        boolArr[i] = false;  
        } 
    }
};

void dosyaOkuma(int graph[V][V]){//mesafe dosyas�n� okuyoruz
	ifstream dosya("mesafe.txt");
	int loop1=0;
	string satir="";
	if(dosya.is_open()){
		while(getline(dosya,satir)){
			int loop2=0;
			#pragma omp parallel for
			for(int loop=0;loop<satir.length();loop++){// int �ekilde alg�lamas� i�in sat�rlarda replace �zelli�i yap�yoz 
				if(satir[loop]==','){//virg�lleri tek bo�luk yap�yoruz
					#pragma omp critical
					{
						satir[loop]=' ';
					}					
				}
				else if(satir[loop]=='-'){//tireleri 0 olarak de�i�tiriyoruz
					#pragma omp critical
					{
						satir[loop]='0';
					}
				}					
			}
			#pragma omp barrier
			
			stringstream degerler(satir);
			int n;
			while (degerler >> n) {//2 boyutlu diziye aktar�yoruz dosyadan okuduklar�m�z�
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
    int n = 13; //Gezilecek d���m adet say�s�
  
    int graph[V][V];//2 boyulu dizimizi olu�turuyoz
    dosyaOkuma(graph);//Dizimi dosyadan okudu�umuz verilerle dolduruyoruz
  
    vector<bool> boolArr(n); //gezildi mi i�in boolean kontrol�
    for (int i = 0; i < n; i++) //bool dizisinin hepsini false yap�yoruz
        boolArr[i] = false; 
  
    boolArr[0] = true; //Ba�lang�c� true yap�yoz
    int ans=INT_MAX;//Minimum a��rl�kl� cycle bulmak i�in de�i�ken belirliyoruz.
  
    hamiltonCycle(graph, boolArr, 0, n, 1, 0, ans); //Minimum a��rl�k Cycle bulma
  
    cout <<"Minimum yol Agirligi : "<< ans<<endl; //Ans minimum a��rl�k Cycle de�erimiz
  
    return 0; 
} 
