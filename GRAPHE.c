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
        g->V        = V;
        g->articles = (ELEMENT *)malloc(V * sizeof(ELEMENT));
        g->adjList  = (LISTE *)  malloc(V * sizeof(LISTE));
        g->degre_in = (int *)    malloc(V * sizeof(int));

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
                if (line[0] != '#' && line[0] != '\n' && line[0] != '\r') {
                    char type = line[0];

                    if (type == 'A') {
                        int id, jour, mois, annee, heure, minute, score;
                        char titre[100], source[50];

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

int supprimerArticle(grapheReseau g, int idArt) {
    int resultat = 0;

    if (g != NULL && idArt >= 0 && idArt < g->V && g->articles[idArt] != ELEMENT_VIDE) {

        /* Etape 1 : decrementer degre_in des articles que idArt cite */
        int tailleSortant = listeTaille(g->adjList[idArt]);
        for (int j = 1; j <= tailleSortant; j++) {
            ELEMENT e = recuperer(g->adjList[idArt], j);
            if (e != ELEMENT_VIDE && e->id >= 0 && e->id < g->V) {
                g->degre_in[e->id]--;
            }
        }

        /* Etape 2 : retirer idArt des listes d'adjacence des autres articles */
        for (int i = 0; i < g->V; i++) {
            if (i != idArt && g->articles[i] != ELEMENT_VIDE) {
                int taille   = listeTaille(g->adjList[i]);
                int j        = 1;
                int supprime = 0;
                while (j <= taille && supprime == 0) {
                    ELEMENT e = recuperer(g->adjList[i], j);
                    if (e != ELEMENT_VIDE && e->id == idArt) {
                        g->adjList[i] = supprimer(g->adjList[i], j);
                        supprime = 1;
                    } else {
                        j++;
                    }
                }
            }
        }

        /* Etape 3 : detruire la liste sortante et l'article */
        g->adjList[idArt] = detruireListe(g->adjList[idArt]);
        g->adjList[idArt] = creerListe();
        detruireArticle(g->articles[idArt]);
        g->articles[idArt] = ELEMENT_VIDE;
        g->degre_in[idArt] = 0;

        resultat = 1;
    }

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
    } else {
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
}

void sauvegarderGraphe(grapheReseau g, const char *filename) {
    if (g == NULL) {
        printf("!!! Graphe vide, rien a sauvegarder !!!\n");
    } else {
        FILE *f = fopen(filename, "w");
        if (f == NULL) {
            printf("!!! Erreur ouverture fichier !!!\n");
        } else {
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
    }
}

/* ============================================================
   SECTION 4 - INTERROGATION DU RESEAU
   ============================================================ */

void articlesCites(grapheReseau g, int idSrc) {
    if (g == NULL || idSrc < 0 || idSrc >= g->V || g->articles[idSrc] == ELEMENT_VIDE) {
        printf("!!! Article inexistant !!!\n");
    } else {
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
}

void articlesCitants(grapheReseau g, int idDest) {
    if (g == NULL || idDest < 0 || idDest >= g->V || g->articles[idDest] == ELEMENT_VIDE) {
        printf("!!! Article inexistant !!!\n");
    } else {
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
}

void sourcesOriginales(grapheReseau g) {
    if (g == NULL) {
        printf("!!! Graphe vide !!!\n");
    } else {
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
}

void articlesIsoles(grapheReseau g) {
    if (g == NULL) {
        printf("!!! Graphe vide !!!\n");
    } else {
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
}

ELEMENT articlePlusCite(grapheReseau g) {
    ELEMENT result = ELEMENT_VIDE;

    if (g != NULL) {
        int maxDegre = -1;
        for (int i = 0; i < g->V; i++) {
            if (g->articles[i] != ELEMENT_VIDE && g->degre_in[i] > maxDegre) {
                maxDegre = g->degre_in[i];
                result   = g->articles[i];
            }
        }

        if (result != ELEMENT_VIDE) {
            for (int i = 0; i < g->V; i++) {
                if (g->articles[i] != ELEMENT_VIDE && g->degre_in[i] == maxDegre) {
                    printf("--> %s (cite par %d articles)\n",
                           g->articles[i]->titre, g->degre_in[i]);
                }
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
    } else {
        ELEMENT *temp = (ELEMENT *)malloc(g->V * sizeof(ELEMENT));
        if (temp == NULL) {
            printf("!!! Erreur d'allocation memoire !!!\n");
        } else {
            int count = 0;
            for (int i = 0; i < g->V; i++) {
                if (g->articles[i] != ELEMENT_VIDE) {
                    temp[count++] = g->articles[i];
                }
            }

            if (count == 0) {
                printf("!!! Aucun article a trier !!!\n");
            } else {
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
            }

            free(temp);
        }
    }
}

void premierCitant(grapheReseau g, int idDest) {
    if (g == NULL || idDest < 0 || idDest >= g->V || g->articles[idDest] == ELEMENT_VIDE) {
        printf("!!! Article inexistant !!!\n");
    } else {
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
}

void chainePropagation(grapheReseau g, int idSrc) {
    if (g == NULL || idSrc < 0 || idSrc >= g->V || g->articles[idSrc] == ELEMENT_VIDE) {
        printf("!!! Article inexistant !!!\n");
    } else {
        int *visited = (int *)calloc(g->V, sizeof(int));
        if (visited == NULL) {
            printf("!!! Erreur d'allocation memoire !!!\n");
        } else {
            LISTE file = creerListe();
            file = inserer(file, g->articles[idSrc], 1);
            visited[idSrc] = 1;

            LISTE chaine = creerListe();
            chaine = inserer(chaine, g->articles[idSrc], 1);

            while (!estVide(file)) {
                ELEMENT courant = recuperer(file, 1);
                file = supprimer(file, 1);

                for (int i = 0; i < g->V; i++) {
                    if (g->articles[i] != ELEMENT_VIDE && !visited[i]) {
                        int taille = listeTaille(g->adjList[i]);
                        int j      = 1;
                        int trouve = 0;
                        while (j <= taille && trouve == 0) {
                            ELEMENT e = recuperer(g->adjList[i], j);
                            if (e != ELEMENT_VIDE && e->id == courant->id) {
                                visited[i] = 1;
                                file   = inserer(file,   g->articles[i], listeTaille(file)   + 1);
                                chaine = inserer(chaine, g->articles[i], listeTaille(chaine) + 1);
                                trouve = 1;
                            } else {
                                j++;
                            }
                        }
                    }
                }
            }

            file = detruireListe(file);

            int count    = listeTaille(chaine);
            ELEMENT *arr = (ELEMENT *)malloc(count * sizeof(ELEMENT));
            if (arr == NULL) {
                printf("!!! Erreur d'allocation memoire !!!\n");
            } else {
                for (int i = 0; i < count; i++) {
                    arr[i] = recuperer(chaine, i + 1);
                }

                for (int i = 1; i < count; i++) {
                    ELEMENT key = arr[i];
                    int j = i - 1;
                    while (j >= 0 && comparerDates(arr[j], key) > 0) {
                        arr[j + 1] = arr[j];
                        j--;
                    }
                    arr[j + 1] = key;
                }

                afficherArticle(arr[0]);
                for (int i = 1; i < count; i++) {
                    printf("  --> cite par ");
                    afficherArticle(arr[i]);
                }

                free(arr);
            }

            chaine = detruireListe(chaine);
            free(visited);
        }
    }
}

/* ============================================================
   SECTION 6 - SIMULATION DE PROPAGATION (BFS)
   ============================================================ */

void simulerPropagation(grapheReseau g, int idSrc) {
    if (g == NULL || idSrc < 0 || idSrc >= g->V || g->articles[idSrc] == ELEMENT_VIDE) {
        printf("!!! Article inexistant !!!\n");
    } else {
        int *visited = (int *)calloc(g->V, sizeof(int));
        if (visited == NULL) {
            printf("!!! Erreur d'allocation memoire !!!\n");
        } else {
            LISTE file = creerListe();
            file = inserer(file, g->articles[idSrc], 1);
            visited[idSrc] = 1;

            int niveau        = 0;
            int totalArticles = 1;
            int niveauActuel  = 1;

            while (niveauActuel > 0) {
                int niveauSuivant = 0;

                printf("Niveau %d : ", niveau);

                for (int i = 0; i < niveauActuel; i++) {
                    ELEMENT courant = recuperer(file, 1);
                    file = supprimer(file, 1);

                    if (i > 0) printf(", ");
                    printf("%s", courant->titre);

                    for (int k = 0; k < g->V; k++) {
                        if (g->articles[k] != ELEMENT_VIDE && !visited[k]) {
                            int taille = listeTaille(g->adjList[k]);
                            int j      = 1;
                            int trouve = 0;
                            while (j <= taille && trouve == 0) {
                                ELEMENT e = recuperer(g->adjList[k], j);
                                if (e != ELEMENT_VIDE && e->id == courant->id) {
                                    visited[k] = 1;
                                    file = inserer(file, g->articles[k], listeTaille(file) + 1);
                                    niveauSuivant++;
                                    totalArticles++;
                                    trouve = 1;
                                } else {
                                    j++;
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
    }
}

void articlesAccessibles(grapheReseau g, int idSrc) {
    if (g == NULL || idSrc < 0 || idSrc >= g->V || g->articles[idSrc] == ELEMENT_VIDE) {
        printf("!!! Article inexistant !!!\n");
    } else {
        int *visited = (int *)calloc(g->V, sizeof(int));
        if (visited == NULL) {
            printf("!!! Erreur d'allocation memoire !!!\n");
        } else {
            LISTE file = creerListe();
            file = inserer(file, g->articles[idSrc], 1);
            visited[idSrc] = 1;

            int trouve = 0;

            while (!estVide(file)) {
                ELEMENT courant = recuperer(file, 1);
                file = supprimer(file, 1);

                printf("--> %s\n", courant->titre);
                trouve = 1;

                for (int k = 0; k < g->V; k++) {
                    if (g->articles[k] != ELEMENT_VIDE && !visited[k]) {
                        int taille       = listeTaille(g->adjList[k]);
                        int j            = 1;
                        int trouveVoisin = 0;
                        while (j <= taille && trouveVoisin == 0) {
                            ELEMENT e = recuperer(g->adjList[k], j);
                            if (e != ELEMENT_VIDE && e->id == courant->id) {
                                visited[k] = 1;
                                file = inserer(file, g->articles[k], listeTaille(file) + 1);
                                trouveVoisin = 1;
                            } else {
                                j++;
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
    }
}

/* ============================================================
   SECTION 7 - BONUS : ROBUSTESSE
   ============================================================ */

void simulerSuppression(grapheReseau g, int idArt) {
    if (g == NULL || idArt < 0 || idArt >= g->V || g->articles[idArt] == ELEMENT_VIDE) {
        printf("!!! Article inexistant !!!\n");
    } else {
        printf("Suppression de %s :\n", g->articles[idArt]->titre);

        int *citantsIds = (int *)malloc(g->V * sizeof(int));
        int nbCitants   = 0;

        for (int i = 0; i < g->V; i++) {
            if (g->articles[i] != ELEMENT_VIDE && i != idArt) {
                int taille    = listeTaille(g->adjList[i]);
                int j         = 1;
                int trouveArt = 0;
                while (j <= taille && trouveArt == 0) {
                    ELEMENT e = recuperer(g->adjList[i], j);
                    if (e != ELEMENT_VIDE && e->id == idArt) {
                        citantsIds[nbCitants++] = i;
                        trouveArt = 1;
                    } else {
                        j++;
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
        supprimerArticle(g, idArt);
    }
}

int neutraliserPropagation(grapheReseau g, int idSrc, int idDest) {
    int removed = 0;

    if (g == NULL || idSrc < 0 || idSrc >= g->V ||
        idDest < 0 || idDest >= g->V ||
        g->articles[idSrc] == ELEMENT_VIDE ||
        g->articles[idDest] == ELEMENT_VIDE) {
        printf("!!! Articles inexistants !!!\n");
    } else {
        int *parent   = (int *)malloc(g->V * sizeof(int));
        int *visited  = (int *)malloc(g->V * sizeof(int));
        /*
         * supprime[i] = 1 : noeud bloque pour cette simulation uniquement.
         * L'article reste intact dans g->articles[], on l'ignore juste
         * dans le BFS. Rien n'est free() ni modifie dans le graphe.
         */
        int *supprime = (int *)calloc(g->V, sizeof(int));

        if (parent == NULL || visited == NULL || supprime == NULL) {
            printf("!!! Erreur d'allocation memoire !!!\n");
            free(parent);
            free(visited);
            free(supprime);

        } else {
            int cheminExiste = 1;
            while (cheminExiste) {

                for (int i = 0; i < g->V; i++) {
                    parent[i]  = -1;
                    visited[i] = 0;
                }

                LISTE file = creerListe();
                file = inserer(file, g->articles[idSrc], 1);
                visited[idSrc] = 1;

                int found = 0;

                while (!estVide(file) && found == 0) {
                    ELEMENT courant = recuperer(file, 1);
                    file = supprimer(file, 1);

                    int taille = listeTaille(g->adjList[courant->id]);
                    int j      = 1;
                    while (j <= taille && found == 0) {
                        ELEMENT voisin = recuperer(g->adjList[courant->id], j);
                        if (voisin != ELEMENT_VIDE
                            && g->articles[voisin->id] != ELEMENT_VIDE
                            && visited[voisin->id] == 0
                            && supprime[voisin->id] == 0) {

                            visited[voisin->id] = 1;
                            parent[voisin->id]  = courant->id;

                            if (voisin->id == idDest) {
                                found = 1;
                            } else {
                                file = inserer(file, g->articles[voisin->id], listeTaille(file) + 1);
                                j++;
                            }
                        } else {j++;}


                    }
                }

                file = detruireListe(file);
//  chemin 7aadher cb
                if (found == 0) {
                    cheminExiste = 0;
                } else {
                    int chemin[10000];
                    int len = 0;
                    int cur = idDest;
                    // on creer la nouvelle liste avec un chemin claire
                    while (cur != -1) {
                        chemin[len++] = cur;
                        cur = parent[cur];
                    }

                    for (int i = 0; i < len / 2; i++) {
                        int tmp = chemin[i];
                        chemin[i] = chemin[len - 1 - i];
                        chemin[len - 1 - i] = tmp;
                    }
            //aucun intermidiaire
                    if (len <= 2) {
                        supprime[idDest] = 1;
                        printf("Citation directe bloquee : %s --> %s\n",
                               g->articles[idSrc]->titre,
                               g->articles[idDest]->titre);
                    }
                    //il ya intermidiaire on cherche maintenatn l article qui admt les bigger degree_in
                     else {
                        int bestIdx   = 1;
                        int bestDegre = g->degre_in[chemin[1]];
                        for (int i = 2; i <= len - 2; i++) {
                            if (g->degre_in[chemin[i]] > bestDegre) {
                                bestDegre = g->degre_in[chemin[i]];
                                bestIdx   = i;
                            }
                        }

                        int toBlock = chemin[bestIdx];// selectione larticle qui admet le bigger degree in
                        printf("Article supprime : %s (score:%d)\n",
                               g->articles[toBlock]->titre,
                               g->articles[toBlock]->score_fiabilite);

                        supprime[toBlock] = 1;
                        removed++;
                    }
                }
            }

            printf("Plus aucun chemin de %s vers %s.\n",
                   g->articles[idSrc]->titre,
                   g->articles[idDest]->titre);
            printf("Nombre d'articles supprimes : %d\n", removed);

            free(parent);
            free(visited);
            free(supprime);
        }
    }

    return removed;
}
