#ifndef LISTE_H_INCLUDED
#define LISTE_H_INCLUDED
#include"ELTARTICLE.H"

typedef struct Cellule {
    ELEMENT donnee;
    struct Cellule *suivant;
} Cellule,*LISTE;

LISTE listeCreer(void);
int estVide(LISTE L);
int listeTaille(LISTE L);
ELEMENT recuperer(LISTE L, int pos);
int inserer(LISTE *L, ELEMENT e, int pos);
int supprimer(LISTE *L, int pos);
void listeDetruire(LISTE L);

#define ELEMENT_VIDE NULL
#define LISTE_VIDE NULL
#endif // LISTE_H_INCLUDED
