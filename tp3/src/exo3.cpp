///////////////////
// EXO3 DIJKSTRA //
///////////////////

#include "CuttingPlanesEngine.h"
#include <iostream>
#include <cstdlib>
#include <ilcplex/ilocplex.h>
#include <limits.h>
#include <stdio.h>
#define EPSILON 1.0e-6
#define COUT(x) {}
#define COUT2(x) {cout<<x;}
using namespace std;

int  n;         //no sommets
int  m;         //no arêtes
int* b;         //demandes
int**mat;       //matrice d'adjacence
int* aretes[2]; //m arêtes, chacune avec deux sommets
int bnd = 1;

int minDistance(int dist[], bool sptSet[])
{
    // Initialize min value
    int min = INT_MAX, min_index;

    for (int v = 0; v < n; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;

    return min_index;
}

int printSolution(int dist[])
{
    printf("Vertex \t\t Distance from Source\n");
    double sol = 0;
    for (int i = 0; i < n; i++){
        if (b[i]>1.0e-6){
            sol+=dist[i];
            printf("%d \t\t %d\n", i, dist[i]);
        }
    }
    printf("Solution : %f\n",sol);
}

void dijkstra(int src)
{
    int dist[n];

    bool sptSet[n];

    for (int i = 0; i < n; i++)
        dist[i] = INT_MAX, sptSet[i] = false;

    dist[src] = 0;

    for (int count = 0; count < n - 1; count++) {
        int u = minDistance(dist, sptSet);

        sptSet[u] = true;

        for (int v = 0; v < n; v++)

            if (!sptSet[v] && mat[u][v] && dist[u] != INT_MAX
                && dist[u] + mat[u][v] < dist[v])
                dist[v] = dist[u] + mat[u][v];
    }

    printSolution(dist);
}

int main(int argc, char const *argv[])
{
    if(argc==1){
        cerr<<"Donner un nom de fichier à lire svp\n";
        return EXIT_FAILURE;
    }
    fstream fic(argv[1]);

    fic>>n;
    fic>>m;
    if(!fic.good()){
        cerr<<"Pb lecture "<<argv[1]<<endl;
        return EXIT_FAILURE;
    }

    b   = new int[n];
    mat = new int*[n];
    aretes[0] = new int[m];
    aretes[1] = new int[m];
    for(int i=0;i<n;i++){
        mat[i] = new int[n];
        for(int j=0;j<n;j++)
            mat[i][j] = 0;
        b[i] = 0;
    }
    for(int i=0;i<m;i++){
        int sommet1, sommet2;
        fic>>sommet1;
        fic>>sommet2;
        aretes[0][i] = sommet1;
        aretes[1][i] = sommet2;
        mat[sommet1][sommet2] = 1;
        mat[sommet2][sommet1] = 1;
    }
    for(int i=0;i<n;i++){                 //n demandes, la source est 0
        int sommet, demande;
        fic>>sommet;
        fic>>demande;
        b[sommet] = demande;
    }
    //Fin lecture, affichage arêtes mat adjacence
    cout<<"n="<<n<<" sommets"<<endl;
    cout<<"m="<<m<<" arêtes"<<endl;
    for(int i=0;i<m;i++)
        cout<<"{"<<aretes[0][i]<<","<<aretes[1][i]<<"},";
    cout<<"\nMat adj:\n";
    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++)
            cout<<mat[i][j];
        cout<<endl;
    }
    cout<<"Demandes:";
    for(int i=0;i<n;i++)
        cout<<i<<" veut "<<b[i]<<"; ";
    cout<<endl;

    dijkstra(0);

    return 0;
}
