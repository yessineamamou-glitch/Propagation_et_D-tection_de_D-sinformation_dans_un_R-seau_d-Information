#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ELTARTICLE.h"


ELEMENT elementCreer(ELEMENT e) {
    ELEMENT nouveau = (ELEMENT)malloc(sizeof(articleStruct));
    if (nouveau != NULL && e != NULL) {
        elementCopier(&nouveau, e);
    }
    return nouveau;
}

void elementCopier(ELEMENT *e1, ELEMENT e2) {
    (*e1)->id = e2->id;
    strcpy((*e1)->titre, e2->titre);
    strcpy((*e1)->source, e2->source);
    (*e1)->score_fiabilite = e2->score_fiabilite;
    (*e1)->jour = e2->jour;
    (*e1)->mois = e2->mois;
    (*e1)->annee = e2->annee;
    (*e1)->heure = e2->heure;
    (*e1)->minute = e2->minute;
}


void elementDetruire(ELEMENT e) {
    if (e != NULL) {
        free(e);
    }
}

void elementAffecter(ELEMENT *e1, ELEMENT e2) {
    *e1 = e2;
}
void elementLire(ELEMENT *e) {
    if (*e == NULL) *e = elementCreer(ELEMENT_VIDE);
    printf("ID : "); scanf("%d", &((*e)->id));
    printf("Titre : "); scanf("%s", (*e)->titre);
    printf("Source : "); scanf("%s", (*e)->source);
    printf("Score (0-10) : "); scanf("%d", &((*e)->score_fiabilite));
    printf("Date (JJ MM AAAA) : ");
    scanf("%d %d %d", &((*e)->jour), &((*e)->mois), &((*e)->annee));
    printf("Heure (HH MM) : ");
    scanf("%d %d", &((*e)->heure), &((*e)->minute));
}

int elementComparer(ELEMENT e1, ELEMENT e2) {
    return (e1->id - e2->id);
}

void elementAfficher(ELEMENT e) {
    if (e != NULL) {
        printf("ID: %d | Titre: %s | Source: %s | Score: %d | Date: %02d/%02d/%d %02d:%02d\n",
               e->id, e->titre, e->source, e->score_fiabilite,
               e->jour, e->mois, e->annee, e->heure, e->minute);
    }
}
