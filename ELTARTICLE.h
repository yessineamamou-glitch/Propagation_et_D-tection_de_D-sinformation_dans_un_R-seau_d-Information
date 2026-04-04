#ifndef ELTARTICLE_H
#define ELTARTICLE_H

typedef struct {
    int id;
    char titre[100];
    char source[50];
    int score_fiabilite;
    int jour, mois, annee;
    int heure, minute;
} articleStruct, *ELEMENT;

#define ELEMENT_VIDE NULL


ELEMENT creerArticle(int id, const char *titre, const char *source, int score, int jour, int mois, int annee, int heure, int minute);

ELEMENT copierArticle(ELEMENT art);

void afficherArticle(ELEMENT art);

void afficherArticleSimple(ELEMENT art);


void detruireArticle(ELEMENT art);


int egauxArticles(ELEMENT art1, ELEMENT art2);


char* articleToString(ELEMENT art);

#endif
