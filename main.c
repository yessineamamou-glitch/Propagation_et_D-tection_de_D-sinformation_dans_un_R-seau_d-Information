 //cd C:\Users\Bejia\OneDrive\Desktop\projet asd2\Propagation_et_D-tection_de_D-sinformation_dans_un_R-seau_d-Information
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "GRAPHE.h"


void QuitterLapp() {
    system("cls");
    printf("\n\n\n");
    printf("||---------------------------------------------------------------------------------------------------------||\n");
    printf("||---------------------------------------------------------------------------------------------------------||\n");
    printf("||---------------------------------------------------------------------------------------------------------||\n");
    printf("||---------------------------------------------------------------------------------------------------------||\n");
    printf("||                                         Au Revoir                                                       ||\n");
    printf("||---------------------------------------------------------------------------------------------------------||\n");
    printf("||---------------------------------------------------------------------------------------------------------||\n");
    printf("||---------------------------------------------------------------------------------------------------------||\n");
    printf("||---------------------------------------------------------------------------------------------------------||\n");
    Sleep(3000);
    exit(0);
}

int main() {
    grapheReseau g = NULL;
    int option = -1;
    char fnom[100];
    int id_tmp;

    while (1) {
        system("cls");
        printf("**********************************************************\n");
        printf("* GESTIONNAIRE DE PROPAGATION D'ARTICLES           *\n");
        printf("**********************************************************\n");
        printf("\n==================== MENU DE GESTION =====================\n");
        printf("|  [1]  Charger un reseau depuis un fichier              |\n");
        printf("|  [2]  Afficher la structure complete du graphe         |\n");
        printf("|--------------------------------------------------------|\n");
        printf("|  [3]  Rechercher les articles cites par (Sortants)     |\n");
        printf("|  [4]  Rechercher qui cite l'article (Entrants)         |\n");
        printf("|  [5]  Lister les sources originales                    |\n");
        printf("|  [6]  Lister les articles isoles                       |\n");
        printf("|  [7]  Identifier l'article le plus influent            |\n");
        printf("|--------------------------------------------------------|\n");
        printf("|  [8]  Classement chronologique global                  |\n");
        printf("|  [9]  Trouver le premier citant historique             |\n");
        printf("|  [10] Reconstituer la chaine de propagation            |\n");
        printf("|  [11] Simulation de diffusion (BFS)                     |\n");
        printf("|--------------------------------------------------------|\n");
        printf("|  [0]  Quitter le programme                             |\n");
        printf("==========================================================\n");
        printf(" >> Choix : ");

        scanf("%d", &option);

        switch (option) {
            case 1:
                printf("\nNom du fichier source : ");
                scanf("%s", fnom);
                if (g != NULL) detruireGraphe(g);
                g = chargerGraphe(fnom);
                if (g) printf("\n>>> Succes : Graphe charge.\n");
                Sleep(5000);
                break;

            case 2:
                if (g) { printf("\n--- STRUCTURE DU GRAPHE ---\n"); afficherGraphe(g); }
                else printf("\nErreur : Aucun graphe en memoire.\n");
                Sleep(5000);
                break;

            case 3:
                printf("\nID source : "); scanf("%d", &id_tmp);
                articlesCites(g, id_tmp);
                Sleep(5000);
                break;

            case 4:
                printf("\nID cible : "); scanf("%d", &id_tmp);
                articlesCitants(g, id_tmp);
                Sleep(5000);
                break;

            case 5:
                sourcesOriginales(g);
                Sleep(5000);
                break;

            case 6:
                articlesIsoles(g);
                Sleep(5000);
                break;

            case 7:
                {
                    ELEMENT top = articlePlusCite(g);
                    if (top) printf("\nL'article le plus cite est : %s\n", top->titre);
                }
                Sleep(5000);
                break;

            case 8:
                printf("\n--- TRI CHRONOLOGIQUE ---\n");
                trierParDate(g);
                Sleep(5000);
                break;

            case 9:
                printf("\nID de l'article : "); scanf("%d", &id_tmp);
                premierCitant(g, id_tmp);
                Sleep(5000);
                break;

            case 10:
                printf("\nID source de propagation : "); scanf("%d", &id_tmp);
                chainePropagation(g, id_tmp);
                Sleep(5000);
                break;

            case 11:
                printf("\nID de depart BFS : "); scanf("%d", &id_tmp);
                simulationPropagation(g, id_tmp);
                Sleep(5000);
                break;

            case 0:
                if (g != NULL) detruireGraphe(g);
                QuitterLapp();
                break;

            default:
                printf("\nChoix invalide !\n");
                Sleep(2000);
                break;
        }
    }

    return 0;
}
