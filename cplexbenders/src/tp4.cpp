/////////////////////////////
// Résolution ultra-rapide //
/////////////////////////////

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

int bmax = INT_MIN;
int *v_sol ;
double separateFunc (const int n, double*y, double * a, double&rHand)
{
    double objVal;

    // TEST RESOLUTION RAPIDE
    int sum_y = 0;

    for(int i=0;i<n-1;i++){
        sum_y += y[i];
    }

    cout << "sum y : "<<sum_y << endl;
    cout << "d : "<<d << endl;

    if (sum_y == d){
        printf("Resolution rapide\n");
        objVal = 0;
        for(int i=0;i<n-1;i++){
            objVal += y[i]*c[i];
        }

        if(bmax==INT_MIN){
            bmax=c[0];
            for(int i=1;i<n-1;i++)
                if(c[i]>bmax)
                    bmax=c[i];
            v_sol = new int[n];
            for(int i=0;i<n-1;i++)
                v_sol[i] = bmax-c[i];
        }
        for(int i=0;i<n-1;i++)
            a[i]=v_sol[i];
        a[n-1] = 1;

        rHand = d*bmax;

        return -objVal+y[n-1];
    }

    cout<<"Starting sep function 1...";
    cout.flush();
    COUT("Cutting y=");
    for(int i=0;i<n;i++)
        COUT(y[i]<<" ");
    COUT(endl);

    IloEnv env;

    // -----------------------MODELE  -----------------------
    IloModel model(env, "exo1");


    // -----------------------VARIABLES  -----------------------
    IloNumVarArray v(env, n-1, 0, INT_MAX);
    IloNumVar b(env, INT_MIN, INT_MAX);
    IloExpr obj(env);


    // -----------------------OBJECTIF  -----------------------
    obj =  d*b;

    for(int i=0;i<n-1;i++){
        obj+= -y[i] * v[i];
    }

    IloObjective ilo_obj(env, obj, IloObjective::Maximize);
    model.add(ilo_obj);

    // -----------------------CONTRAINTES  -----------------------
    for (int i=0;i<n-1;i++){
        model.add(b - v[i] <= c[i]);
    }

    // -----------------------SOLVE  1 ----------------------
    IloCplex cplex(model);
    cplex.exportModel("model.lp");
    cplex.solve();

    // -----------------------SOLVE  2 ----------------------
    IloNumArray newvals(env,n-1);
    // IloNum value;

    for(int i=0;i<n-1;i++){
        if(y[i]<0.1)
            newvals[i] = -1;
        else
            newvals[i] = 0;
    }
    // value=0;

    ilo_obj.setLinearCoefs(v,newvals);
    // ilo_obj.setLinearCoef(b,value);

    obj.end();
    model.add(ilo_obj);
    cout<<"solving again.."<<flush;
    cplex.solve();
    cout<<"...done"<<endl;
    ilo_obj.end();
    newvals.end();

    // -----------------------RETOUR  ----------------------
    IloNumArray vals(env);
    cplex.getValues(vals, v);

    rHand = d*cplex.getValue(b);

    objVal = rHand;
    for(int i=0;i<n-1;i++){
        a[i]=vals[i];
        objVal-=y[i]*vals[i];
    }
    a[n-1]=1;

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
    // cutPlanes.activateLog();              //voir messages de log

    int * y0_greater_y1 = new int[n];
    int * y0_greater_y2 = new int[n];
    int *        sum_y  = new int[n];

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

    cout<<"Open facilities (y)  :";

    for(int i=0;i<((n-1<60)?n-1:60);i++)
        if(y[i]>1.0e-6)
                cout<<1;
            else
                cout<<0;
    cout<<endl;
    cout<<"w="<<y[n-1]<<endl;
    cout<<"Temps d'execution : "<<time<<endl;

    exit(1);
}
