#include "ADC_Config.h"
#include "SystemClock.h"

/**
  * @brief Initialisation de l'ADC
  *        Configure l'ADC1 pour effectuer des conversions analogiques-num�riques sur les canaux s�lectionn�s.
  */
void ADC_Init (void)
{
	/************** �TAPES � SUIVRE *****************
	1. Activer les horloges ADC et GPIO
	2. Configurer le diviseur d'horloge dans le registre CCR
	3. R�gler le mode SCAN et la r�solution dans CR1
	4. Configurer le mode de conversion continue, EOC et alignement des donn�es dans CR2
	5. R�gler le temps d'�chantillonnage pour les canaux dans ADC_SMPRx
	6. D�finir la longueur de la s�quence des canaux dans ADC_SQR1
	7. Configurer les broches GPIO correspondantes en mode analogique
	************************************************/

	// 1. Activer les horloges ADC et GPIO
	RCC->APB2ENR |= (1<<8);  // Activer l'horloge ADC1
	RCC->AHB1ENR |= (1<<0);  // Activer l'horloge GPIOA

	// 2. Configurer le diviseur d'horloge dans CCR
	ADC->CCR |= 1<<16; // Diviseur de PCLK2 par 4

	// 3. Configurer le mode SCAN et la r�solution dans CR1
	ADC1->CR1 = (1<<8);    // Mode SCAN activ�
	ADC1->CR1 &= ~(1<<24); // R�solution 12 bits

	// 4. Configurer le mode de conversion continue, EOC et alignement des donn�es dans CR2
	ADC1->CR2 |= (1<<1);   // Conversion continue activ�e
	ADC1->CR2 |= (1<<10);  // EOC apr�s chaque conversion
	ADC1->CR2 &= ~(1<<11); // Alignement des donn�es � droite

	// 5. Configurer le temps d'�chantillonnage
	ADC1->SMPR2 &= ~((1<<3) | (1<<12));  // Temps d'�chantillonnage de 3 cycles pour les canaux 1 et 4

	// 6. D�finir la longueur de la s�quence des canaux
	ADC1->SQR1 |= (1<<20); // Longueur de la s�quence : 2 conversions

	// 7. Configurer les broches GPIO en mode analogique
	GPIOA->MODER |= (3<<2);  // Mode analogique pour PA1 (canal 1)
	GPIOA->MODER |= (3<<8);  // Mode analogique pour PA4 (canal 4)
}

/**
  * @brief Activer l'ADC
  *        Initialise l'ADC en r�glant le bit ADON dans CR2 et attend la stabilisation.
  */
void ADC_Enable (void)
{
	ADC1->CR2 |= 1<<0;  // Activer l'ADC (ADON = 1)

	uint32_t delay = 10000;
	while (delay--); // Attendre environ 10 �s pour la stabilisation
}

/**
  * @brief D�marrer une conversion sur un canal sp�cifique
  * @param channel : Num�ro du canal � convertir
  */
void ADC_Start (int channel)
{
	/************** �TAPES � SUIVRE *****************
	1. R�gler le canal dans le registre de s�quence SQR3
	2. Effacer le registre de statut
	3. D�marrer la conversion en r�glant le bit SWSTART dans CR2
	************************************************/
	ADC1->SQR3 = 0;                 // R�initialiser la s�quence
	ADC1->SQR3 |= (channel<<0);     // Configurer le canal pour la conversion
	ADC1->SR = 0;                   // Effacer le registre de statut
	ADC1->CR2 |= (1<<30);           // Lancer la conversion
}

/**
  * @brief Attendre la fin de la conversion
  */
void ADC_WaitForConv (void)
{
	// Attendre que le drapeau EOC soit activ� (fin de conversion)
	while (!(ADC1->SR & (1<<1)));
}

/**
  * @brief Obtenir la valeur convertie de l'ADC
  * @retval Valeur 12 bits de la conversion
  */
uint16_t ADC_GetVal (void)
{
	return ADC1->DR; // Lire le registre de donn�es
}

/**
  * @brief D�sactiver l'ADC
  *        R�initialise l'ADC en d�sactivant le bit ADON dans CR2.
  */
void ADC_Disable (void)
{
	ADC1->CR2 &= ~(1<<0); // D�sactiver l'ADC (ADON = 0)
}
