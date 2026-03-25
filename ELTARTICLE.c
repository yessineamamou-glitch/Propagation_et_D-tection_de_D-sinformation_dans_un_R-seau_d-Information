#include "ELTARTICLE.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ELEMENT elementCreer()
{
    ELEMENT e;
    e = (ELEMENT)malloc(sizeof(articleStruct));
    if (e == ELEMENT_VIDE) {
        return ELEMENT_VIDE;
    }
    return e;
}


void elementLire(ELEMENT *e){
    if (*e == ELEMENT_VIDE)
        {*e = elementCreer();}

    printf("Entrez l'ID : ");
    scanf("%d",&((*e)->id));
    getchar();
    printf("Titre : ");
    scanf("%[^\n]s",(*e)->titre);
    printf("\n");
    printf("Source :");
    scanf("%s",(*e)->source);
    printf("\n");
    printf("Score de fiabilite : ");
    scanf("%d",&(*e)->score_fiabilite);
    printf("\n");
    printf("date:J M A H M ");
    scanf("%d %d %d %d %d",&(*e)->jour,&(*e)->mois,&(*e)->annee,&(*e)->heure,&(*e)->minute);
}


void elementAfficher(ELEMENT e){
    if (e!= ELEMENT_VIDE) {
        printf("%s id:%d , source:%s , score:%d , %d/%d/%d %d,%d",e->titre, e->id, e->source, e->score_fiabilite,e->jour, e->mois, e->annee, e->heure, e->minute);
    printf("\n");}
}

void elementAffecter(ELEMENT *e1,ELEMENT e2){
    *e1=e2;
}

int elementComparer(ELEMENT e1, ELEMENT e2)
{
    if (e1==ELEMENT_VIDE||e2==ELEMENT_VIDE){
        return -1;}
    return (((e1)->id)-(e2->id));
}


int elementGetId(ELEMENT e){
    if (e ==ELEMENT_VIDE){
        return -1;}
    return e->id;
}

void elementCopier(ELEMENT *dest, ELEMENT source) {
    if (source == ELEMENT_VIDE) {
        *dest = ELEMENT_VIDE;
        printf("copie impossible,la source est vide\n");
        return;}

    if (*dest==ELEMENT_VIDE){
        *dest = elementCreer();
        if (*dest == ELEMENT_VIDE) {
            printf("echec de l allocation memoire ");
            printf("\n");
            return;
        }
    }

    (*dest)->id = source->id;
    strcpy((*dest)->titre,source->titre);
    strcpy((*dest)->source,source->source);
    (*dest)->score_fiabilite =source->score_fiabilite;

    (*dest)->jour = source->jour;
    (*dest)->mois = source->mois;
    (*dest)->annee = source->annee;
    (*dest)->heure = source->heure;
    (*dest)->minute = source->minute;

}


void elementDetruire(ELEMENT e) {
    if (e !=ELEMENT_VIDE) {
        free(e);}
}

