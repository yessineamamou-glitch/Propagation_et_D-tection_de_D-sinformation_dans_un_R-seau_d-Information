#include "GRAPHE.h"
#include "fakesdb.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

grapheReseau g = NULL;
int grapheCharge = 0;
char nomFichier[100] = "";

int main() {
    int choix;

    while (1) {
        system("cls");
        printf("\n");
        printf("========================================================\n");
        printf("||                                                      ||\n");
        printf("||        RESEAU D'INFORMATION - FAKE NEWS DETECTOR     ||\n");
        printf("||                                                      ||\n");
        printf("========================================================\n");
        printf("||\n");
        printf("||  1.  Charger un reseau depuis un fichier             ||\n");
        printf("||  2.  Ajouter un article                              ||\n");
        printf("||  3.  Ajouter une citation                            ||\n");
        printf("||  4.  Supprimer un article                            ||\n");
        printf("||  5.  Supprimer une citation                          ||\n");
        printf("||  6.  Afficher le reseau                              ||\n");
        printf("||  7.  Articles cites par un article                   ||\n");
        printf("||  8.  Articles qui citent un article                  ||\n");
        printf("||  9.  Sources originales / articles isoles            ||\n");
        printf("|| 10.  Article le plus cite                            ||\n");
        printf("|| 11.  Trier par date de publication                   ||\n");
        printf("|| 12.  Premier article citant                          ||\n");
        printf("|| 13.  Chaine de propagation                           ||\n");
        printf("|| 14.  Simuler la propagation (BFS)                    ||\n");
        printf("|| 15.  Analyser les articles (fake news)               ||\n");
        printf("|| 16.  Articles suspects les plus cites                ||\n");
        printf("|| 17.  [BONUS] Simuler la suppression                  ||\n");
        printf("|| 18.  [BONUS] Neutraliser une propagation             ||\n");
        printf("||  0.  Quitter                                         ||\n");
        printf("||\n");
        printf("========================================================\n");
        printf("\nChoisir une option : ");
        scanf("%d", &choix);

        switch (choix) {
            case 0:
                if (grapheCharge) {
                    sauvegarderGraphe(&g, nomFichier);
                    detruireGraph(&g);
                }
                system("cls");
                printf("\n");
                printf("========================================================\n");
                printf("||                                                      ||\n");
                printf("||                    Au revoir !                        ||\n");
                printf("||                                                      ||\n");
                printf("========================================================\n");
                Sleep(1500);
                return 0;

            case 1: {
                system("cls");
                printf("Entrez le nom du fichier : ");
                scanf("%s", nomFichier);

                if (strlen(nomFichier) < 4 || strcmp(nomFichier + strlen(nomFichier) - 4, ".txt") != 0) {
                    strcat(nomFichier, ".txt");
                }

                if (g != NULL) {
                    detruireGraph(&g);
                }

                chargerGraph(&g, nomFichier);
                if (g != NULL) {
                    grapheCharge = 1;
                    printf("\n");
                    printf("========================================================\n");
                    printf("||                                                      ||\n");
                    printf("||              Chargement reussi !                     ||\n");
                    printf("||                                                      ||\n");
                    printf("========================================================\n");
                    Sleep(1000);
                } else {
                    printf("\n");
                    printf("========================================================\n");
                    printf("||        !!! ERROR - Chargement echoue !!!             ||\n");
                    printf("========================================================\n");
                    grapheCharge = 0;
                    Sleep(1000);
                }
                printf("\nAppuyez sur Entree pour continuer...");
                getchar(); getchar();
                break;
            }

            case 2:
                system("cls");
                if (!grapheCharge) {
                    printf("\n");
                    printf("========================================================\n");
                    printf("||        !!! ERROR - Graphe non charge !!!             ||\n");
                    printf("||   Veuillez d'abord charger le graphe (option 1)     ||\n");
                    printf("========================================================\n");
                    Sleep(2000);
                } else {
                    int id, jour, mois, annee, heure, minute, score;
                    char titre[100], source[50];

                    printf("ID : ");
                    scanf("%d", &id);
                    printf("Titre : ");
                    scanf("%s", titre);
                    printf("Source : ");
                    scanf("%s", source);
                    printf("Score de fiabilite (0-100) : ");
                    scanf("%d", &score);

                    if (score < 0 || score > 100) {
                        printf("\n");
                        printf("========================================================\n");
                        printf("||  !!! ERROR - Score doit etre entre 0 et 100 !!!    ||\n");
                        printf("========================================================\n");
                        Sleep(1000);
                    } else {
                        printf("Jour : ");
                        scanf("%d", &jour);
                        printf("Mois : ");
                        scanf("%d", &mois);
                        printf("Annee : ");
                        scanf("%d", &annee);
                        printf("Heure : ");
                        scanf("%d", &heure);
                        printf("Minute : ");
                        scanf("%d", &minute);

                        // Populate a temporary struct to pass to elementCreer
                        articleStruct temp;
                        temp.id = id;
                        strncpy(temp.titre, titre, 99);
                        temp.titre[99] = '\0';
                        strncpy(temp.source, source, 49);
                        temp.source[49] = '\0';
                        temp.score_fiabilite = score;
                        temp.jour = jour;
                        temp.mois = mois;
                        temp.annee = annee;
                        temp.heure = heure;
                        temp.minute = minute;

                        ELEMENT art = elementCreer(&temp);

                        // Check manually since ajouterArticle is void
                        if (id >= 0 && id < g->V && g->articles[id] == NULL) {
                            ajouterArticle(&g, art);
                            printf("\n");
                            printf("========================================================\n");
                            printf("||                                                      ||\n");
                            printf("||          Article ajoute avec succes !               ||\n");
                            printf("||                                                      ||\n");
                            printf("========================================================\n");
                            Sleep(1000);
                        } else {
                            printf("\n");
                            printf("========================================================\n");
                            printf("||  !!! ERROR - ID invalide ou deja existant !!!       ||\n");
                            printf("========================================================\n");
                            elementDetruire(art);
                            Sleep(1000);
                        }
                    }
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar(); getchar();
                }
                break;

            case 3:
                system("cls");
                if (!grapheCharge) {
                    printf("\n");
                    printf("========================================================\n");
                    printf("||        !!! ERROR - Graphe non charge !!!             ||\n");
                    printf("||   Veuillez d'abord charger le graphe (option 1)     ||\n");
                    printf("========================================================\n");
                    Sleep(2000);
                } else {
                    int idSrc, idDest;
                    printf("ID source : ");
                    scanf("%d", &idSrc);
                    printf("ID destination : ");
                    scanf("%d", &idDest);

                    // Check bounds since ajouterCitation is void
                    if (idSrc >= 0 && idSrc < g->V && idDest >= 0 && idDest < g->V && g->articles[idSrc] != NULL && g->articles[idDest] != NULL) {
                        ajouterCitation(&g, idSrc, idDest);
                        printf("\n");
                        printf("========================================================\n");
                        printf("||                                                      ||\n");
                        printf("||              Citation ajoutee !                      ||\n");
                        printf("||                                                      ||\n");
                        printf("========================================================\n");
                        Sleep(1000);
                    } else {
                        printf("\n");
                        printf("========================================================\n");
                        printf("||         !!! ERROR - Article inexistant !!!          ||\n");
                        printf("========================================================\n");
                        Sleep(1000);
                    }
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar(); getchar();
                }
                break;

            case 4:
                system("cls");
                if (!grapheCharge) {
                    printf("\n");
                    printf("========================================================\n");
                    printf("||        !!! ERROR - Graphe non charge !!!             ||\n");
                    printf("||   Veuillez d'abord charger le graphe (option 1)     ||\n");
                    printf("========================================================\n");
                    Sleep(2000);
                } else {
                    int idArt;
                    printf("ID article a supprimer : ");
                    scanf("%d", &idArt);

                    if (idArt >= 0 && idArt < g->V && g->articles[idArt] != NULL) {
                        supprimerArticle(&g, idArt);
                        printf("\n");
                        printf("========================================================\n");
                        printf("||                                                      ||\n");
                        printf("||              Article supprime !                      ||\n");
                        printf("||                                                      ||\n");
                        printf("========================================================\n");
                        Sleep(1000);
                    } else {
                        printf("\n");
                        printf("========================================================\n");
                        printf("||         !!! ERROR - Article inexistant !!!          ||\n");
                        printf("========================================================\n");
                        Sleep(1000);
                    }
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar(); getchar();
                }
                break;

            case 5:
                system("cls");
                if (!grapheCharge) {
                    printf("\n");
                    printf("========================================================\n");
                    printf("||        !!! ERROR - Graphe non charge !!!             ||\n");
                    printf("||   Veuillez d'abord charger le graphe (option 1)     ||\n");
                    printf("========================================================\n");
                    Sleep(2000);
                } else {
                    int idSrc, idDest;
                    printf("ID source : ");
                    scanf("%d", &idSrc);
                    printf("ID destination : ");
                    scanf("%d", &idDest);

                    if (idSrc >= 0 && idSrc < g->V && idDest >= 0 && idDest < g->V) {
                        supprimerCitation(&g, idSrc, idDest);
                        printf("\n");
                        printf("========================================================\n");
                        printf("||                                                      ||\n");
                        printf("||      Action effectuee (Citation supprimee) !         ||\n");
                        printf("||                                                      ||\n");
                        printf("========================================================\n");
                        Sleep(1000);
                    } else {
                        printf("\n");
                        printf("========================================================\n");
                        printf("||       !!! ERROR - Identifiants invalides !!!        ||\n");
                        printf("========================================================\n");
                        Sleep(1000);
                    }
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar(); getchar();
                }
                break;

            case 6:
                system("cls");
                if (!grapheCharge) {
                    printf("\n");
                    printf("========================================================\n");
                    printf("||        !!! ERROR - Graphe non charge !!!             ||\n");
                    printf("||   Veuillez d'abord charger le graphe (option 1)     ||\n");
                    printf("========================================================\n");
                    Sleep(2000);
                } else {
                    printf("\n");
                    printf("========================================================\n");
                    afficherGraphe(&g);
                    printf("========================================================\n");
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar(); getchar();
                }
                break;

            case 7:
                system("cls");
                if (!grapheCharge) {
                    printf("\n");
                    printf("========================================================\n");
                    printf("||        !!! ERROR - Graphe non charge !!!             ||\n");
                    printf("||   Veuillez d'abord charger le graphe (option 1)     ||\n");
                    printf("========================================================\n");
                    Sleep(2000);
                } else {
                    int idSrc;
                    printf("ID article : ");
                    scanf("%d", &idSrc);
                    printf("\n");
                    printf("========================================================\n");
                    articlesCites(&g, idSrc);
                    printf("========================================================\n");
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar(); getchar();
                }
                break;

            case 8:
                system("cls");
                if (!grapheCharge) {
                    printf("\n");
                    printf("========================================================\n");
                    printf("||        !!! ERROR - Graphe non charge !!!             ||\n");
                    printf("||   Veuillez d'abord charger le graphe (option 1)     ||\n");
                    printf("========================================================\n");
                    Sleep(2000);
                } else {
                    int idDest;
                    printf("ID article : ");
                    scanf("%d", &idDest);
                    printf("\n");
                    printf("========================================================\n");
                    articlesCitants(&g, idDest);
                    printf("========================================================\n");
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar(); getchar();
                }
                break;

            case 9:
                system("cls");
                if (!grapheCharge) {
                    printf("\n");
                    printf("========================================================\n");
                    printf("||        !!! ERREUR - Graphe non charge !!!             ||\n");
                    printf("||   Veuillez d'abord charger le graphe (option 1)     ||\n");
                    printf("========================================================\n");
                    Sleep(2000);
                } else {
                    printf("\n");
                    printf("========================================================\n");
                    printf("||          SOURCES ORIGINALES                         ||\n");
                    printf("========================================================\n");
                    sourcesOriginales(&g);
                    printf("\n");
                    printf("========================================================\n");
                    printf("||            ARTICLES ISOLES                         ||\n");
                    printf("========================================================\n");
                    articlesIsoles(&g);
                    printf("========================================================\n");
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar(); getchar();
                }
                break;

            case 10:
                system("cls");
                if (!grapheCharge) {
                    printf("\n");
                    printf("========================================================\n");
                    printf("||        !!! ERROR - Graphe non charge !!!             ||\n");
                    printf("||   Veuillez d'abord charger le graphe (option 1)     ||\n");
                    printf("========================================================\n");
                    Sleep(2000);
                } else {
                    ELEMENT e = articlePlusCite(&g);
                    printf("\n");
                    printf("========================================================\n");
                    if (e != ELEMENT_VIDE) {
                        elementAfficher(e);
                    } else {
                        printf("||       !!! ERROR - Aucun article trouve !!!          ||\n");
                    }
                    printf("========================================================\n");
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar(); getchar();
                }
                break;

            case 11:
                system("cls");
                if (!grapheCharge) {
                    printf("\n");
                    printf("========================================================\n");
                    printf("||        !!! ERROR - Graphe non charge !!!             ||\n");
                    printf("||   Veuillez d'abord charger le graphe (option 1)     ||\n");
                    printf("========================================================\n");
                    Sleep(2000);
                } else {
                    printf("\n");
                    printf("========================================================\n");
                    trierParDate(&g);
                    printf("========================================================\n");
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar(); getchar();
                }
                break;

            case 12:
                system("cls");
                if (!grapheCharge) {
                    printf("\n");
                    printf("========================================================\n");
                    printf("||        !!! ERROR - Graphe non charge !!!             ||\n");
                    printf("||   Veuillez d'abord charger le graphe (option 1)     ||\n");
                    printf("========================================================\n");
                    Sleep(2000);
                } else {
                    int idDest;
                    printf("ID article : ");
                    scanf("%d", &idDest);
                    printf("\n");
                    printf("========================================================\n");
                    // Note: premierCitant is assumed to be defined in your implementation files
                    premierCitant(&g, idDest);
                    printf("========================================================\n");
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar(); getchar();
                }
                break;

            case 13:
                system("cls");
                if (!grapheCharge) {
                    printf("\n");
                    printf("========================================================\n");
                    printf("||        !!! ERROR - Graphe non charge !!!             ||\n");
                    printf("||   Veuillez d'abord charger le graphe (option 1)     ||\n");
                    printf("========================================================\n");
                    Sleep(2000);
                } else {
                    int idSrc;
                    printf("ID article source : ");
                    scanf("%d", &idSrc);
                    printf("\n");
                    printf("========================================================\n");
                    chainerPropagation(&g, idSrc);
                    printf("========================================================\n");
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar(); getchar();
                }
                break;

            case 14:
                system("cls");
                if (!grapheCharge) {
                    printf("\n");
                    printf("========================================================\n");
                    printf("||        !!! ERROR - Graphe non charge !!!             ||\n");
                    printf("||   Veuillez d'abord charger le graphe (option 1)     ||\n");
                    printf("========================================================\n");
                    Sleep(2000);
                } else {
                    int idSrc;
                    printf("ID article source : ");
                    scanf("%d", &idSrc);
                    printf("\n");
                    printf("========================================================\n");
                    simulerPropagation(&g, idSrc);
                    printf("========================================================\n");
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar(); getchar();
                }
                break;

            case 15:
                system("cls");
                if (!grapheCharge) {
                    printf("\n");
                    printf("========================================================\n");
                    printf("||        !!! ERROR - Graphe non charge !!!             ||\n");
                    printf("||   Veuillez d'abord charger le graphe (option 1)     ||\n");
                    printf("========================================================\n");
                    Sleep(2000);
                } else {
                    printf("\n");
                    printf("========================================================\n");
                    analyserReseau(&g);
                    printf("========================================================\n");
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar(); getchar();
                }
                break;

            case 16:
                system("cls");
                if (!grapheCharge) {
                    printf("\n");
                    printf("========================================================\n");
                    printf("||        !!! ERROR - Graphe non charge !!!             ||\n");
                    printf("||   Veuillez d'abord charger le graphe (option 1)     ||\n");
                    printf("========================================================\n");
                    Sleep(2000);
                } else {
                    printf("\n");
                    printf("========================================================\n");
                    // Note: articlesSuspectsCites is assumed to be defined in your implementation files
                    articlesSuspectsCites(&g);
                    printf("========================================================\n");
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar(); getchar();
                }
                break;

            case 17:
                system("cls");
                if (!grapheCharge) {
                    printf("\n");
                    printf("========================================================\n");
                    printf("||        !!! ERROR - Graphe non charge !!!             ||\n");
                    printf("||   Veuillez d'abord charger le graphe (option 1)     ||\n");
                    printf("========================================================\n");
                    Sleep(2000);
                } else {
                    int idArt;
                    printf("ID article a supprimer : ");
                    scanf("%d", &idArt);
                    printf("\n");
                    printf("========================================================\n");
                    simulerSuppression(&g, idArt);
                    printf("========================================================\n");
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar(); getchar();
                }
                break;

            case 18:
                system("cls");
                if (!grapheCharge) {
                    printf("\n");
                    printf("========================================================\n");
                    printf("||        !!! ERROR - Graphe non charge !!!             ||\n");
                    printf("||   Veuillez d'abord charger le graphe (option 1)     ||\n");
                    printf("========================================================\n");
                    Sleep(2000);
                } else {
                    int idSrc, idDest;
                    printf("ID source : ");
                    scanf("%d", &idSrc);
                    printf("ID destination : ");
                    scanf("%d", &idDest);
                    int removed = neutraliserPropagation(&g, idSrc, idDest);
                    printf("\n");
                    printf("========================================================\n");
                    printf("||                                                      ||\n");
                    printf("||   %d articles supprimes pour neutraliser !           ||\n", removed);
                    printf("||                                                      ||\n");
                    printf("========================================================\n");
                    Sleep(1000);
                    printf("\nAppuyez sur Entree pour continuer...");
                    getchar(); getchar();
                }
                break;

            default:
                system("cls");
                printf("\n");
                printf("========================================================\n");
                printf("||          !!! ERROR - Option invalide !!!            ||\n");
                printf("========================================================\n");
                Sleep(1000);
                printf("\nAppuyez sur Entree pour continuer...");
                getchar(); getchar();
        }
    }

    return 0;
}
