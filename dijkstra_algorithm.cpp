#include<bits/stdc++.h>
#define para pair<int, int> 
using namespace std;

const int inf = INT_MAX; //nieskończoność

//struktura, która będzie przechowywać wierchołki grafu
struct G{
	vector <para > pol; //z jakim wierzchołkiem, waga
	int odl = inf;		//najkrótsza ścieżka od źródła (na początku nieskończoność)
}*wezel;

void dijkstra(int start)
{
	int dystans, a, b;
	wezel[start].odl = 0;
	
	set <para > krawedzie; //odleglosc od źródła, ( -> b)
	
	//dodanie wszystkich wag krawędzi oraz numery wierzchołków docelowych
	for(int i=0; i<wezel[start].pol.size();i++)
		krawedzie.insert({wezel[start].pol[i].second, wezel[start].pol[i].first});
	//lub
	//	krawedzie.insert(make_pair(wezel[start].pol[i].second, wezel[start].pol[i].first));
	
	while(!krawedzie.empty()) //dopóki w secie są jeszcze jakieś krawędzie do analizy
	{
		//pobranie najkrótszej ścieżki oraz wierzchołek, do którego ona prowadzi
		dystans = krawedzie.begin()->first; //dystans
		b = krawedzie.begin()->second; 		//wierzchołek
		
		wezel[b].odl = min(dystans, wezel[b].odl); //ustawienie długości najkrótszej ścieżki
		
		//usunięcie krawędzi z seta
		krawedzie.erase(krawedzie.begin());
		
		for(int i=0; i<wezel[b].pol.size(); i++) //przeglądam wszysktie połączenia z wierzchołka b
		//biorę tylko te połączenia z wierzchołkami, do których odleglosć jest równa nieskonczonosć
			if(wezel[wezel[b].pol[i].first].odl == inf) 
				krawedzie.insert(make_pair(wezel[b].pol[i].second + dystans, wezel[b].pol[i].first));
	}
}

int main()
{
	int w, k, waga, a, b, start;
	
	//wczytanie liczby wierzchołków oraz liczby krawędzi
	cout<<"Podaj liczbę wierzchołków oraz liczbę krawędzi: ";
	cin>>w>>k;
	wezel = new G[w+1];
	
	for(int i=0; i<k; i++)
	{
		//(a -> b), waga
		
		cout<<"a -> b (waga): ";
		cin>>a>>b>>waga;
		wezel[a].pol.push_back(make_pair(b, waga));
		/* w przypadku grafu nieskierowanego dodajemy poniższą linijkę
		wezel[b].pol.push_back(make_pair(a, waga));
		*/
	}
	
	cout<<"Podaj wierzchołek początkowy: ";
	cin>>start;
	
	dijkstra(start);	
	
	cout<<"Najkrótsze ścieżki z wierzchołka źródłowego o numerze "<<start<<": "<<endl;
	for(int i=1; i<=w; i++)
	if(i != start)
		if(wezel[i].odl == inf)
			cout<<start<<" -> "<<i<<": nieskończoność"<<endl;
		else
			cout<<start<<" -> "<<i<<": "<<wezel[i].odl<<endl;
			
	delete [] wezel;
	
	return 0;
}