#include <stdio.h>
#include <stdlib.h>


#define L 4//Longueur de la grille (nombre de colonnes)
#define M 3//Largeur de la grille (nombre dee lignes)

char Grille[L*M] = {'0'};

//retourne l'identifiant d'une case dont on donne les coordonnées
int getID(int ligne, int colonne)
{   
    return ligne*colonne;
}

//retourne la première coordonée (ligne) d'une case dont on donne l'identifiant
int getLigne(int id)
{   
    int col = 0; //pour récupérer la coordonnée correspondant à la colonne
    int lgn = 0; //pour récupérer la coordonnée correspondant à la ligne
    for (int i = 0; i < id; i++)
    {
        if (col != M) col = col + 1;
        else 
            {
                col = 0;
                lgn = lgn+1;
            } 
    } 
    return lgn ;
}

//retourne la première coordonée (ligne) d'une case dont on donne l'identifiant
int getColonne(int id)
{   
    int col = 0; //pour récupérer la coordonnée correspondant à la colonne
    for (int i = 0; i < id; i++)
    {
        if (col != M) col = col + 1;
        else col = 0 ;
    } 
    return col ;
}

//place la valeur x dans le case de coordonnée (i,j)
void modifie(int ligne, int colonne, char x)
{
    int val = getID(ligne, colonne);
    Grille[val] = x ;
}

//retourne la valeur de la case de coordonnées (i,j)
char lit(int ligne, int colonne)
{
    return Grille[ligne*colonne];
}


// Pour l'affichage du labyrtinthe en ligne de commande 
#define AFF_VIDE '-' //Caractère représentant les cases vides pour l'affichage
#define AFF_MUR 'X' //Caractère représentant les murs pour l'affichage
#define AFF_BORD ' ' //Caractère représentant les bords pour l'affichage 

//affiche la grille
void affiche()
{
    for (int i=0; i<M; i++)
    {
        for (int j = 0; j < L; j++)
        {   
            int val = i*j;
            if (lit(i,j) == (char)0) 
               { 
                   printf("%c%c ",AFF_MUR, AFF_BORD);
               }
            else if (lit(i,j) == (char)1) 
               { 
                   printf("%c%c ",AFF_VIDE, AFF_BORD); 
                }
        }
        printf("\n");
    }
}

// Test fonction getID
void test_getID(int ligne, int colonne)
{   
    printf("%d\n", colonne*ligne);
}

// Test fonction getLigne
void test_getLigne(int id)
{   
    printf("%d\n", getLigne(id));
}

// Test fonction getColonne
void test_getColonne(int id)
{   
    printf("%d\n", getColonne(id));
}

// test fonction modifie
int test_modifie(int ligne, int colonne, char x)
{   
    modifie(ligne, colonne, x);
    printf("%c\n", lit(ligne, colonne));
}

int main()
{   
    modifie(0,1,(char)1);
    affiche();
    return 0;
}