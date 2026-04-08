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

ELEMENT elementCreer(void);
void elementDetruire(ELEMENT e);
void elementLire(ELEMENT *e);
void elementAffecter(ELEMENT *e1, ELEMENT e2);
ELEMENT elementCopier(ELEMENT e);
int elementComparer(ELEMENT e1, ELEMENT e2);
void elementAfficher(ELEMENT e);

#endif
