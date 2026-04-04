#ifndef GRAPHE_H
#define GRAPHE_H

#include "LISTE.h"

typedef struct {
    int nombre_articles;
    ELEMENT *articles;
    Liste *adjacence;
} grapheReseau;

grapheReseau createGraph(int V);
void chargerGraph(grapheReseau *g, const char *filename);
void detruireGraph(grapheReseau *g);
void ajouterArticle(grapheReseau *g, ELEMENT art);
void supprimerArticle(grapheReseau *g, int idArt);
void ajouterCitation(grapheReseau *g, int idDrc, int idDest);
void supprimerCitation(grapheReseau *g, int idDrc, int idDest);
void afficherGraphe(grapheReseau *g);
void articlesCites(grapheReseau *g, int idDrc);
void articlesCitants(grapheReseau *g, int idDest);
void sourcesOriginales(grapheReseau *g);
void articlesIsoles(grapheReseau *g);
ELEMENT articlePlusCite(grapheReseau *g);
void trierParDate(grapheReseau *g);
void chainerPropagation(grapheReseau *g, int idDrc);
void simulerPropagation(grapheReseau *g, int idDrc);
void analyserReseau(grapheReseau *g);
void simulerSuppression(grapheReseau *g, int idArt);
int neutraliserPropagation(grapheReseau *g, int idDrc, int idDest);
void sauvegarderGraphe(grapheReseau *g, const char *filename);

#endif
