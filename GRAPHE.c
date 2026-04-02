#include "GRAPHE.h"
#include "FAKESDB.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========== GESTION DU GRAPHE ========== */

grapheReseau creerGraphe(int V) {
    grapheReseau g = (grapheReseau)malloc(sizeof(struct {
        int V;
        ELEMENT *articles;
        LISTE *adjList;
        int *degre_in;
    }));

    if (g == NULL) {
        fprintf(stderr, "Erreur : allocation memoire echouee\n");
        return NULL;
    }

    g->V = V;
    g->articles = (ELEMENT*)calloc(V, sizeof(ELEMENT));
    g->adjList = (LISTE*)calloc(V, sizeof(LISTE));
    g->degre_in = (int*)calloc(V, sizeof(int));

    if (g->articles == NULL || g->adjList == NULL || g->degre_in == NULL) {
        fprintf(stderr, "Erreur : allocation memoire echoueee\n");
        free(g);
        return NULL;
    }

    for (int i = 0; i < V; i++) {
        g->articles[i] = ELEMENT_VIDE;
        g->adjList[i] = listeCreer();
        g->degre_in[i] = 0;
    }

    return g;
}

grapheReseau chargerGraphe(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "Erreur : impossible d'ouvrir le fichier %s\n", filename);
        return NULL;
    }

    int nbArticles = 0;
    char ligne[512];
    rewind(f);

    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        if (ligne[0] == 'A') nbArticles++;
    }

    if (nbArticles == 0) {
        fprintf(stderr, "Erreur : aucun article dans le fichier\n");
        fclose(f);
        return NULL;
    }

    grapheReseau g = creerGraphe(nbArticles);
    if (g == NULL) {
        fclose(f);
        return NULL;
    }

    rewind(f);
    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        if (ligne[0] == 'A') {
            int id, score, jour, mois, annee, heure, minute;
            char titre[100], source[50];

            sscanf(ligne, "A %d \"%99[^\"]\" %49s %d %d %d %d %d %d",
                   &id, titre, source, &score, &jour, &mois, &annee, &heure, &minute);

            ELEMENT art = creerArticle(id, titre, source, score, jour, mois, annee, heure, minute);
            if (art != ELEMENT_VIDE) {
                g->articles[id] = art;
            }
        }
    }

    rewind(f);
    while (fgets(ligne, sizeof(ligne), f) != NULL) {
        if (ligne[0] == 'C') {
            int src, dest;
            sscanf(ligne, "C %d %d", &src, &dest);
            ajouterCitation(g, src, dest);
        }
    }

    fclose(f);
    return g;
}

void detruireGraphe(grapheReseau g) {
    if (g == NULL) return;

    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE) {
            detruireArticle(g->articles[i]);
        }
        listeDetruire(&g->adjList[i]);
    }

    free(g->articles);
    free(g->adjList);
    free(g->degre_in);
    free(g);
}

int ajouterArticle(grapheReseau g, ELEMENT art) {
    if (g == NULL || art == ELEMENT_VIDE) return 0;
    if (art->id < 0 || art->id >= g->V) return 0;

    if (g->articles[art->id] != ELEMENT_VIDE) {
        return 0;
    }

    g->articles[art->id] = copierArticle(art);
    return 1;
}

int supprimerArticle(grapheReseau g, int idArt) {
    if (g == NULL || idArt < 0 || idArt >= g->V) return 0;
    if (g->articles[idArt] == ELEMENT_VIDE) return 0;

    listeDetruire(&g->adjList[idArt]);
    g->adjList[idArt] = listeCreer();

    for (int i = 0; i < g->V; i++) {
        if (i != idArt && g->articles[i] != ELEMENT_VIDE) {
            int pos = rechercher(g->adjList[i], g->articles[idArt]);
            if (pos != 0) {
                supprimer(&g->adjList[i], pos);
                g->degre_in[idArt]--;
            }
        }
    }

    detruireArticle(g->articles[idArt]);
    g->articles[idArt] = ELEMENT_VIDE;
    g->degre_in[idArt] = 0;

    return 1;
}

int ajouterCitation(grapheReseau g, int idSrc, int idDest) {
    if (g == NULL || idSrc < 0 || idSrc >= g->V || idDest < 0 || idDest >= g->V)
        return 0;

    if (g->articles[idSrc] == ELEMENT_VIDE || g->articles[idDest] == ELEMENT_VIDE)
        return 0;

    if (rechercher(g->adjList[idSrc], g->articles[idDest]) != 0)
        return 0;

    inserer(&g->adjList[idSrc], g->articles[idDest], listeTaille(g->adjList[idSrc]) + 1);
    g->degre_in[idDest]++;

    return 1;
}

int supprimerCitation(grapheReseau g, int idSrc, int idDest) {
    if (g == NULL || idSrc < 0 || idSrc >= g->V || idDest < 0 || idDest >= g->V)
        return 0;

    int pos = rechercher(g->adjList[idSrc], g->articles[idDest]);
    if (pos == 0) return 0;

    supprimer(&g->adjList[idSrc], pos);
    g->degre_in[idDest]--;

    return 1;
}

void afficherGraphe(grapheReseau g) {
    if (g == NULL) return;

    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE) {
            afficherArticle(g->articles[i]);

            int taille = listeTaille(g->adjList[i]);
            if (taille == 0) {
                printf("(ne cite aucun article)\n");
            } else {
                afficherListeCompacte(g->adjList[i], 1);
            }
            printf("\n");
        }
    }
}

/* ========== INTERROGATION DU RESEAU ========== */

void articlesCites(grapheReseau g, int idSrc) {
    if (g == NULL || idSrc < 0 || idSrc >= g->V) return;
    if (g->articles[idSrc] == ELEMENT_VIDE) {
        printf("Article non trouve\n");
        return;
    }

    printf("Articles cites par %s :\n", g->articles[idSrc]->titre);

    int taille = listeTaille(g->adjList[idSrc]);
    if (taille == 0) {
        printf("(aucun article cite)\n");
    } else {
        afficherListeCompacte(g->adjList[idSrc], 1);
    }
}

void articlesCitants(grapheReseau g, int idDest) {
    if (g == NULL || idDest < 0 || idDest >= g->V) return;
    if (g->articles[idDest] == ELEMENT_VIDE) {
        printf("Article non trouve\n");
        return;
    }

    printf("Articles qui citent %s :\n", g->articles[idDest]->titre);

    int trouve = 0;
    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE && i != idDest) {
            if (rechercher(g->adjList[i], g->articles[idDest]) != 0) {
                printf("--> ");
                afficherArticleSimple(g->articles[i]);
                printf("\n");
                trouve = 1;
            }
        }
    }

    if (!trouve) {
        printf("(aucun article citant)\n");
    }
}

void sourcesOriginales(grapheReseau g) {
    if (g == NULL) return;

    printf("Sources originales (articles ne citant aucun autre) :\n");

    int trouve = 0;
    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE && listeTaille(g->adjList[i]) == 0) {
            printf("--> ");
            afficherArticleSimple(g->articles[i]);
            printf(" (ne cite personne)\n");
            trouve = 1;
        }
    }

    if (!trouve) {
        printf("(aucune source originale)\n");
    }
}

void articlesIsoles(grapheReseau g) {
    if (g == NULL) return;

    printf("Articles isoles (non cites par personne) :\n");

    int trouve = 0;
    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE && g->degre_in[i] == 0) {
            printf("--> ");
            afficherArticleSimple(g->articles[i]);
            printf(" (non cite)\n");
            trouve = 1;
        }
    }

    if (!trouve) {
        printf("(aucun article isole)\n");
    }
}

ELEMENT articlePlusCite(grapheReseau g) {
    if (g == NULL) return ELEMENT_VIDE;

    int maxDegre = -1;
    ELEMENT plusCite = ELEMENT_VIDE;

    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE && g->degre_in[i] > maxDegre) {
            maxDegre = g->degre_in[i];
            plusCite = g->articles[i];
        }
    }

    return plusCite;
}

/* ========== ANALYSE CHRONOLOGIQUE ========== */

int comparerDates(ELEMENT art1, ELEMENT art2) {
    if (art1->annee != art2->annee)
        return art1->annee - art2->annee;
    if (art1->mois != art2->mois)
        return art1->mois - art2->mois;
    if (art1->jour != art2->jour)
        return art1->jour - art2->jour;
    if (art1->heure != art2->heure)
        return art1->heure - art2->heure;
    return art1->minute - art2->minute;
}

void trierParDate(grapheReseau g) {
    if (g == NULL) return;

    ELEMENT *articles = (ELEMENT*)malloc(g->V * sizeof(ELEMENT));
    int nbArt = 0;

    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE) {
            articles[nbArt++] = g->articles[i];
        }
    }

    for (int i = 1; i < nbArt; i++) {
        ELEMENT cle = articles[i];
        int j = i - 1;
        while (j >= 0 && comparerDates(articles[j], cle) > 0) {
            articles[j + 1] = articles[j];
            j--;
        }
        articles[j + 1] = cle;
    }

    printf("Articles tries par date de publication :\n");
    for (int i = 0; i < nbArt; i++) {
        printf("%d. ", i + 1);
        afficherArticleSimple(articles[i]);
        printf(" (%02d/%02d/%d %02dh%02d)\n",
               articles[i]->jour, articles[i]->mois, articles[i]->annee,
               articles[i]->heure, articles[i]->minute);
    }

    free(articles);
}

void premierCitant(grapheReseau g, int idDest) {
    if (g == NULL || idDest < 0 || idDest >= g->V) return;
    if (g->articles[idDest] == ELEMENT_VIDE) {
        printf("Article non trouve\n");
        return;
    }

    ELEMENT earliestCitant = ELEMENT_VIDE;

    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE && i != idDest) {
            if (rechercher(g->adjList[i], g->articles[idDest]) != 0) {
                if (earliestCitant == ELEMENT_VIDE ||
                    comparerDates(g->articles[i], earliestCitant) < 0) {
                    earliestCitant = g->articles[i];
                }
            }
        }
    }

    if (earliestCitant != ELEMENT_VIDE) {
        printf("Premier article citant %s :\n", g->articles[idDest]->titre);
        printf("--> ");
        afficherArticleSimple(earliestCitant);
        printf(" (%02d/%02d/%d %02dh%02d)\n",
               earliestCitant->jour, earliestCitant->mois, earliestCitant->annee,
               earliestCitant->heure, earliestCitant->minute);
    } else {
        printf("Aucun article citant %s\n", g->articles[idDest]->titre);
    }
}

void chainePropagation(grapheReseau g, int idSrc) {
    if (g == NULL || idSrc < 0 || idSrc >= g->V) return;
    if (g->articles[idSrc] == ELEMENT_VIDE) {
        printf("Article non trouve\n");
        return;
    }

    printf("Chaine chronologique de propagation depuis %s :\n",
           g->articles[idSrc]->titre);

    int *visited = (int*)calloc(g->V, sizeof(int));
    ELEMENT *ordonnance = (ELEMENT*)malloc(g->V * sizeof(ELEMENT));
    int nbAccessibles = 0;

    LISTE file = listeCreer();
    inserer(&file, g->articles[idSrc], 1);
    visited[idSrc] = 1;
    ordonnance[nbAccessibles++] = g->articles[idSrc];

    int pos = 1;
    while (pos <= listeTaille(file)) {
        ELEMENT courant = recuperer(file, pos);

        for (int i = 0; i < g->V; i++) {
            if (g->articles[i] != ELEMENT_VIDE && !visited[i]) {
                if (rechercher(g->adjList[i], courant) != 0) {
                    visited[i] = 1;
                    inserer(&file, g->articles[i], listeTaille(file) + 1);
                    ordonnance[nbAccessibles++] = g->articles[i];
                }
            }
        }
        pos++;
    }

    for (int i = 1; i < nbAccessibles; i++) {
        ELEMENT cle = ordonnance[i];
        int j = i - 1;
        while (j >= 0 && comparerDates(ordonnance[j], cle) > 0) {
            ordonnance[j + 1] = ordonnance[j];
            j--;
        }
        ordonnance[j + 1] = cle;
    }

    for (int i = 0; i < nbAccessibles; i++) {
        if (i > 0) printf("--> cite par ");
        afficherArticleSimple(ordonnance[i]);
        printf(" (%02d/%02d/%d %02dh%02d)\n",ordonnance[i]->jour, ordonnance[i]->mois, ordonnance[i]->annee,ordonnance[i]->heure, ordonnance[i]->minute);
    }

    listeDetruire(&file);
    free(visited);
    free(ordonnance);
}

/* ========== SIMULATION DE PROPAGATION - BFS ========== */

void simulerPropagation(grapheReseau g, int idSrc) {
    if (g == NULL || idSrc < 0 || idSrc >= g->V) return;
    if (g->articles[idSrc] == ELEMENT_VIDE) {
        printf("Article non trouve\n");
        return;
    }

    int *visited = (int*)calloc(g->V, sizeof(int));
    LISTE file = listeCreer();

    inserer(&file, g->articles[idSrc], 1);
    visited[idSrc] = 1;

    printf("Simulation de propagation depuis %s :\n", g->articles[idSrc]->titre);

    int niveau = 0;
    int pos = 1;

    while (pos <= listeTaille(file)) {
        ELEMENT courant = recuperer(file, pos);

        printf("Niveau %d : ", niveau);
        printf("%s", courant->titre);

        for (int i = 0; i < g->V; i++) {
            if (g->articles[i] != ELEMENT_VIDE && !visited[i]) {
                if (rechercher(g->adjList[i], courant) != 0) {
                    visited[i] = 1;
                    inserer(&file, g->articles[i], listeTaille(file) + 1);
                    printf(", %s", g->articles[i]->titre);
                }
            }
        }
        printf("\n");

        niveau++;
        pos++;
    }

    printf("\n%d niveaux, ", niveau);
    int total = 0;
    for (int i = 0; i < g->V; i++) {
        if (visited[i]) total++;
    }
    printf("%d articles atteints.\n", total);

    listeDetruire(&file);
    free(visited);
}

void articlesAccessibles(grapheReseau g, int idSrc) {
    if (g == NULL || idSrc < 0 || idSrc >= g->V) return;
    if (g->articles[idSrc] == ELEMENT_VIDE) {
        printf("Article non trouve\n");
        return;
    }

    printf("Articles accessibles depuis %s :\n", g->articles[idSrc]->titre);

    int *visited = (int*)calloc(g->V, sizeof(int));
    LISTE file = listeCreer();

    inserer(&file, g->articles[idSrc], 1);
    visited[idSrc] = 1;

    int pos = 1;
    while (pos <= listeTaille(file)) {
        ELEMENT courant = recuperer(file, pos);

        for (int i = 0; i < g->V; i++) {
            if (g->articles[i] != ELEMENT_VIDE && !visited[i]) {
                if (rechercher(g->adjList[i], courant) != 0) {
                    visited[i] = 1;
                    inserer(&file, g->articles[i], listeTaille(file) + 1);
                }
            }
        }
        pos++;
    }

    for (int i = 0; i < g->V; i++) {
        if (visited[i]) {
            afficherArticleSimple(g->articles[i]);
            printf("\n");
        }
    }

    listeDetruire(&file);
    free(visited);
}

/* ========== DETECTION DE FAKE NEWS ========== */

int analyserArticle(ELEMENT art) {
    if (art == ELEMENT_VIDE) return 0;

    int score_suspicion = 0;
    char titre_lower[100], fragment_lower[100];

    for (int i = 0; i < NB_FAKES; i++) {
        if (strstrCase(art->titre, BASE_FAKES[i])) {
            score_suspicion += 40;
            break;
        }
    }

    for (int i = 0; i < NB_SUSPECTS; i++) {
        if (strstrCase(art->titre, MOTS_SUSPECTS[i])) {
            score_suspicion += 10;
        }
    }

    art->score_fiabilite = (score_suspicion > 100) ? 0 : (100 - score_suspicion);

    return score_suspicion;
}

void analyserReseau(grapheReseau g) {
    if (g == NULL) return;

    printf("\n========== ANALYSE DU RESEAU ==========\n\n");

    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE) {
            int suspicion = analyserArticle(g->articles[i]);

            if (g->articles[i]->score_fiabilite < 40) {
                printf("[SUSPECT] ");
            } else if (g->articles[i]->score_fiabilite < 70) {
                printf("[DOUTEUX] ");
            } else {
                printf("[FIABLE ] ");
            }

            printf("%s (score: %d)\n",
                   g->articles[i]->titre,
                   g->articles[i]->score_fiabilite);

            char titre_lower[100];
            stringLower(g->articles[i]->titre, titre_lower, 100);

            int trouveFrag = 0;
            for (int j = 0; j < NB_FAKES; j++) {
                if (strstr(titre_lower, BASE_FAKES[j]) != NULL) {
                    printf("\"");
                    stringLower((char*)BASE_FAKES[j], titre_lower, 100);
                    printf("%s\"\n", BASE_FAKES[j]);
                    trouveFrag = 1;
                    break;
                }
            }

            int mots = 0;
            printf("mots: ");
            for (int j = 0; j < NB_SUSPECTS; j++) {
                if (strstrCase(g->articles[i]->titre, MOTS_SUSPECTS[j])) {
                    if (mots > 0) printf(", ");
                    printf("%s", MOTS_SUSPECTS[j]);
                    mots++;
                }
            }
            if (mots == 0) printf("aucun");
            printf("\n\n");
        }
    }
}

void articlesSuspectsCites(grapheReseau g) {
    if (g == NULL) return;

    printf("Articles suspects les plus cites :\n");

    ELEMENT *suspects = (ELEMENT*)malloc(g->V * sizeof(ELEMENT));
    int nbSuspects = 0;

    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE &&
            g->articles[i]->score_fiabilite < 40 &&
            g->degre_in[i] > 0) {
            suspects[nbSuspects++] = g->articles[i];
        }
    }

    for (int i = 1; i < nbSuspects; i++) {
        ELEMENT cle = suspects[i];
        int j = i - 1;
        while (j >= 0 && g->degre_in[suspects[j]->id] < g->degre_in[cle->id]) {
            suspects[j + 1] = suspects[j];
            j--;
        }
        suspects[j + 1] = cle;
    }

    for (int i = 0; i < nbSuspects; i++) {
        printf("--> %s (score:%d, cite par %d articles)\n",
               suspects[i]->titre,
               suspects[i]->score_fiabilite,
               g->degre_in[suspects[i]->id]);
    }

    if (nbSuspects == 0) {
        printf("Aucun article suspect cite\n");
    }

    free(suspects);
}

/* ========== BONUS ========== */

void simulerSuppression(grapheReseau g, int idArt) {
    if (g == NULL || idArt < 0 || idArt >= g->V) return;
    if (g->articles[idArt] == ELEMENT_VIDE) {
        printf("Article non trouve\n");
        return;
    }

    printf("Suppression de %s :\n", g->articles[idArt]->titre);

    LISTE citants = listeCreer();
    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE && i != idArt) {
            if (rechercher(g->adjList[i], g->articles[idArt]) != 0) {
                inserer(&citants, g->articles[i], listeTaille(citants) + 1);
            }
        }
    }

    printf("- Etait cite par : ");
    int taille_citants = listeTaille(citants);
    if (taille_citants == 0) {
        printf("aucun");
    } else {
        for (int i = 1; i <= taille_citants; i++) {
            if (i > 1) printf(", ");
            printf("%s", recuperer(citants, i)->titre);
        }
    }
    printf(" (%d)\n", taille_citants);

    int taille_cites = listeTaille(g->adjList[idArt]);
    printf("- Citait : ");
    if (taille_cites == 0) {
        printf("aucun");
    } else {
        for (int i = 1; i <= taille_cites; i++) {
            if (i > 1) printf(", ");
            printf("%s", recuperer(g->adjList[idArt], i)->titre);
        }
    }
    printf(" (%d)\n", taille_cites);

    printf("- Articles deconnectes : ");
    for (int i = 1; i <= taille_citants; i++) {
        if (i > 1) printf(", ");
        printf("%s", recuperer(citants, i)->titre);
    }
    if (taille_citants == 0) printf("aucun");
    printf("\n");

    listeDetruire(&citants);
}

int neutraliserPropagation(grapheReseau g, int idSrc, int idDest) {
    if (g == NULL || idSrc < 0 || idSrc >= g->V || idDest < 0 || idDest >= g->V)
        return 0;
    if (g->articles[idSrc] == ELEMENT_VIDE || g->articles[idDest] == ELEMENT_VIDE)
        return 0;

    int *visited = (int*)calloc(g->V, sizeof(int));
    LISTE file = listeCreer();
    inserer(&file, g->articles[idSrc], 1);
    visited[idSrc] = 1;

    int pathExists = 0;
    int pos = 1;
    while (pos <= listeTaille(file) && !pathExists) {
        ELEMENT courant = recuperer(file, pos);

        for (int i = 0; i < g->V; i++) {
            if (g->articles[i] != ELEMENT_VIDE && !visited[i]) {
                if (rechercher(g->adjList[i], courant) != 0) {
                    if (i == idDest) {
                        pathExists = 1;
                        break;
                    }
                    visited[i] = 1;
                    inserer(&file, g->articles[i], listeTaille(file) + 1);
                }
            }
        }
        pos++;
    }

    if (!pathExists) {
        printf("Aucun chemin entre source et destination\n");
        free(visited);
        listeDetruire(&file);
        return 0;
    }

    int articleSupprime = -1;
    int minDegree = g->V + 1;

    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE && i != idSrc && i != idDest) {
            if (g->degre_in[i] + listeTaille(g->adjList[i]) < minDegree) {
                minDegree = g->degre_in[i] + listeTaille(g->adjList[i]);
                articleSupprime = i;
            }
        }
    }

    if (articleSupprime != -1) {
        printf("Article supprime : ");
        afficherArticleSimple(g->articles[articleSupprime]);
        printf(" (score:%d)\n", g->articles[articleSupprime]->score_fiabilite);
        printf("Plus aucun chemin de %s vers %s.\n",
               g->articles[idSrc]->titre, g->articles[idDest]->titre);
        printf("Nombre d articles supprimes : 1\n");
    }

    free(visited);
    listeDetruire(&file);
    return (articleSupprime != -1) ? 1 : 0;
}

/* ========== FONCTIONS AUXILIAIRES ========== */

int trouvIndexArticle(grapheReseau g, int id) {
    if (g == NULL || id < 0 || id >= g->V) return -1;
    if (g->articles[id] != ELEMENT_VIDE && g->articles[id]->id == id)
        return id;
    return -1;
}

void afficherStatistiques(grapheReseau g) {
    if (g == NULL) return;

    printf("\n========== STATISTIQUES DU RESEAU ==========\n");
    int nbArticles = 0, nbCitations = 0;

    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE) {
            nbArticles++;
            nbCitations += listeTaille(g->adjList[i]);
        }
    }

    printf("Nombre d'articles : %d\n", nbArticles);
    printf("Nombre de citations : %d\n", nbCitations);
    printf("=========================================\n\n");
}
