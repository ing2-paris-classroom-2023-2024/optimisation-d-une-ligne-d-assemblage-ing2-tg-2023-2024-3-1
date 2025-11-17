# Optimisation d'une ligne d'assemblage

Description
-----------

Ce projet implémente plusieurs contraintes et heuristiques pour l'optimisation d'une ligne d'assemblage (assignation des opérations aux stations) en langage C. Le code lit des fichiers d'entrée décrivant les opérations, les exclusions, les précédences et le temps de cycle, puis applique des contraintes successives (exclusion, temps, précédence, combinaisons) et affiche des affectations d'opérations par station.

Fonctionnalités principales
--------------------------

- Lecture des opérations et de leurs temps d'exécution.
- Gestion des contraintes d'exclusion (opérations incompatibles entre elles).
- Gestion des contraintes de précédence (graphe d'opérations dépendantes).
- Affectation des opérations aux stations selon le temps de cycle et d'autres heuristiques (ex. Welsh-Powell pour coloration/exclusion).

Prérequis
---------

- Un compilateur C (gcc/clang ou MSVC).
- CMake (version 3.10+ recommandée). 
- Outils de build (make / ninja / Visual Studio selon votre plate-forme).

Structure du dépôt
------------------

- `main.c` : point d'entrée qui orchestre la lecture des données et l'appel des contraintes.
- `ligne_assemblage.h` / `ligne_assemblage.c` : définitions et fonctions principales (lecture, affichage, structures).
- `exclusion.c`, `precedence.c`, `exclusion_temps.c`, `precedence_temps.c`, `precedence_temps.c` : différents modules de contraintes.
- `operations.txt`, `temps_cycle.txt`, `precedences.txt`, `exclusions.txt` : fichiers d'entrée attendus (exemples / formats ci-dessous).
- `CMakeLists.txt` : configuration de build.

Fichiers d'entrée et formats
----------------------------

1. `operations.txt`
	 - Chaque ligne décrit une opération :
		 numero temps
	 - Exemple :
		 1 2.5
		 2 1.75
	 - `numero` est un entier (identifiant de l'opération). `temps` est un flottant (durée).

2. `temps_cycle.txt`
	 - Contient un seul nombre flottant qui correspond au temps de cycle (capacité de chaque station).
	 - Exemple :
		 8.0

3. `precedences.txt`
	 - Chaque ligne contient une paire d'entiers décrivant une relation de précédence :
		 a b
	 - Signification : l'opération `a` doit précéder `b` (ou `a -> b`).
	 - Les identifiants doivent correspondre aux numéros utilisés dans `operations.txt`.

4. `exclusions.txt`
	 - Chaque ligne contient une paire d'entiers indiquant deux opérations incompatibles (ne peuvent pas être sur la même station) :
		 a b
	 - Exemple :
		 1 3
		 2 4

Remarques sur la numérotation
-----------------------------
Les modules utilisent les identifiants présents dans `operations.txt` pour relier les contraintes. Veillez à ce que les identifiants mentionnés dans `precedences.txt` et `exclusions.txt` correspondent bien aux numéros d'opération.

Build (Windows - PowerShell) / générique CMake
----------------------------------------------

Exemple de compilation générique avec CMake (depuis la racine du dépôt) :

```powershell
mkdir build; cmake -S . -B build
cmake --build build --config Debug
```

Ou (générique, multi-plateforme) :

```powershell
cmake -S . -B build
cmake --build build
```

Exécution
---------

Le binaire lit les fichiers d'entrée en path relatif depuis le répertoire de travail. Pour que les chemins relatifs fonctionnent (le code ouvre `../operations.txt` depuis le binaire généré dans le répertoire de build), lancez l'exécutable depuis le dossier `build` (ou `cmake-build-debug` selon votre configuration) ou adaptez les chemins dans le code.

Exemple (depuis PowerShell, depuis la racine du dépôt) :

```powershell
# si l'exécutable est généré dans build\
Set-Location build
./optimisation_d_une_ligne_d_assemblage_ing2_tg_2023_2024_3_1.exe
```

Ou, si vous utilisez l'exécutable directement depuis l'IDE (CLion/Visual Studio), assurez-vous que les fichiers `operations.txt`, `temps_cycle.txt`, `precedences.txt` et `exclusions.txt` se trouvent au bon chemin relatif attendu par le code (par défaut en racine du dépôt, la binaire devant être dans un sous-dossier `build`).

Exemples
--------

Fournissez des exemples simples dans les fichiers `operations.txt`, `temps_cycle.txt`, `precedences.txt` et `exclusions.txt` pour tester rapidement. Le projet inclut des fichiers texte d'exemple.

Auteurs
-------

Equipe : Arthur VAILLANT, Maxence SIMONET et Loan PERRARD

Licence
-------

Vérifier avec les auteurs / enseignants. Aucun fichier de licence explicite n'est inclus dans ce dépôt.

Contact / amélioration
----------------------

Si vous souhaitez ajouter des fonctionnalités (ex. export des solutions, interface, tests unitaires, gestion d'options en ligne de commande), ouvrez une issue ou contactez les auteurs.

---

Fichier généré automatiquement — mis à jour pour décrire le projet C et les instructions de build/usage.

