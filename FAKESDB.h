#ifndef FAKESDB_H
#define FAKESDB_H

#include <string.h>
#include <ctype.h>
#include "ELTARTICLE.h"
#define NB_FAKES 6
#define NB_SUSPECTS 8

static const char *BASE_FAKES[] = {
    "ils vous cachent",
    "partagez avant suppression",
    "ce qu on vous cache",
    "la verite sur",
    "les medias ne disent pas",
    "wake up"
};

static const char *MOTS_SUSPECTS[] = {
    "alerte",
    "urgent",
    "exclusif",
    "censure",
    "complot",
    "secret",
    "interdit",
    "choc"
};
static inline void stringLower(char *str, char *result, int maxLen)
static inline int strstrCase(const char *haystack, const char *needle)
int analyserArticle(ELEMENT art)
void analyserReseau(grapheReseau *g_ptr)
void articlesSuspectsCites(grapheReseau *g_ptr)


#endif
