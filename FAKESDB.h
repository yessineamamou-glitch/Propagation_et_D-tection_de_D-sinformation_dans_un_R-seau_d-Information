#ifndef FAKESDB_H
#define FAKESDB_H

#include <string.h>
#include <ctype.h>

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

/* Convertit une chaîne en minuscules */
static inline void stringLower(char *str, char *result, int maxLen) {
    int i = 0;
    while (i < maxLen - 1 && str[i] != '\0') {
        result[i] = tolower((unsigned char)str[i]);
        i++;
    }
    result[i] = '\0';
}

/* Recherche insensible à la casse */
static inline int strstrCase(const char *haystack, const char *needle) {
    char h[500], n[200];
    stringLower((char*)haystack, h, 500);
    stringLower((char*)needle, n, 200);
    return strstr(h, n) != NULL;
}

#endif
