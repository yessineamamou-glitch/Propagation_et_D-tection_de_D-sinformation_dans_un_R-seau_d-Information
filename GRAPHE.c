#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ELTARTICLE.h"
#include "LISTE.h"
#include "GRAPHE.h"

/* |--------------------------------------------------------|
   |              les fonctions qui vont nous aider         |
   |--------------------------------------------------------|
*/

int recherche_id(grapheReseau g, int id) {
    int test = 1;
    if (id < 0 || id >= g->V) test = 0;
    if (g->articles[id] == NULL) test = 0;
    return test;
}

int recherche_pos_LS_chaine(LISTE L, ELEMENT art) {
    int pos = 1, t = 0;
    NOEUD p = L->tete;
    while (p != NULL && t == 0) {
        ELEMENT current = p->info;
        if (art->annee > current->annee) { p = p->suivant; pos++; }
        else if (art->annee == current->annee && art->mois > current->mois) { p = p->suivant; pos++; }
        else if (art->annee == current->annee && art->mois == current->mois && art->jour > current->jour) { p = p->suivant; pos++; }
        else if (art->annee == current->annee && art->mois == current->mois && art->jour == current->jour && art->heure > current->heure) { p = p->suivant; pos++; }
        else if (art->annee == current->annee && art->mois == current->mois && art->jour == current->jour && art->heure == current->heure && art->minute > current->minute) { p = p->suivant; pos++; }
        else { t = 1; }
    }
    return pos;
}



/* |--------------------------------------------------------|
   |              functions                                 |
   |--------------------------------------------------------|
*/

grapheReseau createGraph(int V) {
    grapheReseau g = (grapheReseau)malloc(sizeof(structuregraphe));
    if (g == NULL) return NULL;

    g->V = V;
    g->articles = (ELEMENT *)malloc(V * sizeof(ELEMENT));
    g->adjList = (LISTE *)malloc(V * sizeof(LISTE));
    g->degre_in = (int *)malloc(V * sizeof(int));

    if (g->articles == NULL || g->adjList == NULL || g->degre_in == NULL) {
        free(g->articles);
        free(g->adjList);
        free(g->degre_in);
        free(g);
        return NULL;
    }

    for (int i = 0; i < V; i++) {
        g->articles[i] = ELEMENT_VIDE;
        g->adjList[i] = listeCreer();
        g->degre_in[i] = 0;
    }

    return g; // Added missing brace and return
}

void chargerGraph(grapheReseau *g_ptr, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        printf("!!! Fichier introuvable !!!\n");
        return;
    }

    grapheReseau g = createGraph(10000);
    if (g == NULL) {
        fclose(f);
        return;
    }

    char line[500];
    char type;

    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#' || line[0] == '\n') continue;
        sscanf(line, "%c", &type);

        if (type == 'A') {
            int id;
            articleStruct temp;
            sscanf(line, "A %d \"%99[^\"]\" %49s %d %d %d %d %d %d",
                   &temp.id, temp.titre, temp.source, &temp.score_fiabilite,
                   &temp.jour, &temp.mois, &temp.annee, &temp.heure, &temp.minute);

            id = temp.id;
            ELEMENT art = elementCreer(&temp);
            if (art != ELEMENT_VIDE && id < g->V) {
                g->articles[id] = art;
            }
        } else if (type == 'C') {
            int idSrc, idDest;
            sscanf(line, "C %d %d", &idSrc, &idDest);
            ajouterCitation(&g, idSrc, idDest);
        }
    }
    fclose(f);
    *g_ptr = g;
}

void detruireGraph(grapheReseau *g_ptr) {
    grapheReseau g = *g_ptr;
    if (g == NULL) return;
    for (int i = 0; i < g->V; i++) {
        if (g->articles[i]) free(g->articles[i]);
        if (g->adjList[i]) listeDetruire(g->adjList[i]);
    }
    free(g->articles);
    free(g->adjList);
    free(g->degre_in);
    free(g);
    *g_ptr = NULL;
}

void ajouterArticle(grapheReseau *g_ptr, ELEMENT art) {
    grapheReseau g = *g
    _ptr;
    if (g != NULL && art != ELEMENT_VIDE && art->id >= 0 && art->id < g->V) {
        if (g->articles[art->id] == ELEMENT_VIDE) {
          articlesInserer(g,art,art->id);
        }
    }

}

void supprimerArticle(grapheReseau *g_ptr, int idArt) {
    grapheReseau g = *g_ptr;
    if (g == NULL || idArt < 0 || idArt >= g->V || g->articles[idArt] == ELEMENT_VIDE) {
        return;
    }



    // . Supprimer idArt de toutes les citations entrantes et faire -- pour degreein[i] si il est cite par idart
    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE && i != idArt) {
            supprimerCitation(g_ptr, i, idArt);
        }
    }

    // . Nettoyage des listes
    listeDetruire(g->adjList[idArt]);
    g->adjList[idArt] = listeCreer();

    // 4. Libération
    elementDetruire(g->articles[idArt]);
    g->articles[idArt] = ELEMENT_VIDE;
    g->degre_in[idArt] = 0;
}

void ajouterCitation(grapheReseau *g_ptr, int idSrc, int idDest) {
    grapheReseau g = *g_ptr;
    if (g != NULL && recherche_id(g, idSrc) && recherche_id(g, idDest)) {
        int pos = recherche_pos_LS_chaine(g->adjList[idSrc], g->articles[idDest]);
        inserer(g->adjList[idSrc], g->articles[idDest], pos);
        g->degre_in[idDest]++;
    }
}

void supprimerCitation(grapheReseau *g_ptr, int idSrc, int idDest) {
    grapheReseau g = *g_ptr;
    int succes = 0;
    if (g != NULL && idSrc >= 0 && idSrc < g->V && idDest >= 0 && idDest < g->V) {
        if (g->articles[idSrc] != NULL && g->articles[idDest] != NULL) {
            int taille = g->adjList[idSrc]->lg;
            int j = 1;
            while (j <= taille && succes == 0) {
                ELEMENT e = recuperer(g->adjList[idSrc], j);
                if (e != ELEMENT_VIDE && e->id == idDest) {
                    supprimer(g->adjList[idSrc], j);
                    g->degre_in[idDest]--;
                    succes = 1;
                } else {
                    j++;
                }
            }
        }
    }
}

void afficherGraphe(grapheReseau *g_ptr) {
    grapheReseau g = *g_ptr;
    if (g == NULL) {
        printf("Graphe vide\n");
        return;
    }
    printf("=== GRAPHE : %d capacite maximale ===\n", g->V);
    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE) {
            printf("\n[%d] %s | %s | fiabilite:%d | %02d/%02d/%d %02d:%02d",
                g->articles[i]->id, g->articles[i]->titre, g->articles[i]->source,
                g->articles[i]->score_fiabilite, g->articles[i]->jour, g->articles[i]->mois,
                g->articles[i]->annee, g->articles[i]->heure, g->articles[i]->minute
            );
            printf("\n  cite        (%d) : ", g->adjList[i]->lg);
            listeAfficher(g->adjList[i]);
            printf("\n  cite par    (%d articles)", g->degre_in[i]);
        }
    }
    printf("\n==============================\n");
}

void articlesCites(grapheReseau *g_ptr, int idSrc) {
    grapheReseau g = *g_ptr;
    if (g == NULL || idSrc < 0 || idSrc >= g->V || g->articles[idSrc] == ELEMENT_VIDE) {
        printf("!!! Article inexistant !!!\n");
    } else {
        int taille = g->adjList[idSrc]->lg;
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

void articlesCitants(grapheReseau *g_ptr, int idDest) {
    grapheReseau g = *g_ptr;
    if (g == NULL || idDest < 0 || idDest >= g->V || g->articles[idDest] == ELEMENT_VIDE) {
        printf("!!! Article inexistant !!!\n");
    } else {
        int trouve = 0;
        for (int i = 0; i < g->V; i++) {
            if (g->articles[i] != ELEMENT_VIDE) {
                int taille = g->adjList[i]->lg;
                for (int j = 1; j <= taille; j++) {
                    ELEMENT e = recuperer(g->adjList[i], j);
                    if (e != ELEMENT_VIDE && e->id == idDest) {
                        printf("--> %s\n", g->articles[i]->titre);
                        trouve = 1;
                    }
                }
            }
        }
        if (trouve == 0) printf("!!! Aucun article ne cite celui-ci !!!\n");
    }
}

void sourcesOriginales(grapheReseau *g_ptr) {
    grapheReseau g = *g_ptr;
    if (g == NULL) {
        printf("!!! Graphe vide !!!\n");
    } else {
        int trouve = 0;
        for (int i = 0; i < g->V; i++) {
            if (g->articles[i] != ELEMENT_VIDE && g->adjList[i]->lg == 0) {
                printf("--> %s\n", g->articles[i]->titre);
                trouve = 1;
            }
        }
        if (trouve == 0) printf("!!! Aucune source originale !!!\n");
    }
}

void articlesIsoles(grapheReseau *g_ptr) {
    grapheReseau g = *g_ptr;
    if (g == NULL) {
        printf("!!! Graphe vide !!!\n");
    } else {
        int trouve = 0;
        for (int i = 0; i < g->V; i++) {
            if (g->articles[i] != ELEMENT_VIDE && g->degre_in[i] == 0) {
                printf("--> %s\n", g->articles[i]->titre);
                trouve = 1;
            }
        }
        if (trouve == 0) printf("!!! Aucun article isole !!!\n");
    }
}

ELEMENT articlePlusCite(grapheReseau *g_ptr) {
    grapheReseau g = *g_ptr;
    ELEMENT result = ELEMENT_VIDE;
    int maxDegre = -1;

    if (g != NULL) {
        for (int i = 0; i < g->V; i++) {
            if (g->articles[i] != ELEMENT_VIDE && g->degre_in[i] > maxDegre) {
                maxDegre = g->degre_in[i];
                result = g->articles[i];
            }
        }
        for(int i = 0; i < g->V; i++) {
            if(g->articles[i] != ELEMENT_VIDE && g->degre_in[i] == maxDegre) {//si il ya 1 ou plus max
                elementAfficher(g->articles[i]);
            }
        }
    }
    return result;
}




int comparerDates(ELEMENT art1, ELEMENT art2) {
    int resultat;

    if (art1->annee != art2->annee) {
        resultat = art1->annee - art2->annee;
    } else if (art1->mois != art2->mois) {
        resultat = art1->mois - art2->mois;
    } else if (art1->jour != art2->jour) {
        resultat = art1->jour - art2->jour;
    } else if (art1->heure != art2->heure) {
        resultat = art1->heure - art2->heure;
    } else {
        resultat = art1->minute - art2->minute;
    }

    return resultat;
}
void trierParDate(grapheReseau *g_ptr) {
    grapheReseau g = *g_ptr;
    if (g == NULL) {
        printf("!!! Graphe vide !!!\n");
    } else {
        ELEMENT *temp = (ELEMENT *)malloc(g->V * sizeof(ELEMENT));
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
                elementAfficher(temp[i]);
            }
        }
        free(temp);
    }
}

ELEMENT premierCitant(grapheReseau *g_ptr, int idDest) {
    grapheReseau g = *g_ptr;
    int execution_valide = 1;
    int i = 0;
    ELEMENT premier = ELEMENT_VIDE;

    if (g == NULL || idDest < 0 || idDest >= g->V || g->articles[idDest] == ELEMENT_VIDE) {
        printf("!!! Article inexistant ou graphe invalide !!!\n");
        execution_valide = 0;
    }

    if (execution_valide == 1) {
        for (i = 0; i < g->V; i++) {
            if (g->articles[i] != ELEMENT_VIDE) {
                int cite_cible = 0;
                int taille = g->adjList[i]->lg;
                int j = 1;
                while (j <= taille && cite_cible == 0) {
                    ELEMENT e = recuperer(g->adjList[i], j);
                    if (e != ELEMENT_VIDE && e->id == idDest) {
                        cite_cible = 1;
                    }
                    j++;
                }
                if (cite_cible == 1) {
                    if (premier == ELEMENT_VIDE) {
                        premier = g->articles[i];
                    } else {
                        if (comparerDates(g->articles[i], premier) < 0) {
                            premier = g->articles[i];
                        }
                    }
                }
            }
        }
        if (premier != ELEMENT_VIDE) {
            printf("--> ");
            elementAfficher(premier);
        } else {
            printf("!!! Aucun article ne cite celui-ci !!!\n");
        }
    }
    return premier; /* <-- seul ajout */
}

ELEMENT premierCitant(grapheReseau *g_ptr, int idDest) {
    grapheReseau g = *g_ptr;
    int i = 0;
    ELEMENT premier = ELEMENT_VIDE;

    /* Verification de la validite de l'article cible */
    if (g == NULL || idDest < 0 || idDest >= g->V ||
        g->articles[idDest] == ELEMENT_VIDE) {
        printf("!!! Article inexistant ou graphe invalide !!!\n");
        return ELEMENT_VIDE;
    }

    /* Parcourir TOUS les articles du graphe */
    for (i = 0; i < g->V; i++) {

        /* Ignorer les cases vides */
        if (g->articles[i] != ELEMENT_VIDE) {
            int cite_cible = 0;
            int taille = g->adjList[i]->lg;
            int j = 1;

            /* Parcourir la liste des citations de l'article i */
            /* On cherche si l'article i cite idDest */
            while (j <= taille && cite_cible == 0) {
                ELEMENT e = recuperer(g->adjList[i], j);
                if (e != ELEMENT_VIDE && e->id == idDest) {
                    cite_cible = 1; /* oui, l'article i cite idDest */
                }
                j++;
            }

            /* Si l'article i cite idDest, on le compare au meilleur actuel */
            if (cite_cible == 1) {
                if (premier == ELEMENT_VIDE) {
                    /* Premier candidat trouve, on le garde */
                    premier = g->articles[i];
                } else {
                    /* On garde le plus ancien des deux */
                    if (comparerDates(g->articles[i], premier) < 0) {
                        premier = g->articles[i];
                    }
                }
            }
        }
    }

    /* Affichage du resultat */
    if (premier != ELEMENT_VIDE) {
        printf("--> ");
        elementAfficher(premier);
    } else {
        printf("!!! Aucun article ne cite celui-ci !!!\n");
    }

    return premier;
}

void chainerPropagation(grapheReseau *g_ptr, int idSrc) {
    grapheReseau g = *g_ptr;

    // Vérification de sécurité
    if (g == NULL || idSrc < 0 || idSrc >= g->V || g->articles[idSrc] == ELEMENT_VIDE) {
        printf("Article source invalide.\n");
        return;
    }

    // 1. Initialisation du tableau de présence (0 et 1)
    // tab_helper[i] == 1 signifie que l'article i appartient à la chaîne
    int *tab_helper = (int *)calloc(g->V, sizeof(int));
    if (tab_helper == NULL) return;

    tab_helper[idSrc] = 1; // La source est le point de départ

    // 2. Création du tableau qui va stocker les articles trouvés
    // On alloue V car au maximum tous les articles pourraient être dans la chaîne
    ELEMENT *sorted_articles = (ELEMENT *)malloc(g->V * sizeof(ELEMENT));
    if (sorted_articles == NULL) { free(tab_helper); return; }

    int nb_trouves = 0;
    // On ajoute la source comme premier élément du tableau à afficher
    sorted_articles[nb_trouves++] = g->articles[idSrc];///////////////////////////////////////////////

    // 3. Remplissage du tableau selon la propagation
    // On parcourt les articles pour voir qui cite qui
    // Note: On peut boucler plusieurs fois pour s'assurer de capter les citations indirectes (gràce à mod=1)
    int modification = 1;
    while (modification) {
        modification = 0;
        for (int i = 0; i < g->V; i++) {
            // Si l'article i n'est pas encore dans notre chaîne (tab_helper[i] == 0)
            if (g->articles[i] != ELEMENT_VIDE && tab_helper[i] == 0) {

                // On regarde ses citations dans sa liste d'adjacence
                LISTE citations = g->adjList[i];
                int trouve_citation = 0;

                for (int j = 1; j <= citations->lg && !trouve_citation; j++) {
                    ELEMENT art_cite = recuperer(citations, j);
                    // Si l'article i cite un article qui est déjà marqué à 1
                    if (art_cite != ELEMENT_VIDE && tab_helper[art_cite->id] == 1) {
                        tab_helper[i] = 1;        // On le marque à 1
                        sorted_articles[nb_trouves++] = g->articles[i]; // On l'ajoute au tableau
                        modification = 1;         // On a trouvé un nouveau maillon
                        trouve_citation = 1;
                    }
                }
            }
        }
    }

    // 4. TRI DU TABLEAU FINAL (Sauf la source qui reste en tête)
    // On trie les articles trouvés (du 2ème au dernier) par date
    // même principe pour la fonction  trierpar date juste afin que laffichage correspond à lennonce
    for (int i = 1; i < nb_trouves; i++) {
        ELEMENT key = sorted_articles[i];
        int j = i - 1;
        // On utilise la fonction comparerDates pour le tri
        while (j >= 1 && comparerDates(sorted_articles[j], key) > 0) {
            sorted_articles[j + 1] = sorted_articles[j];
            j--;

        }
        sorted_articles[j + 1] = key;
    }

    // 5. AFFICHAGE DU TABLEAU TRIÉ
    printf("\n--- Chaine de Propagation ---\n");
    // Affichage de l'article source
    printf("%s\n", sorted_articles[0]->titre);

    // Affichage des articles qui ont propagé l'info
    for (int i = 1; i < nb_trouves; i++) {
       printf("--> cite par %s (%02d/%02d/%d %02dh%02d)\n",
            sorted_articles[i]->titre,
            sorted_articles[i]->jour,
            sorted_articles[i]->mois,
            sorted_articles[i]->annee,
            sorted_articles[i]->heure,
            sorted_articles[i]->minute);
    }

    // Libération de la mémoire locale
    free(tab_helper);
    free(sorted_articles);
}
void simulerPropagation(grapheReseau *g_ptr, int idSrc)
{
    grapheReseau g = *g_ptr;
    int *visited = NULL;
    LISTE file = NULL;

    // Single entry check for safety
    if (g == NULL || idSrc < 0 || idSrc >= g->V || g->articles[idSrc] == ELEMENT_VIDE) {
        printf("!!! Article source invalide !!!\n");
    } else {
        visited = (int *)calloc(g->V, sizeof(int));
        if (visited != NULL) {
            file = listeCreer();
            if (file != NULL) {

                visited[idSrc] = 1;
                inserer(file, g->articles[idSrc], listeTaille(file) + 1);

                int total_articles = 0;
                int niveau_actuel = 0;

                printf("\n--- Simulation de Propagation depuis [%d] %s ---\n",
                       idSrc, g->articles[idSrc]->titre);

                while (!estVide(file)) {
                    // Logic fix: Capture how many articles are at this specific level
                    int nb_articles_niveau = listeTaille(file);

                    printf("Niveau %d : ", niveau_actuel);
                    int premier = 1;

                    // Process only the articles belonging to the current level
                    for (int k = 0; k < nb_articles_niveau; k++) {
                        ELEMENT courant = recuperer(file, 1);
                        supprimer(file, 1);
                        total_articles++;

                        if (!premier) printf(", ");
                        printf("%s", courant->titre);
                        premier = 0;

                        int idCourant = courant->id;

                        // Scan the graph for articles citing 'idCourant'
                        for (int i = 0; i < g->V; i++) {
                            if (g->articles[i] != ELEMENT_VIDE && !visited[i]) {
                                int taille_adj = listeTaille(g->adjList[i]);
                                int cite = 0;
                                for (int j = 1; j <= taille_adj && !cite; j++) {
                                    ELEMENT e = recuperer(g->adjList[i], j);
                                    if (e != ELEMENT_VIDE && e->id == idCourant)
                                        cite = 1;
                                }

                                if (cite) {
                                    visited[i] = 1;
                                    inserer(file, g->articles[i], listeTaille(file) + 1);
                                }
                            }
                        }
                    }
                    printf("\n");

                    // Increment level only after the entire "batch" is done
                    if (!estVide(file)) {
                        niveau_actuel++;
                    }
                }

                printf("%d niveau(x), %d article(s) atteint(s).\n",
                       niveau_actuel + 1, total_articles);

                listeDetruire(file);
            }
            free(visited);
        }
    }
}

void articlesAccessibles(grapheReseau *g_ptr, int idSrc)
{
    grapheReseau g = *g_ptr;

    if (g == NULL || idSrc < 0 || idSrc >= g->V ||
        g->articles[idSrc] == ELEMENT_VIDE) {
        printf("!!! Article source invalide !!!\n");
        return;
    }

    int *visited = (int *)calloc(g->V, sizeof(int));
    if (visited == NULL) return;

    LISTE file = listeCreer();

    visited[idSrc] = 1;
    inserer(file, g->articles[idSrc], listeTaille(file) + 1);

    while (!estVide(file)) {
        ELEMENT courant = recuperer(file, 1);
        supprimer(file, 1);

        int idCourant = courant->id;

        for (int i = 0; i < g->V; i++) {
            if (g->articles[i] != ELEMENT_VIDE && !visited[i]) {
                int taille = g->adjList[i]->lg;
                int cite   = 0;
                for (int j = 1; j <= taille && !cite; j++) {
                    ELEMENT e = recuperer(g->adjList[i], j);
                    if (e != ELEMENT_VIDE && e->id == idCourant) {
                        cite = 1;
                    }
                }
                if (cite) {
                    visited[i] = 1;
                    inserer(file, g->articles[i], listeTaille(file) + 1);
                }
            }
        }
    }

    listeDetruire(file);

    printf("\n--- Articles accessibles depuis [%d] %s ---\n",
           idSrc, g->articles[idSrc]->titre);

    /* collecter dans un tableau temporaire pour trier par date */
    ELEMENT *temp = (ELEMENT *)malloc(g->V * sizeof(ELEMENT));
    int count = 0;

    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE && visited[i]) {
            temp[count++] = g->articles[i];
        }
    }

    /* tri par insertion par date */
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
        elementAfficher(temp[i]);
    }

    free(temp);
    free(visited);
}





void simulerSuppression(grapheReseau *g_ptr, int idArt)
{
    grapheReseau g = *g_ptr;

    if (g == NULL || idArt < 0 || idArt >= g->V ||
        g->articles[idArt] == ELEMENT_VIDE) {
        printf("!!! Article inexistant !!!\n");
        return;
    }

    printf("\nSuppression de %s :\n", g->articles[idArt]->titre);

    /* --- ce que idArt citait --- */
    int nb_cites = g->adjList[idArt]->lg;
    printf("- Citait (%d) : ", nb_cites);
    for (int j = 1; j <= nb_cites; j++) {
        ELEMENT e = recuperer(g->adjList[idArt], j);
        if (e != ELEMENT_VIDE) {
            printf("%s ", e->titre);
        }
    }
    printf("\n");

    /* --- ce qui citait idArt + on marque leurs ids --- */
    int *citants = (int *)calloc(g->V, sizeof(int));
    int nb_citants = g->degre_in[idArt];
    printf("- Etait cite par (%d) : ", nb_citants);
    int trouve = nb_citants;
    int i = 0;
    while (trouve != 0 && i < g->V) {
        if (g->articles[i] != ELEMENT_VIDE && i != idArt) {
            int taille = g->adjList[i]->lg;
            int cite = 0;
            for (int j = 1; j <= taille && !cite; j++) {
                ELEMENT e = recuperer(g->adjList[i], j);
                if (e != ELEMENT_VIDE && e->id == idArt) {
                    cite = 1;
                }
            }
            if (cite) {
                printf("%s ", g->articles[i]->titre);
                citants[i] = 1;  /* on marque */
                trouve--;
            }
        }
        i++;
    }
    printf("\n");

    /* --- articles deconnectes = reutiliser citants[] --- */
    printf("- Articles deconnectes : ");
    for (int k = 0; k < g->V; k++) {
        if (citants[k] == 1) {
            printf("%s ", g->articles[k]->titre);
        }
    }
    printf("\n");

    free(citants);

    /* --- suppression effective --- */
    supprimerArticle(g_ptr, idArt);
}


















