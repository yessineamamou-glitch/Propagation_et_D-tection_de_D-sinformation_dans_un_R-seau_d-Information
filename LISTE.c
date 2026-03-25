#include <stdio.h>
#include <stdlib.h>
#include "LISTE.h"


LISTE listeCreer(void) {
    return LISTE_VIDE;
}


int estVide(LISTE L) {
    if (L == LISTE_VIDE) {
        return 1;
    }
    return 0;
}

int listeTaille(LISTE L) {
    int nb=0;
    LISTE p = L;
    while (p!= LISTE_VIDE) {
        nb++;
        p=p->suivant;
    }
    return nb;
}


ELEMENT recuperer(LISTE L, int pos) {
    int n = listeTaille(L);
    if (pos < 1 || pos > n) {
        return ELEMENT_VIDE;
    }
    LISTE p = L;
    int i;
    for (i = 1; i < pos; i++) {
        p = p->suivant;
    }
    return p->donnee;
}

int inserer(LISTE *L, ELEMENT e, int pos) {
    int n = listeTaille(*L);
    if (pos < 1 || pos > n + 1) {
        return 0;
    }

    LISTE nouv = (LISTE)malloc(sizeof(Cellule));
    if (nouv == LISTE_VIDE) {
        return 0;
    }
    nouv->donnee= e;

    if (pos == 1) {
        nouv->suivant = *L;
        *L = nouv;
    } else {
        LISTE prec = *L;
        int i;
        for (i = 1; i < pos - 1; i++) {
            prec = prec->suivant;
        }
        nouv->suivant = prec->suivant;
        prec->suivant = nouv;
    }
    return 1;
}

int supprimer(LISTE *L, int pos) {
    int n = listeTaille(*L);
    if (pos < 1 || pos > n) {
        return 0;
    }

    LISTE tmp;
    if (pos == 1) {
        tmp = *L;
        *L = (*L)->suivant;
    } else {
        LISTE prec = *L;
        int i;
        for (i = 1; i < pos - 1; i++) {
            prec = prec->suivant;
        }
        tmp = prec->suivant;
        prec->suivant = tmp->suivant;
    }
    free(tmp);
    return 1;
}

void listeDetruire(LISTE L) {
    while (L!=LISTE_VIDE){
        supprimer(&L, 1);
    }
}
