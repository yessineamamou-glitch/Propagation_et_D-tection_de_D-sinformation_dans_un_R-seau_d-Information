#ifndef LISTE_H
#define LISTE_H

#include "ELTARTICLE.h"


typedef struct structNoeud {
    ELEMENT info;
    struct structNoeud * precedent;
    struct structNoeud * suivant;
} structNoeud, * NOEUD;

typedef struct {
    NOEUD tete;
    NOEUD queue;
    int lg;
} laStruct, *LISTE;


LISTE listeCreer(void);
void listeDetruire(LISTE L);
int estVide(LISTE L);
int estSaturee(LISTE L);
int listeTaille(LISTE L);
ELEMENT recuperer(LISTE L, int pos);
int inserer(LISTE L, ELEMENT e, int pos);
int supprimer(LISTE L, int pos);
void listeAfficher(LISTE L);

#endif
