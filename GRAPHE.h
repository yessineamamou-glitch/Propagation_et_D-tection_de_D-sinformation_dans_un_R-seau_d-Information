#ifndef GRAPHE_H_INCLUDED
#define GRAPHE_H_INCLUDED
#include "LISTE.h"
#include "ELTARTICLE.h"

typedef struct {
    int V;
    ELEMENT *articles; /* tableau des articles [0..V-1] */
    LISTE *adjList; /* adjList[i] = LISTE des articles */
                    /* cites par l’article i */
    int *degre_in; /* degre_in[i] = nb d’articles */
                    /* qui citent l’article i */
} *grapheReseau;



#endif // GRAPHE_H_INCLUDED
