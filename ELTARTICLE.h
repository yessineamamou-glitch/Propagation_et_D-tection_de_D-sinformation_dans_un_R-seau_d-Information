#ifndef ELTARTICLE_H
#define ELTARTICLE_H

typedef struct {
    int id;
    char titre[100];
    char source[50];
    int score_fiabilite;
    int jour, mois, annee;
    int heure, minute;
} articleStruct, *ELEMENT;

#define ELEMENT_VIDE NULL

/* Crée un nouvel article */
ELEMENT creerArticle(int id, const char *titre, const char *source, int score, int jour, int mois, int annee, int heure, int minute);

/* Copie un article */
ELEMENT copierArticle(ELEMENT art);

/* Affiche un article */
void afficherArticle(ELEMENT art);

/* Affiche les infos courtes d'un article */
void afficherArticleSimple(ELEMENT art);

/* Libère la mémoire d'un article */
void detruireArticle(ELEMENT art);

/* Vérifie si deux articles sont identiques */
int egauxArticles(ELEMENT art1, ELEMENT art2);

/* Convertit un article en chaîne */
char* articleToString(ELEMENT art);

#endif
