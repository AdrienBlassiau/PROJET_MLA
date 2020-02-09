#include "CuttingPlanesEngine.h"
#include <iostream>
#include <cstdlib>
#include <ilcplex/ilocplex.h>
#define EPSILON 1.0e-6
#define COUT(x) {}
#define COUT2(x) {cout<<x;}
using namespace std;

int n,d;
int *f,*c;

void loadData(int nn, int benders){
    n=nn;
    d=n/2;
    f = new int[n+1];
    c = new int[n];
    f[0] = 7;
    c[0] = 8;
    for(int i=1;i<n;i++){
        f[i] = (f[i-1]*f[0])%159;
        c[i] = (c[i-1]*c[0])%61;
    }
    if(benders){
        f[n] = 1;
        n++;
    }
}

int main(int argc, char**  argv)
{
    if(argc==1){
        cerr<<"Give me a size"<<endl;
        return 0;
    }
    int nn = atoi(argv[1]);
    loadData(nn,1);

    IloEnv env;
    try{
        // -----------------------MODELE  -----------------------
        IloModel model(env, "exo 2");


        // -----------------------VARIABLES  -----------------------
        IloNumVarArray x(env, n-1, 0, INT_MAX);
        IloBoolVarArray y(env, n-1);
        IloExpr obj(env);

        // -----------------------OBJECTIF  -----------------------
        for(int i=0;i<n-1;i++){
            obj+= y[i] * f[i];
        }

        for(int i=0;i<n-1;i++){
            obj+= x[i] * c[i];
        }

        IloObjective ilo_obj(env, obj, IloObjective::Minimize);
        model.add(ilo_obj);


        // -----------------------CONTRAINTES  -----------------------

        // CONTRAINTE 1
        model.add(y[0] >= y[1]);
        //--

        // CONTRAINTE 2
        model.add(y[0] >= y[2]);
        //--

        // CONTRAINTE 3
        IloExpr sc(env) ;

        for (int i=0;i<n-1;i++){
            sc += x[i];
        }

        model.add(sc == d);
        //--

        // CONTRAINTE 4
        for (int i=0;i<n-1;i++){
            model.add(x[i] <= y[i]);
        }
        //--

        // -----------------------SOLVE  ----------------------

        IloCplex cplex(model);
        cplex.exportModel("model.lp");
        cplex.solve();


        // -----------------------AFFICHAGE  ----------------------
        IloNumArray valsx(env);
        IloNumArray valsy(env);
        cplex.getValues(valsx, x);
        cplex.getValues(valsy, y);

        cout<<endl<<"############## RESULTATS #############"<<endl;

        cout<<"\nObj="<<cplex.getObjValue()<<endl;;

        cout<<"Open facilities (y)  :";

        for(int i=0;i<((n-1<60)?n-1:60);i++)
        if(valsy[i]>1.0e-6)
                cout<<1;
            else
                cout<<0;
        cout<<endl;

        cout<<"Temps d'execution : "<<cplex.getTime()<<endl;
    }
    catch(IloException& e) {
        cerr << " ERREUR : exception = "<< e << endl;
    }

    env.end();
    return 0;
}
