/*
Code de Salaï loïc

Pour la compilation
  gcc SALAI_Loïc_Partie2.c -g -Wall -o SALAI_Loïc_Partie2

Pour l'éxécution :
  ./SALAI_Loïc_Partie2

*/

#include <stdio.h>
#include <stdlib.h>

/*
    On définit les caractères qui vont permettre de visualiser le labyrinthe :
    '-' pour représenter les cases vides du labyrinthe 
    'X' pour la représentation des murs du labyrinthe
    '+' pour qu'on affiche les bords du labyrinthe 
*/
char AFF_VIDE = '-'; 
char AFF_MUR = 'X'; 
char AFF_BORD ='+';  

int NB_COLONNES = 2; // Correspond aux nombres de colonnes du labyrinthe
int NB_LIGNES = 3; // Correspond aux nombres de lignes du labyrinthe
char* Grille=NULL;

/* 
    La fonction getID retourne un entier qui est l'indice de la case dans à la ligne et la colonne indiqué en paramètres
    Paramètres : le numéro de ligne et le numéro de colonne
*/

int getID(int ligne, int colonne)
{  
   return NB_COLONNES*ligne+colonne;
}

/* 
    La fonction getLigne retourne un entier qui est la ligne de la case à l'indice id
    Paramètre : l'indice de la case
*/

int getLigne(int id)
{   
    int col = 0; // Variable qui contiendra la coordonnée correspondant à la colonne
    int lgn = 0; // Variable qui contiendra la coordonnée correspondant à la ligne et qui est retournée en fin de fonction
    for (int i = 0; i < id; i++)
    {
        if (col != NB_LIGNES) col = col + 1;
        else 
            {
                col = 0;
                lgn = lgn+1;
            } 
    } 
    return lgn ;
}

/* 
    La fonction getColonne retourne un entier qui est la colonne de la case à l'indice id
    Paramètre : l'indice de la case
*/

int getColonne(int id)
{   
    int col = 0; // Variable qui contiendra la coordonnée correspondant à la colonne et qui sera retourné 
    for (int i = 0; i < id; i++)
    {
        if (col != NB_COLONNES) col = col + 1;
        else col = 0 ;
    } 
    return col ;
}

/*
    La fonction modifie permet de donner une valeur à la case, dont la ligne et la colonne sont données en paramètre, sous la forme d'un caractère 
    Les caractères possibles sont : 
    - 0 lorsque c'est vide
    - 1 lorsqu'il y a un mur
    (- 2 pour marqué une case de la grille ) Voir fonction connexe
    Paramètres : la ligne de la case, la colonne de la case, et le caractère x que l'on veut mettre dans la case
*/

void modifie(int ligne, int colonne, char x)
{
    Grille[getID(ligne, colonne)] = x ;
}

/* 
    La fonction lit permet de retourner le caractère (char) contenu d'une case
    Paramètres : la ligne et la colonne de la case dont on souhaite retourner le caractère (char)
*/

char lit(int ligne, int colonne)
{
    return Grille[getID(ligne,colonne)];
}

/* 
    La fonction affiche nous permet de visualiser la grille 
*/

void affiche()
{   for (int i = -1; i <= NB_LIGNES; i++)
    {
        for (int j = -1; j <= NB_COLONNES; j++)
        {    
            if (i == -1 || j == -1 || i == NB_LIGNES || j ==  NB_COLONNES)
            {
                printf("%c", AFF_BORD);
            }
            else
            {
                printf("%c", lit(i,j));
            }
        }
        printf("\n");
    }

}



int* Pile = NULL;
int Sommet = 0;

// Partie 2 du Projet

//empile un entier
void push(int x)    
{   
    Pile[Sommet] = x;
    Sommet++; 
}

//depile un entier et le retourne
int pop()
{   
    Sommet--; 
    int val = Pile[Sommet];
    return val;
}

// dépile un entier et le retournes
int connexe()
{   
    int CaseB, CaseMarq, verif, id = 0;

    // pour compter le nombre de case blanches
    for (int i = 0; i < NB_COLONNES*NB_LIGNES; i++)
    {
        if (Grille[i] == '0')
        {
            CaseB++;
            id = i;
        }
    }

    if (CaseB !=  0)
    {
        Grille[id] = '2'; // on marque la case blanche
        push(id); // on empile la valeur 
    }
    else
    {
        printf("Erreur (aucune case blanche");
        verif = 1;
    }

    // pour remplacer toutes les cases avec la valeur 2  par 0
    for (int i = 0; i < NB_COLONNES*NB_LIGNES; i++)
    {
        if (Grille[i] == '2')
        {
            Grille[i] = '0';
            CaseMarq++;
        }
    }

    //vérification que le nombre de cases blanches est égale aux nombres de cases Marquée
    if (CaseB == CaseMarq)
    {
        verif = 1;
    }
    
    // si la valeur retournée est 1 alors les cases blanches sont toutes connectées, sinon 0
    return verif; 
}

int main()
{   
    Grille = (char*)calloc(NB_LIGNES*NB_COLONNES,sizeof(char));
    Pile = (int*)calloc(NB_LIGNES*NB_COLONNES,sizeof(int));

    // Initialisation de la Grille de cases vides
    for (int i = 0; i < NB_LIGNES; i++)
    {
        for (int j = 0; j < NB_COLONNES; j++)
        {
            modifie(i,j, AFF_VIDE);
        }
    }

    // Reste du code pour tester les fonctions
    modifie(1,1,AFF_MUR);
    affiche();

    free(Grille);
    free(Pile);
    return 0;
}