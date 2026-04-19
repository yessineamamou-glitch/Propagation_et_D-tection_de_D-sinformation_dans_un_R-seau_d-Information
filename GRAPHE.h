#ifndef GRAPHE_H
#define GRAPHE_H

#include "LISTE.h"

typedef struct {
    int V;
    ELEMENT *articles;
    LISTE *adjList;
    int *degre_in;
} *grapheReseau;

grapheReseau creerGraphe(int V);
grapheReseau chargerGraphe(const char *filename);
void detruireGraphe(grapheReseau g);
int ajouterArticle(grapheReseau g, ELEMENT art);
int supprimerArticle(grapheReseau g, int idArt);
int ajouterCitation(grapheReseau g, int idSrc, int idDest);
int supprimerCitation(grapheReseau g, int idSrc, int idDest);
void afficherGraphe(grapheReseau g);
void sauvegarderGraphe(grapheReseau g, const char *filename);
void articlesCites(grapheReseau g, int idSrc);
void articlesCitants(grapheReseau g, int idDest);
void sourcesOriginales(grapheReseau g);
void articlesIsoles(grapheReseau g);
ELEMENT articlePlusCite(grapheReseau g);
void trierParDate(grapheReseau g);
void premierCitant(grapheReseau g, int idDest);
void chainePropagation(grapheReseau g, int idSrc);
void simulerPropagation(grapheReseau g, int idSrc);
void articlesAccessibles(grapheReseau g, int idSrc);
void simulerSuppression(grapheReseau g, int idArt);
static void dfsTousPaths(grapheReseau g, int idCourant, int idDest, int *chemin, int profondeur, int *visited, int *meilleurChemin, int *meilleurLen);
int neutraliserPropagation(grapheReseau g, int idSrc, int idDest);
#endif
