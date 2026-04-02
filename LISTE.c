#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LISTE.h"

LISTE listeCreer() {
    return NULL;
}

LISTE listeCopier(LISTE l) {
    LISTE nouvelle = listeCreer();
    LISTE actuel = l;
    int pos = 1;

    while (actuel != NULL) {
        inserer(&nouvelle, copierArticle(actuel->info), pos);
        actuel = actuel->succ;
        pos++;
    }
    return nouvelle;
}

int listeTaille(LISTE l) {
    int taille = 0;
    LISTE actuel = l;
    while (actuel != NULL) {
        taille++;
        actuel = actuel->succ;
    }
    return taille;
}

int inserer(LISTE *l, ELEMENT elem, int pos) {
    if (elem == ELEMENT_VIDE || l == NULL) {
        return 0;
    }

    int taille = listeTaille(*l);
    if (pos < 1 || pos > taille + 1) {
        return 0;
    }

    Noeud *nouveau = (Noeud*)malloc(sizeof(Noeud));
    if (nouveau == NULL) {
        fprintf(stderr, "Erreur : allocation memoire echouee\n");
        return 0;
    }

    nouveau->info = copierArticle(elem);
    nouveau->succ = NULL;

    if (pos == 1) {
        nouveau->succ = *l;
        *l = nouveau;
        return 1;
    }

    LISTE actuel = *l;
    int cpt = 1;

    while (cpt < pos - 1 && actuel != NULL) {
        actuel = actuel->succ;
        cpt++;
    }

    if (actuel == NULL) {
        free(nouveau);
        return 0;
    }

    nouveau->succ = actuel->succ;
    actuel->succ = nouveau;

    return 1;
}

ELEMENT recuperer(LISTE l, int pos) {
    if (pos < 1) return ELEMENT_VIDE;

    LISTE actuel = l;
    int cpt = 1;

    while (cpt < pos && actuel != NULL) {
        actuel = actuel->succ;
        cpt++;
    }

    if (actuel == NULL) return ELEMENT_VIDE;
    return actuel->info;
}

int rechercher(LISTE l, ELEMENT elem) {
    if (elem == ELEMENT_VIDE) return 0;

    LISTE actuel = l;
    int pos = 1;

    while (actuel != NULL) {
        if (egauxArticles(actuel->info, elem)) {
            return pos;
        }
        actuel = actuel->succ;
        pos++;
    }
    return 0;
}

int supprimer(LISTE *l, int pos) {
    if (l == NULL || *l == NULL || pos < 1) {
        return 0;
    }

    if (pos == 1) {
        Noeud *temp = *l;
        *l = (*l)->succ;
        detruireArticle(temp->info);
        free(temp);
        return 1;
    }

    LISTE actuel = *l;
    int cpt = 1;

    while (cpt < pos - 1 && actuel != NULL) {
        actuel = actuel->succ;
        cpt++;
    }

    if (actuel == NULL || actuel->succ == NULL) {
        return 0;
    }

    Noeud *temp = actuel->succ;
    actuel->succ = temp->succ;
    detruireArticle(temp->info);
    free(temp);

    return 1;
}

void listeDetruire(LISTE *l) {
    while (*l != NULL) {
        supprimer(l, 1);
    }
}

void afficherListe(LISTE l) {
    LISTE actuel = l;
    int pos = 1;

    while (actuel != NULL) {
        printf("  Pos %d : ", pos);
        afficherArticle(actuel->info);
        actuel = actuel->succ;
        pos++;
    }
}

void afficherListeCompacte(LISTE l, int afficherTous) {
    LISTE actuel = l;
    int pos = 0;

    while (actuel != NULL && (afficherTous || pos < 3)) {
        printf("--> ");
        afficherArticleSimple(actuel->info);
        printf("\n");
        actuel = actuel->succ;
        pos++;
    }

    if (!afficherTous && actuel != NULL) {
        printf("... et %d autres\n", listeTaille(l) - 3);
    }
}

ELEMENT rechercherParId(LISTE l, int id) {
    LISTE actuel = l;

    while (actuel != NULL) {
        if (actuel->info->id == id) {
            return actuel->info;
        }
        actuel = actuel->succ;
    }
    return ELEMENT_VIDE;
}
