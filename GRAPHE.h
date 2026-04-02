/***************************************************
 * Fichier : GRAPHE.H
 * TDA Graphe - Reseau d'Information
 * Realisation avec liste d'adjacence
 ***************************************************/

#ifndef GRAPHE_H
#define GRAPHE_H

#include "LISTE.h"

typedef struct {
    int V;
    ELEMENT *articles;
    LISTE *adjList;
    int *degre_in;
} *grapheReseau;

/* ========== GESTION DU GRAPHE ========== */
grapheReseau creerGraphe(int V);
grapheReseau chargerGraphe(const char *filename);
void detruireGraphe(grapheReseau g);
int ajouterArticle(grapheReseau g, ELEMENT art);
int supprimerArticle(grapheReseau g, int idArt);
int ajouterCitation(grapheReseau g, int idSrc, int idDest);
int supprimerCitation(grapheReseau g, int idSrc, int idDest);
void afficherGraphe(grapheReseau g);

/* ========== INTERROGATION DU RESEAU ========== */
void articlesCites(grapheReseau g, int idSrc);
void articlesCitants(grapheReseau g, int idDest);
void sourcesOriginales(grapheReseau g);
void articlesIsoles(grapheReseau g);
ELEMENT articlePlusCite(grapheReseau g);

/* ========== ANALYSE CHRONOLOGIQUE ========== */
int comparerDates(ELEMENT art1, ELEMENT art2);
void trierParDate(grapheReseau g);
void premierCitant(grapheReseau g, int idDest);
void chainePropagation(grapheReseau g, int idSrc);

/* ========== SIMULATION DE PROPAGATION - BFS ========== */
void simulerPropagation(grapheReseau g, int idSrc);
void articlesAccessibles(grapheReseau g, int idSrc);

/* ========== DETECTION DE FAKE NEWS ========== */
int analyserArticle(ELEMENT art);
void analyserReseau(grapheReseau g);
void articlesSuspectsCites(grapheReseau g);

/* ========== BONUS ========== */
void simulerSuppression(grapheReseau g, int idArt);
int neutraliserPropagation(grapheReseau g, int idSrc, int idDest);

/* ========== FONCTIONS AUXILIAIRES ========== */
int trouvIndexArticle(grapheReseau g, int id);
void afficherStatistiques(grapheReseau g);

#endif