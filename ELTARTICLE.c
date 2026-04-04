#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ELTARTICLE.h"

ELEMENT creerElement(int id, char titre[100], char source[50], int score_fiabilite,
                     int jour, int mois, int annee, int heure, int minute) {
    ELEMENT e = (ELEMENT)malloc(sizeof(articleStruct));
    e->id = id;
    strcpy(e->titre, titre);
    strcpy(e->source, source);
    e->score_fiabilite = score_fiabilite;
    e->jour = jour;
    e->mois = mois;
    e->annee = annee;
    e->heure = heure;
    e->minute = minute;

    return e;
}

ELEMENT copierElement(ELEMENT e) {
    ELEMENT copie = (ELEMENT)malloc(sizeof(articleStruct));
    copie->id = e->id;
    strcpy(copie->titre, e->titre);
    strcpy(copie->source, e->source);
    copie->score_fiabilite = e->score_fiabilite;
    copie->jour = e->jour;
    copie->mois = e->mois;
    copie->annee = e->annee;
    copie->heure = e->heure;
    copie->minute = e->minute;

    return copie;
}

int comparerElement(ELEMENT e1, ELEMENT e2) {
    int x=0;
    if (e1->id < e2->id) {
        x=-1;
    }
    else if (e1->id > e2->id) {
        x=1;
    }

    return x;
}

int egalElement(ELEMENT e1, ELEMENT e2) {
    return e1->id == e2->id;
}

void afficherElement(ELEMENT e) {
    printf("ID: %d | Titre: %s | Source: %s | Score: %d | Date: %02d/%02d/%d %02d:%02d\n",
           e->id,
           e->titre,
           e->source,
           e->score_fiabilite,
           e->jour,
           e->mois,
           e->annee,
           e->heure,
           e->minute);
}
