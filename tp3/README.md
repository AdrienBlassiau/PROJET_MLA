# TP3 MLA (disponible aussi à cette [adresse](https://github.com/AdrienBlassiau/PROJET_MLA/tree/master/tp3))

Nous vous conseillons de  lire ce rapport sous https://dillinger.io/ par exemple pour une meilleure lisibilité (ou directement sur le github du projet). Le rapport étant très court, nous avons décidé de le faire sous ce format. N'hésitez pas à nous signaler si cela ne vous convient pas :).

# 1. Conseils d'utilisation

Chaque exercice possède son propre exécutable : 

**exo1 Benders entier** : 
> make exo1 && ./exo1 FILENAME

**exo1.2 PL + Benders auto (BONUS)** : 
> make exo1_2 && ./exo1_2 FILENAME

**exo2 Bender fractionnaire puis Benders entier** : 
> make exo2 && ./exo2 FILENAME

**exo3 Dijkstra** : 
> make exo3 && ./exo3 FILENAME

# 2. Résultats obtenus

La machine utilisée pour les tests possède **8 GO de ram** et un **processeur i5**. On utilise cplex 12.8 sur les instances fournies.

* EXO1 signifie que la méthode utilisée est celle de l'exercice 1
* EXO2 signifie que la méthode utilisée est celle de l'exercice 2

Pour une **bande passant de 1**, on obtient les résultats suivants : 

| benders-graphe-hexagone.txt (cplex 12.8) | nombre d'itérations | temps (s) |
|------------------------------------------|---------------------|-----------|
| EXO1                                     | 8                   | 0.0316132 |
| EXO2                                     | 8                   | 0.0276813 |

| benders1.txt (cplex 12.8)                | nombre d'itérations | temps (s) |
|------------------------------------------|---------------------|-----------|
| EXO1                                     | 75                  | 0.391275  |
| EXO2                                     | 67                  | 0.19703   |

| benders2.txt (cplex 12.8)                | nombre d'itérations | temps (s) |
|------------------------------------------|---------------------|-----------|
| EXO1                                     | 109                 | 1.78401   |
| EXO2                                     | 71                  | 0.37575   |

| benders3.txt (cplex 12.8)                | nombre d'itérations | temps (s) |
|------------------------------------------|---------------------|-----------|
| EXO1                                     | 112                 | 7.41228   |
| EXO2                                     | 113                 | 0.689917  |

| benders4.txt (cplex 12.8)                | nombre d'itérations | temps (s) |
|------------------------------------------|---------------------|-----------|
| EXO1                                     | 109                 | 0.827707  |
| EXO2                                     | 71                  | 0.361992  |

Pour une **bande passant de 3**, on obtient les résultats suivants : 

| benders-graphe-hexagone.txt (cplex 12.8) | nombre d'itérations | temps (s)  |
|------------------------------------------|---------------------|------------|
| EXO1                                     | 6                   | 0.00714982 |
| EXO2                                     | 6                   | 0.00759742 |

| benders1.txt (cplex 12.8)                | nombre d'itérations | temps (s)  |
|------------------------------------------|---------------------|------------|
| EXO1                                     | 46                  | 0.198053   |
| EXO2                                     | 44                  | 0.143196   |

| benders2.txt (cplex 12.8)                | nombre d'itérations | temps (s)  |
|------------------------------------------|---------------------|------------|
| EXO1                                     | 63                  | 0.568966   |
| EXO2                                     | 79                  | 0.40688    |

| benders3.txt (cplex 12.8)                | nombre d'itérations | temps (s)  |
|------------------------------------------|---------------------|------------|
| EXO1                                     | 75                  | 0.865002   |
| EXO2                                     | 75                  | 0.45157    |

| benders4.txt (cplex 12.8)                | nombre d'itérations | temps (s)  |
|------------------------------------------|---------------------|------------|
| EXO1                                     | 54                  | 0.474284   |
| EXO2                                     | 50                  | 0.270198   |
 
*******************************************************************************

Auteurs
---------------------

* **Adrien BLASSIAU** _alias_ [@AdrienBlassiau](https://github.com/AdrienBlassiau)
* **Pierre LAPLAIZE** _alias_ [@Laplaize](https://github.com/Laplaize)

*Merci pour votre lecture !*
