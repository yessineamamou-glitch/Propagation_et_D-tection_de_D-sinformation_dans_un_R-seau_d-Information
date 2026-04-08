#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ELTARTICLE.h"

ELEMENT elementCreer(void) {
    ELEMENT e = (ELEMENT)malloc(sizeof(articleStruct));
    if (!e) {
        printf("\nErreur d'allocation ELEMENT");
    }
    return e;
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
    *e = elementCreer();
    printf("ID : "); scanf("%d", &((*e)->id));
    printf("Titre : "); scanf("%s", (*e)->titre);
    printf("Source : "); scanf("%s", (*e)->source);
    printf("Score (0-10) : "); scanf("%d", &((*e)->score_fiabilite));
    printf("Date (JJ MM AAAA) : ");
    scanf("%d %d %d", &((*e)->jour), &((*e)->mois), &((*e)->annee));
    printf("Heure (HH MM) : ");
    scanf("%d %d", &((*e)->heure), &((*e)->minute));
}

ELEMENT elementCopier(ELEMENT e) {
    ELEMENT copie = elementCreer();
    if (copie != NULL && e != NULL) {
        copie->id = e->id;
        strcpy(copie->titre, e->titre);
        strcpy(copie->source, e->source);
        copie->score_fiabilite = e->score_fiabilite;
        copie->jour = e->jour;
        copie->mois = e->mois;
        copie->annee = e->annee;
        copie->heure = e->heure;
        copie->minute = e->minute;
    }
    return copie;
}

int elementComparer(ELEMENT e1, ELEMENT e2) {
    if (e1->id < e2->id) return -1;
    if (e1->id > e2->id) return 1;
    return 0;
}

void elementAfficher(ELEMENT e) {
    if (e != NULL) {
        printf("ID: %d | Titre: %s | Source: %s | Score: %d | Date: %02d/%02d/%d %02d:%02d\n",
               e->id, e->titre, e->source, e->score_fiabilite,
               e->jour, e->mois, e->annee, e->heure, e->minute);
    }
}
