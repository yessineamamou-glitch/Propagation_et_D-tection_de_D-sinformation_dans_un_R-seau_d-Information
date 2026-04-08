#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GRAPHE.h"

grapheReseau createGraph(int V) {
    grapheReseau g;
    g.nombre_articles = V;
    g.articles = (ELEMENT *)malloc(V * sizeof(ELEMENT));
    g.adjacence = (LISTE *)malloc(V * sizeof(LISTE));

    for (int i = 0; i < V; i++) {
        g.articles[i] = NULL;
        g.adjacence[i] = listeCreer();
    }
    return g;
}

void chargerGraph(grapheReseau *g, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) return;

    char ligne[256];
    while (fgets(ligne, sizeof(ligne), file)) {
        if (ligne[0] == 'A') {
            int id, score, j, m, a, h, mi;
            char t[100], s[50];
            sscanf(ligne, "A %d %s %s %d %d %d %d %d %d", &id, t, s, &score, &j, &m, &a, &h, &mi);

            ELEMENT e = elementCreer();
            e->id = id; strcpy(e->titre, t); strcpy(e->source, s);
            e->score_fiabilite = score; e->jour = j; e->mois = m;
            e->annee = a; e->heure = h; e->minute = mi;

            if (id < g->nombre_articles) g->articles[id] = e;
        }
        else if (ligne[0] == 'C') {
            int src, dest;
            sscanf(ligne, "C %d %d", &src, &dest);
            if (src < g->nombre_articles) {
                ELEMENT eDest = elementCreer();
                eDest->id = dest;
                inserer(g->adjacence[src], eDest, listeTaille(g->adjacence[src]) + 1);
            }
        }
    }
    fclose(file);
}

void detruireGraph(grapheReseau *g) {
    for (int i = 0; i < g->nombre_articles; i++) {
        listeDetruire(g->adjacence[i]);
        if (g->articles[i]) elementDetruire(g->articles[i]);
    }
    free(g->articles);
    free(g->adjacence);
}

void ajouterArticle(grapheReseau *g, ELEMENT art) {
    if (art->id < g->nombre_articles) {
        g->articles[art->id] = elementCopier(art);
        sauvegarderGraphe(g, "fichier.txt");
    }
}

void supprimerArticle(grapheReseau *g, int idArt) {
    for (int i = 0; i < g->nombre_articles; i++) {
        for (int j = 1; j <= listeTaille(g->adjacence[i]); j++) {
            ELEMENT e = recuperer(g->adjacence[i], j);
            if (e && e->id == idArt) {
                supprimer(g->adjacence[i], j);
                break;
            }
        }
    }
    listeDetruire(g->adjacence[idArt]);
    g->adjacence[idArt] = listeCreer();
    if (g->articles[idArt]) {
        elementDetruire(g->articles[idArt]);
        g->articles[idArt] = NULL;
    }
    sauvegarderGraphe(g, "fichier.txt");
}

void ajouterCitation(grapheReseau *g, int idSrc, int idDest) {
    ELEMENT e = elementCreer();
    e->id = idDest;
    inserer(g->adjacence[idSrc], e, listeTaille(g->adjacence[idSrc]) + 1);
    sauvegarderGraphe(g, "fichier.txt");
}

void supprimerCitation(grapheReseau *g, int idSrc, int idDest) {
    for (int i = 1; i <= listeTaille(g->adjacence[idSrc]); i++) {
        ELEMENT e = recuperer(g->adjacence[idSrc], i);
        if (e && e->id == idDest) {
            supprimer(g->adjacence[idSrc], i);
            break;
        }
    }
    sauvegarderGraphe(g, "fichier.txt");
}

void afficherGraphe(grapheReseau *g) {
    for (int i = 0; i < g->nombre_articles; i++) {
        if (g->articles[i]) {
            elementAfficher(g->articles[i]);
            printf("  Citations: ");
            LISTE L = g->adjacence[i];
            NOEUD curr = L->tete;
            while (curr) {
                printf("%d ", curr->info->id);
                curr = curr->suivant;
            }
            printf("\n");
        }
    }
}

void articlesCites(grapheReseau *g, int idSrc) {
    LISTE L = g->adjacence[idSrc];
    NOEUD curr = L->tete;
    while (curr) {
        if (g->articles[curr->info->id]) elementAfficher(g->articles[curr->info->id]);
        curr = curr->suivant;
    }
}

void articlesCitants(grapheReseau *g, int idDest) {
    for (int i = 0; i < g->nombre_articles; i++) {
        NOEUD curr = g->adjacence[i]->tete;
        while (curr) {
            if (curr->info->id == idDest) {
                elementAfficher(g->articles[i]);
                break;
            }
            curr = curr->suivant;
        }
    }
}

void sourcesOriginales(grapheReseau *g) {
    for (int i = 0; i < g->nombre_articles; i++) {
        if (!g->articles[i]) continue;
        int cite = 0;
        for (int j = 0; j < g->nombre_articles; j++) {
            NOEUD curr = g->adjacence[j]->tete;
            while (curr) {
                if (curr->info->id == i) { cite = 1; break; }
                curr = curr->suivant;
            }
            if (cite) break;
        }
        if (!cite) elementAfficher(g->articles[i]);
    }
}

void articlesIsoles(grapheReseau *g) {
    for (int i = 0; i < g->nombre_articles; i++) {
        if (!g->articles[i]) continue;
        if (listeTaille(g->adjacence[i]) == 0) {
            int cite = 0;
            for (int j = 0; j < g->nombre_articles; j++) {
                NOEUD curr = g->adjacence[j]->tete;
                while (curr) {
                    if (curr->info->id == i) { cite = 1; break; }
                    curr = curr->suivant;
                }
            }
            if (!cite) elementAfficher(g->articles[i]);
        }
    }
}

ELEMENT articlePlusCite(grapheReseau *g) {
    int max = -1, idMax = -1;
    for (int i = 0; i < g->nombre_articles; i++) {
        if (!g->articles[i]) continue;
        int count = 0;
        for (int j = 0; j < g->nombre_articles; j++) {
            NOEUD curr = g->adjacence[j]->tete;
            while (curr) {
                if (curr->info->id == i) count++;
                curr = curr->suivant;
            }
        }
        if (count > max) { max = count; idMax = i; }
    }
    return (idMax != -1) ? elementCopier(g->articles[idMax]) : NULL;
}

void trierParDate(grapheReseau *g) {
    for (int i = 0; i < g->nombre_articles - 1; i++) {
        for (int j = 0; j < g->nombre_articles - i - 1; j++) {
            if (g->articles[j] && g->articles[j+1]) {
                if (g->articles[j]->annee > g->articles[j+1]->annee) {
                    ELEMENT tmp = g->articles[j];
                    g->articles[j] = g->articles[j+1];
                    g->articles[j+1] = tmp;
                }
            }
        }
    }
}

void simulerPropagation(grapheReseau *g, int idSrc) {
    int *visite = calloc(g->nombre_articles, sizeof(int));
    int *file = malloc(g->nombre_articles * sizeof(int));
    int d = 0, f = 0;

    file[f++] = idSrc;
    visite[idSrc] = 1;
    while (d < f) {
        int c = file[d++];
        if (c != idSrc) elementAfficher(g->articles[c]);
        NOEUD curr = g->adjacence[c]->tete;
        while (curr) {
            if (!visite[curr->info->id]) {
                visite[curr->info->id] = 1;
                file[f++] = curr->info->id;
            }
            curr = curr->suivant;
        }
    }
    free(visite); free(file);
}

void analyserReseau(grapheReseau *g) {
    char *mots[] = {"cache", "urgent", "complot", "choc"};
    for (int i = 0; i < g->nombre_articles; i++) {
        if (!g->articles[i]) continue;
        int suspicion = 0;
        for (int j = 0; j < 4; j++) if (strstr(g->articles[i]->titre, mots[j])) suspicion += 20;
        if (g->articles[i]->score_fiabilite < 50) suspicion += 30;
        if (suspicion >= 40) printf("SUSPECT: %s (ID: %d)\n", g->articles[i]->titre, i);
    }
}

void sauvegarderGraphe(grapheReseau *g, const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return;
    for (int i = 0; i < g->nombre_articles; i++) {
        if (g->articles[i])
            fprintf(f, "A %d %s %s %d %d %d %d %d %d\n", g->articles[i]->id, g->articles[i]->titre, g->articles[i]->source, g->articles[i]->score_fiabilite, g->articles[i]->jour, g->articles[i]->mois, g->articles[i]->annee, g->articles[i]->heure, g->articles[i]->minute);
    }
    for (int i = 0; i < g->nombre_articles; i++) {
        NOEUD curr = g->adjacence[i]->tete;
        while (curr) {
            fprintf(f, "C %d %d\n", i, curr->info->id);
            curr = curr->suivant;
        }
    }
    fclose(f);
}
