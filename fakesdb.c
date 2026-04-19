#include "FAKESDB.h"
#include "GRAPHE.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

int contientMotClef(char *texte, const char *motclef) {
    char *copie = (char *)malloc(strlen(texte) + 1);
    strcpy(copie, texte);

    for (int i = 0; copie[i]; i++) {
        copie[i] = tolower(copie[i]);
        if (copie[i] == '_') copie[i] = ' ';
    }

    int result = strstr(copie, motclef) != NULL ? 1 : 0;
    free(copie);
    return result;
}

int analyserArticle(ELEMENT art) {
    int score_suspicion = 0;

    if (art != ELEMENT_VIDE) {
        for (int i = 0; i < NB_FAKES; i++) {
            if (contientMotClef(art->titre, BASE_FAKES[i])) {
                score_suspicion += 40;
            }
        }

        for (int i = 0; i < NB_SUSPECTS; i++) {
            if (contientMotClef(art->titre, MOTS_SUSPECTS[i])) {
                score_suspicion += 10;
            }
        }

        art->score_fiabilite = score_suspicion > 100 ? 0 : 100 - score_suspicion;
    }

    return score_suspicion;
}

void analyserReseau(grapheReseau g) {
    if (g == NULL) {
        printf("!!! Graphe vide !!!\n");
    } else {
        for (int i = 0; i < g->V; i++) {
            if (g->articles[i] != ELEMENT_VIDE) {
                int score_susp = analyserArticle(g->articles[i]);

                if (g->articles[i]->score_fiabilite < 40) {
                    printf("[SUSPECT] %s (score: %d)\n", g->articles[i]->titre, g->articles[i]->score_fiabilite);
                    for (int j = 0; j < NB_FAKES; j++) {
                        if (contientMotClef(g->articles[i]->titre, BASE_FAKES[j])) {
                            printf("          \"%s\"\n", BASE_FAKES[j]);
                        }
                    }
                    for (int j = 0; j < NB_SUSPECTS; j++) {
                        if (contientMotClef(g->articles[i]->titre, MOTS_SUSPECTS[j])) {
                            printf("          \"%s\"\n", MOTS_SUSPECTS[j]);
                        }
                    }
                } else if (g->articles[i]->score_fiabilite < 70) {
                    printf("[DOUTEUX] %s (score: %d)\n", g->articles[i]->titre, g->articles[i]->score_fiabilite);
                } else {
                    printf("[FIABLE ] %s (score: %d)\n", g->articles[i]->titre, g->articles[i]->score_fiabilite);
                }
            }
        }
    }
}

void articlesSuspectsCites(grapheReseau g) {
    if (g == NULL) {
        printf("!!! Graphe vide !!!\n");
    } else {
        int *suspects = (int *)malloc(g->V * sizeof(int));
        int count = 0;

        for (int i = 0; i < g->V; i++) {
            if (g->articles[i] != ELEMENT_VIDE && g->articles[i]->score_fiabilite < 40 && g->degre_in[i] > 0) {
                suspects[count] = i;
                count++;
            }
        }

        if (count == 0) {
            printf("!!! Aucun article suspect cite !!!\n");
        } else {
            for (int i = 0; i < count - 1; i++) {
                for (int j = i + 1; j < count; j++) {
                    if (g->degre_in[suspects[i]] < g->degre_in[suspects[j]]) {
                        int temp = suspects[i];
                        suspects[i] = suspects[j];
                        suspects[j] = temp;
                    }
                }
            }

            for (int i = 0; i < count; i++) {
                printf("%s (score:%d, cite par %d articles)\n",
                       g->articles[suspects[i]]->titre,
                       g->articles[suspects[i]]->score_fiabilite,
                       g->degre_in[suspects[i]]);
            }
        }

        free(suspects);
    }
}
