#include "Timer_Config.h"
#include "SystemClock.h"

/**
  * @brief Configuration du Timer 6 (TIM6)
  *        Ce module configure le Timer 6 pour g�n�rer des d�lais pr�cis en microsecondes.
  *        La fr�quence est r�gl�e de mani�re � obtenir une r�solution de 1 �s par incr�ment.
  */

void TIM6Config (void)
{
	/************** �TAPES DE CONFIGURATION ***************
	1. Activer l'horloge du Timer 6
	2. Configurer le prescaler et le registre de rechargement automatique (ARR)
	3. D�marrer le Timer et attendre que le drapeau de mise � jour soit activ�
	*******************************************************/

	// 1. Activation de l'horloge du Timer 6
	RCC->APB1ENR |= (1<<4);  // Activation de l'horloge pour TIM6
	
	// 2. Configuration du prescaler et du registre ARR
	TIM6->PSC = 90-1;        // Division de la fr�quence d'horloge (90 MHz) par 90 pour obtenir 1 MHz (~ 1 �s)
	TIM6->ARR = 0xffff;      // Valeur maximale du registre ARR (p�riode max)

	// 3. Activation du Timer et attente du drapeau de mise � jour
	TIM6->CR1 |= (1<<0);     // Activation du compteur
	while (!(TIM6->SR & (1<<0)));  // Attente du drapeau "UIF" indiquant la mise � jour des registres
}

/**
  * @brief G�n�rer un d�lai en microsecondes
  * @param us : Nombre de microsecondes � attendre
  */
void Delay_us (uint16_t us)
{
	/************** �TAPES POUR LE D�LAI *****************
	1. R�initialiser le compteur du Timer
	2. Attendre que le compteur atteigne la valeur souhait�e.
	   Chaque incr�ment correspond � un d�lai de 1 �s.
	*****************************************************/
	TIM6->CNT = 0;                // R�initialisation du compteur
	while (TIM6->CNT < us);       // Attente jusqu'� ce que le compteur atteigne la valeur "us"
}

/**
  * @brief G�n�rer un d�lai en millisecondes
  * @param ms : Nombre de millisecondes � attendre
  */
void Delay_ms (uint16_t ms)
{
	for (uint16_t i = 0; i < ms; i++)
	{
		Delay_us(1000);           // 1 ms correspond � 1000 �s
	}
}
