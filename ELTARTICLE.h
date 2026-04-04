#ifndef ELTARTICLE_H
#define ELTARTICLE_H

typedef struct {
    int id;
    char titre[100];
    char source[50];
    int score_fiabilite;
    int jour;
    int mois;
    int annee;
    int heure;
    int minute;
} articleStruct, *ELEMENT;

#define ELEMENT_VIDE NULL

ELEMENT creerElement(int id, char titre[100], char source[50], int score_fiabilite, int jour, int mois, int annee, int heure, int minute);
ELEMENT copierElement(ELEMENT e);
int comparerElement(ELEMENT e1, ELEMENT e2);
int egalElement(ELEMENT e1, ELEMENT e2);
void afficherElement(ELEMENT e);

#endif
