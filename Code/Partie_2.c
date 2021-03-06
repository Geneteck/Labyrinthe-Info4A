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

int NB_COLONNES = 10; // Correspond aux nombres de colonnes du labyrinthe
int NB_LIGNES = 3; // Correspond aux nombres de lignes du labyrinthe
char* Grille = NULL;

int* Pile = NULL;
int Sommet = 0;

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

/* 
    La fonction push permet de concaténer/empiler l'indice de la case sélectionner dans Pile
    Paramètre : l'indice de la case que l'on concatène dans Pile
*/

void push(int x)    
{   
    Sommet++; 
    Pile[Sommet] = x;
}

/*
    La fonction pop retourne le dernier entier qui a été concaténer dans la pile (Pile)
*/

int pop()
{   
    Sommet--; 
    int val = Pile[Sommet];
    return val;
}

/* 
    La fonction connexe permet de déterminer si toutes les cases blanches sont connexes (1) ou pas (0)
    - Elle en renvoie un entier : 1 si les cases blanches sont connexes et 0 dans le cas contraire
    
*/

int connexe()
{   
    int verif = 1;
    int CaseB, CaseMarq, id = 0;

    for (int i = 0; i < NB_COLONNES*NB_LIGNES; i++) // Boucle qui compte le nombre de case blanches
    {
        if (Grille[i] == "0")
        {   
            CaseB++;
            id = i;
        }
    }
    printf("Il y a %d cases blanches",CaseB);
    if (CaseB !=  0) // Vérifie si le nombre de cases blanches est différent de 0
    {   
          while(Sommet!=0) // Tant que la pile n'est pas vide 
          {
              Grille[id] = "2"; // La case est "marquée" dans la grille
              // Vérification de la case à droite de l'indice courant
              if(Grille[id+1]=="0" && getLigne(id)==getLigne(id+1) && id+1<NB_COLONNES*NB_LIGNES) 
              {   
                  Grille[id+1] = "2";
                  push(id+1); 
              }
              // Vérification de la case à gauche de l'indice courant
              if(Grille[id-1]=="0" && getLigne(id)==getLigne(id-1) && id-1>=0)
              {   
                  Grille[id-1] = "2";
                  push(id-1);
              }
              // Vérification de la case en haut de la case à l'indice courant
              if(Grille[id-NB_COLONNES]=="0" && getColonne(id)==getColonne(id-NB_COLONNES) && id-NB_COLONNES>=0)
              {   
                  Grille[id-NB_COLONNES] = "2";
                  push(id-NB_COLONNES);
              }
              // Vérification de la case en haut de la case à l'indice courant
              if(Grille[id+NB_COLONNES]=="0" && getColonne(id)==getColonne(id+NB_COLONNES) && id+NB_COLONNES<NB_COLONNES*NB_LIGNES)
              {  
                 Grille[id+NB_COLONNES] = "2";
                 push(id+NB_COLONNES); 
              }
              id = pop();
          }
    }
    else
    {
        printf("Erreur aucune case blanche"); // Message d'erreur si il n'y a aucune case blanche
        verif = 0;
    }

    for (int i = 0; i < NB_COLONNES*NB_LIGNES; i++) // Boucle pour placé remettre les cases marquées(2) en cases blanches (0)
    {
        if ((*Grille+i) == 2)
        {
            Grille[i] = "0";
            CaseMarq++;
        }
    }
    printf("\nIl y a %d cases marquées", CaseMarq);
    if (CaseB != CaseMarq) // Vérification que le nombre de cases blanches est égale aux nombres de cases Marquée
    {
        verif = 0;
    }
    
    return verif; // Si verif = 1 : les cases sont blanches sont connexes. Dans le cas contraire, verif = 0
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
    printf("\n%x",connexe());

    free(Grille);
    free(Pile);
    return 0;
}