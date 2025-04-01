#include "Timer_Config.h"
#include "SystemClock.h"

/**
  * @brief Configuration du Timer 6 (TIM6)
  *        Ce module configure le Timer 6 pour générer des délais précis en microsecondes.
  *        La fréquence est réglée de manière à obtenir une résolution de 1 µs par incrément.
  */

void TIM6Config (void)
{
	/************** ÉTAPES DE CONFIGURATION ***************
	1. Activer l'horloge du Timer 6
	2. Configurer le prescaler et le registre de rechargement automatique (ARR)
	3. Démarrer le Timer et attendre que le drapeau de mise à jour soit activé
	*******************************************************/

	// 1. Activation de l'horloge du Timer 6
	RCC->APB1ENR |= (1<<4);  // Activation de l'horloge pour TIM6
	
	// 2. Configuration du prescaler et du registre ARR
	TIM6->PSC = 90-1;        // Division de la fréquence d'horloge (90 MHz) par 90 pour obtenir 1 MHz (~ 1 µs)
	TIM6->ARR = 0xffff;      // Valeur maximale du registre ARR (période max)

	// 3. Activation du Timer et attente du drapeau de mise à jour
	TIM6->CR1 |= (1<<0);     // Activation du compteur
	while (!(TIM6->SR & (1<<0)));  // Attente du drapeau "UIF" indiquant la mise à jour des registres
}

/**
  * @brief Générer un délai en microsecondes
  * @param us : Nombre de microsecondes à attendre
  */
void Delay_us (uint16_t us)
{
	/************** ÉTAPES POUR LE DÉLAI *****************
	1. Réinitialiser le compteur du Timer
	2. Attendre que le compteur atteigne la valeur souhaitée.
	   Chaque incrément correspond à un délai de 1 µs.
	*****************************************************/
	TIM6->CNT = 0;                // Réinitialisation du compteur
	while (TIM6->CNT < us);       // Attente jusqu'à ce que le compteur atteigne la valeur "us"
}

/**
  * @brief Générer un délai en millisecondes
  * @param ms : Nombre de millisecondes à attendre
  */
void Delay_ms (uint16_t ms)
{
	for (uint16_t i = 0; i < ms; i++)
	{
		Delay_us(1000);           // 1 ms correspond à 1000 µs
	}
}
