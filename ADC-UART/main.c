#include "SystemClock.h" // Configuration de l'horloge
#include "Timer_Config.h"     // Fonctions de temporisation
#include "UART_Config.h"      // Communication UART
#include "ADC_Config.h"        // Configuration et lecture ADC
#include "ASCII_Config.h"      // Conversion des valeurs ADC en ASCII
#include <stdio.h>      // Fonctions pour sprintf
#include <stdlib.h>     // Gestion de la mémoire dynamique
#include <string.h>     // Gestion des chaînes de caractères

int main(void) {
    // Initialiser l'horloge système
    SysClockConfig();

    // Configurer le timer pour la gestion des délais
    TIM6Config();

    // Configurer l'UART2
    Uart2Config();

    // Initialiser et activer l'ADC
    ADC_Init();
    ADC_Enable();

    while (1) {
        // Démarrer une conversion ADC sur le canal 1
        ADC_Start(1);
        ADC_WaitForConv();

        // Lire la valeur brute ADC
        uint16_t raw = ADC_GetVal();

        // Calculer la tension en volts
        float vin = raw * (3.3 / 4096);

        // Convertir la valeur brute ADC en chaîne ASCII
        char *msg2 = shift_digits(raw);

        // Construire le message final
        char msg[50];
        sprintf(msg, "ASCII Code: %s, Voltage: %.2f V\r\n", msg2, vin);

        // Envoyer le message via UART
        UART2_SendString(USART2, (uint8_t *)msg, strlen(msg), 300);

        // Libérer la mémoire allouée dynamiquement
        free(msg2);

        // Délai de 1 seconde
        Delay_ms(1000);
    }

    return 0;
}
