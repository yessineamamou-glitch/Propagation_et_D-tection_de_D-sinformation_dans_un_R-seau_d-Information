#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GRAPHE.h"
#include "FAKESDB.h"

// Fonction locale pour trouver l'indice d'un article par son ID
int trouverIndice(grapheReseau g, int id) {
    for (int i = 0; i < g.nombre_articles; i++) {
        if (g.articles[i] != NULL && g.articles[i]->id == id) {
            return i;
        }
    }
    return -1;
}

grapheReseau creerGraphe(int max_articles) {
    grapheReseau g;
    g.nombre_articles = 0;
    g.articles = (ELEMENT *)malloc(max_articles * sizeof(ELEMENT));
    g.adjList = (Liste *)malloc(max_articles * sizeof(Liste));
    g.degre_in = (int *)malloc(max_articles * sizeof(int));

    for (int i = 0; i < max_articles; i++) {
        g.articles[i] = NULL;
        g.adjList[i] = creerListeVide();
        g.degre_in[i] = 0;
    }
    return g;
}

void chargerGraphe(grapheReseau *g, const char *nomFichier) {
    FILE *f = fopen(nomFichier, "r");
    if (!f) return;

    char ligne[512];
    while (fgets(ligne, sizeof(ligne), f)) {
        if (ligne[0] == '#' || ligne[0] == '\n' || ligne[0] == '\r') continue;

        if (ligne[0] == 'A') {
            articleStruct tmp;
            sscanf(ligne, "A %d %s %s %d %d %d %d %d %d",
                   &tmp.id, tmp.titre, tmp.source, &tmp.score_fiabilite,
                   &tmp.jour, &tmp.mois, &tmp.annee, &tmp.heure, &tmp.minute);

            if (trouverIndice(*g, tmp.id) == -1) {
                ajouterArticle(g, &tmp);
            }
        } else if (ligne[0] == 'C') {
            int idSrc, idDest;
            sscanf(ligne, "C %d %d", &idSrc, &idDest);
            ajouterCitation(g, idSrc, idDest);
        }
    }
    fclose(f);
}

void ajouterArticle(grapheReseau *g, ELEMENT art) {
    int libre = -1;
    for(int i=0; i<100; i++) { // Supposons max 100 articles
        if(g->articles[i] == NULL) { libre = i; break; }
    }
    if (libre != -1) {
        copierElement(&(g->articles[libre]), art);
        if (g->nombre_articles <= libre) g->nombre_articles = libre + 1;
    }
}

void ajouterCitation(grapheReseau *g, int idSrc, int idDest) {
    int u = trouverIndice(*g, idSrc);
    int v = trouverIndice(*g, idDest);

    if (u != -1 && v != -1) {
        // Vérifier doublon
        if (obtenirElement(g->adjList[u], idDest) == NULL) {
            inserer(g->adjList[u], g->articles[v]);
            g->degre_in[v]++;
        }
    }
}

void supprimerCitation(grapheReseau *g, int idSrc, int idDest) {
    int u = trouverIndice(*g, idSrc);
    int v = trouverIndice(*g, idDest);

    if (u != -1 && v != -1) {
        if (supprimerElement(g->adjList[u], idDest)) {
            g->degre_in[v]--;
        }
    }
}

void supprimerArticle(grapheReseau *g, int id) {
    int idx = trouverIndice(*g, id);
    if (idx == -1) return;

    // 1. Supprimer toutes les citations sortantes et mettre à jour les degre_in des cibles
    NOEUD curr = g->adjList[idx]->tete;
    while(curr != NULL) {
        int idCible = curr->donnee->id;
        int idxCible = trouverIndice(*g, idCible);
        if (idxCible != -1) g->degre_in[idxCible]--;
        curr = curr->suivant;
    }
    detruireListe(g->adjList[idx]);
    g->adjList[idx] = creerListeVide();

    // 2. Supprimer les citations entrantes depuis les autres articles
    for (int i = 0; i < g->nombre_articles; i++) {
        if (i != idx && g->articles[i] != NULL) {
            if (supprimerElement(g->adjList[i], id)) {
                // degre_in[idx] n'a pas besoin d'être décrémenté car l'article est supprimé
            }
        }
    }

    free(g->articles[idx]);
    g->articles[idx] = NULL;
    g->degre_in[idx] = 0;
}

void afficherGraphe(grapheReseau *g) {
    printf("\n--- ETAT DU RESEAU ---\n");
    for (int i = 0; i < g->nombre_articles; i++) {
        if (g->articles[i] != NULL) {
            printf("[%d] ", g->articles[i]->id);
            afficherElement(g->articles[i]);
            printf("    Degré entrant: %d | Cite: ", g->degre_in[i]);
            afficherListe(g->adjList[i]);
            printf("\n");
        }
    }
}

void simulerPropagation(grapheReseau g, int idDepart) {
    int start = trouverIndice(g, idDepart);
    if (start == -1) return;

    int *file = malloc(g.nombre_articles * sizeof(int));
    int *visite = calloc(g.nombre_articles, sizeof(int));
    int tete = 0, queue = 0;

    file[queue++] = start;
    visite[start] = 1;

    printf("\nPropagation depuis l'article %d :\n", idDepart);
    while (tete < queue) {
        int u = file[tete++];
        printf("Article atteint : %s\n", g.articles[u]->titre);

        NOEUD curr = g.adjList[u]->tete;
        while (curr != NULL) {
            int v = trouverIndice(g, curr->donnee->id);
            if (v != -1 && !visite[v]) {
                visite[v] = 1;
                file[queue++] = v;
            }
            curr = curr->suivant;
        }
    }
    free(file); free(visite);
}

void analyserReseau(grapheReseau *g) {
    printf("\n--- ANALYSE DE FIABILITE ---\n");
    for (int i = 0; i < g->nombre_articles; i++) {
        if (g->articles[i] != NULL) {
            int score = g->articles[i]->score_fiabilite;
            const char *label = (score < 40) ? "[SUSPECT]" : (score < 70) ? "[DOUTEUX]" : "[FIABLE]";
            printf("%s ID %d: %s (Score: %d)\n", label, g->articles[i]->id, g->articles[i]->titre, score);
        }
    }
}

void detruireGraphe(grapheReseau *g) {
    for (int i = 0; i < g->nombre_articles; i++) {
        if (g->articles[i]) free(g->articles[i]);
        detruireListe(g->adjList[i]);
    }
    free(g->articles);
    free(g->adjList);
    free(g->degre_in);
}
