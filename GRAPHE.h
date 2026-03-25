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

grapheReseau creerGraphe(int V);
grapheReseau chargerGraphe(const char *fname);
void detruireGraphe(grapheReseau g);
int ajouterArticle(grapheReseau g, ELEMENT art);
int ajouterCitation(grapheReseau g, int idSrc, int idDest);
int supprimerArticle(grapheReseau g, int idArt);
int supprimerCitation(grapheReseau g, int idSrc, int idDest);
void afficherGraphe(grapheReseau g);

//Interrogation Res

void articlesCites(grapheReseau g, int idSrc);
void articlesCitants(grapheReseau g, int idDest);
void sourcesOriginales(grapheReseau g);
void articlesIsoles(grapheReseau g);
ELEMENT articlePlusCite(grapheReseau g);

//Analyse Chronologique

int comparerDates(ELEMENT art1, ELEMENT art2);
void trierParDate(grapheReseau g);
void premierCitant(grapheReseau g, int idDest);
void chainePropagation(grapheReseau g, int idSrc);

//BFS
void simulationPropagation(grapheReseau g, int idDepart);



#endif // GRAPHE_H_INCLUDED
