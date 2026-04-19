#include "GRAPHE.h"
#include "fakesdb.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>
#include "ELTARTICLE.h"  // Add this line
grapheReseau g = NULL;
int grapheCharge = 0;
char nomFichier[100] = "";


void nettoyerChaine(char *str) {
    if (str == NULL) return;

    int i = 0, j = 0;
    int len = strlen(str);
    int dansUnMot = 0;

    char temp[100];
    while (i < len && isspace(str[i])) i++;

    while (i < len) {
        if (isspace(str[i])) {
            if (dansUnMot) {
                temp[j++] = '_';
                dansUnMot = 0;
            }
            while (i < len && isspace(str[i])) i++;
        } else {
            temp[j++] = str[i++];
            dansUnMot = 1;
        }
    }

    if (j > 0 && temp[j-1] == '_') j--;
    temp[j] = '\0';

    strcpy(str, temp);
}

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
                    sauvegarderGraphe(g, nomFichier);
                    detruireGraphe(g);
                }
                system("cls");
                printf("\n");
                printf(" =======================================================\n");
                printf("||                                                      ||\n");
                printf("||                    Au revoir !                       ||\n");
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
                    detruireGraphe(g);
                }

                g = chargerGraphe(nomFichier);
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
                    printf("!!! ERROR - Graphe non charge !!!\n");
                    Sleep(1000);
                } else {
                    int id, jour, mois, annee, heure, minute, score;
                    char titre[100], source[50];

                    do {
                        printf("Entrez l'ID (0-%d) : ", g->V - 1);
                        scanf("%d", &id);
                        if (id < 0 || id >= g->V ) {
                            printf("!!! ID invalide. Reessayez. !!!\n");}
                        if (g->articles[id] != ELEMENT_VIDE) {
                            printf("!!! ID deja utilise. Reessayez. !!!\n");}

                    } while (id < 0 || id >= g->V || g->articles[id] != ELEMENT_VIDE);

                while (getchar() != '\n');

                printf("Titre : ");
                fgets(titre, sizeof(titre), stdin);
                titre[strcspn(titre, "\n")] = 0;
                nettoyerChaine(titre);           // Remplacer les espaces par '_'

                printf("Source : ");
                fgets(source, sizeof(source), stdin);
                source[strcspn(source, "\n")] = 0;
                nettoyerChaine(source);          // Remplacer les espaces par '_'

                do {
                    printf("Score de fiabilite (0-100) : ");
                    scanf("%d", &score);
                } while (score < 0 || score > 100);
                // CONTROLE SEPARE DATE
                int dateValide = 0;
                while (!dateValide) {
                    printf("Date (JJ MM AAAA) : ");
                    if (scanf("%d %d %d", &jour, &mois, &annee) == 3) {
                        if (mois >= 1 && mois <= 12 && jour >= 1 && jour <= 31){
                            dateValide = 1;}
                    else printf("!!! Erreur : Date invalide (ex: 15 03 2026) !!!\n");
                } else {
                    while(getchar() != '\n');
                }
            }

                // CONTROLE SEPARE HEURE
                int heureValide = 0;
                while (!heureValide) {
                    printf("Heure (HH MM) : ");
                    if (scanf("%d %d", &heure, &minute) == 2) {
                        if (heure >= 0 && heure <= 23 && minute >= 0 && minute <= 59) heureValide = 1;
                        else printf("!!! Erreur : Heure invalide (ex: 14 30) !!!\n");
                    } else {
                        while(getchar() != '\n');
                }
            }
                ELEMENT art = creerArticle(id, titre, source, score, jour, mois, annee, heure, minute);
                if (art != ELEMENT_VIDE) {
                    if (ajouterArticle(g, art)) {
                        printf("\nArticle ajoute avec succes !\n");
                    } else {
                        detruireArticle(art);
                        printf("\n!!! Erreur lors de l'ajout au graphe !!!\n");}
                }
            }
            printf("\nAppuyez sur Entree pour continuer...");
            getchar(); getchar();
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

                    if (ajouterCitation(g, idSrc, idDest)) {
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

                    if (supprimerArticle(g, idArt)) {
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

                    if (supprimerCitation(g, idSrc, idDest)) {
                        printf("\n");
                        printf("========================================================\n");
                        printf("||                                                      ||\n");
                        printf("||              Citation supprimee !                    ||\n");
                        printf("||                                                      ||\n");
                        printf("========================================================\n");
                        Sleep(1000);
                    } else {
                        printf("\n");
                        printf("========================================================\n");
                        printf("||       !!! ERROR - Citation inexistante !!!          ||\n");
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
                    afficherGraphe(g);
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
                    articlesCites(g, idSrc);
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
                    articlesCitants(g, idDest);
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
                    sourcesOriginales(g);
                    printf("\n");
                    printf("========================================================\n");
                    printf("||            ARTICLES ISOLES                         ||\n");
                    printf("========================================================\n");
                    articlesIsoles(g);
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
                    ELEMENT e = articlePlusCite(g);
                    printf("\n");
                    printf("========================================================\n");
                    if (e != ELEMENT_VIDE) {
                        afficherArticle(e);
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
                    trierParDate(g);
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
                    premierCitant(g, idDest);
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
                    chainePropagation(g, idSrc);
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
                    simulerPropagation(g, idSrc);
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
                    analyserReseau(g);
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
                    articlesSuspectsCites(g);
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
                    simulerSuppression(g, idArt);
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
                    int removed = neutraliserPropagation(g, idSrc, idDest);
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
