#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"GRAPHE.h"
#include"LISTE.h"

grapheReseau creerGraphe(int V) {
    int i;
    grapheReseau g = (grapheReseau)malloc(sizeof(*g));
    if (g == NULL) return NULL;

    g->V = V;
    g->articles = (ELEMENT*)malloc(V * sizeof(ELEMENT));
    g->adjList = (LISTE*)malloc(V * sizeof(Cellule));
    g->degre_in = (int*)malloc(V * sizeof(int));

    for (i = 0; i <V; i++) {
        g->articles[i] = ELEMENT_VIDE;
        g->adjList[i] =listeCreer();
        g->degre_in[i] = 0;
    }
    return g ;
}

grapheReseau chargerGraphe(const char *fname) {
    FILE *file = fopen(fname,"r");
    if (file == NULL) {
        printf("Impossible d'ouvrir le fichier %s\n", fname);
        return NULL;
    }

    char ligne[256];
    int V_count = 0;
    while (fgets(ligne, sizeof(ligne), file)) {
        if (ligne[0] == 'A') {
            V_count++;
        }
    }

    grapheReseau g = creerGraphe(V_count);
    if (g == NULL) {
        fclose(file);
        return NULL;
    }

    rewind(file);
    while (fgets(ligne, sizeof(ligne), file)) {
        if (ligne[0] == '#' || ligne[0] == '\n' || ligne[0] == '\r') {
            continue;
        }

        if (ligne[0] == 'A') {
            ELEMENT art = (ELEMENT)malloc(sizeof(articleStruct));
            if (art != ELEMENT_VIDE) {
                if (sscanf(ligne, "A %d \"%[^\"]\" %s %d %d %d %d %d %d",
                    &art->id, art->titre, art->source, &art->score_fiabilite,
                    &art->jour, &art->mois, &art->annee, &art->heure, &art->minute) == 9) {
                    ajouterArticle(g, art);
                }
            }
        }
        else if (ligne[0] == 'C') {
            int idSrc, idDest;
            if (sscanf(ligne, "C %d %d", &idSrc, &idDest) == 2) {
                ajouterCitation(g, idSrc, idDest);
            }
        }
    }

    fclose(file);
    return g;
}

int ajouterArticle(grapheReseau g, ELEMENT art) {
    if (g == NULL || art == ELEMENT_VIDE || art->id < 0 || art->id >= g->V) {
        return 0;
    }
    g->articles[art->id] = art;
    return 1;
}

int ajouterCitation(grapheReseau g, int idSrc, int idDest) {
    if (g == NULL || idSrc >= g->V || idDest >= g->V) return 0;


    if (inserer(&(g->adjList[idSrc]), g->articles[idDest], 1)) {
        g->degre_in[idDest]++;
        return 1;
    }
    return 0;
}

void afficherGraphe(grapheReseau g) {
    int i, j, taille;
    if (g == NULL) return;

    for (i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE) {
            printf("%s (id:%d, source:%s, score:%d)\n",
                   g->articles[i]->titre, g->articles[i]->id,
                   g->articles[i]->source, g->articles[i]->score_fiabilite);

            taille = listeTaille(g->adjList[i]);
            if (taille == 0) {
                printf("ne cite aucun article\n");
            } else {
                for (j = 1; j <= taille; j++) {
                    ELEMENT cite = recuperer(g->adjList[i], j);
                    printf("  --> %s\n", cite->titre);
                }
            }
        }
    }
}

int supprimerCitation(grapheReseau g, int idSrc, int idDest) {
    int i, taille;
    if (g == NULL) return 0;

    taille = listeTaille(g->adjList[idSrc]);
    for (i = 1; i <= taille; i++) {
        ELEMENT art = recuperer(g->adjList[idSrc], i);
        if (art->id == idDest) {
            supprimer(&(g->adjList[idSrc]), i);
            g->degre_in[idDest]--;
            return 1;
        }
    }
    return 0;
}

int supprimerArticle(grapheReseau g, int idArt) {
    int i;
    if (g == NULL || g->articles[idArt] == ELEMENT_VIDE) {
            return 0;}

    while (estVide(g->adjList[idArt]) == 0) {
        ELEMENT dest = recuperer(g->adjList[idArt], 1);
        g->degre_in[dest->id]--;
        supprimer(&(g->adjList[idArt]), 1);
    }
    for (i = 0; i < g->V; i++) {
        if (i != idArt) {
            supprimerCitation(g, i, idArt);
        }}
    free(g->articles[idArt]);
    g->articles[idArt] = ELEMENT_VIDE;
    g->degre_in[idArt] = 0;

    return 1;
}
//INTERROGATION RESEAU

void articlesCites(grapheReseau g, int idSrc) {
    if (g == NULL || idSrc < 0 || idSrc >= g->V) return;

    LISTE maListe = g->adjList[idSrc];
    int total = listeTaille(maListe);

    if (total == 0) {
        printf("L'article %d ne cite aucune personne \n", idSrc);
        return;
    }

    for (int i = 1; i <= total; i++) {
        ELEMENT art = recuperer(maListe, i);
        if (art != ELEMENT_VIDE) {
            printf(" --> %s\n", art->titre);
        }
    }
}


void articlesCitants(grapheReseau g, int idDest) {
    int i, j, nb;
    int flagGlobal = 0;

    if (g == NULL || idDest < 0 || idDest >= g->V) {
        return;
    }

    for (i = 0; i < g->V; i++) {
        LISTE lst = g->adjList[i];
        nb = listeTaille(lst);
        int dejaTrouve = 0;

        for (j = 1; j <= nb && dejaTrouve == 0; j++) {
            ELEMENT cible = recuperer(lst, j);

            if (cible != ELEMENT_VIDE && cible->id == idDest) {
                if (g->articles[i] != ELEMENT_VIDE) {
                    printf(" --> %s\n", g->articles[i]->titre);
                }
                dejaTrouve = 1;
                flagGlobal = 1;
            }
        }
    }

    if (flagGlobal == 0) {
        printf("Aucun article n est lie a cette ID %d\n", idDest);
    }
}


void sourcesOriginales(grapheReseau g) {
    if (g == NULL) return;

    printf("Sources originales :\n");
    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE && estVide(g->adjList[i])) {
            printf(" --> %s\n", g->articles[i]->titre);
        }
    }
}

void articlesIsoles(grapheReseau g) {
    if (g == NULL) return;

    printf("Articles isoles :\n");
    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE && g->degre_in[i] == 0) {
            printf(" --> %s\n", g->articles[i]->titre);
        }
    }
}

ELEMENT articlePlusCite(grapheReseau g) {
    if (g == NULL) {
        return ELEMENT_VIDE;}

    int maxCitations = -1;
    int indiceGagnant = -1;

    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE) {
            if (g->degre_in[i] > maxCitations) {
                maxCitations = g->degre_in[i];
                indiceGagnant = i;
            }
        }
    }

    if (indiceGagnant != -1) {
        return g->articles[indiceGagnant];
    }
    return ELEMENT_VIDE;
}

/* ANALYSE CHRONOLOGIQUE --- */

int comparerDates(ELEMENT art1, ELEMENT art2) {
    if(!art1 || !art2) return 0;

    if(art1->annee != art2->annee) return art1->annee - art2->annee;
    if(art1->mois != art2->mois) return art1->mois - art2->mois;
    if(art1->jour != art2->jour) return art1->jour - art2->jour;
    if(art1->heure != art2->heure) return art1->heure - art2->heure;

    return art1->minute - art2->minute;
}

void trierParDate(grapheReseau g) {
    if(g == NULL) return;

    int i, j, pos, nb = 0;
    ELEMENT *t = (ELEMENT*)malloc(g->V * sizeof(ELEMENT));

    for(i=0; i<g->V; i++) {
        if(g->articles[i] != ELEMENT_VIDE) t[nb++] = g->articles[i];
    }

    for(i=0; i < nb-1; i++) {
        pos = i;
        for(j=i+1; j < nb; j++) {
            if(comparerDates(t[j], t[pos]) < 0) pos = j;
        }
        ELEMENT tmp = t[pos];
        t[pos] = t[i];
        t[i] = tmp;
    }

    for(i=0; i<nb; i++) {
        printf("%d. %s (%02d/%02d/%d %02dh%02d)\n", i+1, t[i]->titre,
               t[i]->jour, t[i]->mois, t[i]->annee, t[i]->heure, t[i]->minute);
    }
    free(t);
}

void premierCitant(grapheReseau g, int idDest) {
    if(g == NULL || idDest < 0 || idDest >= g->V) return;

    ELEMENT prem = ELEMENT_VIDE;
    int i, j, sz;

    for(i=0; i < g->V; i++) {
        LISTE l = g->adjList[i];
        sz = listeTaille(l);
        int trouve = 0;

        for(j=1; j <= sz && trouve == 0; j++) {
            ELEMENT e = recuperer(l, j);
            if(e != ELEMENT_VIDE && e->id == idDest) {
                if(prem == ELEMENT_VIDE || comparerDates(g->articles[i], prem) < 0) {
                    prem = g->articles[i];
                }
                trouve = 1;
            }
        }
    }

    if(prem != ELEMENT_VIDE) {
        printf("Premier citant: %s (%02d/%02d/%d)\n", prem->titre,
               prem->jour, prem->mois, prem->annee);
    }
}

void chainePropagation(grapheReseau g, int idSrc) {
    if(!g || idSrc < 0 || idSrc >= g->V) return;

    int i, j, total = 0;
    int *estDansChaine = (int*)calloc(g->V, sizeof(int));
    ELEMENT *triee = (ELEMENT*)malloc(g->V * sizeof(ELEMENT));

    for(i=0; i < g->V; i++) {
        if(g->articles[i]) triee[total++] = g->articles[i];
    }

    for(i=0; i < total-1; i++) {
        int m = i;
        for(j=i+1; j < total; j++) {
            if(comparerDates(triee[j], triee[m]) < 0) m = j;
        }
        ELEMENT s = triee[m]; triee[m] = triee[i]; triee[i] = s;
    }

    printf("Chaine de propagation depuis: %s\n", g->articles[idSrc]->titre);
    estDansChaine[idSrc] = 1;

    for(i=0; i < total; i++) {
        int idCurent = triee[i]->id;

        if(estDansChaine[idCurent] == 0) {
            LISTE adj = g->adjList[idCurent];
            int n = listeTaille(adj);
            int citeUnMembre = 0;

            for(j=1; j <= n && citeUnMembre == 0; j++) {
                ELEMENT cible = recuperer(adj, j);
                if(cible && estDansChaine[cible->id]) {
                    printf(" --> %s (%02d/%02d/%d)\n", triee[i]->titre,
                           triee[i]->jour, triee[i]->mois, triee[i]->annee);
                    estDansChaine[idCurent] = 1;
                    citeUnMembre = 1;
                }
            }
        }
    }

    free(estDansChaine);
    free(triee);
}

/*SIMULATION DE PROPAGATION (BFS) */

void simulationPropagation(grapheReseau g, int idDepart) {
    if (g == NULL || idDepart < 0 || idDepart >= g->V || g->articles[idDepart] == ELEMENT_VIDE) {
        printf("Article de depart invalide\n");
        return;
    }


    int *visite = (int*)calloc(g->V, sizeof(int));
    int *file = (int*)malloc(g->V * sizeof(int));
    int tete = 0, queue = 0;

    visite[idDepart] = 1;
    file[queue++] = idDepart;

    printf("Simulation de propagation depuis : %s\n", g->articles[idDepart]->titre);
    int niveau = 0;

    while (tete < queue) {
        int idActuel = file[tete++];

        for (int i = 0; i < g->V; i++) {
            if (g->articles[i] != ELEMENT_VIDE && visite[i] == 0) {

                LISTE l = g->adjList[i];
                int n = listeTaille(l);
                int estCitant = 0;

                for (int j = 1; j <= n && estCitant == 0; j++) {
                    ELEMENT cible = recuperer(l, j);
                    if (cible != ELEMENT_VIDE && cible->id == idActuel) {
                        estCitant = 1;
                    }
                }

                if (estCitant == 1) {
                    visite[i] = 1;
                    file[queue++] = i;
                    printf(" >>Touche : %s (ID: %d)\n", g->articles[i]->titre, i);
                }
            }
        }
    }

    if (queue == 1) {
        printf("L'information ne s'est pas propagee au-dela de la source.\n");
    } else {
        printf("Total d'articles touches: %d\n", queue);
    }

    free(visite);
    free(file);
}

void detruireGraphe(grapheReseau g) {
    int i;
    if (g == NULL) {
            return;}

    for (i = 0; i < g->V; i++) {
        listeDetruire(g->adjList[i]);
        if (g->articles[i] != ELEMENT_VIDE){
            free(g->articles[i]);
        }}
    free(g->adjList);
    free(g->articles);
    free(g->degre_in);
    free(g);
}


