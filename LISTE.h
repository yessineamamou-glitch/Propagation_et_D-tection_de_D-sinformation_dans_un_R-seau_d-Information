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
LISTE    listeCreer();
void     listeDetruire(LISTE L);
int      inserer(LISTE L, ELEMENT e, int pos);
int      supprimer(LISTE L, int pos);
ELEMENT  recuperer(LISTE L, int pos);
void     listeAfficher(LISTE L);
int      listeTaille(LISTE L);
int      estVide(LISTE L);
#endif
