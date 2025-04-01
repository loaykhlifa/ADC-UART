#include "ADC_Config.h"
#include "SystemClock.h"

/**
  * @brief Initialisation de l'ADC
  *        Configure l'ADC1 pour effectuer des conversions analogiques-numériques sur les canaux sélectionnés.
  */
void ADC_Init (void)
{
	/************** ÉTAPES À SUIVRE *****************
	1. Activer les horloges ADC et GPIO
	2. Configurer le diviseur d'horloge dans le registre CCR
	3. Régler le mode SCAN et la résolution dans CR1
	4. Configurer le mode de conversion continue, EOC et alignement des données dans CR2
	5. Régler le temps d'échantillonnage pour les canaux dans ADC_SMPRx
	6. Définir la longueur de la séquence des canaux dans ADC_SQR1
	7. Configurer les broches GPIO correspondantes en mode analogique
	************************************************/

	// 1. Activer les horloges ADC et GPIO
	RCC->APB2ENR |= (1<<8);  // Activer l'horloge ADC1
	RCC->AHB1ENR |= (1<<0);  // Activer l'horloge GPIOA

	// 2. Configurer le diviseur d'horloge dans CCR
	ADC->CCR |= 1<<16; // Diviseur de PCLK2 par 4

	// 3. Configurer le mode SCAN et la résolution dans CR1
	ADC1->CR1 = (1<<8);    // Mode SCAN activé
	ADC1->CR1 &= ~(1<<24); // Résolution 12 bits

	// 4. Configurer le mode de conversion continue, EOC et alignement des données dans CR2
	ADC1->CR2 |= (1<<1);   // Conversion continue activée
	ADC1->CR2 |= (1<<10);  // EOC après chaque conversion
	ADC1->CR2 &= ~(1<<11); // Alignement des données à droite

	// 5. Configurer le temps d'échantillonnage
	ADC1->SMPR2 &= ~((1<<3) | (1<<12));  // Temps d'échantillonnage de 3 cycles pour les canaux 1 et 4

	// 6. Définir la longueur de la séquence des canaux
	ADC1->SQR1 |= (1<<20); // Longueur de la séquence : 2 conversions

	// 7. Configurer les broches GPIO en mode analogique
	GPIOA->MODER |= (3<<2);  // Mode analogique pour PA1 (canal 1)
	GPIOA->MODER |= (3<<8);  // Mode analogique pour PA4 (canal 4)
}

/**
  * @brief Activer l'ADC
  *        Initialise l'ADC en réglant le bit ADON dans CR2 et attend la stabilisation.
  */
void ADC_Enable (void)
{
	ADC1->CR2 |= 1<<0;  // Activer l'ADC (ADON = 1)

	uint32_t delay = 10000;
	while (delay--); // Attendre environ 10 µs pour la stabilisation
}

/**
  * @brief Démarrer une conversion sur un canal spécifique
  * @param channel : Numéro du canal à convertir
  */
void ADC_Start (int channel)
{
	/************** ÉTAPES À SUIVRE *****************
	1. Régler le canal dans le registre de séquence SQR3
	2. Effacer le registre de statut
	3. Démarrer la conversion en réglant le bit SWSTART dans CR2
	************************************************/
	ADC1->SQR3 = 0;                 // Réinitialiser la séquence
	ADC1->SQR3 |= (channel<<0);     // Configurer le canal pour la conversion
	ADC1->SR = 0;                   // Effacer le registre de statut
	ADC1->CR2 |= (1<<30);           // Lancer la conversion
}

/**
  * @brief Attendre la fin de la conversion
  */
void ADC_WaitForConv (void)
{
	// Attendre que le drapeau EOC soit activé (fin de conversion)
	while (!(ADC1->SR & (1<<1)));
}

/**
  * @brief Obtenir la valeur convertie de l'ADC
  * @retval Valeur 12 bits de la conversion
  */
uint16_t ADC_GetVal (void)
{
	return ADC1->DR; // Lire le registre de données
}

/**
  * @brief Désactiver l'ADC
  *        Réinitialise l'ADC en désactivant le bit ADON dans CR2.
  */
void ADC_Disable (void)
{
	ADC1->CR2 &= ~(1<<0); // Désactiver l'ADC (ADON = 0)
}
