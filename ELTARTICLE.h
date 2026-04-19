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


ELEMENT elementCreer(ELEMENT e);
void elementDetruire(ELEMENT e);
void elementLire(ELEMENT *e);
void elementAffecter(ELEMENT *e1, ELEMENT e2);
void elementCopier(ELEMENT *e1, ELEMENT e2);
int elementComparer(ELEMENT e1, ELEMENT e2);
void elementAfficher(ELEMENT e);

#endif
