#ifndef LISTE_H
#define LISTE_H

#include "ELTARTICLE.h"

typedef struct structNoeud {
    ELEMENT info;
    struct structNoeud * suivant;
} structNoeud, * NOEUD;

typedef struct {
    NOEUD tete;
    int lg;
} laStruct, *LISTE;

LISTE creerListe();
int estVide(LISTE L);
int listeTaille(LISTE L);
LISTE inserer(LISTE L, ELEMENT e, int pos);
ELEMENT recuperer(LISTE L, int pos);
LISTE supprimer(LISTE L, int pos);
void afficherListe(LISTE L);
LISTE detruireListe(LISTE L);

#endif
