#include <stdio.h>
#include <stdlib.h>

ELEMENT elementCreer() {
    ELEMENT e = (ELEMENT)malloc(sizeof(articleStruct));
    if (e == NULL)
        {return NULL;}
    return e;
}

void elementLire(ELEMENT *e) {
    if (*e == NULL)
        {*e = elementCreer();}

    printf("Entrez l'ID : ");
    scanf("%d", &((*e)->id));
    printf("Titre (sans espaces) : ");
    scanf("%s",(*e)->titre);
    printf("Source : ");
    scanf("%s",(*e)->source);
    printf("Score de fiabilite (0-100) : ");
    scanf("%d", &((*e)->score_fiabilite));
    printf("Date (JJ MM AAAA HH MM) : ");
    scanf("%d %d %d %d %d", &((*e)->jour), &((*e)->mois), &((*e)->annee), &((*e)->heure), &((*e)->minute));
}

void elementAfficher(ELEMENT e) {
    if (e != ELEMENT_VIDE) {
        printf("%s (id:%d, source:%s, score:%d, %d/%d/%d %d h%02d)\n",
               e->titre, e->id, e->source, e->score_fiabilite,
               e->jour, e->mois, e->annee, e->heure, e->minute);
    }
}
void elementDetruire(ELEMENT e) {
    if (e != ELEMENT_VIDE) {
        free(e);
    }
}
