#include <stdio.h>
#include <stdlib.h>


#define L 4//Longueur de la grille (nombre de colonnes)
#define M 3//Largeur de la grille (nombre dee lignes)

char Grille[L*M] = {'0','0','0','0','0','1','1','0','0','0','0', '0'};

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
        if (col != L) col = col + 1;
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
#define AFF_BORD ' ' //Caractère représentant les bords pour l'affichage 

//affiche la grille
void affiche()
{   for (int i = -1; i <= M; i++)
    {
        for (int j = -1; j <= L; j++)
        {      
            if (i == -1 || j == -1 || i == M || j == L)
            {
                printf("%c ", AFF_BORD);
            }
            else if (lit(i,j) == '0')
                {
                    printf("%c", AFF_VIDE);
                }
            else if (lit(i,j) == '1')
            {
                printf("%c", AFF_MUR);
            }
        }
        printf("\n");
    }

}



int Pile[L*M];
int Sommet;

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
    for (int i = 0; i < L*M; i++)
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
    for (int id = 0; id < L*M; id++)
    {
        if (Grille[id] == '2')
        {
            Grille[id] = '0';
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
    return 0;
}