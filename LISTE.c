#include <stdio.h>
#include <stdlib.h>
#include "LISTE.h"




/* =========================================================
   IMPLEMENTATION: contigue
   ========================================================= */


int articlesEstVide(grapheReseau g) {
    return (g->V == 0);
}

int articlesInserer(grapheReseau g, ELEMENT e, int pos) {
    int i;
    if ((pos < 1) || (pos > g->V + 1)) {
        printf("\nPosition invalide");
        return 0;
    }
    for (i = g->V; i >= pos; i--) {
        elementAffecter(&g->articles[i + 1], g->articles[i]);
    }
    elementAffecter(&g->articles[pos], e);
    (g->V)++;
    return 1;
}

int articlesSupprimer(grapheReseau g, int pos) {
    int i;
    if (articlesEstVide(g)) {
        printf("\nListe vide");
        return 0;
    }
    if ((pos < 1) || (pos > g->V)) {
        printf("\nPosition invalide");
        return 0;
    }
    elementDetruire(g->articles[pos]);
    for (i = pos; i < g->V; i++) {
        elementAffecter(&g->articles[i], g->articles[i + 1]);
    }
    (g->V)--;
    return 1;
}

void articlesAfficher(grapheReseau g) {
    int i;
    for (i = 1; i <= g->V; i++) {
        elementAfficher(g->articles[i]);
    }
}

ELEMENT recupererArticle(grapheReseau g, int pos) {
    if ((pos < 1) || (pos > g->V)) {
        return NULL;
    }
    return g->articles[pos];
}

/* =========================================================
   IMPLEMENTATION: simplement chaine
   ========================================================= */


NOEUD noeudCreer(ELEMENT e) {
    NOEUD n = (NOEUD)malloc(sizeof(structNoeud));
    if(!n) {
        printf("\nPlus d'espace");
    } else {
        elementAffecter(&n->info, e);
        n->suivant = NULL;
    }
    return n;
}

void noeudDetruire(NOEUD n) {
    elementDetruire(n->info);
    free(n);
}

/* --- Opérations sur la Liste --- */

LISTE listeCreer(void) {
    LISTE L = (LISTE)malloc(sizeof(laStruct));
    if (L) {
        L->lg = 0;
        L->tete = NULL;
    }
    return L;
}

int estVide(LISTE L) {
    return (L->lg == 0);
}

int estSaturee(LISTE L) {
    NOEUD temp = (NOEUD)malloc(sizeof(structNoeud));
    if(temp != NULL) {
        free(temp);
        return 0; // Mémoire non saturée
    }
    return 1;
}

int inserer(LISTE L, ELEMENT e, int pos) {
    if (estSaturee(L)) return 0;
    if (pos < 1 || pos > L->lg + 1) return 0;

    NOEUD n = noeudCreer(e);
    if (pos == 1) {
        n->suivant = L->tete;
        L->tete = n;
    } else {
        NOEUD p, q = L->tete;
        for (int i = 1; i < pos; i++) {
            p = q;
            q = q->suivant;
        }
        p->suivant = n;
        n->suivant = q;
    }
    L->lg++;
    return 1;
}

int supprimer(LISTE L, int pos) {
    if (estVide(L) || pos < 1 || pos > L->lg) return 0;

    NOEUD p, q = L->tete;
    if (pos == 1) {
        L->tete = L->tete->suivant;
    } else {
        for (int i = 1; i < pos; i++) {
            p = q;
            q = q->suivant;
        }
        p->suivant = q->suivant;
    }
    noeudDetruire(q);
    L->lg--;
    return 1;
}

ELEMENT recuperer(LISTE L, int pos) {
    if (estVide(L) || pos < 1 || pos > L->lg) return elementCreer(ELEMENT_VIDE);

    NOEUD p = L->tete;
    for (int i = 1; i < pos; i++)
        p = p->suivant;

    ELEMENT elt = elementCreer(p->info);

    return elt;
}

void listeAfficher(LISTE L) {
    NOEUD p = L->tete;
    while (p != NULL) {
        elementAfficher(p->info);
        p = p->suivant;
    }
}

void listeDetruire(LISTE L) {
    NOEUD q = L->tete;
    while (q != NULL) {
        NOEUD p = q;
        q = q->suivant;
        noeudDetruire(p);
    }
    free(L);
}
