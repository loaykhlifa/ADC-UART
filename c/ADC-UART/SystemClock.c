/**
  * @brief  Configuration du syst�me d'horloge
  *         Ce fichier configure l'horloge principale du microcontr�leur en suivant ces param�tres :
  *            - Source de l'horloge : PLL avec HSE (8 MHz)
  *            - Fr�quence principale (SYSCLK) : 180 MHz
  *            - Fr�quence du bus AHB (HCLK) : 180 MHz
  *            - Fr�quence des bus APB1 et APB2 : 45 MHz et 90 MHz respectivement
  *            - Configuration du r�gulateur de tension : mode Scale 1
  *            - Latence m�moire Flash : 5 cycles
  * @note   Cette configuration est optimis�e pour des performances �lev�es
  * @param  Aucun
  * @retval Aucun
  */

#include "SystemClock.h"

// Fonction principale pour configurer l'horloge syst�me
void SysClockConfig (void)
{
		/************* �TAPES POUR CONFIGURER L'HORLOGE ************
	
	1. Activer la source d'horloge externe HSE et v�rifier son �tat.
	2. Initialiser l'horloge d'alimentation et activer le r�gulateur de tension.
	3. R�gler la m�moire Flash pour supporter des fr�quences �lev�es.
	4. Appliquer les diviseurs n�cessaires aux bus AHB, APB1 et APB2.
	5. Configurer le PLL pour atteindre la fr�quence cible de 180 MHz.
	6. Activer le PLL et attendre qu'il soit op�rationnel.
	7. S�lectionner le PLL comme source principale pour le microcontr�leur.
	
	********************************************************/
	
	// D�finition des param�tres du PLL
	#define PLL_M 	4    // Diviseur de l'oscillateur HSE
	#define PLL_N 	180  // Multiplicateur pour atteindre 180 MHz
	#define PLL_P 	0    // Correspond � une division par 2 (PLLP = 2)

	// �tape 1 : Activer HSE et attendre son �tat pr�t
	RCC->CR |= RCC_CR_HSEON;             // Activation de l'oscillateur externe
	while (!(RCC->CR & RCC_CR_HSERDY));  // Attente de sa stabilisation
	
	// �tape 2 : Activer l'alimentation et configurer le r�gulateur de tension
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;  // Activation de l'horloge pour le r�gulateur
	PWR->CR |= PWR_CR_VOS;              // R�gulateur en mode "Scale 1"
	
	// �tape 3 : Configuration des param�tres de la m�moire Flash
	FLASH->ACR = FLASH_ACR_ICEN |       // Activation du cache des instructions
	             FLASH_ACR_DCEN |       // Activation du cache des donn�es
	             FLASH_ACR_PRFTEN |     // Pr�lecture activ�e
	             FLASH_ACR_LATENCY_5WS; // 5 cycles d'attente pour 180 MHz
	
	// �tape 4 : R�gler les diviseurs pour les bus AHB, APB1 et APB2
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;    // AHB sans division (HCLK = SYSCLK)
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;   // APB1 divis� par 4 (PCLK1 = 45 MHz)
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;   // APB2 divis� par 2 (PCLK2 = 90 MHz)
	
	// �tape 5 : Configurer le PLL avec les param�tres d�finis
	RCC->PLLCFGR = (PLL_M << 0) |       // Diviseur HSE
	               (PLL_N << 6) |       // Multiplicateur PLL
	               (PLL_P << 16) |      // Diviseur PLLP (par 2)
	               RCC_PLLCFGR_PLLSRC_HSE; // Source PLL : HSE
	
	// �tape 6 : Activer le PLL et attendre qu'il soit pr�t
	RCC->CR |= RCC_CR_PLLON;            // Activation du PLL
	while (!(RCC->CR & RCC_CR_PLLRDY)); // Attente de sa stabilisation
	
	// �tape 7 : S�lectionner le PLL comme source principale
	RCC->CFGR |= RCC_CFGR_SW_PLL;       // Source syst�me : PLL
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); // V�rification
}
