# TP MLA (disponible aussi à cette [adresse](https://github.com/AdrienBlassiau/PROJET_MLA/tree/master/cplexbenders))

Nous vous conseillons de  lire ce rapport sous https://dillinger.io/ par exemple pour une meilleure lisibilité (ou directement sur le github du projet).

# 1. Conseils d'utilisation

Chaque TP possède son propre exécutable : 

**1. TP Benders** : 
> make tp1 && ./tp1

**2.1 TP Modèle de base sans décomposition** : 
> make tp2_1 && ./tp2_1 n

**2.2 TP Décomposition de Benders manuelle** : 
> make tp2_2 && ./tp2_2 n

**3. TP Décomposition automatique** : 
> make tp3 && ./tp3 n

**4. TP Résolution ultra-rapide** : 
> make tp4 && ./tp4 n

# 2. Résultats obtenus

La machine utilisée pour les tests possède **8 GO de ram** et un **processeur i5**. On utilise cplex 12.8 sur une instance de taille 50 000. La résolution ultra-rapide est bien la meilleure !

| n = 50 000 (cplex 12.8)                   | temps (s) |
|-------------------------------------------|-----------|
| TP2.1 : Modèle de base sans décomposition | 28.31     |
| TP2.2 : Décomposition de Benders manuelle | 3.88221   |
| TP3 : Décomposition automatique           | 4.6       |
| TP4 : Résolution ultra-rapide             | 0.28      |
 
*******************************************************************************

Auteurs
---------------------

* **Adrien BLASSIAU** _alias_ [@AdrienBlassiau](https://github.com/AdrienBlassiau)
* **Juliette GRIÉ** _alias_ [@ZenonLigre](https://github.com/ZenonLigre)
* **Pierre LAPLAIZE** _alias_ [@Laplaize](https://github.com/Laplaize)

*Merci pour votre lecture !*
