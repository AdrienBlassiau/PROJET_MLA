# TP3 MLA (disponible aussi à cette [adresse](https://github.com/AdrienBlassiau/PROJET_MLA/tree/master/tp3))

Nous vous conseillons de  lire ce rapport sous https://dillinger.io/ par exemple pour une meilleure lisibilité (ou directement sur le github du projet). Le rapport étant très court, nous avons décidé de le faire sous ce format. N'hésitez pas à nous signaler si cela ne vous convient pas :).

# 1. Conseils d'utilisation

Chaque exercice possède son propre exécutable : 

**exo1 Benders entier** : 
> make exo1 && ./exo1 FILENAME

**exo2 Benders fractionnaire puis Benders entier** : 
> make exo2 && ./exo2 FILENAME

**exo3 PL** : 
> make exo3 && ./exo3 FILENAME

# 2. Résultats obtenus

## Exercice 1 et 2

La machine utilisée pour les tests possède **8 GO de ram** et un **processeur i5**. On utilise cplex 12.8 sur les instances fournies.

* EXO1 signifie que la méthode utilisée est celle de l'exercice 1
* EXO2 signifie que la méthode utilisée est celle de l'exercice 2

Pour une **bande passant de 1**, on obtient les résultats suivants : 

| benders-graphe-hexagone.txt | nombre total d'itérations  | temps total (s) |
|-----------------------------|----------------------------|-----------------|
| EXO1                        | 8                          | 0.0316132       |
| EXO2                        | 8 (0 entière)              | 0.0276813       |

| benders1.txt                | nombre total d'itérations  | temps total (s) |
|-----------------------------|----------------------------|-----------------|
| EXO1                        | 75                         | 0.391275        |
| EXO2                        | 67 (0 entière)             | 0.19703         |      

| benders2.txt                | nombre total d'itérations  | temps total (s) |
|-----------------------------|----------------------------|-----------------|
| EXO1                        | 109                        | 1.78401         |
| EXO2                        | 71 (0 entière)             | 0.37575         |      

| benders3.txt                | nombre total d'itérations  | temps total (s) |
|-----------------------------|----------------------------|-----------------|
| EXO1                        | 112                        | 7.41228         |
| EXO2                        | 113 (0 entière)            | 0.689917        |       

| benders4.txt                | nombre total d'itérations  | temps total (s) |
|-----------------------------|----------------------------|-----------------|
| EXO1                        | 109                        | 0.827707        |
| EXO2                        | 71 (0 entière)             | 0.361992        |      

Pour une **bande passant de 3**, on obtient les résultats suivants : 

| benders-graphe-hexagone.txt | nombre total d'itérations | temps total (s)  |
|-----------------------------|---------------------------|------------------|
| EXO1                        | 9                         | 0.0107221        |
| EXO2                        | 8 (0 entière)             | 0.00895971       |

| benders1.txt                | nombre total d'itérations | temps total (s)  |
|-----------------------------|---------------------------|------------------|
| EXO1                        | 201                       | 23.1322          |
| EXO2                        | 192 (dont 120 entières)   | 12.2783          |

Pour les autres instances, on ne trouve pas de solution dans un temps raisonnable.

## Exercice 3

On a implémenté le programme linéaire. Les solutions obtenues pour une **bande passant de 1** sont les mêmes que celles données par la décomposition de benders :

| benders-graphe-hexagone.txt | solution obtenue           | temps total (s) |
|-----------------------------|----------------------------|-----------------|
| BENDERS                     | 21                         | 0.00945418      |
| PL                          | 21                         | 0.00001         |

| benders1.txt                | solution obtenue           | temps total (s) |
|-----------------------------|----------------------------|-----------------|
| BENDERS                     | 688                        | 0.14121         |
| PL                          | 688                        | 0.01            |      
| benders2.txt                | solution obtenue           | temps total (s) |
|-----------------------------|----------------------------|-----------------|
| BENDERS                     | 390                        | 0.307605        |
| PL                          | 390                        | 0.04            |      

| benders3.txt                | solution obtenue           | temps total (s) |
|-----------------------------|----------------------------|-----------------|
| BENDERS                     | 579                        | 7.45729         |
| PL                          | 579                        | 0.04            |       

| benders4.txt                | solution obtenue           | temps total (s) |
|-----------------------------|----------------------------|-----------------|
| BENDERS                     | 93                         | 0.775179        |
| PL                          | 93                         | 0.02            |

## Exercice 4

b) ;)

*******************************************************************************

Auteurs
---------------------

* **Adrien BLASSIAU** _alias_ [@AdrienBlassiau](https://github.com/AdrienBlassiau)
* **Pierre LAPLAIZE** _alias_ [@Laplaize](https://github.com/Laplaize)

*Merci pour votre lecture !*
