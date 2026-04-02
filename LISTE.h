/***************************************************
 * Fichier : LISTE.H
 * Realisation du TDA LISTE - Liste simplement chainee
 * Projet : Propagation et Detection de Desinformation
 ***************************************************/

#ifndef LISTE_H
#define LISTE_H

#include "ELTARTICLE.h"

typedef struct Noeud {
    ELEMENT info;
    struct Noeud *succ;
} Noeud, *LISTE;

/* Crée une liste vide */
LISTE listeCreer();

/* Copie une liste */
LISTE listeCopier(LISTE l);

/* Retourne la taille de la liste */
int listeTaille(LISTE l);

/* Insère un élément à une position donnée */
int inserer(LISTE *l, ELEMENT elem, int pos);

/* Récupère l'élément à une position donnée */
ELEMENT recuperer(LISTE l, int pos);

/* Recherche la position d'un élément */
int rechercher(LISTE l, ELEMENT elem);

/* Supprime l'élément à une position donnée */
int supprimer(LISTE *l, int pos);

/* Supprime tous les éléments de la liste */
void listeDetruire(LISTE *l);

/* Affiche tous les éléments */
void afficherListe(LISTE l);

/* Affiche la liste en format compact */
void afficherListeCompacte(LISTE l, int afficherTous);

/* Recherche un élément par son id */
ELEMENT rechercherParId(LISTE l, int id);

#endif