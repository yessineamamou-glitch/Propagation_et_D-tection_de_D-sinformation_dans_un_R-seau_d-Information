#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "GRAPHE.h"

void afficherMenuPrincipal() {
    printf("\n\n");
    printf("          ========================================\n");
    printf("          |  RESEAU D'INFORMATION ET ANALYSE      |\n");
    printf("          |  DE DESINFORMATION EN TUNISIE         |\n");
    printf("          |=======================================|\n");
    printf("          |                                       |\n");
    printf("          |  1.  Charger reseau depuis fichier    |\n");
    printf("          |  2.  Ajouter un article               |\n");
    printf("          |  3.  Ajouter une citation             |\n");
    printf("          |  4.  Supprimer un article             |\n");
    printf("          |  5.  Supprimer une citation           |\n");
    printf("          |  6.  Afficher le reseau               |\n");
    printf("          |  7.  Articles cites par un article    |\n");
    printf("          |  8.  Articles qui citent un article   |\n");
    printf("          |  9.  Sources originales / Isoles      |\n");
    printf("          |  10. Article le plus cite             |\n");
    printf("          |  11. Trier par date de publication    |\n");
    printf("          |  12. Afficher chaine de propagation   |\n");
    printf("          |  13. Simulation de propagation (BFS)  |\n");
    printf("          |  14. Analyser les articles            |\n");
    printf("          |  15. Analyser les articles (fakes)    |\n");
    printf("          |  16. Articles suspects les plus cites |\n");
    printf("          |  17. [BONUS] Simuler suppression      |\n");
    printf("          |  18. [BONUS] Neutraliser propagation  |\n");
    printf("          |  0.  Quitter                          |\n");
    printf("          |                                       |\n");
    printf("          |=======================================|\n");
    printf("          Entrez votre choix: ");
}

int saisirEntier() {
    int valeur;
    int resultat =scanf("%d", &valeur);

    while (resultat != 1) {
        while (getchar() != '\n');
        printf("          Erreur: Veuillez entrer un nombre valide.\n");
        printf("          Reessayez: ");
        resultat = scanf("%d", &valeur);
    }

    while (getchar() != '\n');

    return valeur;
}

int saisirId(grapheReseau *g) {
    int id = saisirEntier();

    while (id < 0 || id >= g->nombre_articles) {
        printf("          Erreur: ID hors limites (0-%d).\n", g->nombre_articles - 1);
        printf("          Reessayez: ");
        id = saisirEntier();
    }

    return id;
}

int saisirScore() {
    int score = saisirEntier();

    while (score < 0 || score > 100) {
        printf("          Erreur: Le score doit etre entre 0 et 100.\n");
        printf("          Reessayez: ");
        score = saisirEntier();
    }

    return score;
}

int saisirDate(int type) {
    int valeur = saisirEntier();

    if (type == 1) {
        while (valeur < 1 || valeur > 31) {
            printf("          Erreur: Le jour doit etre entre 1 et 31.\n");
            printf("          Reessayez: ");
            valeur = saisirEntier();
        }
    }
    else if (type == 2) {
        while (valeur < 1 || valeur > 12) {
            printf("          Erreur: Le mois doit etre entre 1 et 12.\n");
            printf("          Reessayez: ");
            valeur = saisirEntier();
        }
    }
    else if (type == 3) {
        while (valeur < 2020 || valeur > 2030) {
            printf("          Erreur: L'annee doit etre entre 2020 et 2030.\n");
            printf("          Reessayez: ");
            valeur = saisirEntier();
        }
    }
    else if (type == 4) {
        while (valeur < 0 || valeur > 23) {
            printf("          Erreur: L'heure doit etre entre 0 et 23.\n");
            printf("          Reessayez: ");
            valeur = saisirEntier();
        }
    }
    else if (type == 5) {
        while (valeur < 0 || valeur > 59) {
            printf("          Erreur: La minute doit etre entre 0 et 59.\n");
            printf("          Reessayez: ");
            valeur = saisirEntier();
        }
    }

    return valeur;
}

void saisirChaineCaracteres(char *chaine, int max_length, const char *message) {
    printf("          %s", message);

    if (fgets(chaine, max_length, stdin) != NULL) {
        int longueur = strlen(chaine);
        if (longueur > 0 && chaine[longueur - 1] == '\n') {
            chaine[longueur - 1] = '\0';
        }
    }

    while (strlen(chaine) == 0 || strlen(chaine) >= max_length - 1) {
        printf("          Erreur: Saisie invalide ou trop longue.\n");
        printf("          %s", message);
        if (fgets(chaine, max_length, stdin) != NULL) {
            int longueur = strlen(chaine);
            if (longueur > 0 && chaine[longueur - 1] == '\n') {
                chaine[longueur - 1] = '\0';
            }
        }
    }
}

int verifierArticleExiste(grapheReseau *g, int id) {
    return g->articles[id] != NULL;
}

int main() {
    system("cls");
    grapheReseau g = createGraph(100);
    chargerGraph(&g,"fichier.txt");
    Sleep(2000);
    system("cls");

    int choix = 1;
    int idArticle, idSource, idDest;
    ELEMENT nouvelArticle;
    char titre[100];
    char source[50];

    while (choix != 0) {
        afficherMenuPrincipal();
        choix = saisirEntier();
        system("cls");

        if (choix == 1) {
            chargerGraph(&g, "fichier.txt");
            Sleep(1500);
            system("cls");
        }
        else if (choix == 2) {
            printf("\n");
            printf("          Entrez l'ID de l'article: ");
            idArticle = saisirEntier();

            if (verifierArticleExiste(&g, idArticle) == 1) {
                printf("          Erreur: Un article avec cet ID existe deja.\n\n");
                Sleep(2000);
                system("cls");
            }
            else {
                saisirChaineCaracteres(titre, 100, "Entrez le titre: ");
                saisirChaineCaracteres(source, 50, "Entrez la source: ");

                printf("          Entrez le score de fiabilite (0-100): ");
                int score = saisirScore();

                printf("          Entrez le jour (1-31): ");
                int jour = saisirDate(1);

                printf("          Entrez le mois (1-12): ");
                int mois = saisirDate(2);

                printf("          Entrez l'annee (2020-2030): ");
                int annee = saisirDate(3);

                printf("          Entrez l'heure (0-23): ");
                int heure = saisirDate(4);

                printf("          Entrez la minute (0-59): ");
                int minute = saisirDate(5);

                nouvelArticle = creerElement(idArticle, titre, source, score, jour, mois, annee, heure, minute);
                ajouterArticle(&g, nouvelArticle);
                free(nouvelArticle);
                Sleep(2000);
                system("cls");
            }
        }
        else if (choix == 3) {
            printf("\n");
            printf("          Entrez l'ID source: ");
            idSource = saisirId(&g);

            if (verifierArticleExiste(&g, idSource) == 0) {
                printf("          Erreur: L'article source n'existe pas.\n\n");
                Sleep(2000);
                system("cls");
            }
            else {
                printf("          Entrez l'ID destination: ");
                idDest = saisirId(&g);

                if (verifierArticleExiste(&g, idDest) == 0) {
                    printf("          Erreur: L'article destination n'existe pas.\n\n");
                    Sleep(2000);
                    system("cls");
                }
                else if (idSource == idDest) {
                    printf("          Erreur: Un article ne peut pas se citer lui-meme.\n\n");
                    Sleep(2000);
                    system("cls");
                }
                else {
                    ajouterCitation(&g, idSource, idDest);
                    Sleep(2000);
                    system("cls");
                }
            }
        }
        else if (choix == 4) {
            printf("\n");
            printf("          Entrez l'ID de l'article a supprimer: ");
            idArticle = saisirId(&g);

            if (verifierArticleExiste(&g, idArticle) == 0) {
                printf("          Erreur: L'article n'existe pas.\n\n");
                Sleep(2000);
                system("cls");
            }
            else {
                supprimerArticle(&g, idArticle);
                Sleep(2000);
                system("cls");
            }
        }
        else if (choix == 5) {
            printf("\n");
            printf("          Entrez l'ID source: ");
            idSource = saisirId(&g);

            if (verifierArticleExiste(&g, idSource) == 0) {
                printf("          Erreur: L'article source n'existe pas.\n\n");
                Sleep(2000);
                system("cls");
            }
            else {
                printf("          Entrez l'ID destination: ");
                idDest = saisirId(&g);

                if (verifierArticleExiste(&g, idDest) == 0) {
                    printf("          Erreur: L'article destination n'existe pas.\n\n");
                    Sleep(2000);
                    system("cls");
                }
                else {
                    supprimerCitation(&g, idSource, idDest);
                    Sleep(2000);
                    system("cls");
                }
            }
        }
        else if (choix == 6) {
            afficherGraphe(&g);
            Sleep(2000);
            printf("          Appuyez sur Entree pour continuer...\n");
            getchar();
            system("cls");
        }
        else if (choix == 7) {
            printf("\n");
            printf("          Entrez l'ID de l'article: ");
            idArticle = saisirId(&g);

            if (verifierArticleExiste(&g, idArticle) == 0) {
                printf("          Erreur: L'article n'existe pas.\n\n");
                Sleep(2000);
                system("cls");
            }
            else {
                articlesCites(&g, idArticle);
                Sleep(2000);
                printf("          Appuyez sur Entree pour continuer...\n");
                getchar();
                system("cls");
            }
        }
        else if (choix == 8) {
            printf("\n");
            printf("          Entrez l'ID de l'article: ");
            idArticle = saisirId(&g);

            if (verifierArticleExiste(&g, idArticle) == 0) {
                printf("          Erreur: L'article n'existe pas.\n\n");
                Sleep(2000);
                system("cls");
            }
            else {
                articlesCitants(&g, idArticle);
                Sleep(2000);
                printf("          Appuyez sur Entree pour continuer...\n");
                getchar();
                system("cls");
            }
        }
        else if (choix == 9) {
            printf("\n");
            printf("          Choisir:\n");
            printf("          1. Sources originales\n");
            printf("          2. Articles isoles\n");
            printf("          Votre choix: ");
            int sous_choix = saisirEntier();

            while (sous_choix < 1 || sous_choix > 2) {
                printf("          Erreur: Choix invalide (1 ou 2).\n");
                printf("          Reessayez: ");
                sous_choix = saisirEntier();
            }

            if (sous_choix == 1) {
                sourcesOriginales(&g);
            }
            else if (sous_choix == 2) {
                articlesIsoles(&g);
            }

            Sleep(2000);
            printf("          Appuyez sur Entree pour continuer...\n");
            getchar();
            system("cls");
        }
        else if (choix == 10) {
            ELEMENT plus_cite = articlePlusCite(&g);

            if (plus_cite == ELEMENT_VIDE) {
                printf("\n          Aucun article dans le reseau.\n\n");
            }
            else {
                printf("\n          Article le plus cite:\n");
                afficherElement(plus_cite);
                printf("\n");
                free(plus_cite);
            }

            Sleep(2000);
            printf("          Appuyez sur Entree pour continuer...\n");
            getchar();
            system("cls");
        }
        else if (choix == 11) {
            trierParDate(&g);
            Sleep(2000);
            printf("          Appuyez sur Entree pour continuer...\n");
            getchar();
            system("cls");
        }
        else if (choix == 12) {
            printf("\n");
            printf("          Entrez l'ID de l'article: ");
            idArticle = saisirId(&g);

            if (verifierArticleExiste(&g, idArticle) == 0) {
                printf("          Erreur: L'article n'existe pas.\n\n");
                Sleep(2000);
                system("cls");
            }
            else {
                chainerPropagation(&g, idArticle);
                Sleep(2000);
                printf("          Appuyez sur Entree pour continuer...\n");
                getchar();
                system("cls");
            }
        }
        else if (choix == 13) {
            printf("\n");
            printf("          Entrez l'ID de l'article: ");
            idArticle = saisirId(&g);

            if (verifierArticleExiste(&g, idArticle) == 0) {
                printf("          Erreur: L'article n'existe pas.\n\n");
                Sleep(2000);
                system("cls");
            }
            else {
                simulerPropagation(&g, idArticle);
                Sleep(2000);
                printf("          Appuyez sur Entree pour continuer...\n");
                getchar();
                system("cls");
            }
        }
        else if (choix == 14) {
            analyserReseau(&g);
            Sleep(2000);
            printf("          Appuyez sur Entree pour continuer...\n");
            getchar();
            system("cls");
        }
        else if (choix == 15) {
            analyserReseau(&g);
            Sleep(2000);
            printf("          Appuyez sur Entree pour continuer...\n");
            getchar();
            system("cls");
        }
        else if (choix == 16) {
            analyserReseau(&g);
            Sleep(2000);
            printf("          Appuyez sur Entree pour continuer...\n");
            getchar();
            system("cls");
        }
        else if (choix == 17) {
            printf("\n");
            printf("          Entrez l'ID de l'article a supprimer: ");
            idArticle = saisirId(&g);

            if (verifierArticleExiste(&g, idArticle) == 0) {
                printf("          Erreur: L'article n'existe pas.\n\n");
                Sleep(2000);
                system("cls");
            }
            else {
                simulerSuppression(&g, idArticle);
                Sleep(2000);
                printf("          Appuyez sur Entree pour continuer...\n");
                getchar();
                system("cls");
            }
        }
        else if (choix == 18) {
            printf("\n");
            printf("          Entrez l'ID source: ");
            idSource = saisirId(&g);

            if (verifierArticleExiste(&g, idSource) == 0) {
                printf("          Erreur: L'article source n'existe pas.\n\n");
                Sleep(2000);
                system("cls");
            }
            else {
                printf("          Entrez l'ID destination: ");
                idDest = saisirId(&g);

                if (verifierArticleExiste(&g, idDest) == 0) {
                    printf("          Erreur: L'article destination n'existe pas.\n\n");
                    Sleep(2000);
                    system("cls");
                }
                else {
                    neutraliserPropagation(&g, idSource, idDest);
                    Sleep(2000);
                    system("cls");
                }
            }
        }
        else if (choix == 0) {
            system("cls");
            printf("\n\n");
            printf("          |========================================|\n");
            printf("          |                                        |\n");
            printf("          |           Au revoir!                   |\n");
            printf("          |                                        |\n");
            printf("          |========================================|\n");
            Sleep(2000);
        }
        else {
            printf("          Erreur: Choix invalide (0-18).\n\n");
            Sleep(2000);
            system("cls");
        }
    }

    detruireGraph(&g);

    return 0;
}
