/*
    Code de Salaï loïc

    Pour la compilation
    g++ Contribution_Perso_SALAI_Loïc.cpp Labyrinthe.o -g -Wall -o ContribPerso.cpp

    Pour l'éxécution :
    ./ContribPerso
*/

#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <time.h> 
#include "Labyrinthe.h"

using namespace std;

/* 
    La fonction synchro attend que le paramètres duree se soit écoulée depuis le dernier appel.
    Fourni par O. Bailleux
*/

void synchro(double duree)
{
    static clock_t jalon = clock();
    jalon = jalon + (clock_t)(duree * CLOCKS_PER_SEC);;
    while(clock()<jalon);
}


class Laby : public Labyrinthe
{
private:
    // Positions des robots
    int idRobotA;
    int idRobotB;

public:
    // Produit un labyrinthe avec une densité de cases blanches égale à density.
    // Les positions initiale des deux robots sont situées dans les coins supérieur gauche
    // et inférieur droit. Les codes des robots ne sont pas placés dans le grilles, dont
    // toutes les cases vides sont initialisées à 0.
    Laby(int nLignes, int nColonnes, double density);

    // Construit un labyrinthe d'après un descripteur.
    Laby(char* descr);

    int getIdRobotA() {return idRobotA;}
    int getIdRobotB() {return idRobotB;}    
    void setIdRobotA(int id) {idRobotA = id;}
    void setIdRobotB(int id) {idRobotB = id;}

    // Produit un identifiant aléatoire de case vide (PAS un mur)/
    int caseRandom();

    // Réalise un mouvemement du robot prédateur. La grille ne doit contenir que des cases 
    // vides et des murs.
    // L'attribut idRobotA est modifié. La grille n'est pas modifiée.
    // Retourne false si les robots sont en contact (dans ce cas A n'est pas déplacé),
    // et true si les robots n'étaient pas en contact avant le déplacement de A.
    // Au moins un algorithme de déplacement doit être implémenté. Les algorithmes
    // implémentés doivent être numéroté à partir de 1. Par exemple, si deux algorithmes sont
    // implémentés, ils sont numérotés 1 et 2. Le paramètre algo désigne l'algorithme à
    // utiliser. Si un seul algorithme est utilisé, sa valuer doit être 1.
    bool deplaceRobotA(int algo);
    
    // Réalise un mouvemement du robot proie. Mêmes convension que deplaceRobotB.
    bool deplaceRobotB(int algo);

    // Place les robots (identifiés par les valeurs 2 et 3) dans la grille. Efface l'écran.
    // Affiche la grille. Enlève les robots de lagrille (pour ne pas perturber distMin).
    void visualisation();

    // Lance une poursuite dans le labyrinthe courant. Si silent == true, aucun affichage n'est
    // réalisé, sinon il y a affichage de chaque étape.
    // Une étape = un mouvement de chaque robot. Si le nombre d'étape atteint timeout, la
    // valeur timeout+1 est retournée, sinon le nombre d'étape avant contact des robots est 
    // retourné. Si random == true, les position initiales des robots sont aléatoires, sinon ce 
    // sont les coins supérieur gauche et inféieur droit.
    // Les paramètres algoA et algoB indiquent le algorithmes utilisé pour les robots.
    int course(int timeout, bool silent, bool random, int algoA, int algoB);

    // Réalise nCourses courses et retourne la médiane des valeurs retournées par les
    // appels à la méthode courses. Le apramètres algoA et algoB donnent les algorithmes
    // à utiliser pour les robots A et B.
    int evalue(int nCourses, int timeOut, int algoA, int algoB);
};

/* 
    Constructeur de la classe Laby. A complété si besoin.
    Fourni par O. Bailleux 
*/

Laby::Laby(int nLignes, int nColonnes, double density) : Labyrinthe(nLignes, nColonnes)
{
    setAff(" XAB.");
    idRobotA = getID(0,0);
    idRobotB = getID(getNbLignes()-1, getNbColonnes()-1);
    genLaby((int)(density * getNbColonnes() * getNbLignes()));
    // Ajouter les initialisations supplémentaire nécessaires, si applicable
}

/* 
    Constructeur de la classe Laby.
    Fourni gracieusement par O. Bailleux. Peut être complété si nécessaire.
*/

Laby::Laby(char* descr) : Labyrinthe(descr)
{
    setAff(" XAB.");
    idRobotA = getID(0,0);
    idRobotB = getID(getNbLignes()-1, getNbColonnes()-1);
    // Ajouter les initialisations supplémentaire nécessaires, si applicable
}

// Méthode fournie par O. Bailleux

void Laby::visualisation()
{
    // Les robots sont placés dans la grille avant l'affichage
    modifie(getIdRobotA(),2);
    modifie(getIdRobotB(),3);

    system("clear");
    affiche();

    // Les robots sont retirés de la grille pour ne pas perturber la recherche de chemins
    // optimaux par la méthode distMin.
    modifie(getIdRobotA(),0);
    modifie(getIdRobotB(),0);
}

// Méthode fournie par O. Bailleux

int Laby::course(int timeout, bool silent, bool random, int algoA, int algoB)
{
    if(random)
    {// Course avec positions initiales des robots aléatoire
        idRobotB = caseRandom();
        idRobotA = caseRandom();
    }
    else
    {// Course avec départ dans les coins opposés.
        idRobotA = getID(0, 0);
        idRobotB = getID(getNbLignes()-1, getNbColonnes()-1);
    }
    
    int nEtapes = 0;
    bool contact = false;

    // Arret quand les robots se touchent ou de nombre maxi d'étapes atteint
    while(!contact && nEtapes<timeout)
    {
        // On déplace le robot A
        contact =! deplaceRobotA(algoA);

        // On déplace le robot B sauf s'il est en contact avec A
        if (!contact) contact =! deplaceRobotB(algoB);

        nEtapes++;
        if(!silent) 
        {// Délai et affichage seulement si on est pas en mode silencieux
            synchro(0.1);
            visualisation();
        }
    }
    if(contact) return nEtapes;
    else return timeout+1;
}

// Méthode fournie par O. Bailleux
int Laby::evalue(int nCourses, int timeout, int algoA, int algoB)
{
    std::vector<int> tab;
    // int* tab = new int[nCourses]; // Tableau des scores

    for(int i=0; i<nCourses; i++)
    {// On lance une course et récupère le score
        cout << i << " / " << nCourses;
        int score = course(timeout, true, true, algoA, algoB);
        tab.push_back(score);
        cout << " --> " << score << endl;
    }

    // Tri du tableau de score pour calcul de la médiane.
    sort(tab.begin(), tab.end());
    return tab[nCourses/2];
}


/*   
   La fonction caseRandom renvoie l'identifiant d'une des cases du plateau.
   Cette identifiant ne peut pas être celle d'un mur.
*/

int Laby::caseRandom()
{
    int idAlea;
    do
    {
        idAlea = rand() % (getNbLignes()*getNbColonnes());
    }while(lit(idAlea) != 0); 
    return idAlea;      
}

/* 
    La fonction deplacementRobotA permet de déplacer le robot prédateur lors d'une course poursuite.
    Elle renvoie un booléen pour savoir si les robots sont en contact.
    Paramètre : le chiffre de l'algo que l'on souhaite utiliser pour chasser le robot proie
*/

bool Laby::deplaceRobotA(int algo)
{   
    bool verif = true;              // Pour savoir s'il y a eu contact entre les deux robots
    bool verifDep = false;          // Vérifie si le déplacement de l'utilisateur dans l'algo 3 est juste
    int idAlea;                     // Variable aléatoire pour l'algorithme direct prédateur
    int idA = getIdRobotA();        // Indice du robotA
    int idB = getIdRobotB();        // Indice du robotB
    int val;                        // Permet de réalisé des comparaison de distance minimal dans l'algo 2
    string touche;                  // Permet de contenir la saisie de l'utilisateur 
     
    if(distMin(idA, idB) == 1)
    {
        verif = false;
    }
    else if(algo == 1)              // Algorithme direct prédateur 
    {    
        do
        {   
            setIdRobotA(idA);
            modifie(idA, 0);  
            idAlea= rand() % 4;
            if(idAlea == 0 && getLigne(idA)==getLigne(idA-1) && lit(idA-1) == 0 )
            {
                setIdRobotA(idA-1);
            }
            else if(idAlea == 1 && getLigne(idA)==getLigne(idA+1) && lit(idA+1) == 0 )
            {
                setIdRobotA(idA+1);
            }
            else if(idAlea == 2 && getLigne(idA)!=getNbLignes()-1 && lit(idA+getNbColonnes()) == 0)
            {
                setIdRobotA(idA+getNbColonnes());  
            }
            else if (idAlea == 3 && getLigne(idA)!=0 && lit(idA-getNbColonnes()) ==0 )
            {
                setIdRobotA(idA-getNbColonnes());  
            }
        } while (distMin(getIdRobotA(), idB)>distMin(idA, idB));

        for (int i = 0; i < getNbLignes()*getNbColonnes(); i++)
        {
            if(lit(i) == 2) 
           {
               modifie(i, 0);
           } 
        }
        modifie(getIdRobotA(), 2);
    }
    else if (algo == 2)             // Deuxième algorithme de prédateur de score médian 101
    {   
        modifie(idA, 0);
        val = distMin(idA, idB);
        if( getLigne(idA)==getLigne(idA-1) && distMin(idA-1, idB)<val && lit(idA-1) == 0)
        {
            setIdRobotA(idA-1);
            val = distMin(idA-1, idB);
        }
        if(getLigne(idA)==getLigne(idA+1) && distMin(idA+1, idB)<val && lit(idA+1) == 0)
        {
            setIdRobotA(idA+1);
            val = distMin(idA+1, idB);
        }
        if(getLigne(idA)!=getNbLignes()-1 && distMin(idA+getNbColonnes(), idB)<val && lit(idA+getNbColonnes()) == 0)
        {
            setIdRobotA(idA+getNbColonnes());
            val = distMin(idA+getNbColonnes(), idB);  
        }
        if (getLigne(idA)!=0 && distMin(idA-getNbColonnes(), idB)<val && lit(idA-getNbColonnes()) == 0)
        {
            setIdRobotA(idA-getNbColonnes()); 
        }
        modifie(getIdRobotA(), 2);
    }
    else if(algo == 3)      // DÃ©placement du robot prÃ©dateur par l'utilisateur 
    {   
        modifie(idA, 0);   
        do
        {   
            setIdRobotA(idA);
            cout << "Saisir un déplacement parmis : z,q,s,d " << endl;
            cin >> touche;
            cout << touche << endl;
            if(touche[0] == 'q' && getLigne(idA)==getLigne(idA-1) && lit(idA-1)==0)
            {
                setIdRobotA(idA-1);
                verifDep = true;
            }
            else if (touche[0] == 'd' && getLigne(idA)==getLigne(idA+1) && lit(idA+1)==0) 
            {
                setIdRobotA(idA+1);
                verifDep = true;
            }
            else if (touche[0]  == 's' && getLigne(idA)!=getNbLignes()-1 && lit(idA+getNbColonnes())==0) 
            {
                setIdRobotA(idA+getNbColonnes());
                verifDep = true;
            }
            else if (touche[0] == 'z'  && getLigne(idA)!=0 && lit(idA-getNbColonnes())==0)
            { 
                setIdRobotA(idA-getNbColonnes());
                verifDep = true;
            }
            else
            {
                cout << " ERREUR " << endl;
            }    
        }while( verifDep != true);
        modifie(getIdRobotA(), 2);      
    } 
    return verif;
}


/* 
    La fonction deplacementRobotB permet de déplacer le robot proie lors d'une course poursuite.
    Elle renvoie un booléen pour savoir si les robots sont en contact.
    Paramètre : le chiffre de l'algo que l'on souhaite utiliser pour fuir le robot prédateur.
*/

bool Laby::deplaceRobotB(int algo)
{
    bool verif = true;          // Pour savoir s'il y a eu contact entre les deux robots
    bool verifDep = false;      // Vérifie si le déplacement de l'utilisateur dans l'algo 2 est juste
    int idA = getIdRobotA();    // Indice du robotA
    int idB = getIdRobotB();    // Indice du robotB
    int idAlea;                 // Variable aléatoire pour l'algorithme random proie
    string touche;              // Permet de contenir la saisie de l'utilisateur

    if(distMin(idA, idB) == 1)
    {
        verif = false;
    }
    else if(algo == 1)          // Algorithme random proie
    {   
        modifie(idB, 0);   
        do
        {   
            setIdRobotB(idB);
            idAlea = rand() % 4;
            if(idAlea == 0 && getLigne(idB)==getLigne(idB-1))
            {
                setIdRobotB(idB-1);
            }
            else if (idAlea == 1 && getLigne(idB)==getLigne(idB+1)) 
            {
                setIdRobotB(idB+1);
            }
            else if (idAlea == 2 && getLigne(idB)!=getNbLignes()-1) 
            {
                setIdRobotB(idB+getNbColonnes());
            }
            else if (idAlea == 3 && getLigne(idB)!=0) 
            { 
                setIdRobotB(idB-getNbColonnes());
            }        
        }while(lit(getIdRobotB()) == 1);
        modifie(getIdRobotB(), 3);      
    }
    else if(algo == 2)      // Déplacement du robot B par l'utilisateur 
    {   
        modifie(idB, 0);   
        do
        {   
            setIdRobotB(idB);
            cout << "Saisir un délacement parmis : z,q,s,d s" << endl;
            cin >> touche;
            if(touche[0] == 'q' && getLigne(idB)==getLigne(idB-1) && lit(idB-1)==0)
            {
                setIdRobotB(idB-1);
                verifDep = true;
            }
            else if (touche[0] == 'd' && getLigne(idB)==getLigne(idB+1) && lit(idB+1)==0) 
            {
                setIdRobotB(idB+1);
                verifDep = true;
            }
            else if (touche[0] == 's' && getLigne(idB)!=getNbLignes()-1 && lit(idB+getNbColonnes())==0) 
            {
                setIdRobotB(idB+getNbColonnes());
                verifDep = true;
            }
            else if (touche[0] == 'z'  && getLigne(idB)!=0 && lit(idB-getNbColonnes())==0) 
            { 
                setIdRobotB(idB-getNbColonnes());
                verifDep = true;
            }
            else
            {
                cout << " ERREUR " << endl;
            }        
        }while(verifDep!= true);
        modifie(getIdRobotB(), 2);
    }
    return verif;
}

/* 
    Les descripteurs de la classe pour réaliser des test 
*/

// Gille de 20 lignes et 30 colonnes densité de cases vides 0.8
char descripteur1[] = 
{   
    20, 30,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 
    0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 
    0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 
    0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 
    0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 
    0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
    1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 
    1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
};

// Gille de 20 lignes et 30 colonnes sans murs
char descripteur2[] = 
{   
    20, 30,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
};

// Gille de 20 lignes et 30 colonnes densité de cases vides 0.9
char descripteur3[] = 
{   
    20, 30,
    0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 
    0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 
    0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 
    1, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 
    0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
    0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
};

// Gille de 20 lignes et 30 colonnes densité de cases vides 0.7
char descripteur4[] =
{   
    20, 30,
    0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 0, 
    0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 
    0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 
    0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 
    1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 
    1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 
    0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 
    1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 
    1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 
    1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 
    1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 
    0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 
    0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 0, 
    0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 
    0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 
    0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 
    1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 
    0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 
};

/* 
    La fonction testPoursuite simule une course poursuite entre les deux robots A et B, avec une durée maximale
*/

void testPoursuite()
{
    int dureeMax = 300; 
    Laby laby(30, 60, 0.9);

    int duree = laby.course(dureeMax, false, true, 1, 1);

    if(duree > dureeMax)
    {
        cout << "Echec apres " << dureeMax << " etapes" << endl;
    }
    else
    {
        cout << "Succes apres " << duree << " etapes" << endl;
    }
}

/* 
    La fonction testEval permet d'évaluer la performance des algorithmes du robot prédateur A, par rapport
    à l'algortihme random proie. A la fin, elle affiche le score médian de l'algortihme testé.
*/

void testEval()
{
    Laby laby(descripteur2);

    int score = laby.evalue(100, 100, 1, 2); 
 
    cout << "Mediane : " << score << endl;
}

/* 
    Contribution personnelle : Permet au joueur de jouer le robot prédateur ou bien le robot proie.
*/

void jeuUtilisateur()
{   
    int choix;              // Permet de récupérer le choix de l'utilisateur 
    int algoA;              // Contient l'algorithme approprié pour le robotA en fonction du choix de l'utilisateur 
    int algoB;              // Contient l'algorithme approprié pour le robotB en fonction du choix de l'utilisateur 
    int dureeMax = 300;     // Duree maximal de la course

    cout << " Voulez vous jouer le prédateur (1) ou la proie (2)? " << endl;
    cout << "Saisissez votre choix (chiffre) " << endl;

    do
    {   choix = 0;
        cin  >> choix;
        if(!(choix == 1 || choix == 2))
        {
            cout << "\n  Erreur de saisie " << endl;
        } 
    } while (!(choix == 1 || choix == 2));

    if( choix == 1 )
    {   
        algoA = 3;
        algoB = 1;
        cout << "\n Vous jouez en tant que prÃ©dateur ! Chassez ! ";
    }
    else 
    {
        algoB = 2;
        algoA = 2;
        cout << "\n Vous jouez en tant que proie ! Fuyez ! ";
    }
    Laby laby(20, 40, 0.9);

    int duree = laby.course(dureeMax, false, true, algoA, algoB);

    if(duree > dureeMax)
    {
        cout << "Echec apres " << dureeMax << " etapes" << endl;
    }
    else
    {
        cout << "Succes apres " << duree << " etapes" << endl;
    } 
}

// Main
int main(int argc, const char * argv[])
{   
    
    srand((unsigned)time(NULL));
    // testPoursuite();
    //testEval();
    jeuUtilisateur();

    return 0;
}