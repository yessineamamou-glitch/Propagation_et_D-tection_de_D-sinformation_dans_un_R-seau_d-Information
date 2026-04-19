#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Convertit une chaîne en minuscules */
/* static : limite la fonction au fichier actuel (évite les conflits de noms).
   inline : suggère au compilateur d'intégrer le code directement pour gagner en vitesse. */
static inline void stringLower(char *str, char *result, int maxLen) {
    int i = 0;

    /* Boucle tant qu'on n'atteint pas la fin de la chaîne (\0)
       ET qu'on ne dépasse pas la taille maximale autorisée (maxLen - 1). */
    while (i < maxLen - 1 && str[i] != '\0') {

        /* tolower : convertit un caractère en minuscule.
           (unsigned char)str[i] : "cast" de sécurité pour éviter les erreurs avec les accents.
           result[i] : on stocke le résultat dans le nouveau tableau. */
        result[i] = stringLower((unsigned char)str[i]);

        i++; // Incrémentation du compteur
    }

    /* Important : On ajoute manuellement le caractère nul à la fin
       pour marquer la fin de la nouvelle chaîne de caractères. */
    result[i] = '\0';
}


/* Recherche insensible à la casse */
/* strstrCase : renvoie 1 (vrai) si trouvé, 0 (faux) sinon.
   const char * : indique que la fonction ne modifiera pas les chaînes originales. */
static inline int strstrCase(const char *haystack, const char *needle) {

    /* Déclaration de deux zones mémoires temporaires (tampons).
       h pour le texte principal, n pour la recherche. */
    char h[500], n[200];

    /* On convertit les deux chaînes en minuscules avant de comparer.
       (char*) : on convertit temporairement le 'const' en pointeur simple
       pour que la fonction stringLower l'accepte. */
    stringLower((char*)haystack, h, 500);
    stringLower((char*)needle, n, 200);

    /* strstr : cherche la première occurrence de 'n' dans 'h'.
       Si strstr ne renvoie pas NULL, cela signifie que le mot a été trouvé. */
    return strstr(h, n) != NULL;
}

int analyserArticle(ELEMENT art)
{
    int score_suspicion = 0;

    /* +40 si le titre contient un element de BASE_FAKES */
    for (int i = 0; i < NB_FAKES; i++) {
        if (strstrCase(art->titre, BASE_FAKES[i])) {
            score_suspicion += 40;
        }
    }

    /* +10 par mot suspect trouve dans le titre */
    for (int i = 0; i < NB_SUSPECTS; i++) {
        if (strstrCase(art->titre, MOTS_SUSPECTS[i])) {
            score_suspicion += 10;
        }
    }

    /* Mise a jour du score de fiabilite */
    int nouveau_score = 100 - score_suspicion;
    if (nouveau_score < 0) nouveau_score = 0;
    art->score_fiabilite = nouveau_score;

    return score_suspicion;
}
void analyserReseau(grapheReseau *g_ptr)
{
    grapheReseau g = *g_ptr;

    if (g == NULL) {
        printf("!!! Graphe vide !!!\n");
        return;
    }

    printf("\n--- Analyse du Reseau ---\n");

    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE) {
            int score_suspicion = analyserArticle(g->articles[i]);
            int fiabilite = g->articles[i]->score_fiabilite;

            if (fiabilite < 40) {
                printf("[SUSPECT] %s (score: %d)\n", g->articles[i]->titre, fiabilite);
            } else if (fiabilite < 70) {
                printf("[DOUTEUX] %s (score: %d)\n", g->articles[i]->titre, fiabilite);
            } else {
                printf("[FIABLE ] %s (score: %d)\n", g->articles[i]->titre, fiabilite);
            }
        }
    }
}
void articlesSuspectsCites(grapheReseau *g_ptr)
{
    grapheReseau g = *g_ptr;

    if (g == NULL) {
        printf("!!! Graphe vide !!!\n");
        return;
    }

    /* collecter les articles suspects qui sont cites */
    ELEMENT *temp = (ELEMENT *)malloc(g->V * sizeof(ELEMENT));
    int count = 0;

    for (int i = 0; i < g->V; i++) {
        if (g->articles[i] != ELEMENT_VIDE &&
            g->articles[i]->score_fiabilite < 40 &&
            g->degre_in[i] > 0) {
            temp[count++] = g->articles[i];
        }
    }

    if (count == 0) {
        printf("!!! Aucun article suspect cite !!!\n");
        free(temp);
        return;
    }

    /* tri par insertion par degre_in decroissant */
    for (int i = 1; i < count; i++) {
        ELEMENT key = temp[i];
        int key_id  = key->id;
        int j = i - 1;
        while (j >= 0 && g->degre_in[temp[j]->id] < g->degre_in[key_id]) {
            temp[j + 1] = temp[j];
            j--;
        }
        temp[j + 1] = key;
    }

    printf("\n--- Articles Suspects les Plus Cites ---\n");
    for (int i = 0; i < count; i++) {
        printf("%s (score:%d, cite par %d article(s))\n",
               temp[i]->titre,
               temp[i]->score_fiabilite,
               g->degre_in[temp[i]->id]);
    }

    free(temp);
}
