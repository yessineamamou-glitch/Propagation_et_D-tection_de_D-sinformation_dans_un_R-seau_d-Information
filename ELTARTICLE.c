#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ELTARTICLE.h"
//idArticle, titre, source, score, jour, mois, annee, heure, minute
ELEMENT elementCreer(ELEMENT e) {
    ELEMENT e_copie = (ELEMENT)malloc(sizeof(articleStruct));
    if (!e_copie) {
        printf("\nErreur d'allocation ELEMENT");
    }
    else{
        e_copie->id = e->id;
        strcpy(e_copie->titre, e->titre);
        strcpy(e_copie->source, e->source);
        e_copie->score_fiabilite = e->score_fiabilite;
        e_copie->jour = e->jour;
        e_copie->mois = e->mois;
        e_copie->annee = e->annee;
        e_copie->heure = e->heure;
        e_copie->minute = e->minute;
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
    ELEMENT e_copie = elementCreer();
    if (e_copie != NULL && e != NULL) {
        e_copie->id = e->id;
        strcpy(e_copie->titre, e->titre);
        strcpy(e_copie->source, e->source);
        e_copie->score_fiabilite = e->score_fiabilite;
        e_copie->jour = e->jour;
        e_copie->mois = e->mois;
        e_copie->annee = e->annee;
        e_copie->heure = e->heure;
        e_copie->minute = e->minute;
    }
    return e_copie;
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
