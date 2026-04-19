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

ELEMENT creerArticle(int id, char *titre, char *source, int score, int j, int m, int a, int h, int min);
void afficherArticle(ELEMENT e);
void detruireArticle(ELEMENT e);
int comparerDates(ELEMENT art1, ELEMENT art2);

#endif
