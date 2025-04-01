#include "UART_Config.h"
#include "SystemClock.h"

/**
  * @brief Configuration de l'UART2
  *        Cette fonction initialise l'UART2 pour la communication série, avec un débit en bauds
  *        configuré à 115200. Les broches PA2 et PA3 sont utilisées en mode fonction alternative.
  */
void Uart2Config (void)
{
	/************** ÉTAPES DE CONFIGURATION ***************
	1. Activer les horloges pour l'UART2 et les GPIO
	2. Configurer les broches de l'UART en mode fonction alternative
	3. Activer l'UART en réglant le bit UE dans le registre USART_CR1
	4. Définir la longueur des mots avec le bit M dans le registre USART_CR1
	5. Configurer le débit en bauds dans le registre USART_BRR
	6. Activer l'émetteur et le récepteur en réglant les bits TE et RE dans USART_CR1
	*******************************************************/

	// 1. Activer les horloges pour l'UART2 et GPIOA
	RCC->APB1ENR |= (1<<17);  // Activer l'horloge de l'UART2
	RCC->AHB1ENR |= (1<<0);   // Activer l'horloge de GPIOA
	
	// 2. Configurer les broches PA2 (TX) et PA3 (RX) en mode fonction alternative
	GPIOA->MODER |= (2<<4);  // PA2 : mode fonction alternative
	GPIOA->MODER |= (2<<6);  // PA3 : mode fonction alternative
	
	GPIOA->OSPEEDR |= (3<<4) | (3<<6);  // PA2 et PA3 : haute vitesse
	
	GPIOA->AFR[0] |= (7<<8);   // PA2 : AF7 (USART2)
	GPIOA->AFR[0] |= (7<<12);  // PA3 : AF7 (USART2)
	
	// 3. Activer l'UART2
	USART2->CR1 = 0x00;            // Réinitialiser le registre CR1
	USART2->CR1 |= (1<<13);        // Activer l'UART2 (UE = 1)
	
	// 4. Définir la longueur des mots à 8 bits
	USART2->CR1 &= ~(1<<12);       // M = 0 (8 bits)
	
	// 5. Configurer le débit en bauds (115200 bauds)
	USART2->BRR = (7<<0) | (24<<4); // Baud Rate à 115200 pour une horloge PCLK1 de 45 MHz
	
	// 6. Activer le récepteur et l'émetteur
	USART2->CR1 |= (1<<2); // Activer le récepteur (RE = 1)
	USART2->CR1 |= (1<<3); // Activer l'émetteur (TE = 1)
}

/**
  * @brief Envoyer un caractère via UART2
  * @param c : Caractère à envoyer
  */
void UART2_SendChar (uint8_t c)
{
	/************** ÉTAPES POUR L'ENVOI ***************
	1. Charger le caractère à envoyer dans le registre USART_DR
	2. Attendre que le drapeau TC (Transmission Complete) soit activé
	***************************************************/
	USART2->DR = c;  // Charger le caractère dans le registre DR
	while (!(USART2->SR & (1<<6)));  // Attendre que le drapeau TC soit activé
}

/**
  * @brief Envoyer une chaîne de caractères via UART2
  * @param USARTx : Périphérique UART
  * @param string : Pointeur vers la chaîne à envoyer
  * @param length : Longueur de la chaîne
  * @param timeout : Délai d'attente (non utilisé ici)
  */
void UART2_SendString(USART_TypeDef *USARTx, uint8_t *string, uint32_t length, uint32_t timeout)
{
	while (*string && length--) {
		UART2_SendChar(*string++);
	}
}

/**
  * @brief Recevoir un caractère via UART2
  * @retval Caractère reçu
  */
uint8_t UART2_GetChar (void)
{
	/************** ÉTAPES POUR LA RÉCEPTION ***************
	1. Attendre que le drapeau RXNE soit activé (données reçues prêtes)
	2. Lire les données dans le registre USART_DR, ce qui réinitialise le drapeau RXNE
	*******************************************************/
	uint8_t data;
	while (!(USART2->SR & (1<<5)));  // Attendre que RXNE soit activé
	data = USART2->DR;               // Lire les données reçues
	return data;
}
