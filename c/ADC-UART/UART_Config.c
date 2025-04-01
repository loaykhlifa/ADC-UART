#include "UART_Config.h"
#include "SystemClock.h"

/**
  * @brief Configuration de l'UART2
  *        Cette fonction initialise l'UART2 pour la communication s�rie, avec un d�bit en bauds
  *        configur� � 115200. Les broches PA2 et PA3 sont utilis�es en mode fonction alternative.
  */
void Uart2Config (void)
{
	/************** �TAPES DE CONFIGURATION ***************
	1. Activer les horloges pour l'UART2 et les GPIO
	2. Configurer les broches de l'UART en mode fonction alternative
	3. Activer l'UART en r�glant le bit UE dans le registre USART_CR1
	4. D�finir la longueur des mots avec le bit M dans le registre USART_CR1
	5. Configurer le d�bit en bauds dans le registre USART_BRR
	6. Activer l'�metteur et le r�cepteur en r�glant les bits TE et RE dans USART_CR1
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
	USART2->CR1 = 0x00;            // R�initialiser le registre CR1
	USART2->CR1 |= (1<<13);        // Activer l'UART2 (UE = 1)
	
	// 4. D�finir la longueur des mots � 8 bits
	USART2->CR1 &= ~(1<<12);       // M = 0 (8 bits)
	
	// 5. Configurer le d�bit en bauds (115200 bauds)
	USART2->BRR = (7<<0) | (24<<4); // Baud Rate � 115200 pour une horloge PCLK1 de 45 MHz
	
	// 6. Activer le r�cepteur et l'�metteur
	USART2->CR1 |= (1<<2); // Activer le r�cepteur (RE = 1)
	USART2->CR1 |= (1<<3); // Activer l'�metteur (TE = 1)
}

/**
  * @brief Envoyer un caract�re via UART2
  * @param c : Caract�re � envoyer
  */
void UART2_SendChar (uint8_t c)
{
	/************** �TAPES POUR L'ENVOI ***************
	1. Charger le caract�re � envoyer dans le registre USART_DR
	2. Attendre que le drapeau TC (Transmission Complete) soit activ�
	***************************************************/
	USART2->DR = c;  // Charger le caract�re dans le registre DR
	while (!(USART2->SR & (1<<6)));  // Attendre que le drapeau TC soit activ�
}

/**
  * @brief Envoyer une cha�ne de caract�res via UART2
  * @param USARTx : P�riph�rique UART
  * @param string : Pointeur vers la cha�ne � envoyer
  * @param length : Longueur de la cha�ne
  * @param timeout : D�lai d'attente (non utilis� ici)
  */
void UART2_SendString(USART_TypeDef *USARTx, uint8_t *string, uint32_t length, uint32_t timeout)
{
	while (*string && length--) {
		UART2_SendChar(*string++);
	}
}

/**
  * @brief Recevoir un caract�re via UART2
  * @retval Caract�re re�u
  */
uint8_t UART2_GetChar (void)
{
	/************** �TAPES POUR LA R�CEPTION ***************
	1. Attendre que le drapeau RXNE soit activ� (donn�es re�ues pr�tes)
	2. Lire les donn�es dans le registre USART_DR, ce qui r�initialise le drapeau RXNE
	*******************************************************/
	uint8_t data;
	while (!(USART2->SR & (1<<5)));  // Attendre que RXNE soit activ�
	data = USART2->DR;               // Lire les donn�es re�ues
	return data;
}
