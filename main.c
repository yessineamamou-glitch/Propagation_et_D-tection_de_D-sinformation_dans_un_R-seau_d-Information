#include "GRAPHE.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void afficherMenu() {
    printf("\n");
    printf("|-------------------------------------------------------------------|\n");
    printf("|                                                                   |\n");
    printf("|         --------- RESEAU D INFORMATION ---------                  |\n");
    printf("|                Propagation et Detection                           |\n");
    printf("|                    de Desinformation                              |\n");
    printf("|                                                                   |\n");
    printf("|-------------------------------------------------------------------|\n");
    printf("|                                                                   |\n");
    printf("|-------------------- MENU PRINCIPAL -------------------------------|\n");
    printf("|                                                                   |\n");
    printf("|---------------------GESTION DU GRAPHE ----------------------------|\n");
    printf("|  1. Charger un reseau depuis un fichier                           |\n");
    printf("|  2. Ajouter un article                                            |\n");
    printf("|  3. Ajouter une citation                                          |\n");
    printf("|  4. Supprimer un article                                          |\n");
    printf("|  5. Supprimer une citation                                        |\n");
    printf("|  6. Afficher le reseau                                            |\n");
    printf("|                                                                   |\n");
    printf("|------------------- INTERROGATION DU RESEAU -----------------------|\n");
    printf("|  7. Articles cites par un article                                 |\n");
    printf("|  8. Articles qui citent un article                                |\n");
    printf("|  9. Sources originales / Articles isoles                          |\n");
    printf("|  10. Article le plus cite                                         |\n");
    printf("|                                                                   |\n");
    printf("|-------------------- ANALYSE CHRONOLOGIQUE ------------------------|\n");
    printf("|  11. Trier par date de publication                                |\n");
    printf("|  12. Premier article citant                                       |\n");
    printf("|  13. Chaine de propagation                                        |\n");
    printf("|                                                                   |\n");
    printf("|------------------- SIMULATION DE PROPAGATION ---------------------|\n");
    printf("|  14. Simuler la propagation (BFS)                                 |\n");
    printf("|  15. Articles accessibles                                         |\n");
    printf("|                                                                   |\n");
    printf("| ------------------- DETECTION DE FAKE NEWS------------------------|\n");
    printf("|  16. Analyser les articles (fake news)                            |\n");
    printf("|  17. Articles suspects les plus cites                             |\n");
    printf("|                                                                   |\n");
    printf("| ------------------------- BONUS ----------------------------------|\n");
    printf("|  18. Simuler la suppression d'un article                          |\n");
    printf("|  19. Neutraliser une propagation                                  |\n");
    printf("|  20. Afficher les statistiques                                    |\n");
    printf("|                                                                   |\n");
    printf("|                       00. Quitter                                 |\n");
    printf("|-------------------------------------------------------------------|\n");
    printf("|                                                                   |\n");
    printf("\n   Votre choix :");
}

int main() {
    grapheReseau g = NULL;
    int choix;
    char filename[256];
    int id, id_src, id_dest, jour, mois, annee, heure, minute, score;
    char titre[100], source[50];

    while (1) {
        afficherMenu();
        scanf("%d", &choix);
        getchar();

        printf("\n");

        switch (choix) {
            case 1:
                system("cls");
                printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
                printf("                    --------------------------------------- CHARGER UN RESEAU -----------------------------------------------------\n");
                printf("\n                         Nom du fichier : ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0;

                if (g != NULL) detruireGraphe(g);
                g = chargerGraphe(filename);

                if (g != NULL) {
                    printf("                        Reseau charge avec succes !\n");
                    Sleep(3000);
                    system("cls");
                    afficherStatistiques(g);
                } else {
                    printf("                        Erreur lors du chargement du fichier\n");
                }
                break;

            case 2:
                system("cls");
                printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
                printf("                    --------------------------------------- AJOUTER UN ARTICLE -----------------------------------------------------\n");
                if (g == NULL) {
                    printf("\n\n                      Veuillez d abord charger ou creer un reseau\n");
                    break;
                }

                printf("\n\n                        ID : ");
                scanf("%d", &id);
                getchar();

                printf("\n\n                        Titre : ");
                fgets(titre, sizeof(titre), stdin);
                titre[strcspn(titre, "\n")] = 0;

                printf("\n\n                        Source : ");
                fgets(source, sizeof(source), stdin);
                source[strcspn(source, "\n")] = 0;

                printf("\n\n                        Score de fiabilite (0-100) : ");
                scanf("%d", &score);

                printf("\n\n                        Date (jour mois année) : ");
                scanf("%d %d %d", &jour, &mois, &annee);

                printf("\n\n                        Heure (heure minute) : ");
                scanf("%d %d", &heure, &minute);
                getchar();

                ELEMENT art = creerArticle(id, titre, source, score, jour, mois, annee, heure, minute);
                if (ajouterArticle(g, art)) {
                    printf("\n\n                     Article ajoute avec succes !\n");
                } else {
                    printf("\n\n                     Erreur lors de l'ajout de l'article\n");
                }
                detruireArticle(art);
                break;

            case 3:
                system("cls");
                printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
                printf("                    --------------------------------------- AJOUTER UNE CITATION  -----------------------------------------------------\n");
                if (g == NULL) {
                    printf("Veuillez d abord charger ou creer un reseau\n");
                    break;
                }

                printf("\n\n                        ID source : ");
                scanf("%d", &id_src);
                printf("\n\n                        ID destination : ");
                scanf("%d", &id_dest);
                getchar();

                if (ajouterCitation(g, id_src, id_dest)) {
                    printf("\n\n                         Citation ajoutee avec succes !\n");
                } else {
                    printf(" \n\n                        Erreur lors de l ajout de la citation\n");
                }
                break;

            case 4:
                system("cls");
                printf("--------- SUPPRIMER UN ARTICLE ---------\n");
                if (g == NULL) {
                    printf("\n\n                        Veuillez d abord charger ou creer un reseau\n");
                    break;
                }

                printf("\n\n                        ID de l'article : ");
                scanf("%d", &id);
                getchar();

                if (supprimerArticle(g, id)) {
                    printf("\n\n                         Article supprime avec succes !\n");
                } else {
                    printf("\n\n                        Erreur lors de la suppression de l article\n");
                }
                break;

            case 5:
                system("cls");
                printf("--------- SUPPRIMER UNE CITATION ---------\n");
                if (g == NULL) {
                    printf("\n\n                        Veuillez d abord charger ou creer un reseau\n");
                    break;
                }

                printf("\n\n                        ID source : ");
                scanf("%d", &id_src);
                printf("\n\n                        ID destination : ");
                scanf("%d", &id_dest);
                getchar();

                if (supprimerCitation(g, id_src, id_dest)) {
                    printf("\n\n                        Citation supprimée avec succes !\n");
                } else {
                    printf("\n\n                        Erreur lors de la suppression de la citation\n");
                }
                break;

            case 6:
                system("cls");
                printf("--------- AFFICHAGE DU RESEAU ---------\n");
                if (g == NULL) {
                    printf("\n\n                        Veuillez d abord charger ou créer un reseau\n");
                    break;
                }
                afficherGraphe(g);
                break;

            case 7:
                system("cls");
                printf("--------- ARTICLES CITES ---------\n");
                if (g == NULL) {
                    printf("\n\n                        Veuillez d abord charger ou creer un reseau\n");
                    break;
                }
                printf("\n\n                        ID de l article : ");
                scanf("%d", &id);
                getchar();
                articlesCites(g, id);
                break;

            case 8:
                system("cls");
                printf("--------- ARTICLES CITANTS ---------\n");
                if (g == NULL) {
                    printf("\n\n                        Veuillez d'abord charger ou creer un reseau\n");
                    break;
                }
                printf("\n\n                        ID de l article : ");
                scanf("%d", &id);
                getchar();
                articlesCitants(g, id);
                break;

            case 9:
                system("cls");
                printf("--------- SOURCES ORIGINALES ET ARTICLES ISOLES ---------\n");
                if (g == NULL) {
                    printf("\n\n                        Veuillez d abord charger ou creer un reseau\n");
                    break;
                }
                sourcesOriginales(g);
                printf("\n");
                articlesIsoles(g);
                break;

            case 10:
                system("cls");
                printf("--------- ARTICLE LE PLUS CITE ---------\n");
                if (g == NULL) {
                    printf("\n\n                        Veuillez d abord charger ou creer un reseau\n");
                    break;
                }
                {
                    ELEMENT plusCite = articlePlusCite(g);
                    if (plusCite != ELEMENT_VIDE) {
                        printf("--> ");
                        afficherArticleSimple(plusCite);
                        printf("\n\n                        (cite par %d articles)\n", g->degre_in[plusCite->id]);
                    } else {
                        printf("\n\n                        Aucun article disponible\n");
                    }
                }
                break;

            case 11:
                system("cls");
                printf("--------- TRI PAR DATE ---------\n");
                if (g == NULL) {
                    printf("\n\n                        Veuillez d abord charger ou creer un reseau\n");
                    break;
                }
                trierParDate(g);
                break;

            case 12:
                system("cls");
                printf("--------- PREMIER CITANT ---------\n");
                if (g == NULL) {
                    printf("\n\n                        Veuillez d abord charger ou creer un reseau\n");
                    break;
                }
                printf("\n\n                        ID de l'article : ");
                scanf("%d", &id);
                getchar();
                premierCitant(g, id);
                break;

            case 13:
                system("cls");
                printf("--------- CHAINE DE PROPAGATION ---------\n");
                if (g == NULL) {
                    printf("\n\n                        Veuillez d abord charger ou creer un reseau\n");
                    break;
                }
                printf("\n\n                        ID de l article source : ");
                scanf("%d", &id);
                getchar();
                chainePropagation(g, id);
                break;

            case 14:
                system("cls");
                printf("--------- SIMULATION DE PROPAGATION (BFS) ---------\n");
                if (g == NULL) {
                    printf("\n\n                        Veuillez d abord charger ou creer un reseau\n");
                    break;
                }
                printf("\n\n                        ID de l article source : ");
                scanf("%d", &id);
                getchar();
                simulerPropagation(g, id);
                break;

            case 15:
                system("cls");
                printf("--------- ARTICLES ACCESSIBLES ---------\n");
                if (g == NULL) {
                    printf("\n\n                        Veuillez d abord charger ou creer un reseau\n");
                    break;
                }
                printf("\n\n                        ID de l article source : ");
                scanf("%d", &id);
                getchar();
                articlesAccessibles(g, id);
                break;

            case 16:
                system("cls");
                printf("--------- ANALYSE DU RESEAU ---------\n");
                if (g == NULL) {
                    printf("\n\n                        Veuillez d abord charger ou creer un reseau\n");
                    break;
                }
                analyserReseau(g);
                break;

            case 17:
                system("cls");
                printf("--------- ARTICLES SUSPECTS LES PLUS CITES ---------\n");
                if (g == NULL) {
                    printf("\n\n                        Veuillez d abord charger ou creer un reseau\n");
                    break;
                }
                articlesSuspectsCites(g);
                break;

            case 18:
                system("cls");
                printf("--------- BONUS : SIMULER LA SUPPRESSION ---------\n");
                if (g == NULL) {
                    printf("\n\n                        Veuillez d abord charger ou creer un reseau\n");
                    break;
                }
                printf("\n\n                        ID de l'article : ");
                scanf("%d", &id);
                getchar();
                simulerSuppression(g, id);
                break;

            case 19:
                system("cls");
                printf("--------- BONUS : NEUTRALISER LA PROPAGATION ---------\n");
                if (g == NULL) {
                    printf("\n\n                        Veuillez d abord charger ou creer un reseau\n");
                    break;
                }
                printf("\n\n                        ID source : ");
                scanf("%d", &id_src);
                printf("\n\n                        ID destination : ");
                scanf("%d", &id_dest);
                getchar();
                neutraliserPropagation(g, id_src, id_dest);
                break;

            case 20:
                system("cls");
                printf("--------- STATISTIQUES DU RESEAU ---------\n");
                if (g == NULL) {
                    printf("\n\n                        Veuillez d abord charger ou creer un reseau\n");
                    break;
                }
                system("cls");
                afficherStatistiques(g);
                break;

            case 0:
                system("cls");
                        printf("||---------------------------------------------------------------------------------------------------------||\n");
                        printf("||---------------------------------------------------------------------------------------------------------||\n");
                        printf("||---------------------------------------------------------------------------------------------------------||\n");
                        printf("||---------------------------------------------------------------------------------------------------------||\n");
                        printf("||                                            Au Revoir                                                    ||\n");
                        printf("||---------------------------------------------------------------------------------------------------------||\n");
                        printf("||---------------------------------------------------------------------------------------------------------||\n");
                        printf("||--------------------Merci d avoir utilise le systeme d analyse de desinformation.------------------------||\n");
                        printf("||---------------------------------------------------------------------------------------------------------||\n");
                        printf("||---------------------------------------------------------------------------------------------------------||\n");
                if (g != NULL) detruireGraphe(g);
                return 0;

            default:
                system("cls");
                printf("\n\n\n\n\n\n\n\n\n\n\n\n");
                printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!Choix invalide. Veuillez ressayer.!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        }
    }

    return 0;
}
