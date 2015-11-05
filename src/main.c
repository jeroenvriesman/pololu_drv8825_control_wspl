

/*
 * Using stepper motor VEXTA PX245-02AA
 * In bipolar mode with pololu DRV8825 to drive the coils
 * Black-Yellow-Green A1-CT-A2 (CT open)
 * Red-White-Blue B1-CT-B2 (CT open)
 * Max current 0.57A (bi), hold-set on driver to 0.7*0.57 = 400mA (8.6V)
 */

/*
 * All pins in output mode to control the drv8825
 * discovery - drv8825 - default
 * PB12      - /ENABLE - 0
 * PB11      - M0      - 1
 * PB10      - M1      - 1
 * PB2       - M2      - 1
 * PB1       - /RESET  - 1
 * PB0       - /SLEEP  - 1
 * PC5       - STEP    - 0
 * PC4       - DIR     - 0 ( 0 -> opening, 1 -> closing )
 */

/*
 * M2, M1, M0 - Step
 * 000 - 1
 * 001 - 1/2
 * 010 - 1/4
 * 011 - 1/8
 * 100 - 1/16
 * 101 - 1/32
 * 110 - 1/32
 * 111 - 1/32
 */


/*
 * Pins in input mode to read the end sensors of the valve, connected with emitter follower as a level translator for the photodiode
 * discovery - sensor (open = 0, closed = 1)
 * PB9      - C (valve fully open => 1)
 * PB8      - D (valve fully closed => 1)
 */




/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitTypeDef        GPIO_InitStructure;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void delay (int a);
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */

  /* GPIOB Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

  /* GPIOC Periph clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
  
  /* Configure PB1, PB2, PB10, PB11 and PB12 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);   

  /* Configure PC4, PC5, PC8 and PC9 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
  /* Configure PB8 and PB9 in input mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
  
  /* set default input values drv8825 */
  GPIOB->BSRR = GPIO_BSRR_BR_12;
  GPIOB->BSRR = GPIO_BSRR_BS_11;
  GPIOB->BSRR = GPIO_BSRR_BS_10;
  GPIOB->BSRR = GPIO_BSRR_BS_2;
  GPIOB->BSRR = GPIO_BSRR_BS_1;
  GPIOB->BSRR = GPIO_BSRR_BS_0;
  GPIOC->BSRR = GPIO_BSRR_BR_5;
  GPIOC->BSRR = GPIO_BSRR_BR_4;


  while (1)
  {
    /* show end status on leds */
    GPIOC->BSRR = ( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) ? GPIO_BSRR_BS_8:GPIO_BSRR_BR_8 ); // fully closed if blue led is on
    GPIOC->BSRR = ( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) ? GPIO_BSRR_BS_9:GPIO_BSRR_BR_9 ); // fully open if green led is on
    
    /* set direction to opening if fully closed */
    if ( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) ) {
      GPIOC->BSRR = GPIO_BSRR_BR_4;
    }
    /* set direction to closing if fully open */
    if ( GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9) ) {
      GPIOC->BSRR = GPIO_BSRR_BS_4;
    }
    delay(5000);
    /* make a step */
    GPIOC->BSRR = GPIO_BSRR_BS_5;
    delay(100);
    GPIOC->BSRR = GPIO_BSRR_BR_5; 
  }
}

void delay (int a)
{
	volatile int i,j;
	
	for (i=0 ; i < a ; i++)
	{ 
		j++;
	}
	
	return;
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
