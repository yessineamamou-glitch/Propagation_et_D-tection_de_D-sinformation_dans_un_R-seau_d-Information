#ifndef GRAPHE_H
#define GRAPHE_H

#include "LISTE.h"
/* =========================================================
  MAIN LISTE:graphe
   ========================================================= */
typedef struct {
    int V; //lg de la liste article
    ELEMENT *articles;
    LISTE   *adjList;
    LISTE   *citants;      // le contraire du  adjliste
    int     *degre_in;    //degree in =lg de les liste citants   je essayer le plus possible " de respecter la memes structure dans lennonce
} structuregraphe,*grapheReseau;


typedef struct{
LISTE *citants;
}structurecitant;*CITERS

grapheReseau createGraph(int V);
void chargerGraph(grapheReseau *g, const char *filename);
void detruireGraph(grapheReseau *g);
void ajouterArticle(grapheReseau *g, ELEMENT art);
void supprimerArticle(grapheReseau *g, int idArt);
void ajouterCitation(grapheReseau *g, int idSrc, int idDest);
void supprimerCitation(grapheReseau *g, int idSrc, int idDest);
void afficherGraphe(grapheReseau *g);
void articlesCites(grapheReseau *g, int idSrc);
void articlesCitants(grapheReseau *g, int idDest);
void sourcesOriginales(grapheReseau *g);
void articlesIsoles(grapheReseau *g);
ELEMENT articlePlusCite(grapheReseau *g);
void trierParDate(grapheReseau *g);
void chainerPropagation(grapheReseau *g, int idSrc);
void simulerPropagation(grapheReseau *g, int idSrc);
void analyserReseau(grapheReseau *g);
void simulerSuppression(grapheReseau *g, int idArt);
int neutraliserPropagation(grapheReseau *g, int idSrc, int idDest);
void sauvegarderGraphe(grapheReseau *g, const char *filename);

#endif
