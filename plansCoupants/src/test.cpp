#include "CuttingPlanesEngine.h"
#include <iostream>
//étant donnée la solution courante y, trouver une contrainte violée
//a^T y >= d et renvoyer a^T y - d. Remplir vecteur a et valeur d!!
double separateFunc (const int n, double*y, double * a, double&d)
{
    /*Cet exemple cherche une contrainte violée a^T y >=1,
      avec exactement 2 valeurs non nulles dans a    */

    //Trouver indices idxMin,idx2ndMin des 3 plus petites vals de y
    int idxMin = 0, idx2ndMin = 0, idx3rdMin = 0;
    for(int i=1;i<n;i++)
        if(y[i]<y[idxMin])
            idxMin = i;
    if(idxMin==0)
        idx2ndMin=1;
    for(int i=idx2ndMin+1;i<n;i++)
        if(i!=idxMin)
            if(y[i]<y[idx2ndMin])
                idx2ndMin = i;

    if(idxMin==0 || idx2ndMin==1)
        idx3rdMin=2;

    for(int i=idx3rdMin+2;i<n;i++)
        if(i!=idx2ndMin)
            if(y[i]<y[idx3rdMin])
                idx3rdMin = i;

    std::cout<<"Le sous-problème de séparation a trouvé la coupe:";
    std::cout<<"y["<<idxMin<<"]+y["<<idx2ndMin<<"]+y["<<idx3rdMin<<"]>=5\n";

    //Remplir le tableau a dans la coupe a^T y >= d
    for (int i=0;i<n;i++)
        a[i]=0;

    a[idxMin]    = 1;
    a[idx2ndMin] = 1;
    a[idx3rdMin] = 1;
    d            = 6;

    //renvoyer a^T y - d (négatif si a^T y >= d est violée)
    return 2*y[idxMin]+2*y[idx2ndMin]+2*y[idx3rdMin]-d;
}
int main()
{
    int iters, n=20;               //itérations et no de variables
    double *ones = new double[n]; //vecteur de la fonc. objectif
    double finalObj, time;

    //construire l'objet de plans coupants avec fonc de séparation
    CuttingPlanesEngine cutPlanes(n,separateFunc);
    // cutPlanes.turnAllVarsInteger();
    cutPlanes.setVarBounds(0,100);
    for (int i=0;i<n;i++)
        ones[i] = 1;
    cutPlanes.setObjCoefsMinimize(ones);
    //cutPlanes.activateLog();              //voir messages de log
    //cutPlanes.turnAllVarsInteger();       //utile pour Benders!

    //Faire tourner les plans coupants
    cutPlanes.runCutPlanes(iters, time);    //renvoie 0 si échec
    cutPlanes.turnAllVarsInteger();
    finalObj = cutPlanes.getObjVal();

    //Imprimer la valeur objectif finale
    std::cout<<"\nObj="<<finalObj<<", après "<<iters<<" iters.\n";
    double*y = new double[n];
    cutPlanes.getPrimals(y);
    for(int i=0;i<n;i++)
        if(y[i]>1.0e-6)
            std::cout<<"y["<<i<<"]="<<y[i]<<"; ";
    std::cout<<std::endl;
}
