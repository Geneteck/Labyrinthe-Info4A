/*
    Code de Salaï loïc

    Pour la compilation
    g++ SALAI_Loïc_Partie4a.cpp -g -Wall -o SALAI_Loïc_Partie4a

    Pour l'éxécution :
    ./SALAI_Loïc_Partie4a
*/

#include "Labyrinthe.h"
#include <time.h> 

using namespace std;

// Constructeurs et destructeurs


/* 
    Crée un labyrinthe vide.
    Paramètres : le nombre de lignes et de colonnes du labyrinthe
*/

Labyrinthe::Labyrinthe(int nLignes, int nColonnes)
{   
    setAff(" XA~");                         
    this->nb_lignes = nLignes;
    this->nb_colonnes = nColonnes;
    this->taille = nLignes * nColonnes;
    this->grille = (char *)calloc(this->taille, sizeof(char));
    this->pile =(int*)calloc(this->taille, sizeof(int));
}


/* 
    Crée un labyrinthe qui est complété par un descipteur fournis en paramètre.
    Paramètres : un descripteur (comme ceux fournis dans la partie 3), 
*/

Labyrinthe::Labyrinthe(char data[])
{   
    setAff(".XA~");                          
    this->nb_lignes = data[0];
    this->nb_colonnes = data[1];
    this->taille = getNbLignes()*getNbColonnes();
    this->grille = (char  *)calloc(this->taille,sizeof(char));
    this->pile =(int*)calloc(this->taille, sizeof(int));

    for(int i = 2; i<getTaille();i++)
    {   
        this->grille[i-2]=data[i];
    }
}

/* 
    Destructeur de l'instance de labyrinthe courant
*/

Labyrinthe::~Labyrinthe()
{
    delete this->grille;
    delete this->pile;
}

// Méthodes d'accès

/* 
    La fonction getAff retourne un caractère en fonction de son paramètre val.
    Paramètre : un caractère val de type char
 */

char Labyrinthe::getAff(char val)
{
    if(val == '0')          // Vides
    {
        return (this->aff[0]);
    }
    else if( val == '1')    // Murs
    {
        return (this->aff[1]);  
    }
    else if( val == '2')    // Chemin optimal 
    {
        return (this->aff[2]);
    }
    else 
    {
        return (this->aff[3]);
    } 
}

/* 
    La fonction getID retourne l'indice d'une case idéntifiée par les paramètres.
    Paramètres : la ligne et la colonne d'une case.
 */

int Labyrinthe::getID(int ligne, int colonne)
{
    if (ligne == 0)
    {
        return nb_colonnes * ligne + colonne;
    }
    return (getNbColonnes() * ligne) + colonne;
}

/* 
    La fonction getLigne retourne la ligne d'indice id
    Paramètre : l'indice d'une case 
 */

int Labyrinthe::getLigne(int id)
{
    return id / getNbColonnes();
}

/* 
    La fonction getCol retourne la colonne d'indice id
    Paramètre : l'indice d'une case 
 */

int Labyrinthe::getCol(int id)
{   
    int col = 0;     // Variable qui contiendra la coordonnée correspondant à la colonne et qui sera retourné
    for (int i = 0; i < id; i++)
    {
        if (col != nb_colonnes-1)
            col = col+1;
        else
            col = 0;
    }
    return col;
}

/* 
    Permet de modifier le caractère d'une case de la grille idéntifié par sa ligne et sa colonne
    Paramètres : la ligne et la colonne de la case, ainsi que le caractère que l'on souhaite placé dans la case 
 */

void Labyrinthe::modifie(int ligne, int colonne, char x)
{
    (this->grille[getID(ligne, colonne)]) = x;
}

/* 
    Retourne le caractère placé dans la case de la grille identifée par sa ligne et sa colonne
    Paramètre : la ligne et la colonne de la case
 */

char Labyrinthe::lit(int ligne, int colonne)
{
    return (this->grille[getID(ligne, colonne)]);
}


// Méthode d'accès supplémentaires

/* 
    Retourne le nombre de colonnes du labyrinthe
*/

int Labyrinthe::getNbColonnes()
{
    return this->nb_colonnes;
}

/* 
    Retourne le nombre de lignes du labyrinthe
*/

int Labyrinthe::getNbLignes()
{
    return this->nb_lignes;
}

/* 
    Retourne le nombre total de cases dans la grille 
*/

int Labyrinthe::getTaille()
{
    return this->taille;
}

/* 
    Effectue la même action que la précédente fonction lit, mais en utilisant l'indice de la case
    Paramètre : l'indice de la case
*/

int Labyrinthe::lit(int id){
    return (this->grille[id]);
}

/* 
    Effectue la même action que la précédente fonction modifie, mais en utilisant l'indice de la case
    Paramètre : l'indice de la case, et le caractère que l'on souhaite placer
*/

void Labyrinthe::modifie(int id, char x)
{
    this->grille[id] = x;
}

/* 
    Retourne la valeur du sommet S dans la pile (ajouté dans le fichier Labyrinthe.h)
*/

int Labyrinthe::getS(){
    return this-> s;
}

/* 
    Permet de placer l'indice id dans la pile, au sommet S.
    Paramètre : l'indice de la case que l'on ajoute à la piles 
*/

void Labyrinthe::push(int id)
{
    (this->pile[getS()]) = id;
    this->s = getS() + 1;
}

/* 
    Retourne le dernier indice id placé dans la pile, au sommet S.
*/

int Labyrinthe::pop()
{
    this->s = getS() - 1;
    int val = (this->pile[getS()]);
    return val;
}

/* 
    Permet de définir la chaine de motifs utiliser par la fonction affichage 
*/

void Labyrinthe::setAff(const char* motifs)
{
    this->aff = motifs;
}

/* 
    Permet d'afficher l'instance d'un labyrinthe en console
*/

void Labyrinthe::affiche()
{
    for(int i=-1 ; i<= getNbLignes(); i++)
    {
        for(int j=-1 ; j<= getNbColonnes(); j++)
        {
            if(i == -1 || j == -1 || i == getNbLignes() || j == getNbColonnes())
            {
                cout << getAff('d') ;
            }
            else if(lit(i,j) == 0)
            {
                cout << getAff('0') ;
            }
            else if( lit(i,j) == 1)
            {
                cout << getAff('1') ;
            }
            else if( lit(i,j) == 2)
            {
                cout << getAff('2') ;
            } 
        }
        cout << "\n" ; 
    } 
}

// Méthodes de Haut Niveau

/* 
    Vérifie si un labyrinthe à sa case tout en  haut à gauche et tout en bas à droite lié par des cases vides
*/

bool Labyrinthe::connexe()
{   
    bool verif = true;        // La variable verif renvoie (en fin de méthode) : true si le labyrinthe est connexe, sinon false
    int casesM = 0;           // Permet de compter le nombre de cases qui aura été marqué
    int id = 0;               // Variable qui contient l'id d'une case 
    int caseB = 0;            // Permet de compter le nombre de cases blanches initiales 

    // La boucle compte le nombre de case blanches
    for (int i = 0; i < taille; i++) 
    {   
        if (grille[i] == 0) 
        {   
            caseB++;
            id = i;
        }
    }

    if (caseB !=  0) // Vérifie si le nombre de cases blanches est différent de 0
    { 
        push(id);
        while(getS()!=0) // Tant que la pile n'est pas vide 
        {  
            grille[id] = 2; // La case est "marquée" dans la grille
            // Vérification de la case à droite de l'indice courant
            if(grille[id+1] == 0 && getLigne(id)==getLigne(id+1) && id+1<getTaille()) 
            {   
                grille[id+1] = 2;
                push(id+1); 
            }
            // Vérification de la case à gauche de l'indice courant
            if(grille[id-1]== 0 && getLigne(id)==getLigne(id-1) && id-1>=0)
            {   
                grille[id-1] = 2;
                push(id-1);
            }
            // Vérification de la case en haut de la case à l'indice courant
            if(grille[id-nb_colonnes]==0 && getCol(id)==getCol(id-nb_colonnes) && id-getNbColonnes()>=0)
            {   
                grille[id-nb_colonnes] = 2;
                push(id-nb_colonnes);
            }
            // Vérification de la case en haut de la case à l'indice courant
            if(grille[id+nb_colonnes]==0 && getCol(id)==getCol(id+nb_colonnes) && id+nb_colonnes<getTaille())
            {  
                grille[id+nb_colonnes] = 2;
                push(id+nb_colonnes); 
            }
            id = pop();
        }
    }
    else
    {
        printf("Erreur aucune case blanche"); // Message d'erreur si il n'y a aucune case blanche
        verif = false;
    }
    for (int i = 0; i < taille; i++) // Boucle pour replacer les cases marquées(2) en cases blanches (0)
    {
        if ((grille[i]) == 2)
        {
            grille[i] = 0;
            casesM++;
        }
    }
    if (caseB != casesM) // Vérification que le nombre de cases blanches est égale au nombre de cases marquée
    {
        verif = false;
    }
    
    return verif; // Si verif = 1 : les cases sont blanches sont connexes. Dans le cas contraire, verif = 0
}

/* 
    Permet de générer dans une instance de labyrinthe vide, un labyrinthe aléatoire dont 
    le nombre de cases blanches s'approches au maximum du pramètre nb, mais en restant intéressant.
*/

void Labyrinthe::genLaby(int nb)
{
    srand(time(NULL)); 
    int lig;                   // Variable pour contenir la ligne de l'identifiant généré aléatoirement 
    int col;                   // Variable pour contenir la colonne de l'identifiant généré aléatoirement 
    int idAl;                  // Variable qui contient l'indice d'une case, choisi aléatoirement 
    int caseDep = 0;           // Nombre de cases blanches au départ
    int nbTour;                // Définie le nombre de tour que réalise la boucle pour placer des murs

    // On remplit inititalement tout le tableau par des cases vides 
    for(int i = 0; i<getTaille(); i++)
    {
        this->grille[i] = 0;
        caseDep++;
    } 

    // On vérifie la valeur de nb 
    if (nb <= getNbColonnes()+getNbLignes()-1)
    {
        nbTour = caseDep*10;
    }
    else if(nb>getNbColonnes()+getNbLignes()-1 && nb <= (caseDep/4))
    {
       nbTour = (caseDep*nb)/10;
    }
    else if(nb>(caseDep/4) && nb <= (caseDep/2))
    {
         nbTour = caseDep+nb*2;
    }
    else
    {
        nbTour = caseDep-nb;
    }

    // On place des mur de façons aléatoire tant que les cases blanches reste connexe
    for (int x = 0; x <= nbTour;x++)  
    {   
        idAl = rand() % caseDep;
        lig = getLigne(idAl);
        col = getCol(idAl);
        if((lig == 0 && col == 0) || (lig == getNbLignes()-1 && col == getNbColonnes()-1))
        {
            modifie(idAl, 0);
        }
        else if(lit(lig, col)!= 1)
        {
            modifie(idAl, 1);
            if(!connexe())
            {
                modifie(idAl, 0);
            }
        }
    }
}




