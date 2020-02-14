/////////////////////////////////
// EXO1.2 PL avec Benders auto //
/////////////////////////////////

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
int**mat;       //matrice d'adjacence
int* aretes[2]; //m arêtes, chacune avec deux sommets
int bnd = 1;

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


    IloEnv env;
    try{
        // -----------------------MODELE  -----------------------
        IloModel model(env, "model_tp2_bis");


        // -----------------------VARIABLES  -----------------------
        IloNumVarArray y(env, m, 0, INT_MAX,ILOINT);
        IloNumVarArray x(env, n*n, 0, INT_MAX);
        IloExpr obj(env);

        // -----------------------OBJECTIF  -----------------------
        for(int i=0;i<m;i++){
            obj+= y[i];
        }

        IloObjective ilo_obj(env, obj, IloObjective::Minimize);
        model.add(ilo_obj);


        // -----------------------CONTRAINTES  -----------------------

        for (int i = 0; i<n; ++i){
            for (int j = 0; j<n; ++j){
                model.add(x[i+(n-1)*j]>= 0);
            }
        }
        // CONTRAINTE b et c
        for (int i=0;i<n;i++){
            IloExpr s1(env) ;
            IloExpr s2(env) ;
            IloExpr s3(env) ;
            IloExpr s4(env) ;
            if (i!=0){
                for (int j=0;j<n;j++){
                    if (mat[i][j] == 1){
                        if (b[i]==0){
                            s1 += x[j+(n-1)*i];
                            s2 += x[i+(n-1)*j];
                        }
                        else{
                            s3 += x[j+(n-1)*i];
                            s4 += x[i+(n-1)*j];
                        }
                    }
                }

                if (b[i]==0){
                    model.add(s1-s2 >= 0);
                }
                else{
                    model.add(s3-s4 >= b[i]);
                }

            }
        }

        // CONTRAINTE d
        int i_j = 0;
        for (int i=0;i<n;i++){
            for (int j=i;j<n;j++){
                if (mat[i][j] == 1){
                    IloConstraint c(bnd*y[i_j]-x[i+(n-1)*j]-x[j+(n-1)*i] >= 0);
                    c.setName("1d");
                    model.add(c);
                    i_j++;
                }
            }
        }

        // -----------------------SOLVE  ----------------------

        IloCplex cplex(model);
        cplex.setParam(IloCplex::Param::Benders::Strategy,IloCplex::BendersFull);
        cplex.solve();


        // -----------------------AFFICHAGE  ----------------------
        IloNumArray valsy(env);
        // IloNumArray valsx(env);
        cplex.getValues(valsy, y);
        // cplex.getValues(valsx, x);

        cout<<endl<<"############## RESULTATS #############"<<endl;
        cout<<"\nObj="<<cplex.getObjValue()<<endl;;

        for (int i = 0; i < m; ++i){
            if (valsy[i]>1.0e-6){
                cout<<"y["<<i<<"]="<<valsy[i]<<"; ";
            }
        }
        cout<<endl;

        cout<<"Temps d'execution : "<<cplex.getTime()<<endl;
    }
    catch(IloException& e) {
        cerr << " ERREUR : exception = "<< e << endl;
    }

    env.end();
    return 0;
}
