#include "LISTE.h"
#include <stdlib.h>
#include <stdio.h>

LISTE creerListe() {
    LISTE L = (LISTE)malloc(sizeof(laStruct));

    if (L != NULL) {
        L->tete = NULL;
        L->lg = 0;
    }

    return L;
}

int estVide(LISTE L) {
    int vide;

    if (L == NULL) {
        vide = 1;
    } else {
        vide = (L->tete == NULL) ? 1 : 0;
    }

    return vide;
}

int listeTaille(LISTE L) {
    int taille;

    if (L == NULL) {
        taille = 0;
    } else {
        taille = L->lg;
    }

    return taille;
}

LISTE inserer(LISTE L, ELEMENT e, int pos) {
    LISTE result = L;

    if (L != NULL) {
        NOEUD nouveau = (NOEUD)malloc(sizeof(structNoeud));

        if (nouveau != NULL) {
            nouveau->info = e;

            if (pos >= 1 && pos <= L->lg + 1) {
                if (pos == 1) {
                    nouveau->suivant = L->tete;
                    L->tete = nouveau;
                    L->lg++;
                } else {
                    int count = 1;
                    NOEUD temp = L->tete;

                    while (temp != NULL && count < pos - 1) {
                        temp = temp->suivant;
                        count++;
                    }

                    if (temp != NULL) {
                        nouveau->suivant = temp->suivant;
                        temp->suivant = nouveau;
                        L->lg++;
                    } else {
                        printf("!!! Position invalide !!!\n");
                        free(nouveau);
                    }
                }
            } else {
                printf("!!! Position invalide !!!\n");
                free(nouveau);
            }
        }
    }

    return result;
}

ELEMENT recuperer(LISTE L, int pos) {
    ELEMENT element = ELEMENT_VIDE;

    if (L != NULL) {
        int count = 1;
        NOEUD temp = L->tete;

        while (temp != NULL && count <= pos) {
            if (count == pos) {
                element = temp->info;
            }
            temp = temp->suivant;
            count++;
        }
    }

    return element;
}

LISTE auxSupprimerPos1(LISTE L) {
    LISTE result = L;

    if (L != NULL && L->tete != NULL) {
        NOEUD temp = L->tete->suivant;
        free(L->tete);
        L->tete = temp;
        L->lg--;
    }

    return result;
}

LISTE auxSupprimerPosN(LISTE L, int pos) {
    LISTE result = L;

    if (L != NULL) {
        int count = 1;
        NOEUD temp = L->tete;

        while (temp != NULL && count < pos - 1) {
            temp = temp->suivant;
            count++;
        }

        if (temp != NULL && temp->suivant != NULL) {
            NOEUD toDelete = temp->suivant;
            temp->suivant = toDelete->suivant;
            free(toDelete);
            L->lg--;
        } else {
            printf("!!! Position invalide !!!\n");
        }
    }

    return result;
}

LISTE supprimer(LISTE L, int pos) {
    LISTE result = L;

    if (L != NULL) {
        if (pos >= 1 && pos <= L->lg) {
            if (pos == 1) {
                result = auxSupprimerPos1(L);
            } else {
                result = auxSupprimerPosN(L, pos);
            }
        } else {
            printf("!!! Position invalide !!!\n");
        }
    }

    return result;
}

void auxAfficherListe(NOEUD tete) {
    if (tete == NULL) {
        printf("!!! Liste vide !!!\n");
    } else {
        while (tete != NULL) {
            afficherArticle(tete->info);
            tete = tete->suivant;
        }
    }
}

void afficherListe(LISTE L) {
    if (L == NULL) {
        printf("!!! Liste vide !!!\n");
    } else {
        auxAfficherListe(L->tete);
    }
}

LISTE detruireListe(LISTE L) {
    LISTE result = NULL;

    if (L != NULL) {
        NOEUD temp = L->tete;

        while (temp != NULL) {
            NOEUD suivant = temp->suivant;
            free(temp);
            temp = suivant;
        }

        free(L);
    }

    return result;
}
