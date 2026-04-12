#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GRAPHE.h"
grapheReseau creerGraphe(int V) {
    //hatit citan en commentaire le temp leprofa tjewbni
    grapheReseau g = malloc(sizeof(structuregraphe));
    g->V        = V;
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
    char line[256];// afin de rendre qui est ecrit on fichier en char pour que le compilateur peut les lire

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
            sscanf(line, "C %d %d", &src, &dest);//sscanf is like scanf but reads from a string instead of keyboard:
            ajouterCitation(g, src, dest);
        }
    }

    fclose(f);
    return g;
}
void supprimerGraphe(grapheReseau g) {
    // afin de supp quelque chose d un structure il faut commencé du petit ensemble jusque le grand kima lexpl lena
     (!g) return;

    for (int i = 0; i < g->V; i++) {
        //  free chaque article
        if (g->articles[i])
            free(g->articles[i]);

        //  free les noeuds de adjList[i] puis la liste
        if (g->adjList[i])
            listeSupprimer(g->adjList[i]);

        // free les noeuds de citants[i] puis la liste
        if (g->citants[i])
            listeSupprimer(g->citants[i]);
    }

    //  free les liste bidhom
    free(g->articles);
    free(g->adjList);
    free(g->citants);
    free(g->degre_in);

    //  free le graphe
    free(g);
}
