#include "ELTARTICLE.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ELEMENT creerArticle(int id, const char *titre, const char *source,int score, int jour, int mois, int annee,int heure, int minute) {
    ELEMENT art = (ELEMENT)malloc(sizeof(articleStruct));
    if (art == NULL) {
        fprintf(stderr, "Allocation memoire echouee\n");
        return ELEMENT_VIDE;
    }

    art->id = id;
    strncpy(art->titre, titre,99);
    art->titre[99] = '\0';
    strncpy(art->source, source,49);
    art->source[49] = '\0';
    art->score_fiabilite = score;
    art->jour = jour;
    art->mois = mois;
    art->annee = annee;
    art->heure = heure;
    art->minute = minute;

    return art;
}

ELEMENT copierArticle(ELEMENT art) {
    if (art == ELEMENT_VIDE) return ELEMENT_VIDE;
    return creerArticle(art->id, art->titre, art->source, art->score_fiabilite,art->jour, art->mois, art->annee, art->heure, art->minute);
}

void afficherArticle(ELEMENT art) {
    if (art == ELEMENT_VIDE) return;
    printf("%s (id:%d, source:%s, score:%d, %02d/%02d/%d %02dh%02d)\n",art->titre, art->id, art->source, art->score_fiabilite,art->jour, art->mois, art->annee, art->heure, art->minute);
}

void afficherArticleSimple(ELEMENT art) {
    if (art == ELEMENT_VIDE) return;
    printf("%s", art->titre);
}

void detruireArticle(ELEMENT art) {
    if (art != ELEMENT_VIDE) {
        free(art);
    }
}

int egauxArticles(ELEMENT art1, ELEMENT art2) {
    if (art1 == ELEMENT_VIDE || art2 == ELEMENT_VIDE) return 0;
    return art1->id == art2->id;
}

char* articleToString(ELEMENT art) {
    static char buffer[200];
    if (art == ELEMENT_VIDE) {
        strcpy(buffer,"Article vide");
        return buffer;
    }
    snprintf(buffer,199,"%s(id:%d)",art->titre,art->id);
    return buffer;
}
