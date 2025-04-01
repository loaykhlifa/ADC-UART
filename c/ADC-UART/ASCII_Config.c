#include "ASCII_Config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Fonction qui convertit un entier en une cha�ne repr�sentant les valeurs ASCII 
 *        de ses chiffres, s�par�s par des espaces.
 *
 * @param input_integer : L'entier � convertir.
 * @return Une cha�ne contenant les valeurs ASCII des chiffres de l'entier.
 *         La m�moire pour cette cha�ne est allou�e dynamiquement et doit �tre lib�r�e
 *         par l'appelant � l'aide de `free()`.
 */
char* shift_digits(int input_integer) {
    // Convertir l'entier en une cha�ne
    char input_str[20]; // Suffisant pour contenir les chiffres d'un entier 64 bits
    sprintf(input_str, "%d", input_integer);

    // Allouer de la m�moire pour la cha�ne r�sultante
    // Chaque chiffre donne une valeur ASCII (max 3 chiffres) + 1 espace + '\0'
    char* result_str = malloc(3 * strlen(input_str) + 1); // +1 pour le terminateur '\0'

    if (result_str == NULL) {
        // G�rer l'erreur d'allocation m�moire
        perror("Erreur d'allocation m�moire");
        exit(EXIT_FAILURE);
    }

    // Calculer les valeurs ASCII de chaque chiffre et les concat�ner avec des espaces
    int index = 0;
    for (int i = 0; i < strlen(input_str); i++) {
        int shifted_digit = input_str[i]; // Obtenir la valeur ASCII du chiffre
        index += sprintf(result_str + index, "%d ", shifted_digit); // Ajouter � la cha�ne
    }

    // Supprimer l'espace final et ajouter le terminateur de cha�ne
    if (index > 0) {
        result_str[index - 1] = '\0'; // Remplacer le dernier espace par '\0'
    }

    return result_str; // Retourner la cha�ne r�sultat
}
