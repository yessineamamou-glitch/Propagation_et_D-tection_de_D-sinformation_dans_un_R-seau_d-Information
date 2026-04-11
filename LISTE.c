#include <stdio.h>
#include <stdlib.h>
#include "LISTE.h"

/* =========================================================
   IMPLEMENTATION: CITATION LIST
   ========================================================= */

static CitationNoeudPtr cit_noeudCreer(int id) {
    CitationNoeudPtr n = (CitationNoeudPtr)malloc(sizeof(CitationListeNoeud));
    if (n == NULL) {
        printf("\nPlus d'espace memoire pour la citation.");
    } else {
        n->id_cite = id;
        n->suivant = NULL;
    }
    return n;
}

Citation_LISTE cit_listeCreer(void) {
    Citation_LISTE L = (Citation_LISTE)malloc(sizeof(CitationListeStruct));
    if (L != NULL) {
        L->tete = NULL;
        L->lg = 0;
    }
    return L;
}

int cit_estVide(Citation_LISTE L) {
    return (L->lg == 0);
}

int cit_listeTaille(Citation_LISTE L) {
    return L->lg;
}

int cit_inserer(Citation_LISTE L, int id, int pos) {
    int success = 0;
    if (L != NULL && pos >= 1 && pos <= L->lg + 1) {
        CitationNoeudPtr n = cit_noeudCreer(id);
        if (n != NULL) {
            if (pos == 1) {
                n->suivant = L->tete;
                L->tete = n;
            } else {
                CitationNoeudPtr p = L->tete;
                for (int i = 1; i < pos - 1; i++) {
                    p = p->suivant;
                }
                n->suivant = p->suivant;
                p->suivant = n;
            }
            L->lg++;
            success = 1;
        }
    }
    return success;
}

int cit_supprimer(Citation_LISTE L, int pos) {
    int success = 0;
    if (L != NULL && !cit_estVide(L) && pos >= 1 && pos <= L->lg) {
        CitationNoeudPtr q = NULL;
        if (pos == 1) {
            q = L->tete;
            L->tete = L->tete->suivant;
        } else {
            CitationNoeudPtr p = L->tete;
            for (int i = 1; i < pos - 1; i++) {
                p = p->suivant;
            }
            q = p->suivant;
            p->suivant = q->suivant;
        }
        free(q);
        L->lg--;
        success = 1;
    }
    return success;
}

void cit_listeAfficher(Citation_LISTE L) {
    if (L != NULL) {
        CitationNoeudPtr courant = L->tete;
        while (courant != NULL) {
            printf("%d ", courant->id_cite);
            courant = courant->suivant;
        }
        printf("\n");
    }
}

void cit_listeDetruire(Citation_LISTE L) {
    if (L != NULL) {
        while (!cit_estVide(L)) {
            cit_supprimer(L, 1);
        }
        free(L);
    }
}

Citation_LISTE cit_listeCopier(Citation_LISTE L) {
    Citation_LISTE nL = cit_listeCreer();
    if (L != NULL && nL != NULL) {
        CitationNoeudPtr courant = L->tete;
        int pos = 1;
        while (courant != NULL) {
            cit_inserer(nL, courant->id_cite, pos);
            courant = courant->suivant;
            pos++;
        }
    }
    return nL;
}

/* =========================================================
   IMPLEMENTATION: MAIN LIST
   ========================================================= */

static MainNoeudPtr main_noeudCreer(ELEMENT e, Citation_LISTE citations) {
    MainNoeudPtr n = (MainNoeudPtr)malloc(sizeof(MainListeNoeud));
    if (n == NULL) {
        printf("\nPlus d'espace memoire pour l'element principal.");
    } else {
        n->info = e;
        n->citations = citations;
        n->suivant = NULL;
    }
    return n;
}

LISTE main_listeCreer(void) {
    LISTE L = (LISTE)malloc(sizeof(MainListeStruct));
    if (L != NULL) {
        L->tete = NULL;
        L->lg = 0;
    }
    return L;
}

int main_estVide(LISTE L) {
    return (L == NULL || L->lg == 0);
}

int main_listeTaille(LISTE L) {
    int taille = 0;
    if (L != NULL) {
        taille = L->lg;
    }
    return taille;
}

int main_inserer(LISTE L, ELEMENT e, Citation_LISTE citations, int pos) {
    int success = 0;
    if (L != NULL && pos >= 1 && pos <= L->lg + 1) {
        MainNoeudPtr n = main_noeudCreer(e, citations);
        if (n != NULL) {
            if (pos == 1) {
                n->suivant = L->tete;
                L->tete = n;
            } else {
                MainNoeudPtr p = L->tete;
                for (int i = 1; i < pos - 1; i++) {
                    p = p->suivant;
                }
                n->suivant = p->suivant;
                p->suivant = n;
            }
            L->lg++;
            success = 1;
        }
    }
    return success;
}
//                                                                        faute fi supp maamltch noeud detrui w lezem maaha cit detrue
int main_supprimer(LISTE L, int pos) {
    int success = 0;
    if (L != NULL && !main_estVide(L) && pos >= 1 && pos <= L->lg) {
        MainNoeudPtr q = NULL;
        if (pos == 1) {
            q = L->tete;
            L->tete = L->tete->suivant;
        } else {
            MainNoeudPtr p = L->tete;
            for (int i = 1; i < pos - 1; i++) {
                p = p->suivant;
            }
            q = p->suivant;
            p->suivant = q->suivant;
        }
        // If you need to clean up citations during deletion, do it here
        // cit_listeDetruire(q->citations);
        free(q);
        L->lg--;
        success = 1;
    }
    return success;
}

ELEMENT main_recuperer(LISTE L, int pos) {
    ELEMENT e = NULL;
    if (L != NULL && pos >= 1 && pos <= L->lg) {
        MainNoeudPtr courant = L->tete;
        for (int i = 1; i < pos; i++) {
            courant = courant->suivant;
        }
        e = courant->info;
    }
    return e;
}

LISTE main_listeCopier(LISTE L) {
    LISTE nL = main_listeCreer();
    if (L != NULL && nL != NULL) {
        MainNoeudPtr courant = L->tete;
        int pos = 1;
        while (courant != NULL) {
            ELEMENT eCopy = elementCopier(courant->info);
            Citation_LISTE citCopy = cit_listeCopier(courant->citations);
            main_inserer(nL, eCopy, citCopy, pos);
            courant = courant->suivant;
            pos++;
        }
    }
    return nL;
}

void main_listeAfficher(LISTE L) {
    if (L != NULL) {
        MainNoeudPtr courant = L->tete;
        while (courant != NULL) {
            elementAfficher(courant->info);
            printf(" Citations: ");
            cit_listeAfficher(courant->citations);
            courant = courant->suivant;
        }
    }
}

void main_listeDetruire(LISTE L) {
    if (L != NULL) {
        while (!main_estVide(L)) {
            main_supprimer(L, 1);
        }
        free(L);
    }
}
