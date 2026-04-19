#include "ELTARTICLE.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ELEMENT creerArticle(int id, char *titre, char *source, int score, int j, int m, int a, int h, int min) {
    ELEMENT e = (ELEMENT)malloc(sizeof(articleStruct));
    if (e == NULL) return ELEMENT_VIDE;

    e->id = id;
    strncpy(e->titre, titre, 99);
    e->titre[99] = '\0';
    strncpy(e->source, source, 49);
    e->source[49] = '\0';
    e->score_fiabilite = score;
    e->jour = j;
    e->mois = m;
    e->annee = a;
    e->heure = h;
    e->minute = min;

    return e;
}

void afficherArticle(ELEMENT e) {
    if (e == ELEMENT_VIDE) {
        printf("!!! Article invalide !!!\n");
    } else {
        printf("%s (id:%d, source:%s, score:%d, %02d/%02d/%04d %02dh%02d)\n", e->titre, e->id, e->source, e->score_fiabilite,e->jour, e->mois, e->annee, e->heure, e->minute);
    }
}

void detruireArticle(ELEMENT e) {
    if (e != ELEMENT_VIDE) {
        free(e);
    }
}

int comparerDates(ELEMENT art1, ELEMENT art2) {
    int resultat;

    if (art1->annee != art2->annee) {
        resultat = art1->annee - art2->annee;
    } else if (art1->mois != art2->mois) {
        resultat = art1->mois - art2->mois;
    } else if (art1->jour != art2->jour) {
        resultat = art1->jour - art2->jour;
    } else if (art1->heure != art2->heure) {
        resultat = art1->heure - art2->heure;
    } else {
        resultat = art1->minute-art2->minute;
    }

    return resultat;
}
