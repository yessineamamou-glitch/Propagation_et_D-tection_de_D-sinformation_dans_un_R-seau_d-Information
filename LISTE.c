#include <stdio.h>
#include <stdlib.h>
#include "LISTE.h"

Liste creerListeVide() {
    return NULL;
}

int estVide(Liste l) {
    return l == NULL;
}

Liste inserer(Liste l, ELEMENT e) {
    noeud *nouveau = (noeud *)malloc(sizeof(noeud));
    nouveau->donnee = copierElement(e);
    nouveau->suivant = l;
    nouveau->precedent = NULL;

    if (l != NULL) {
        l->precedent = nouveau;
    }

    return nouveau;
}

Liste supprimerElement(Liste l, int id) {
    if (estVide(l)) {
        return l;
    }

    if (l->donnee->id == id) {
        noeud *temp = l;
        l = l->suivant;
        if (l != NULL) {
            l->precedent = NULL;
        }
        free(temp->donnee);
        free(temp);
        return l;
    }

    noeud *courant = l;
    while (courant->suivant != NULL) {
        if (courant->suivant->donnee->id == id) {
            noeud *temp = courant->suivant;
            courant->suivant = courant->suivant->suivant;
            if (courant->suivant != NULL) {
                courant->suivant->precedent = courant;
            }
            free(temp->donnee);
            free(temp);
            return l;
        }
        courant = courant->suivant;
    }

    return l;
}

ELEMENT obtenirElement(Liste l, int id) {
    ELEMENT elt = ELEMENT_VIDE;

    noeud *courant = l;
    while (courant != ELEMENT_VIDE) {
        if (courant->donnee->id == id) {
            return copierElement(courant->donnee);
        }
        courant = courant->suivant;
    }

    return elt;
}

Liste afficherListe(Liste l) {
    noeud *courant = l;
    while (courant !=NULL) {
        afficherElement(courant->donnee);
        courant = courant->suivant;
    }

    return l;
}

Liste detruireListe(Liste l) {
    noeud *courant = l;
    while (courant != NULL) {
        noeud *temp = courant;
        courant = courant->suivant;
        free(temp->donnee);
        free(temp);
    }

    return NULL;
}

int obtenirTaille(Liste l) {
    int compte = 0;
    noeud *courant = l;
    while (courant != NULL) {
        compte++;
        courant = courant->suivant;
    }

    return compte;
}
