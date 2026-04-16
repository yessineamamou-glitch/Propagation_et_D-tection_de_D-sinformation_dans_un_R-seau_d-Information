#ifndef LISTE_H
#define LISTE_H

#include "ELTARTICLE.h"

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
    int V;
    ELEMENT *articles;
    LISTE   *adjList;
  //  LISTE   *citants;      le contraire du  adjliste
    int     *degre_in;    //degree in =lg de les liste citants   je essayer le plus possible " de respecter la memes structure dans lennonce
} structuregraphe,*grapheReseau;
/* =========================================================
   LISTE DES ELEMENTS (aritcle)  mawjouda deja fel graphe.h
   ========================================================= */
int articlesEstVide(grapheReseau g);
int articlesEstSature(grapheReseau g, int maxCap);
int articlesInserer(grapheReseau g, ELEMENT e, int pos);
int articlesSupprimer(grapheReseau g, int pos);
void articlesAfficher(grapheReseau g);

/* =========================================================
   primitive de liste simplement chaine
   ========================================================= */
LISTE listeCreer(void);
void listeDetruire(LISTE L);
int estVide(LISTE L);
int estSaturee(LISTE L);
int listeTaille(LISTE L);
int inserer(LISTE L, ELEMENT e, int pos);
int supprimer(LISTE L, int pos);
ELEMENT recuperer(LISTE L, int pos);
void listeAfficher(LISTE L);





#endif
