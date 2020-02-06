#include "CuttingPlanesEngine.h"
#include <iostream>
#include <cstdlib>
#include <ilcplex/ilocplex.h>
#define EPSILON 1.0e-6
#define COUT(x) {}
#define COUT2(x) {cout<<x;}
using namespace std;

// int n=6;                     //number of vars including w
// int f[] = {7, 2, 2, 7, 7, 1};//last entry is the coef of w
// int c[] = {66, 5, 4, 3, 2};
// int d   = 3;
int n,d;
int *f,*c;

void loadData(int nn, int benders){
    int f2[] = {7, 2, 2, 7, 7, 1};//last entry is the coef of w
    int c2[] = {66, 5, 4, 3, 2};
    int d2   = 3;

    n=nn;
    d=n/2;

    f = new int[n+1];
    c = new int[n];
    f[0] = f2[0];
    c[0] = c2[0];
    for(int i=1;i<n;i++){
        f[i] = f2[i];
        c[i] = c2[i];
    }

    // n=nn;
    // d=n/2;
    // f = new int[n+1];
    // c = new int[n];
    // f[0] = 7;
    // c[0] = 8;
    // for(int i=1;i<n;i++){
    //     f[i] = (f[i-1]*f[0])%159;
    //     c[i] = (c[i-1]*c[0])%61;
    // }
    // if(benders){
    //     f[n] = 1;
    //     n++;
    // }
}
//étant donnée la solution courante y, trouver une contrainte violée
//a^T y >= d et renvoyer a^T y - d. Remplir vecteur a et valeur d!!
double separateFunc (const int n, double*y, double * a, double&rHand)
{
    int i;
    // printf("d = %d\n",d);
    // for (i = 0; i < n; ++i){
    //     printf("c[%d] = %d\n",i,c[i]);
    // }
    cout<<"Starting sep function 1...";
    cout.flush();
    COUT("Cutting y=");
    for(int i=0;i<n;i++)
        COUT(y[i]<<" ");
    COUT(endl);
    IloEnv env;               //environnement toujours nécessaire
    IloModel model(env);      //le modèle contient les contraintes+l'objectif
    IloCplex cplex (model);     //L'objet Cplex s'occupe de l'algorithmique

    IloNumVarArray v(env, n, 0, INT_MAX);
    IloExpr obj(env);

    obj =  d*v[n-1];            //v[n-1] is b in the slides

    for(int i=0;i<n-1;i++)
        obj+= -y[i] * v[i];
    IloObjective ilo_obj(env, obj, IloObjective::Maximize);
    model.add(ilo_obj);

    for (int i=0;i<n-1;i++){
        printf("c[%d]=%d\n",i,c[i]);
        model.add(v[n-1] - v[i] <= c[i]);
    }

    //ajouter le code ci-dessous pour demander à cplex de minimiser
    //les valeurs des v lorsqu'il y a plusieurs solutions v optimales
    for(int i=0;i<n-1;i++)
        if(y[i]<0.1)
            ilo_obj.setLinearCoef(v[i],-1);
        else
            ilo_obj.setLinearCoef(v[i],0);
    ilo_obj.setLinearCoef(v[n-1],0);
    model.add(ilo_obj);
    cplex.solve();

    cout << " objectif = "<< cplex.getObjValue() << endl;

    IloNumArray vals(env);
    cplex.getValues(vals, v);
    cout << " variables =  "<< vals << endl;


    double objVal = rHand;
    for(int i=0;i<n-1;i++){
        a[i]=vals[i];
        objVal-=y[i]*vals[i];
    }
    a[n-1]=vals[n-1];

    cout<<"objValSep2="<<objVal<<endl;
    return -objVal+y[n-1];
}

int main(int argc, char**  argv)
{
    if(argc==1){
        cerr<<"Give me a size"<<endl;
        return 0;
    }
    int nn = atoi(argv[1]);
    loadData(nn,1);
    int iters;
    double finalObj, time;

    //construire l'objet de plans coupants avec fonc de séparation
    CuttingPlanesEngine cutPlanes(n,separateFunc);
    cutPlanes.setObjCoefsMinimize(f);
    cutPlanes.turnAllVarsInteger();
    cutPlanes.activateLog();              //voir messages de log

    int * y0_greater_y1 = new int[n];
    int * y0_greater_y2 = new int[n];
    int *        sum_y  = new int[n];
    cout<<"n="<<n<<endl;
    for(int i=0;i<n;i++){
        y0_greater_y1[i] = 0;
        y0_greater_y2[i] = 0;
               sum_y [i] = 1;
    }
    y0_greater_y1[0]   = 1;
    y0_greater_y2[0]   = 1;
    y0_greater_y1[1]   = -1;
    y0_greater_y2[2]   = -1;
           sum_y [n-1] = 0;

    cutPlanes.modelAddCut(y0_greater_y1, 0);
    cutPlanes.modelAddCut(y0_greater_y2, 0);
    cutPlanes.modelAddCut(       sum_y , d);
    //fix =d for(int i=0;i<n-1;i++) sum_y [i] = - 1; cutPlanes.modelAddCut(sum_y , -d);
    cout<<"new cuts..."<<flush;
    cutPlanes.setVarBounds(0,n-2,0,1);
    cout<<"done";

    ////cout<<"Affectation costs c=";
    ////for(int i=0;i<n-1;i++)
    ////    cout<<c[i]<<" ";
    ////cout<<endl;
    //Faire tourner les plans coupants
    cutPlanes.runCutPlanes(iters, time);    //renvoie 0 si échec



    finalObj = cutPlanes.getObjVal();

    //Imprimer la valeur objectif finale
    cout<<"\nObj="<<finalObj<<", after "<<iters<<" iters.\n";
    double*y = new double[n];
    cutPlanes.getPrimals(y);

    cout<<"Open facilities   :";

    for(int i=0;i<((n-1<60)?n-1:60);i++)
        if(y[i]>1.0e-6)
                cout<<1;
            else
                cout<<0;
    cout<<endl;


    exit(1);
    //cout<<y[n-1];
    //cout<<"d="<<d<<endl;
    //for(int i=0;i<n-1;i++)
    //    if(y[i]>EPSILON)
    //        cout<<f[i]<<"/"<<c[i]<<endl;
}
