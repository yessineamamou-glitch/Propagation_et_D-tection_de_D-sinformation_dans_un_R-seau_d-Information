#ifndef LISTE_H_INCLUDED
#define LISTE_H_INCLUDED

#include"ELTARTICLE.H"

typedef struct Cellule {
    ELEMENT donnee;
    struct Cellule *suivant;
} Cellule,*LISTE;


#define ELEMENT_VIDE NULL
#define LISTE_VIDE NULL
#endif // LISTE_H_INCLUDED
