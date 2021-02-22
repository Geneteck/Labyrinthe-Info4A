#include <stdio.h>
#include <stdlib.h>


int NB_COLONNES = 4;//Longueur
int NB_LIGNES = 3; //largeur

char* Grille=NULL;


//retourne l'identifiant d'une case dont on donne les coordonnées
int getID(int ligne, int colonne)
{  
   return 4*ligne+colonne;
}

//retourne la première coordonée (ligne) d'une case dont on donne l'identifiant
int getLigne(int id)
{   
    int col = 0; //pour récupérer la coordonnée correspondant à la colonne
    int lgn = 0; //pour récupérer la coordonnée correspondant à la ligne
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

//retourne la première coordonée (ligne) d'une case dont on donne l'identifiant
int getColonne(int id)
{   
    int col = 0; //pour récupérer la coordonnée correspondant à la colonne
    for (int i = 0; i < id; i++)
    {
        if (col != NB_COLONNES) col = col + 1;
        else col = 0 ;
    } 
    return col ;
}

//place la valeur x dans le case de coordonnée (i,j)
void modifie(int ligne, int colonne, char x)
{
    Grille[getID(ligne, colonne)] = x ;
}

//retourne la valeur de la case de coordonnées (i,j)
char lit(int ligne, int colonne)
{
    return Grille[getID(ligne,colonne)];
}

// Pour l'affichage du labyrtinthe en ligne de commande 
#define AFF_VIDE '-' //Caractère représentant les cases vides pour l'affichage
#define AFF_MUR 'X' //Caractère représentant les murs pour l'affichage
#define AFF_BORD '+' //Caractère représentant les bords pour l'affichage 

//affiche la grille
void affiche()
{   for (int i = -1; i <= NB_LIGNES; i++)
    {
        for (int j = -1; j <= NB_COLONNES; j++)
        {      
            if (i == -1 || j == -1 || i == NB_LIGNES || j ==  NB_COLONNES)
            {
                printf("%c ", AFF_BORD);
            }
            else if (lit(i,j) == 0)
                {
                    printf("%c", AFF_VIDE);
                }
            else if (lit(i,j) == 1)
            {
                printf("%c", AFF_MUR);
            }
        }
        printf("\n");
    }

}

int main()
{   
    Grille = (char*)calloc(NB_LIGNES*NB_COLONNES,sizeof(char));
    modifie(1,1,1);
    modifie(2,3,1);
    affiche();
    free(Grille);
    return 0;
}