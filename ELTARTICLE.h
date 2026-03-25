#ifndef ELTARTICLE_H_INCLUDED
#define ELTARTICLE_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>

typedef struct{
    int id;
    char titre[100];
    char source[50];
    int score_fiabilite;
    int jour, mois, annee;
    int heure, minute;
}articleStruct, *ELEMENT ;



ELEMENT elementCreer();
void elementLire(ELEMENT *e);
void elementAfficher (ELEMENT e);
void elementAffecter(ELEMENT *e1, ELEMENT e2);
int elementComparer(ELEMENT e1, ELEMENT e2);
int elementGetId(ELEMENT e);
void elementCopier(ELEMENT *dest, ELEMENT source);
void elementDetruire(ELEMENT e);



#define ELEMENT_VIDE NULL
#endif // ELTARTICLE_H_INCLUDED
