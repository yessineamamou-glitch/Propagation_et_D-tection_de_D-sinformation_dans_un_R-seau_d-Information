#ifndef LISTE_H
#define LISTE_H

#include "ELTARTICLE.h"

/* =========================================================
    Citation LISTE:
   ========================================================= */
typedef struct s_citation_noeud {
    int id_cite;
    struct s_citation_noeud *suivant;
} CitationListeNoeud, *CitationNoeudPtr;

/* Wrapper struct manages ONLY the head and length */
typedef struct s_citation_liste {
    CitationNoeudPtr tete;
    int lg;
} CitationListeStruct, *Citation_LISTE;

/* =========================================================
  MAIN LISTE:
   ========================================================= */
typedef struct s_main_noeud {
    ELEMENT info;
    Citation_LISTE citations;
    struct s_main_noeud *suivant;
} MainListeNoeud, *MainNoeudPtr;


typedef struct s_main_liste {
    MainNoeudPtr tete;
    int lg;
} MainListeStruct, *LISTE;

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
