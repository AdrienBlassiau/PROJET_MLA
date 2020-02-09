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

//étant donnée la solution courante y, trouver une contrainte violée
//a^T y >= d et renvoyer a^T y - d. Remplir vecteur a et valeur d!!
double separateFunc (const int n, double*y, double * a, double&rHand)
{
    cout<<"Starting sep function 1...";
    cout.flush();
    COUT("Cutting y=");
    for(int i=0;i<n;i++)
        COUT(y[i]<<" ");
    COUT(endl);
    IloEnv env;
    IloModel model(env);

    IloNumVarArray v(env, n, 0, INT_MAX);
    IloExpr obj(env);

    obj =  d*v[n-1];

    for(int i=0;i<n-1;i++){
        obj+= -y[i] * v[i];
    }

    IloObjective ilo_obj(env, obj, IloObjective::Maximize);
    model.add(ilo_obj);

    for (int i=0;i<n-1;i++){
        model.add(v[n-1] - v[i] <= c[i]);
    }

    IloCplex cplex(model);
    cplex.exportModel("model.lp");
    cplex.solve();

    IloNumArray newvals(env,n);
    for(int i=0;i<n-1;i++){
        if(y[i]<0.1)
            newvals[i] = -1;
        else
            newvals[i] = 0;
    }
    newvals[n-1]=0;
    ilo_obj.setLinearCoefs(v,newvals);
    obj.end();
    model.add(ilo_obj);
    cout<<"solving again.."<<flush;
    cplex.solve();
    cout<<"...done"<<endl;
    ilo_obj.end();
    newvals.end();


    IloNumArray vals(env);
    cplex.getValues(vals, v);


    for (int i = 0; i < n; ++i){
        printf("vals[%d] = %f\n",i,vals[i]);
    }

    rHand = d*vals[n-1];

    printf("rHand : %f\n",rHand);
    double objVal = rHand;
    for(int i=0;i<n-1;i++){
        a[i]=vals[i];
        objVal-=a[i]*y[i];
    }
    a[n-1]=1;

    cout<<"objValSep2="<<objVal<<endl;
    return -objVal+y[n-1];
}

int bmax = INT_MIN;
int *v_sol ;
int cmax = INT_MIN;
int *a_unique ;
double separateFunc2 (const int n, double*y, double * a, double&rHand){
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

    COUT("Cutting y=");
    for(int i=0;i<n;i++)
        COUT(y[i]<<" ");
    COUT(endl);
    if(cmax==INT_MIN){
        cmax=c[0];
        for(int i=1;i<n-1;i++)
            if(c[i]>cmax)
                cmax=c[i];
        a_unique = new int[n];
        for(int i=0;i<n-1;i++)
            a_unique[i] = cmax-c[i];
    }
    for(int i=0;i<n-1;i++)
        a[i]=a_unique[i];
    a[n-1] = 1;
    printf("CMAX = %d\n",cmax);
    rHand = d*cmax;

    COUT("Separation sub-problem form 1 finds:");
    for(int i=0;i<n;i++)
        if(a[i]!=0)
            COUT(a[i]<<"y_"<<(i+1)<<"+");
    COUT(">="<<rHand<<"-w"<<endl);
    COUT(endl);

    objVal = rHand;
    for(int i=0;i<n-1;i++){
        objVal-=a[i]*y[i];
    }

    cout<<"RHand : "<<rHand<<endl;

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
    CuttingPlanesEngine cutPlanes(n,separateFunc2);
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
