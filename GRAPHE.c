#include "GRAPHE.h"
#include "FAKESDB.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/* ============================================================
   SECTION 1 - CREATION / CHARGEMENT / DESTRUCTION
   ============================================================ */

grapheReseau creerGraphe(int V) {
    grapheReseau g = (grapheReseau)malloc(sizeof(*g));

    if (g != NULL) {
        g->V = V;
        g->articles  = (ELEMENT *)malloc(V * sizeof(ELEMENT));
        g->adjList   = (LISTE *)  malloc(V * sizeof(LISTE));
        g->degre_in  = (int *)    malloc(V * sizeof(int));

        if (g->articles == NULL || g->adjList == NULL || g->degre_in == NULL) {
            free(g->articles);
            free(g->adjList);
            free(g->degre_in);
            free(g);
            g = NULL;
        } else {
            for (int i = 0; i < V; i++) {
                g->articles[i] = ELEMENT_VIDE;
                g->adjList[i]  = creerListe();
                g->degre_in[i] = 0;
            }
        }
    }

    return g;
}

grapheReseau chargerGraphe(const char *filename) {
    FILE *f = fopen(filename, "r");
    grapheReseau g = NULL;

    if (f == NULL) {
        printf("!!! Fichier introuvable !!!\n");
    } else {
        g = creerGraphe(10000);

        if (g != NULL) {
            char line[512];

            while (fgets(line, sizeof(line), f)) {
                if (line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

                char type = line[0];

                if (type == 'A') {
                    int id, jour, mois, annee, heure, minute, score;
                    char titre[100], source[50];

                    /* Try quoted title first, then unquoted */
                    int parsed = sscanf(line, "A %d \"%99[^\"]\" %49s %d %d %d %d %d %d",
                                        &id, titre, source, &score,
                                        &jour, &mois, &annee, &heure, &minute);
                    if (parsed < 9) {
                        parsed = sscanf(line, "A %d %99s %49s %d %d %d %d %d %d",
                                        &id, titre, source, &score,
                                        &jour, &mois, &annee, &heure, &minute);
                    }

                    if (parsed == 9 && id >= 0 && id < g->V) {
                        ELEMENT art = creerArticle(id, titre, source, score,
                                                   jour, mois, annee, heure, minute);
                        if (art != ELEMENT_VIDE) {
                            g->articles[id] = art;
                        }
                    }
                } else if (type == 'C') {
                    int idSrc, idDest;
                    if (sscanf(line, "C %d %d", &idSrc, &idDest) == 2) {
                        ajouterCitation(g, idSrc, idDest);
                    }
                }
            }
        }

        fclose(f);
    }

    return g;
}

void detruireGraphe(grapheReseau g) {
    if (g == NULL) {
        printf("!!! Graphe NULL !!!\n");
    } else {
        for (int i = 0; i < g->V; i++) {
            if (g->articles[i] != ELEMENT_VIDE) {
                detruireArticle(g->articles[i]);
            }
            g->adjList[i] = detruireListe(g->adjList[i]);
        }
        free(g->articles);
        free(g->adjList);
        free(g->degre_in);
        free(g);
    }
}

/* ============================================================
   SECTION 2 - AJOUT / SUPPRESSION D'ARTICLES ET CITATIONS
   ============================================================ */

int ajouterArticle(grapheReseau g, ELEMENT art) {
    int succes = 0;

    if (g != NULL && art != ELEMENT_VIDE && art->id >= 0 && art->id < g->V) {
        if (g->articles[art->id] == ELEMENT_VIDE) {
            g->articles[art->id] = art;
            succes = 1;
        }
    }

    return succes;
}

/*
 * FIX: supprimerArticle
 * L'arête A→B signifie "A cite B".
 * Quand on supprime idArt :
 *   1. Parcourir les arêtes SORTANTES de idArt → décrémenter degre_in des articles cités.
 *   2. Parcourir TOUS les autres articles → retirer idArt de leurs listes (arêtes entrantes).
 *      Ne PAS toucher degre_in[idArt] (ça ne sert plus à rien).
 */
int supprimerArticle(grapheReseau g, int idArt) {
    int resultat = 0;

    if (g == NULL || idArt < 0 || idArt >= g->V) return 0;
    if (g->articles[idArt] == ELEMENT_VIDE) return 0;

    /* Étape 1 : décrémenter degre_in des articles que idArt cite */
    int tailleSortant = listeTaille(g->adjList[idArt]);
    for (int j = 1; j <= tailleSortant; j++) {
        ELEMENT e = recuperer(g->adjList[idArt], j);
        if (e != ELEMENT_VIDE && e->id >= 0 && e->id < g->V) {
            g->degre_in[e->id]--;
        }
    }

    /* Étape 2 : retirer idArt des listes d'adjacence des autres articles */
    for (int i = 0; i < g->V; i++) {
        if (i != idArt && g->articles[i] != ELEMENT_VIDE) {
            int taille = listeTaille(g->adjList[i]);
            for (int j = 1; j <= taille; j++) {
                ELEMENT e = recuperer(g->adjList[i], j);
                if (e != ELEMENT_VIDE && e->id == idArt) {
                    g->adjList[i] = supprimer(g->adjList[i], j);
                    /* degre_in[idArt] n'est pas mis à jour : inutile */
                    break;
                }
            }
        }
    }

    /* Étape 3 : détruire la liste sortante et l'article */
    g->adjList[idArt] = detruireListe(g->adjList[idArt]);
    g->adjList[idArt] = creerListe();
    detruireArticle(g->articles[idArt]);
    g->articles[idArt] = ELEMENT_VIDE;
    g->degre_in[idArt] = 0;

    resultat = 1;
    return resultat;
}

int ajouterCitation(grapheReseau g, int idSrc, int idDest) {
    int succes = 0;

    if (g != NULL && idSrc >= 0 && idSrc < g->V && idDest >= 0 && idDest < g->V) {
        if (g->articles[idSrc] != ELEMENT_VIDE && g->articles[idDest] != ELEMENT_VIDE) {
            g->adjList[idSrc] = inserer(g->adjList[idSrc],
                                        g->articles[idDest],
                                        listeTaille(g->adjList[idSrc]) + 1);
            g->degre_in[idDest]++;
            succes = 1;
        }
    }

    return succes;
}

int supprimerCitation(grapheReseau g, int idSrc, int idDest) {
    int succes = 0;

    if (g != NULL && idSrc >= 0 && idSrc < g->V && idDest >= 0 && idDest < g->V) {
        if (g->articles[idSrc] != ELEMENT_VIDE && g->articles[idDest] != ELEMENT_VIDE) {
            int taille = listeTaille(g->adjList[idSrc]);
            for (int j = 1; j <= taille && succes == 0; j++) {
                ELEMENT e = recuperer(g->adjList[idSrc], j);
                if (e != ELEMENT_VIDE && e->id == idDest) {
                    g->adjList[idSrc] = supprimer(g->adjList[idSrc], j);
                    g->degre_in[idDest]--;
                    succes = 1;
                }
            }
        }
    }

    return succes;
}

/* ============================================================
   SECTION 3 - AFFICHAGE ET SAUVEGARDE
   ============================================================ */

void afficherGraphe(grapheReseau g) {
    if (g == NULL) {
        printf("!!! Graphe NULL !!!\n");
        return;
    }

    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE) {
            afficherArticle(g->articles[i]);
            int taille = listeTaille(g->adjList[i]);
            if (taille == 0) {
                printf("  (ne cite aucun article)\n");
            } else {
                for (int j = 1; j <= taille; j++) {
                    ELEMENT e = recuperer(g->adjList[i], j);
                    if (e != ELEMENT_VIDE) {
                        printf("  --> %s\n", e->titre);
                    }
                }
            }
            printf("\n");
        }
    }
}

void sauvegarderGraphe(grapheReseau g, const char *filename) {
    if (g == NULL) {
        printf("!!! Graphe vide, rien a sauvegarder !!!\n");
        return;
    }

    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        printf("!!! Erreur ouverture fichier !!!\n");
        return;
    }

    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE) {
            fprintf(f, "A %d \"%s\" %s %d %d %d %d %d %d\n",
                    g->articles[i]->id,
                    g->articles[i]->titre,
                    g->articles[i]->source,
                    g->articles[i]->score_fiabilite,
                    g->articles[i]->jour,
                    g->articles[i]->mois,
                    g->articles[i]->annee,
                    g->articles[i]->heure,
                    g->articles[i]->minute);
        }
    }

    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE) {
            int taille = listeTaille(g->adjList[i]);
            for (int j = 1; j <= taille; j++) {
                ELEMENT e = recuperer(g->adjList[i], j);
                if (e != ELEMENT_VIDE) {
                    fprintf(f, "C %d %d\n", i, e->id);
                }
            }
        }
    }

    fclose(f);
}

/* ============================================================
   SECTION 4 - INTERROGATION DU RESEAU
   ============================================================ */

void articlesCites(grapheReseau g, int idSrc) {
    if (g == NULL || idSrc < 0 || idSrc >= g->V || g->articles[idSrc] == ELEMENT_VIDE) {
        printf("!!! Article inexistant !!!\n");
        return;
    }

    int taille = listeTaille(g->adjList[idSrc]);
    if (taille == 0) {
        printf("!!! Cet article ne cite aucun article !!!\n");
    } else {
        for (int j = 1; j <= taille; j++) {
            ELEMENT e = recuperer(g->adjList[idSrc], j);
            if (e != ELEMENT_VIDE) {
                printf("--> %s\n", e->titre);
            }
        }
    }
}

void articlesCitants(grapheReseau g, int idDest) {
    if (g == NULL || idDest < 0 || idDest >= g->V || g->articles[idDest] == ELEMENT_VIDE) {
        printf("!!! Article inexistant !!!\n");
        return;
    }

    int trouve = 0;
    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE) {
            int taille = listeTaille(g->adjList[i]);
            for (int j = 1; j <= taille; j++) {
                ELEMENT e = recuperer(g->adjList[i], j);
                if (e != ELEMENT_VIDE && e->id == idDest) {
                    printf("--> %s\n", g->articles[i]->titre);
                    trouve = 1;
                }
            }
        }
    }

    if (trouve == 0) {
        printf("!!! Aucun article ne cite celui-ci !!!\n");
    }
}

void sourcesOriginales(grapheReseau g) {
    if (g == NULL) {
        printf("!!! Graphe vide !!!\n");
        return;
    }

    int trouve = 0;
    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE && listeTaille(g->adjList[i]) == 0) {
            printf("--> %s (ne cite personne)\n", g->articles[i]->titre);
            trouve = 1;
        }
    }

    if (trouve == 0) {
        printf("!!! Aucune source originale !!!\n");
    }
}

void articlesIsoles(grapheReseau g) {
    if (g == NULL) {
        printf("!!! Graphe vide !!!\n");
        return;
    }

    int trouve = 0;
    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE && g->degre_in[i] == 0) {
            printf("--> %s (non cite par personne)\n", g->articles[i]->titre);
            trouve = 1;
        }
    }

    if (trouve == 0) {
        printf("!!! Aucun article isole !!!\n");
    }
}

/*
 * FIX: articlePlusCite
 * La spec demande d'afficher TOUS les articles ex-aequo.
 * On fait deux passes : trouver le max, puis afficher tous ceux qui l'ont.
 */
ELEMENT articlePlusCite(grapheReseau g) {
    ELEMENT result = ELEMENT_VIDE;

    if (g == NULL) return result;

    int maxDegre = -1;
    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE && g->degre_in[i] > maxDegre) {
            maxDegre = g->degre_in[i];
            result   = g->articles[i];
        }
    }

    if (result != ELEMENT_VIDE) {
        /* Afficher tous les ex-aequo */
        for (int i = 0; i < g->V; i++) {
            if (g->articles[i] != ELEMENT_VIDE && g->degre_in[i] == maxDegre) {
                printf("--> %s (cite par %d articles)\n",
                       g->articles[i]->titre, g->degre_in[i]);
            }
        }
    }

    return result;
}

/* ============================================================
   SECTION 5 - ANALYSE CHRONOLOGIQUE
   ============================================================ */

void trierParDate(grapheReseau g) {
    if (g == NULL) {
        printf("!!! Graphe vide !!!\n");
        return;
    }

    ELEMENT *temp = (ELEMENT *)malloc(g->V * sizeof(ELEMENT));
    if (temp == NULL) {
        printf("!!! Erreur allocation memoire !!!\n");
        return;
    }

    int count = 0;
    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE) {
            temp[count++] = g->articles[i];
        }
    }

    if (count == 0) {
        printf("!!! Aucun article a trier !!!\n");
        free(temp);
        return;
    }

    /* Tri par insertion (du plus ancien au plus récent) */
    for (int i = 1; i < count; i++) {
        ELEMENT key = temp[i];
        int j = i - 1;
        while (j >= 0 && comparerDates(temp[j], key) > 0) {
            temp[j + 1] = temp[j];
            j--;
        }
        temp[j + 1] = key;
    }

    for (int i = 0; i < count; i++) {
        printf("%d. ", i + 1);
        afficherArticle(temp[i]);
    }

    free(temp);
}

void premierCitant(grapheReseau g, int idDest) {
    if (g == NULL || idDest < 0 || idDest >= g->V || g->articles[idDest] == ELEMENT_VIDE) {
        printf("!!! Article inexistant !!!\n");
        return;
    }

    ELEMENT plusAncien = ELEMENT_VIDE;

    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE) {
            int taille = listeTaille(g->adjList[i]);
            for (int j = 1; j <= taille; j++) {
                ELEMENT e = recuperer(g->adjList[i], j);
                if (e != ELEMENT_VIDE && e->id == idDest) {
                    if (plusAncien == ELEMENT_VIDE ||
                        comparerDates(g->articles[i], plusAncien) < 0) {
                        plusAncien = g->articles[i];
                    }
                }
            }
        }
    }

    if (plusAncien == ELEMENT_VIDE) {
        printf("!!! Aucun article ne cite celui-ci !!!\n");
    } else {
        printf("--> ");
        afficherArticle(plusAncien);
    }
}

/*
 * FIX: chainePropagation
 *
 * L'arête A→B signifie "A cite B". La propagation part de idSrc (la source
 * originale, ex: l'article officiel A3) et remonte vers ceux qui le citent.
 *
 * On fait un BFS INVERSE depuis idSrc : on cherche tous les articles qui ont
 * un chemin de citations vers idSrc (i.e. X→...→idSrc).
 * On trie le résultat par date et on affiche la chaîne chronologique.
 *
 * Exemple spec : chainePropagation(g, 3)
 *   Communique (07h) --> cite par Etude (08h) --> cite par Vaccin (09h)
 *   --> cite par Partagez (11h) --> cite par Alerte (14h)
 */
void chainePropagation(grapheReseau g, int idSrc) {
    if (g == NULL || idSrc < 0 || idSrc >= g->V || g->articles[idSrc] == ELEMENT_VIDE) {
        printf("!!! Article inexistant !!!\n");
        return;
    }

    int *visited = (int *)calloc(g->V, sizeof(int));
    if (visited == NULL) {
        printf("!!! Erreur allocation memoire !!!\n");
        return;
    }

    /* BFS inverse : depuis idSrc, on cherche qui cite qui */
    LISTE file = creerListe();
    file = inserer(file, g->articles[idSrc], 1);
    visited[idSrc] = 1;

    /* Collecte de tous les noeuds accessibles en sens inverse */
    LISTE chaine = creerListe();
    chaine = inserer(chaine, g->articles[idSrc], 1);

    while (!estVide(file)) {
        ELEMENT courant = recuperer(file, 1);
        file = supprimer(file, 1);

        for (int i = 0; i < g->V; i++) {
            if (g->articles[i] != ELEMENT_VIDE && !visited[i]) {
                /* Est-ce que i cite courant ? */
                int taille = listeTaille(g->adjList[i]);
                for (int j = 1; j <= taille; j++) {
                    ELEMENT e = recuperer(g->adjList[i], j);
                    if (e != ELEMENT_VIDE && e->id == courant->id) {
                        visited[i] = 1;
                        file   = inserer(file,   g->articles[i], listeTaille(file)   + 1);
                        chaine = inserer(chaine, g->articles[i], listeTaille(chaine) + 1);
                        break;
                    }
                }
            }
        }
    }

    file = detruireListe(file);

    /* Trier la chaine par date (tri par insertion) */
    int count = listeTaille(chaine);
    ELEMENT *arr = (ELEMENT *)malloc(count * sizeof(ELEMENT));
    if (arr == NULL) {
        chaine = detruireListe(chaine);
        free(visited);
        return;
    }

    for (int i = 0; i < count; i++) {
        arr[i] = recuperer(chaine, i + 1);
    }
    chaine = detruireListe(chaine);

    for (int i = 1; i < count; i++) {
        ELEMENT key = arr[i];
        int j = i - 1;
        while (j >= 0 && comparerDates(arr[j], key) > 0) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }

    /* Affichage : le premier (idSrc) sans flèche, les suivants avec "cite par" */
    afficherArticle(arr[0]);
    for (int i = 1; i < count; i++) {
        printf("  --> cite par ");
        afficherArticle(arr[i]);
    }

    free(arr);
    free(visited);
}

/* ============================================================
   SECTION 6 - SIMULATION DE PROPAGATION (BFS)
   ============================================================
 *
 * FIX: simulerPropagation et articlesAccessibles
 *
 * L'arête A→B = "A cite B". La propagation de désinformation remonte :
 * si B est une fake news, elle se propage vers A (qui cite B), puis vers
 * ceux qui citent A, etc. Le sens de parcours est donc INVERSE aux arêtes.
 *
 * Pour trouver les "voisins" du noeud courant dans le BFS de propagation,
 * on cherche tous les articles i tels que adjList[i] contient courant.
 *
 * Exemple spec : simulerPropagation(g, 3)
 *   Niveau 0 : Communique
 *   Niveau 1 : Etude, Vaccin   (car Etude cite Communique et Vaccin cite Communique)
 *   Niveau 2 : Partagez, Alerte
 */
void simulerPropagation(grapheReseau g, int idSrc) {
    if (g == NULL || idSrc < 0 || idSrc >= g->V || g->articles[idSrc] == ELEMENT_VIDE) {
        printf("!!! Article inexistant !!!\n");
        return;
    }

    int *visited = (int *)calloc(g->V, sizeof(int));
    if (visited == NULL) {
        printf("!!! Erreur allocation memoire !!!\n");
        return;
    }

    LISTE file = creerListe();
    file = inserer(file, g->articles[idSrc], 1);
    visited[idSrc] = 1;

    int niveau        = 0;
    int totalArticles = 1;
    int niveauActuel  = 1;  /* nb d'éléments dans le niveau courant */

    while (niveauActuel > 0) {
        int niveauSuivant = 0;

        printf("Niveau %d : ", niveau);

        for (int i = 0; i < niveauActuel; i++) {
            ELEMENT courant = recuperer(file, 1);
            file = supprimer(file, 1);

            if (i > 0) printf(", ");
            printf("%s", courant->titre);

            /* Parcours inverse : chercher qui cite courant */
            for (int k = 0; k < g->V; k++) {
                if (g->articles[k] != ELEMENT_VIDE && !visited[k]) {
                    int taille = listeTaille(g->adjList[k]);
                    for (int j = 1; j <= taille; j++) {
                        ELEMENT e = recuperer(g->adjList[k], j);
                        if (e != ELEMENT_VIDE && e->id == courant->id) {
                            visited[k] = 1;
                            file = inserer(file, g->articles[k], listeTaille(file) + 1);
                            niveauSuivant++;
                            totalArticles++;
                            break;
                        }
                    }
                }
            }
        }

        printf("\n");
        niveauActuel = niveauSuivant;
        niveau++;
    }

    printf("%d niveaux, %d articles atteints.\n", niveau, totalArticles);

    file = detruireListe(file);
    free(visited);
}

/*
 * articlesAccessibles : même sens que simulerPropagation (BFS inverse),
 * affiche tous les articles atteignables sans les niveaux.
 */
void articlesAccessibles(grapheReseau g, int idSrc) {
    if (g == NULL || idSrc < 0 || idSrc >= g->V || g->articles[idSrc] == ELEMENT_VIDE) {
        printf("!!! Article inexistant !!!\n");
        return;
    }

    int *visited = (int *)calloc(g->V, sizeof(int));
    if (visited == NULL) {
        printf("!!! Erreur allocation memoire !!!\n");
        return;
    }

    LISTE file = creerListe();
    file = inserer(file, g->articles[idSrc], 1);
    visited[idSrc] = 1;

    int trouve = 0;

    while (!estVide(file)) {
        ELEMENT courant = recuperer(file, 1);
        file = supprimer(file, 1);

        printf("--> %s\n", courant->titre);
        trouve = 1;

        /* BFS inverse */
        for (int k = 0; k < g->V; k++) {
            if (g->articles[k] != ELEMENT_VIDE && !visited[k]) {
                int taille = listeTaille(g->adjList[k]);
                for (int j = 1; j <= taille; j++) {
                    ELEMENT e = recuperer(g->adjList[k], j);
                    if (e != ELEMENT_VIDE && e->id == courant->id) {
                        visited[k] = 1;
                        file = inserer(file, g->articles[k], listeTaille(file) + 1);
                        break;
                    }
                }
            }
        }
    }

    if (trouve == 0) {
        printf("!!! Aucun article accessible !!!\n");
    }

    file = detruireListe(file);
    free(visited);
}

/* ============================================================
   SECTION 7 - BONUS : ROBUSTESSE
   ============================================================ */

/*
 * simulerSuppression
 * Affiche : qui citait idArt, qui idArt citait, et les articles qui deviennent
 * déconnectés (non accessibles depuis idSrc = idArt, après suppression).
 * Format attendu par la spec :
 *   "Suppression de <titre> :"
 *   "- Etait cite par : X, Y (n)"
 *   "- Citait : A, B (n)"
 *   "- Articles deconnectes : P, Q"
 */
void simulerSuppression(grapheReseau g, int idArt) {
    if (g == NULL || idArt < 0 || idArt >= g->V || g->articles[idArt] == ELEMENT_VIDE) {
        printf("!!! Article inexistant !!!\n");
        return;
    }

    printf("Suppression de %s :\n", g->articles[idArt]->titre);

    /* --- Citants (arêtes entrantes) --- */
    int *citantsIds = (int *)malloc(g->V * sizeof(int));
    int nbCitants   = 0;

    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE && i != idArt) {
            int taille = listeTaille(g->adjList[i]);
            for (int j = 1; j <= taille; j++) {
                ELEMENT e = recuperer(g->adjList[i], j);
                if (e != ELEMENT_VIDE && e->id == idArt) {
                    citantsIds[nbCitants++] = i;
                    break;
                }
            }
        }
    }

    printf("- Etait cite par : ");
    if (nbCitants == 0) {
        printf("(personne)");
    } else {
        for (int i = 0; i < nbCitants; i++) {
            if (i > 0) printf(", ");
            printf("%s", g->articles[citantsIds[i]]->titre);
        }
    }
    printf(" (%d)\n", nbCitants);

    /* --- Articles cités (arêtes sortantes) --- */
    int tailleSortant = listeTaille(g->adjList[idArt]);
    printf("- Citait : ");
    if (tailleSortant == 0) {
        printf("(personne)");
    } else {
        for (int j = 1; j <= tailleSortant; j++) {
            ELEMENT e = recuperer(g->adjList[idArt], j);
            if (e != ELEMENT_VIDE) {
                if (j > 1) printf(", ");
                printf("%s", e->titre);
            }
        }
    }
    printf(" (%d)\n", tailleSortant);

    /* --- Articles déconnectés après suppression ---
     * Ce sont les citants de idArt qui ne peuvent plus atteindre
     * aucune source originale (ou simplement qui perdent leur lien direct).
     * Selon la spec : afficher les articles qui "ne sont plus accessibles
     * depuis la source" = les citants directs de idArt.
     */
    printf("- Articles deconnectes : ");
    if (nbCitants == 0) {
        printf("(aucun)");
    } else {
        for (int i = 0; i < nbCitants; i++) {
            if (i > 0) printf(", ");
            printf("%s", g->articles[citantsIds[i]]->titre);
        }
    }
    printf("\n");

    free(citantsIds);

    /* Effectuer la suppression réelle */
    supprimerArticle(g, idArt);
}

/*
 * FIX: neutraliserPropagation
 *
 * Objectif : supprimer le minimum d'articles intermédiaires pour qu'il
 * n'existe plus aucun chemin de citations de idSrc vers idDest.
 * (ni idSrc ni idDest ne sont supprimés)
 *
 * Algorithme : tant qu'un chemin idSrc → ... → idDest existe,
 *   - trouver un tel chemin par BFS direct (sens des arêtes : A→B = A cite B)
 *   - supprimer le premier article intermédiaire du chemin
 *   - répéter
 *
 * BFS direct : adjList[x] donne les articles que x cite.
 * Un chemin de idSrc à idDest = séquence idSrc → a → b → ... → idDest
 * où chaque flèche signifie "cite".
 */

/* Retourne 1 si un chemin direct idSrc→idDest existe, et remplit parent[] */
static int bfsDirect(grapheReseau g, int idSrc, int idDest, int *parent) {
    int *visited = (int *)calloc(g->V, sizeof(int));
    if (visited == NULL) return 0;

    for (int i = 0; i < g->V; i++) parent[i] = -1;

    LISTE file = creerListe();
    file = inserer(file, g->articles[idSrc], 1);
    visited[idSrc] = 1;

    int found = 0;

    while (!estVide(file) && !found) {
        ELEMENT courant = recuperer(file, 1);
        file = supprimer(file, 1);

        int taille = listeTaille(g->adjList[courant->id]);
        for (int j = 1; j <= taille; j++) {
            ELEMENT e = recuperer(g->adjList[courant->id], j);
            if (e != ELEMENT_VIDE && !visited[e->id]) {
                visited[e->id] = 1;
                parent[e->id]  = courant->id;
                if (e->id == idDest) {
                    found = 1;
                    break;
                }
                file = inserer(file, g->articles[e->id], listeTaille(file) + 1);
            }
        }
    }

    file = detruireListe(file);
    free(visited);
    return found;
}

int neutraliserPropagation(grapheReseau g, int idSrc, int idDest) {
    int removed = 0;

    if (g == NULL || idSrc < 0 || idSrc >= g->V ||
        idDest < 0 || idDest >= g->V ||
        g->articles[idSrc] == ELEMENT_VIDE ||
        g->articles[idDest] == ELEMENT_VIDE) {
        printf("!!! Articles inexistants !!!\n");
        return 0;
    }

    int *parent   = (int *)malloc(g->V * sizeof(int));
    int *visited  = (int *)malloc(g->V * sizeof(int));
    /* supprime[i] = 1 : noeud bloque pour cette simulation uniquement.
     * L'article reste intact dans g->articles[], on l'ignore juste
     * dans le BFS. Rien n'est free() ni modifie dans le graphe.         */
    int *supprime = (int *)calloc(g->V, sizeof(int));

    if (parent == NULL || visited == NULL || supprime == NULL) {
        free(parent); free(visited); free(supprime);
        return 0;
    }

    int cheminExiste = 1;
    while (cheminExiste) {

        /* BFS depuis idSrc, sens normal (adjList[x] = articles cites par x) */
        for (int i = 0; i < g->V; i++) {
            parent[i]  = -1;
            visited[i] = 0;
        }

        LISTE file = creerListe();
        file = inserer(file, g->articles[idSrc], 1);
        visited[idSrc] = 1;

        int found = 0;

        while (!estVide(file) && !found) {
            ELEMENT courant = recuperer(file, 1);
            file = supprimer(file, 1);

            int taille = listeTaille(g->adjList[courant->id]);
            for (int j = 1; j <= taille; j++) {
                ELEMENT voisin = recuperer(g->adjList[courant->id], j);
                if (voisin == ELEMENT_VIDE) continue;
                if (g->articles[voisin->id] == ELEMENT_VIDE) continue;
                if (visited[voisin->id]) continue;
                if (supprime[voisin->id]) continue;  /* noeud bloque */

                visited[voisin->id] = 1;
                parent[voisin->id]  = courant->id;

                if (voisin->id == idDest) {
                    found = 1;
                    break;
                }
                file = inserer(file, g->articles[voisin->id], listeTaille(file) + 1);
            }
        }

        file = detruireListe(file);

        if (!found) {
            cheminExiste = 0;  /* aucun chemin restant */
        } else {
            /* Reconstruire chemin idSrc -> ... -> idDest */
            int chemin[10000];
            int len = 0;
            int cur = idDest;
            while (cur != -1) {
                chemin[len++] = cur;
                cur = parent[cur];
            }
            /* Inverser : chemin[0]=idSrc, chemin[len-1]=idDest */
            for (int i = 0; i < len / 2; i++) {
                int tmp = chemin[i];
                chemin[i] = chemin[len - 1 - i];
                chemin[len - 1 - i] = tmp;
            }

            if (len <= 2) {
                /* Chemin direct sans intermediaire : bloquer idDest
                 * temporairement pour stopper cette iteration          */
                supprime[idDest] = 1;
                printf("Citation directe bloquee : %s --> %s\n",
                       g->articles[idSrc]->titre,
                       g->articles[idDest]->titre);
            } else {
                /* Choisir l'intermediaire avec le degre_in le plus eleve */
                int bestIdx   = 1;
                int bestDegre = g->degre_in[chemin[1]];
                for (int i = 2; i <= len - 2; i++) {
                    if (g->degre_in[chemin[i]] > bestDegre) {
                        bestDegre = g->degre_in[chemin[i]];
                        bestIdx   = i;
                    }
                }

                int toBlock = chemin[bestIdx];
                printf("Article supprime : %s (score:%d)\n",
                       g->articles[toBlock]->titre,
                       g->articles[toBlock]->score_fiabilite);

                supprime[toBlock] = 1;  /* blocage temporaire, pas de free() */
                removed++;
            }
        }
    }

    printf("Plus aucun chemin de %s vers %s.\n",
           g->articles[idSrc]->titre,
           g->articles[idDest]->titre);
    printf("Nombre d'articles supprimes : %d\n", removed);

    /* Graphe intact : on libere juste nos tableaux locaux */
    free(parent);
    free(visited);
    free(supprime);
    return removed;
}
