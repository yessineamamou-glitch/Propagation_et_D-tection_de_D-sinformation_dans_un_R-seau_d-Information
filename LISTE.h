#ifndef LISTE_H
#define LISTE_H

#include "ELTARTICLE.h"

typedef struct noeud {
    ELEMENT donnee;
    struct noeud *suivant;
    struct noeud *precedent;
} noeud;

typedef noeud *Liste;

Liste creerListeVide();
int estVide(Liste l);
Liste inserer(Liste l, ELEMENT e);
Liste supprimerElement(Liste l, int id);
ELEMENT obtenirElement(Liste l, int id);
Liste afficherListe(Liste l);
Liste detruireListe(Liste l);
int obtenirTaille(Liste l);

#endif
