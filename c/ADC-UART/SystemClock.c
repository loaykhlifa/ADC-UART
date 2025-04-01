/**
  * @brief  Configuration du système d'horloge
  *         Ce fichier configure l'horloge principale du microcontrôleur en suivant ces paramètres :
  *            - Source de l'horloge : PLL avec HSE (8 MHz)
  *            - Fréquence principale (SYSCLK) : 180 MHz
  *            - Fréquence du bus AHB (HCLK) : 180 MHz
  *            - Fréquence des bus APB1 et APB2 : 45 MHz et 90 MHz respectivement
  *            - Configuration du régulateur de tension : mode Scale 1
  *            - Latence mémoire Flash : 5 cycles
  * @note   Cette configuration est optimisée pour des performances élevées
  * @param  Aucun
  * @retval Aucun
  */

#include "SystemClock.h"

// Fonction principale pour configurer l'horloge système
void SysClockConfig (void)
{
		/************* ÉTAPES POUR CONFIGURER L'HORLOGE ************
	
	1. Activer la source d'horloge externe HSE et vérifier son état.
	2. Initialiser l'horloge d'alimentation et activer le régulateur de tension.
	3. Régler la mémoire Flash pour supporter des fréquences élevées.
	4. Appliquer les diviseurs nécessaires aux bus AHB, APB1 et APB2.
	5. Configurer le PLL pour atteindre la fréquence cible de 180 MHz.
	6. Activer le PLL et attendre qu'il soit opérationnel.
	7. Sélectionner le PLL comme source principale pour le microcontrôleur.
	
	********************************************************/
	
	// Définition des paramètres du PLL
	#define PLL_M 	4    // Diviseur de l'oscillateur HSE
	#define PLL_N 	180  // Multiplicateur pour atteindre 180 MHz
	#define PLL_P 	0    // Correspond à une division par 2 (PLLP = 2)

	// Étape 1 : Activer HSE et attendre son état prêt
	RCC->CR |= RCC_CR_HSEON;             // Activation de l'oscillateur externe
	while (!(RCC->CR & RCC_CR_HSERDY));  // Attente de sa stabilisation
	
	// Étape 2 : Activer l'alimentation et configurer le régulateur de tension
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;  // Activation de l'horloge pour le régulateur
	PWR->CR |= PWR_CR_VOS;              // Régulateur en mode "Scale 1"
	
	// Étape 3 : Configuration des paramètres de la mémoire Flash
	FLASH->ACR = FLASH_ACR_ICEN |       // Activation du cache des instructions
	             FLASH_ACR_DCEN |       // Activation du cache des données
	             FLASH_ACR_PRFTEN |     // Prélecture activée
	             FLASH_ACR_LATENCY_5WS; // 5 cycles d'attente pour 180 MHz
	
	// Étape 4 : Régler les diviseurs pour les bus AHB, APB1 et APB2
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;    // AHB sans division (HCLK = SYSCLK)
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;   // APB1 divisé par 4 (PCLK1 = 45 MHz)
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;   // APB2 divisé par 2 (PCLK2 = 90 MHz)
	
	// Étape 5 : Configurer le PLL avec les paramètres définis
	RCC->PLLCFGR = (PLL_M << 0) |       // Diviseur HSE
	               (PLL_N << 6) |       // Multiplicateur PLL
	               (PLL_P << 16) |      // Diviseur PLLP (par 2)
	               RCC_PLLCFGR_PLLSRC_HSE; // Source PLL : HSE
	
	// Étape 6 : Activer le PLL et attendre qu'il soit prêt
	RCC->CR |= RCC_CR_PLLON;            // Activation du PLL
	while (!(RCC->CR & RCC_CR_PLLRDY)); // Attente de sa stabilisation
	
	// Étape 7 : Sélectionner le PLL comme source principale
	RCC->CFGR |= RCC_CFGR_SW_PLL;       // Source système : PLL
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); // Vérification
}
