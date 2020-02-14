/////////////////////////
// EXO1 Benders entier //
/////////////////////////

#include "CuttingPlanesEngine.h"
#include <iostream>
#include <cstdlib>
#include <ilcplex/ilocplex.h>
#define EPSILON 1.0e-6
#define COUT(x) {}
#define COUT2(x) {cout<<x;}
using namespace std;

int  n;         //no sommets
int  m;         //no arêtes
int* b;         //demandes
int** mat;       //matrice d'adjacence
int* aretes[2]; //m arêtes, chacune avec deux sommets
int bnd = 1;

double separateFunc (const int m, double*y, double * a, double&rHand)
{
    // printf("n : %d et m : %d\n",n,m);
    // cout<<"Starting sep function 1...";
    // cout.flush();
    // cout<<"Cutting y=";
    // for(int i=0;i<m;i++)
    //     cout<<y[i]<<" ";
    // cout<<endl;

    double res=0;
    for(int i=0;i<m;i++)
        res+=y[i];
    printf("CUR RES : %f\n",res);

    IloEnv env;

    // -----------------------MODELE  -----------------------
    IloModel model(env, "model_tp2");

    // -----------------------VARIABLES  -----------------------
    IloNumVarArray v(env, m+n-1, 0, INT_MAX);
    IloExpr obj(env);

    // -----------------------OBJECTIF  -----------------------
    for(int i=0;i<m;i++){
        // printf("i:%d, bnd: %d et y[%d]=%f et v[%d]:%f\n",i,bnd,i,y[i],i,v[i]);
        obj -= bnd*y[i]*v[i];
    }
    for(int i=m;i<m+n-1;i++){
        obj+=b[i-m+1]*v[i];
    }

    IloObjective ilo_obj(env,obj,IloObjective::Maximize);
    model.add(ilo_obj);

    // -----------------------CONTRAINTES  -----------------------

    /*-----CONTRAINTE 1    ----------*/
    IloRangeArray constr(env);
    IloExpr e(env);
    for(int i=0;i<m+n-1;i++){
        e+=v[i];
    }
    model.add(e==1);

    /*-----CONTRAINTE 2    ----------*/
    int i_j = 0;
    for (int i=0;i<n;i++){
        for (int j=i;j<n;j++){
            if (mat[i][j] ==1){
                if (i!=0){
                    if (j!=0){
                        // printf("%d et %d et %d et %d\n",i_j,m+i-1,m+j-1,m+n-1);
                        model.add(-v[i_j] - v[m+i-1] + v[m+j-1] <= 0);
                        model.add(-v[i_j] + v[m+i-1] - v[m+j-1] <= 0);
                    }
                    else{
                        model.add(-v[i_j] - v[m+i-1] <= 0);
                        model.add(-v[i_j] + v[m+i-1] <= 0);
                    }
                }
                else{
                    if (j!=0){
                        model.add(-v[i_j] + v[m+j-1] <= 0);
                        model.add(-v[i_j] - v[m+j-1] <= 0);
                    }
                }
                i_j+=1;
            }
        }
    }
    // // -----------------------SOLVE  1 ----------------------
    IloCplex cplex(model);
    // cplex.exportModel("model_tp1.lp");
    cplex.solve();


    //Imprimer la valeur objectif finale
    // cout<<endl<<"############## RESULTATS #############"<<endl;

    // cout<<"\nObj="<<cplex.getObjValue()<<endl;;

    // // -----------------------RETOUR  ----------------------
    IloNumArray vals(env);
    cplex.getValues(vals, v);

    rHand = 0.;
    for(int i=m;i<m+n-1;i++){
        rHand += b[i-m+1]*vals[i];
    }

    double objVal = rHand;
    for(int i=0;i<m;i++){
        a[i] = bnd*vals[i];
        // printf("a[%d] = %f\n",i,a[i]);
        // printf("bnd,y[%d]: %f,vals[%d] : %f\n",i,cur_y,i,vals[i]);
        objVal -= bnd*y[i]*vals[i];
    }

    env.end();
    // printf("rHand : %f\n",rHand);
    // printf("objVal : %f\n",objVal);
    return -objVal;
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


    int iters;
    double *ones = new double[m]; /*Pour la fonction objectif*/
    double finalObj, time;

    //construire l'objet de plans coupants avec fonc de séparation
    CuttingPlanesEngine cutPlanes(m,separateFunc);
    for (int i = 0; i < m; i++){
        ones[i] = 1;
    }
    cutPlanes.setObjCoefsMinimize(ones);

    /*-------Etape 1 -----------------*/

    // cutPlanes.activateLog();              //voir messages de log
    cutPlanes.turnAllVarsInteger();       // Pour benders


    //Faire tourner les plans coupants
    cutPlanes.runCutPlanes(iters, time);    //renvoie 0 si échec
    finalObj = cutPlanes.getObjVal();

    cout<<endl<<"############## RESULTATS #############"<<endl;

    cout<<"\nObj="<<finalObj<<", after "<<iters<<" iters.\n";

    double *y = new double[m];
    cutPlanes.getPrimals(y);

    for (int i = 0; i < m; ++i){
        if (y[i]>1.0e-6){
            cout<<"y["<<i<<"]="<<y[i]<<"; ";
        }
    }
    cout<<endl;
    cout<<"Temps d'execution : "<<time<<endl;

    exit(1);
}
