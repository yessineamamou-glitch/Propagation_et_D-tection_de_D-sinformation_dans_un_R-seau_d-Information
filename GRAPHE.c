#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "GRAPHE.h"

grapheReseau createGraph(int V) {
    grapheReseau g;
    g.nombre_articles = V;
    g.articles = (ELEMENT *)malloc(V * sizeof(ELEMENT));  // lena sta3malna . fi blasset -> khatr grapheReseau esem structure mouch pointeur
    g.adjacence = (Liste *)malloc(V * sizeof(Liste));

    int i = 0;
    while (i < V) {
        g.articles[i] = NULL;
        g.adjacence[i] = creerListeVide();
        i++;
    }

    return g;
}

void chargerGraph(grapheReseau *g, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Fichier %s non trouve.\n", filename);
        return;
    }

    char ligne[256];
    int compte_articles = 0;

    while (fgets(ligne, sizeof(ligne), file) != NULL) {
        if (ligne[0] == 'A') {
            int id, score, jour, mois, annee, heure, minute;
            char titre[100], source[50];
            sscanf(ligne, "A %d %s %s %d %d %d %d %d %d",
                   &id, titre, source, &score, &jour, &mois, &annee, &heure, &minute);

            if (compte_articles < g->nombre_articles) {
                g->articles[compte_articles] = creerElement(id, titre, source, score, jour, mois, annee, heure, minute);
                compte_articles++;
            }
        }
        else if (ligne[0] == 'C') {
            int id_src, id_dest;
            sscanf(ligne, "C %d %d", &id_src, &id_dest);

            ELEMENT elem = creerElement(id_dest, "", "", 0, 0, 0, 0, 0, 0);
            g->adjacence[id_src] = inserer(g->adjacence[id_src], elem);
            free(elem);
        }
    }

    fclose(file);
    printf("Graphe charge avec succes depuis %s\n", filename);
}

void detruireGraph(grapheReseau *g) {
    int i = 0;
    while (i < g->nombre_articles) {
        g->adjacence[i] = detruireListe(g->adjacence[i]);
        if (g->articles[i] != NULL) {
            free(g->articles[i]);
        }
        i++;
    }

    free(g->articles);
    free(g->adjacence);
}

void ajouterArticle(grapheReseau *g, ELEMENT art) {
    int i = 0;
    int trouve = 0;

    while (i < g->nombre_articles && trouve == 0) {
        if (g->articles[i] == NULL) {
            g->articles[i] = copierElement(art);
            trouve = 1;
        }
        i++;
    }

    if (trouve == 1) {
        sauvegarderGraphe(g, "fichier.txt");
        printf("Article ajoute avec succes et fichier sauvegarde.\n");
    }
    else {
        printf("Impossible d'ajouter l'article (graphe plein).\n");
    }
}

void supprimerArticle(grapheReseau *g, int idArt) {
    int i = 0;
    while (i < g->nombre_articles) {
        if (i != idArt) {
            g->adjacence[i] = supprimerElement(g->adjacence[i], idArt);
        }
        i++;
    }

    g->adjacence[idArt] = detruireListe(g->adjacence[idArt]);
    g->adjacence[idArt] = creerListeVide();

    if (g->articles[idArt] != NULL) {
        free(g->articles[idArt]);
        g->articles[idArt] = NULL;
    }

    sauvegarderGraphe(g, "fichier.txt");
    printf("Article supprime avec succes et fichier sauvegarde.\n");
}

void ajouterCitation(grapheReseau *g, int idDrc, int idDest) {
    ELEMENT elem = creerElement(idDest, "", "", 0, 0, 0, 0, 0, 0);
    g->adjacence[idDrc] = inserer(g->adjacence[idDrc], elem);
    free(elem);
    sauvegarderGraphe(g, "fichier.txt");
    printf("Citation ajoutee avec succes et fichier sauvegarde.\n");
}

void supprimerCitation(grapheReseau *g, int idDrc, int idDest) {
    g->adjacence[idDrc] = supprimerElement(g->adjacence[idDrc], idDest);
    sauvegarderGraphe(g, "fichier.txt");
    printf("Citation supprimee avec succes et fichier sauvegarde.\n");
}

void afficherGraphe(grapheReseau *g) {
    printf("\n");
    printf("================================================\n");
    printf("              AFFICHAGE DU RESEAU\n");
    printf("================================================\n\n");

    int i = 0;
    while (i < g->nombre_articles) {
        if (g->articles[i] != NULL) {
            afficherElement(g->articles[i]);

            printf("  Citations: ");
            if (estVide(g->adjacence[i])) {
                printf("Aucune\n");
            }
            else {
                Liste courant = g->adjacence[i];
                while (courant != NULL) {
                    printf("%d ", courant->donnee->id);
                    courant = courant->suivant;
                }
                printf("\n");
            }
        }
        i++;
    }

    printf("================================================\n\n");
}

void articlesCites(grapheReseau *g, int idDrc) {
    printf("\n");
    printf("Articles cites par l'article %d:\n", idDrc);

    if (estVide(g->adjacence[idDrc])) {
        printf("Aucun article cite.\n");
    }
    else {
        Liste courant = g->adjacence[idDrc];
        while (courant != NULL) {
            if (g->articles[courant->donnee->id] != NULL) {
                afficherElement(g->articles[courant->donnee->id]);
            }
            courant = courant->suivant;
        }
    }
    printf("\n");
}

void articlesCitants(grapheReseau *g, int idDest) {
    printf("\n");
    printf("Articles qui citent l'article %d:\n", idDest);

    int trouve = 0;
    int i = 0;
    while (i < g->nombre_articles) {
        if (g->articles[i] != NULL) {
            Liste courant = g->adjacence[i];
            while (courant != NULL) {
                if (courant->donnee->id == idDest) {
                    afficherElement(g->articles[i]);
                    trouve = 1;
                }
                courant = courant->suivant;
            }
        }
        i++;
    }

    if (trouve == 0) {
        printf("Aucun article ne cite cet article.\n");
    }
    printf("\n");
}

void sourcesOriginales(grapheReseau *g) {
    printf("\n");
    printf("Sources originales (articles non cites):\n");

    int i = 0;
    int compte = 0;
    while (i < g->nombre_articles) {
        if (g->articles[i] != NULL) {
            int degre_entrant = 0;
            int j = 0;
            while (j < g->nombre_articles) {
                if (g->articles[j] != NULL) {
                    Liste courant = g->adjacence[j];
                    while (courant != NULL) {
                        if (courant->donnee->id == i) {
                            degre_entrant++;
                        }
                        courant = courant->suivant;
                    }
                }
                j++;
            }

            if (degre_entrant == 0) {
                afficherElement(g->articles[i]);
                compte++;
            }
        }
        i++;
    }

    if (compte == 0) {
        printf("Aucune source originale.\n");
    }
    printf("\n");
}

void articlesIsoles(grapheReseau *g) {
    printf("\n");
    printf("Articles isoles:\n");

    int i = 0;
    int compte = 0;
    while (i < g->nombre_articles) {
        if (g->articles[i] != NULL) {
            int degre_sortant = obtenirTaille(g->adjacence[i]);
            int degre_entrant = 0;

            int j = 0;
            while (j < g->nombre_articles) {
                if (g->articles[j] != NULL) {
                    Liste courant = g->adjacence[j];
                    while (courant != NULL) {
                        if (courant->donnee->id == i) {
                            degre_entrant++;
                        }
                        courant = courant->suivant;
                    }
                }
                j++;
            }

            if (degre_sortant == 0 && degre_entrant == 0) {
                afficherElement(g->articles[i]);
                compte++;
            }
        }
        i++;
    }

    if (compte == 0) {
        printf("Aucun article isole.\n");
    }
    printf("\n");
}

ELEMENT articlePlusCite(grapheReseau *g) {
    ELEMENT plus_cite = ELEMENT_VIDE;

    int max_citations = -1;
    int i = 0;

    while (i < g->nombre_articles) {
        if (g->articles[i] != ELEMENT_VIDE) {
            int degre_entrant = 0;
            int j = 0;

            while (j < g->nombre_articles) {
                if (g->articles[j] != ELEMENT_VIDE) {
                    Liste courant = g->adjacence[j];
                    while (courant != ELEMENT_VIDE) {
                        if (courant->donnee->id == i) {
                            degre_entrant++;
                        }
                        courant = courant->suivant;
                    }
                }
                j++;
            }

            if (degre_entrant > max_citations) {
                max_citations = degre_entrant;
                plus_cite = copierElement(g->articles[i]);
            }
        }
        i++;
    }

    return plus_cite;
}

void trierParDate(grapheReseau *g) {
    printf("\n");
    printf("Articles tries par date de publication:\n");
    printf("================================================\n\n");

    int i = 0;
    int j;
    ELEMENT temp;

    while (i < g->nombre_articles - 1) {
        j = 0;
        while (j < g->nombre_articles - i - 1) {
            int compare = 0;

            if (g->articles[j] != NULL && g->articles[j+1] != NULL) {
                if (g->articles[j]->annee > g->articles[j+1]->annee) {
                    compare = 1;
                }
                else if (g->articles[j]->annee == g->articles[j+1]->annee) {
                    if (g->articles[j]->mois > g->articles[j+1]->mois) {
                        compare = 1;
                    }
                    else if (g->articles[j]->mois == g->articles[j+1]->mois) {
                        if (g->articles[j]->jour > g->articles[j+1]->jour) {
                            compare = 1;
                        }
                    }
                }

                if (compare == 1) {
                    temp = g->articles[j];
                    g->articles[j] = g->articles[j+1];
                    g->articles[j+1] = temp;
                }
            }
            j++;
        }
        i++;
    }

    i = 0;
    while (i < g->nombre_articles) {
        if (g->articles[i] != NULL) {
            afficherElement(g->articles[i]);
        }
        i++;
    }

    printf("================================================\n\n");
}

void chainerPropagation(grapheReseau *g, int idDrc) {
    printf("\n");
    printf("Chaine de propagation depuis l'article %d:\n", idDrc);
    printf("Article source: %s\n", g->articles[idDrc]->titre);
    printf("|\n");

    Liste courant = g->adjacence[idDrc];
    while (courant != NULL) {
        printf("v\n");
        printf("Article cité: %s (ID: %d)\n",
               g->articles[courant->donnee->id]->titre,
               g->articles[courant->donnee->id]->id);
        courant = courant->suivant;
    }

    printf("\n");
}

void simulerPropagation(grapheReseau *g, int idDrc) {
    printf("\n");
    printf("Simulation de propagation (BFS) depuis l'article %d:\n", idDrc);
    printf("================================================\n\n");

    printf("Article initial: %s (ID: %d, Score: %d)\n\n",
           g->articles[idDrc]->titre,
           g->articles[idDrc]->id,
           g->articles[idDrc]->score_fiabilite);

    int *visite = (int *)malloc(g->nombre_articles * sizeof(int));
    int i = 0;
    while (i < g->nombre_articles) {
        visite[i] = 0;
        i++;
    }

    int *file = (int *)malloc(g->nombre_articles * sizeof(int));
    int debut = 0;
    int fin = 0;

    file[fin] = idDrc;
    fin++;
    visite[idDrc] = 1;

    printf("Articles accessibles:\n");

    while (debut < fin) {
        int courant_id = file[debut];
        debut++;

        if (courant_id != idDrc) {
            afficherElement(g->articles[courant_id]);
        }

        Liste adj = g->adjacence[courant_id];
        while (adj != NULL) {
            if (visite[adj->donnee->id] == 0) {
                visite[adj->donnee->id] = 1;
                file[fin] = adj->donnee->id;
                fin++;
            }
            adj = adj->suivant;
        }
    }

    printf("\n================================================\n\n");

    free(visite);
    free(file);
}

void analyserReseau(grapheReseau *g) {
    printf("\n");
    printf("================================================\n");
    printf("    ANALYSE DES ARTICLES SUSPECTS\n");
    printf("================================================\n\n");

    char mots_suspects[8][50] = {
        "cache",
        "urgent",
        "exclusif",
        "censure",
        "complot",
        "secret",
        "interdit",
        "choc"
    };

    int i = 0;
    while (i < g->nombre_articles) {
        if (g->articles[i] != NULL) {
            int score_suspicion = 0;
            int j = 0;

            while (j < 8) {
                if (strstr(g->articles[i]->titre, mots_suspects[j]) != NULL) {
                    score_suspicion = score_suspicion + 10;
                }
                j++;
            }

            if (g->articles[i]->score_fiabilite < 50) {
                score_suspicion = score_suspicion + 20;
            }

            if (score_suspicion >= 30) {
                printf("[SUSPECT] ID: %d | Titre: %s\n",
                       g->articles[i]->id,
                       g->articles[i]->titre);
                printf("          Score de suspicion: %d%%\n", score_suspicion);
                printf("          Score de fiabilite: %d%%\n\n",
                       g->articles[i]->score_fiabilite);
            }
        }
        i++;
    }

    printf("================================================\n\n");
}

void simulerSuppression(grapheReseau *g, int idArt) {
    printf("\n");
    printf("Simulation de suppression de l'article %d:\n", idArt);
    printf("================================================\n\n");

    int articles_affectes = 0;
    int i = 0;

    while (i < g->nombre_articles) {
        if (i != idArt && g->articles[i] != NULL) {
            Liste courant = g->adjacence[i];
            while (courant != NULL) {
                if (courant->donnee->id == idArt) {
                    articles_affectes++;
                }
                courant = courant->suivant;
            }
        }
        i++;
    }

    printf("Article a supprimer: %s (ID: %d)\n",
           g->articles[idArt]->titre,
           g->articles[idArt]->id);
    printf("Nombre d'articles affectes: %d\n", articles_affectes);
    printf("Impact: %s\n",
           articles_affectes > 0 ? "Propagation reduite" : "Aucun impact");

    printf("================================================\n\n");
}

int neutraliserPropagation(grapheReseau *g, int idDrc, int idDest) {
    int supprime = 0;

    Liste courant = g->adjacence[idDrc];
    while (courant != NULL) {
        if (courant->donnee->id == idDest) {
            g->adjacence[idDrc] = supprimerElement(g->adjacence[idDrc], idDest);
            supprime = 1;
        }
        courant = courant->suivant;
    }

    if (supprime == 1) {
        sauvegarderGraphe(g, "fichier.txt");
        printf("Chemin de propagation neutralise et fichier sauvegarde.\n");
    }
    else {
        printf("Aucun chemin direct a neutraliser.\n");
    }

    return supprime;
}

void sauvegarderGraphe(grapheReseau *g, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Erreur: Impossible de sauvegarder le fichier.\n");
        return;
    }

    int i = 0;
    while (i < g->nombre_articles) {
        if (g->articles[i] != NULL) {
            fprintf(file, "A %d %s %s %d %d %d %d %d %d\n",
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
        i++;
    }

    i = 0;
    while (i < g->nombre_articles) {
        if (g->articles[i] != NULL) {
            Liste courant = g->adjacence[i];
            while (courant != NULL) {
                fprintf(file, "C %d %d\n", i, courant->donnee->id);
                courant = courant->suivant;
            }
        }
        i++;
    }

    fclose(file);
}
