#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GRAPHE.h"

/* |--------------------------------------------------------|
   |              fonction qui vont nous aider              |
   |--------------------------------------------------------|
*/

// retourne 1 si id trouver dans la liste des elem articles
int recherche_id(grapheReseau g, int id) {
    int test = 1;
    if (id < 0 || id >= g->V) test = 0;      // hors limite
    if (g->articles[id] == NULL) test = 0;    // case vide
    return test;
}

// recherche la position ou inserer art dans L selon la date (tri croissant)
int recherche_pos_LS_chaine(LISTE L, ELEMENT art) {
    int pos = 1;
    NOEUD p = L->tete;
    // parcourir jusqu a trouver un elem dont la date est superieure a art
    while (p != NULL) {
        ELEMENT current = p->info;
        // comparer annee puis mois puis jour puis heure puis minute
        if (art->annee > current->annee) { p = p->suivant; pos++; }
        else if (art->annee == current->annee && art->mois > current->mois) { p = p->suivant; pos++; }
        else if (art->annee == current->annee && art->mois == current->mois && art->jour > current->jour) { p = p->suivant; pos++; }
        else if (art->annee == current->annee && art->mois == current->mois && art->jour == current->jour && art->heure > current->heure) { p = p->suivant; pos++; }
        else if (art->annee == current->annee && art->mois == current->mois && art->jour == current->jour && art->heure == current->heure && art->minute > current->minute) { p = p->suivant; pos++; }
        else break; // on a trouve la bonne position
    }
    return pos;
}

grapheReseau creerGraphe(int V) {
    // hatit citants en commentaire le temp le profa tjewbni
    grapheReseau g = malloc(sizeof(structuregraphe));
    g->V        = V;
    g->lg       = 0;  // nombre d articles actuellement dans le graphe
    g->articles  = malloc(V * sizeof(ELEMENT));
    g->adjList   = malloc(V * sizeof(LISTE));
    // g->citants   = malloc(V * sizeof(LISTE));
    g->degre_in  = malloc(V * sizeof(int));

    for (int i = 0; i < V; i++) {
        g->articles[i]  = NULL;
        g->adjList[i]   = listeCreer();
        // g->citants[i]   = listeCreer();
        g->degre_in[i]  = 0;
    }
    return g;
}

grapheReseau chargerGraphe(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        printf("Erreur ouverture fichier\n");
        return NULL;
    }
    grapheReseau g = creerGraphe(MAX);
    // afin de rendre ce qui est ecrit dans le fichier en char pour que le compilateur peut les lire
    char line[256];

    while (fgets(line, sizeof(line), f)) {
        // ignorer commentaires et lignes vides
        if (line[0] == '#' || line[0] == '\n') continue;

        if (line[0] == 'A') {
            ELEMENT e = malloc(sizeof(articleStruct));
            sscanf(line, "A %d %s %s %d %d %d %d %d %d",
                &e->id,
                e->titre,
                e->source,
                &e->score_fiabilite,
                &e->jour,
                &e->mois,
                &e->annee,
                &e->heure,
                &e->minute
            );
            ajouterArticle(g, e);

        } else if (line[0] == 'C') {
            int src, dest;
            // sscanf is like scanf but reads from a string instead of keyboard
            sscanf(line, "C %d %d", &src, &dest);
            ajouterCitation(g, src, dest);
        }
    }
    fclose(f);
    return g;
}

void supprimerGraphe(grapheReseau g) {
    // afin de supprimer quelque chose d une structure il faut commencer du petit ensemble jusqu au grand
    for (int i = 0; i < g->V; i++) {
        // free chaque article
        if (g->articles[i])
            free(g->articles[i]);
        // free les noeuds de adjList[i] puis la liste
        if (g->adjList[i])
            listeDetruire(g->adjList[i]);
        // free les noeuds de citants[i] puis la liste
        // if (g->citants[i])
        //     listeDetruire(g->citants[i]);
    }
    // free les tableaux
    free(g->articles);
    free(g->adjList);
    // free(g->citants);
    free(g->degre_in);
    // free le graphe
    free(g);
}

int ajouterArticle(grapheReseau g, ELEMENT art) {
    int succes = 0;
    if (g != NULL && art != NULL) {
        // lg est le nombre actuel, V est la capacite max
        if (g->lg < g->V && recherche_id(g, art->id) == 0) {
            g->articles[art->id] = art;  // index direct par id
            g->lg++;
            succes = 1;
        }
    }
    return succes;
}
int supprimerArticle(grapheReseau g, int idArt) {
    int succes = 0;
    if (g != NULL && recherche_id(g, idArt)) {

        // 1. supprimer toutes les citations sortantes de idArt
        listeDetruire(g->adjList[idArt]);
        g->adjList[idArt] = listeCreer();  // reinitialiser vide

        // 2. supprimer idArt de toutes les adjLists des autres
        for (int i = 0; i < g->V; i++) {
            if (g->articles[i] != NULL && i != idArt)
                supprimerCitation(g, i, idArt);  // plus propre !
        }

        // 3. free l article
        free(g->articles[idArt]);
        g->articles[idArt] = NULL;
        g->degre_in[idArt] = 0;
        g->lg--;
        succes = 1;
    }
    return succes;
}

int ajouterCitation(grapheReseau g, int idSrc, int idDest) {
    int succes = 0;
    if (g != NULL && recherche_id(g, idSrc) && recherche_id(g, idDest)) {
        // selon la date car la liste doit etre triee
        int pos = recherche_pos_LS_chaine(g->adjList[idSrc], g->articles[idDest]);
        inserer(g->adjList[idSrc], g->articles[idDest], pos);
        g->degre_in[idDest]++;
        succes = 1;
    }
    return succes;
}
int supprimerCitation(grapheReseau g, int idSrc, int idDest) {
    int succes = 0;
    if (g != NULL && recherche_id(g, idSrc) && recherche_id(g, idDest)) {
        int pos = 1;
        int trouve = 0;
        // aller dans adjList[idSrc] et chercher idDest
        while (pos <= listeTaille(g->adjList[idSrc]) && !trouve) {
            ELEMENT e = recuperer(g->adjList[idSrc], pos);
            if (e->id == idDest) {
                supprimer(g->adjList[idSrc], pos);  // supprimer de adjList
                g->degre_in[idDest]--;              // mettre a jour degre_in
                // supprimer idSrc de citants[idDest] aussi
                // int pos2 = 1;
                // int trouve2 = 0;
                // while (pos2 <= listeTaille(g->citants[idDest]) && !trouve2) {
                //     ELEMENT e2 = recuperer(g->citants[idDest], pos2);
                //     if (e2->id == idSrc) {
                //         supprimer(g->citants[idDest], pos2);
                //         trouve2 = 1;
                //     }
                //     pos2++;
                // }
                succes = 1;
                trouve = 1;  // remplace break
            }
            pos++;
        }
    }
    return succes;
}
void afficherGraphe(grapheReseau g) {
    if (g == NULL) {
        printf("Graphe vide\n");
        return;
    }
    printf("=== GRAPHE : %d articles ===\n", g->lg);
    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != NULL) {
            // afficher l article
            printf("\n[%d] %s | %s | fiabilite:%d | %02d/%02d/%d %02d:%02d",
                g->articles[i]->id,
                g->articles[i]->titre,
                g->articles[i]->source,
                g->articles[i]->score_fiabilite,
                g->articles[i]->jour,
                g->articles[i]->mois,
                g->articles[i]->annee,
                g->articles[i]->heure,
                g->articles[i]->minute
            );
            // afficher ses citations (adjList)
            printf("\n  cite        (%d) : ", listeTaille(g->adjList[i]));
            listeAfficher(g->adjList[i]);

            // afficher son degre entrant
            printf("\n  cite par    (%d articles)", g->degre_in[i]);

            // afficher citants quand le prof repond
            // printf("\n  cite par    (%d) : ", listeTaille(g->citants[i]));
            // listeAfficher(g->citants[i]);
        }
    }
    printf("\n==============================\n");
}
