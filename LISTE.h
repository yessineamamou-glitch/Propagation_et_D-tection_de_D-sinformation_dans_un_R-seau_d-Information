#ifndef LISTE_H
#define LISTE_H

#include "ELTARTICLE.h"
// article struct

typedef struct {
    int id;
    char titre[100];
    char source[50];
    int score_fiabilite;
    int jour;
    int mois;
    int annee;
    int heure;
    int minute;
} articleStruct, *ELEMENT;

/* =========================================================
    Citation LISTE: simplement chainee
   ========================================================= */
typedef struct structNoeud {
    ELEMENT info;
    struct structNoeud *suivant;
} structNoeud, *NOEUD;

typedef struct {
    NOEUD tete;
    int lg;
} laStruct, *LISTE;
/* =========================================================
  MAIN LISTE:graphe
   ========================================================= */
typedef struct {
    int V; //lg de la liste article
    ELEMENT *articles;
    LISTE   *adjList;
    LISTE   *citants;      // le contraire du  adjliste
    int     *degre_in;    //degree in =lg de les liste citants   je essayer le plus possible " de respecter la memes structure dans lennonce
} structuregraphe,*grapheReseau;
/* =========================================================
   LISTE DES ELEMENTS (aritcle)  mawjouda deja fel graphe.h
   ========================================================= */


/* =========================================================
   LISTE DES ENTIERS QUI CITE I DEGREE IN
   ========================================================= */

/* =========================================================
   PROTOTYPES: CITATION LIST
   ========================================================= */
Citation_LISTE cit_listeCreer(void);
void cit_listeDetruire(Citation_LISTE L);
int cit_estVide(Citation_LISTE L);
int cit_listeTaille(Citation_LISTE L);
int cit_inserer(Citation_LISTE L, int id, int pos);
int cit_supprimer(Citation_LISTE L, int pos);
void cit_listeAfficher(Citation_LISTE L);
Citation_LISTE cit_listeCopier(Citation_LISTE L);
/* =========================================================
   PROTOTYPES: MAIN LIST
   ========================================================= */
LISTE main_listeCreer(void);
void main_listeDetruire(LISTE L);
int main_estVide(LISTE L);
int main_listeTaille(LISTE L);
ELEMENT main_recuperer(LISTE L, int pos);
int main_inserer(LISTE L, ELEMENT e, Citation_LISTE citations, int pos);
int main_supprimer(LISTE L, int pos);
void main_listeAfficher(LISTE L);
LISTE main_listeCopier(LISTE L);
#endif
