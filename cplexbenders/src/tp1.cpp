////////////////
// TP Benders //
////////////////

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

void loadData(){
    int f2[] = {7, 2, 2, 7, 7, 1};
    int c2[] = {66, 5, 4, 3, 2};

    n=6;
    d=3;

    f = new int[n+1];
    c = new int[n];
    f[0] = f2[0];
    c[0] = c2[0];
    for(int i=1;i<n;i++){
        f[i] = f2[i];
        c[i] = c2[i];
    }
}

double separateFunc (const int n, double*y, double * a, double&rHand)
{
    cout<<"Starting sep function 1...";
    cout.flush();
    COUT("Cutting y=");
    for(int i=0;i<n;i++)
        COUT(y[i]<<" ");
    COUT(endl);

    IloEnv env;

    // -----------------------MODELE  -----------------------
    IloModel model(env, "model_tp1");


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
    cplex.exportModel("model_tp1.lp");
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

    double objVal = rHand;
    for(int i=0;i<n-1;i++){
        a[i]=vals[i];
        objVal-=y[i]*vals[i];
    }
    a[n-1]=1;

    return -objVal+y[n-1];
}

int main()
{
    loadData();
    int iters;
    double finalObj, time;

    //construire l'objet de plans coupants avec fonc de s�paration
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
    cutPlanes.runCutPlanes(iters, time);    //renvoie 0 si �chec



    finalObj = cutPlanes.getObjVal();

    cout<<endl<<"############## RESULTATS #############"<<endl;

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
    cout<<"d="<<d<<endl;
    for(int i=0;i<n-1;i++)
       if(y[i]>EPSILON)
           cout<<f[i]<<"/"<<c[i]<<endl;
    exit(1);
}
