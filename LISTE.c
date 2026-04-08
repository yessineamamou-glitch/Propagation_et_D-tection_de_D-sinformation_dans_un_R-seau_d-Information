#include <stdio.h>
#include <stdlib.h>
#include "LISTE.h"

static NOEUD noeudCreer(ELEMENT e) {
    NOEUD n = (NOEUD)malloc(sizeof(structNoeud));
    if (!n) {
        printf("\nPlus d'espace");
    } else {
        n->info = e;
        n->suivant = NULL;
        n->precedent = NULL;
    }
    return n;
}

LISTE listeCreer(void) {
    LISTE L = (LISTE)malloc(sizeof(laStruct));
    if (L != NULL) {
        L->tete = NULL;
        L->queue = NULL;
        L->lg = 0;
    }
    return L;
}

int estVide(LISTE L) {
    return (L->lg == 0);
}

int estSaturee(LISTE L) {
    return 0;
}

int listeTaille(LISTE L) {
    return L->lg;
}

int inserer(LISTE L, ELEMENT e, int pos) {
    int succee = 1;
    int i;
    NOEUD n, p, q;

    if (estSaturee(L)) {
        printf("\nListe saturee");
        succee = 0;
    } else {
        if ((pos < 1) || (pos > L->lg + 1)) {
            printf("\nPosition invalide");
            succee = 0;
        } else {
            n = noeudCreer(e);
            if (L->lg == 0) {
                L->tete = n;
                L->queue = n;
            } else {
                if (pos == 1) {
                    L->tete->precedent = n;
                    n->suivant = L->tete;
                    L->tete = n;
                } else if (pos == (L->lg + 1)) {
                    L->queue->suivant = n;
                    n->precedent = L->queue;
                    L->queue = n;
                } else {
                    q = L->tete;
                    for (i = 1; i < pos; i++) {
                        p = q;
                        q = q->suivant;
                    }
                    p->suivant = n;
                    n->precedent = p;
                    n->suivant = q;
                    q->precedent = n;
                }
            }
            (L->lg)++;
        }
    }
    return succee;
}

int supprimer(LISTE L, int pos) {
    int i;
    int succee = 1;
    NOEUD p, q;

    if (estVide(L)) {
        printf("\nListe vide");
        succee = 0;
    } else {
        if ((pos < 1) || (pos > L->lg)) {
            printf("\nPosition invalide");
            succee = 0;
        } else {
            if (L->lg == 1) {
                q = L->tete;
                L->tete = NULL;
                L->queue = NULL;
            } else {
                if (pos == 1) {
                    q = L->tete;
                    L->tete = L->tete->suivant;
                    L->tete->precedent = NULL;
                } else if (pos == L->lg) {
                    q = L->queue;
                    L->queue = L->queue->precedent;
                    L->queue->suivant = NULL;
                } else {
                    q = L->tete;
                    for (i = 1; i < pos; i++) {
                        p = q;
                        q = q->suivant;
                    }
                    q->suivant->precedent = p;
                    p->suivant = q->suivant;
                }
            }
            free(q);
            (L->lg)--;
        }
    }
    return succee;
}

ELEMENT recuperer(LISTE L, int pos) {
    if (estVide(L) || pos < 1 || pos > L->lg) {
        return NULL;
    }
    NOEUD courant = L->tete;
    for (int i = 1; i < pos; i++) {
        courant = courant->suivant;
    }
    return courant->info;
}

void listeAfficher(LISTE L) {
    NOEUD courant = L->tete;
    while (courant != NULL) {
        elementAfficher(courant->info);
        courant = courant->suivant;
    }
}

void listeDetruire(LISTE L) {
    while (!estVide(L)) {
        supprimer(L, 1);
    }
    free(L);
}
