#include "ASCII_Config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Fonction qui convertit un entier en une chaîne représentant les valeurs ASCII 
 *        de ses chiffres, séparés par des espaces.
 *
 * @param input_integer : L'entier à convertir.
 * @return Une chaîne contenant les valeurs ASCII des chiffres de l'entier.
 *         La mémoire pour cette chaîne est allouée dynamiquement et doit être libérée
 *         par l'appelant à l'aide de `free()`.
 */
char* shift_digits(int input_integer) {
    // Convertir l'entier en une chaîne
    char input_str[20]; // Suffisant pour contenir les chiffres d'un entier 64 bits
    sprintf(input_str, "%d", input_integer);

    // Allouer de la mémoire pour la chaîne résultante
    // Chaque chiffre donne une valeur ASCII (max 3 chiffres) + 1 espace + '\0'
    char* result_str = malloc(3 * strlen(input_str) + 1); // +1 pour le terminateur '\0'

    if (result_str == NULL) {
        // Gérer l'erreur d'allocation mémoire
        perror("Erreur d'allocation mémoire");
        exit(EXIT_FAILURE);
    }

    // Calculer les valeurs ASCII de chaque chiffre et les concaténer avec des espaces
    int index = 0;
    for (int i = 0; i < strlen(input_str); i++) {
        int shifted_digit = input_str[i]; // Obtenir la valeur ASCII du chiffre
        index += sprintf(result_str + index, "%d ", shifted_digit); // Ajouter à la chaîne
    }

    // Supprimer l'espace final et ajouter le terminateur de chaîne
    if (index > 0) {
        result_str[index - 1] = '\0'; // Remplacer le dernier espace par '\0'
    }

    return result_str; // Retourner la chaîne résultat
}
