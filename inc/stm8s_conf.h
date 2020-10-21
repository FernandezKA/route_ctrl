/**
  ******************************************************************************
  * @file    stm8s_conf.h
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    30-September-2014
  * @brief   This file is used to configure the Library.
   ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_CONF_H
#define __STM8S_CONF_H

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"

/* Uncomment the line below to enable peripheral header file inclusion */
#if defined(STM8S105) || defined(STM8S005) || defined(STM8S103) || defined(STM8S003) ||\
    defined(STM8S903) || defined (STM8AF626x) || defined (STM8AF622x)
//#include "stm8s_adc1.h" 
#endif /* (STM8S105) ||(STM8S103) || (STM8S903) || (STM8AF626x) || (STM8AF622x) */

#if defined(STM8S208) || defined(STM8S207) || defined(STM8S007) || defined (STM8AF52Ax) ||\
    defined (STM8AF62Ax)
//#include "stm8s_adc2.h"
#endif /* (STM8S208) || (STM8S207) || (STM8AF62Ax) || (STM8AF52Ax) */

//#include "stm8s_awu.h"
//#include "stm8s_beep.h"

#if defined (STM8S208) || defined (STM8AF52Ax)
//#include "stm8s_can.h"
#endif /* (STM8S208) || (STM8AF52Ax) */

#include "stm8s_clk.h"
//#include "stm8s_exti.h"
//#include "stm8s_flash.h"
#include "stm8s_gpio.h"
#include "stm8s_i2c.h"
#include "stm8s_itc.h"
//#include "stm8s_iwdg.h"
//#include "stm8s_rst.h"
//#include "stm8s_spi.h"
//#include "stm8s_tim1.h"

#if !defined(STM8S903) || !defined(STM8AF622x)
//#include "stm8s_tim2.h"
#endif /* (STM8S903) || (STM8AF622x) */

#if defined(STM8S208) || defined(STM8S207) || defined(STM8S007) ||defined(STM8S105) ||\
    defined(STM8S005) ||  defined (STM8AF52Ax) || defined (STM8AF62Ax) || defined (STM8AF626x)
//#include "stm8s_tim3.h"
#endif /* (STM8S208) ||defined(STM8S207) || defined(STM8S007) ||defined(STM8S105) */ 

#if !defined(STM8S903) || !defined(STM8AF622x)
//#include "stm8s_tim4.h"
#endif /* (STM8S903) || (STM8AF622x) */

#if defined(STM8S903) || defined(STM8AF622x)
//#include "stm8s_tim5.h"
//#include "stm8s_tim6.h"
#endif  /* (STM8S903) || (STM8AF622x) */

#if defined(STM8S208) ||defined(STM8S207) || defined(STM8S007) ||defined(STM8S103) ||\
    defined(STM8S003) || defined(STM8S903) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
#include "stm8s_uart1.h"
#endif /* (STM8S208) || (STM8S207) || (STM8S103) || (STM8S903) || (STM8AF52Ax) || (STM8AF62Ax) */

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
//#include "stm8s_uart2.h"
#endif /* (STM8S105) || (STM8AF626x) */

#if defined(STM8S208) ||defined(STM8S207) || defined(STM8S007) || defined (STM8AF52Ax) ||\
    defined (STM8AF62Ax)
//#include "stm8s_uart3.h"
#endif /* STM8S208 || STM8S207 || STM8AF52Ax || STM8AF62Ax */

#if defined(STM8AF622x)
//#include "stm8s_uart4.h"
#endif /* (STM8AF622x) */      

//#include "stm8s_wwdg.h"



/* Uncomment the line below to create your own interrupt handlers */
//#defineTRAP_IRQ                   /* TRAP */
//#define TLI_IRQ                   /* TLI */
//#define AWU_IRQ                   /* AWU */
//#define CLK_IRQ                   /* CLOCK */
//#define EXTI_PORTA_IRQ            /* EXTI PORTA */
//#define EXTI_PORTB_IRQ            /* EXTI PORTB */
//#define EXTI_PORTC_IRQ            /* EXTI PORTC */
//#define EXTI_PORTD_IRQ            /* EXTI PORTD */
//#define EXTI_PORTE_IRQ            /* EXTI PORTE */

#if defined(STM8S903) || defined(STM8AF622x)
//#define EXTI_PORTF_IRQ            /* EXTI PORTF */
#endif /* (STM8S903) || (STM8AF622x) */

#if defined (STM8S208) || defined (STM8AF52Ax)
//#define CAN_RX_IRQ                /* CAN RX */
//#define CAN_TX_IRQ                /* CAN TX/ER/SC */
#endif /* (STM8S208) || (STM8AF52Ax) */

//#define SPI_IRQ                   /* SPI */
//#define TIM1_CAP_COM_IRQ          /* TIM1 CAP/COM */
//#define TIM1_UPD_OVF_TRG_BRK_IRQ  /* TIM1 UPD/OVF/TRG/BRK */

#if defined(STM8S903) || defined(STM8AF622x)
//#define TIM5_UPD_OVF_BRK_TRG_IRQ  /* TIM5 UPD/OVF/BRK/TRG */
//#define TIM5_CAP_COM_IRQ          /* TIM5 CAP/COM */
#else /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8S103) || (STM8AF52Ax) || (STM8AF62Ax) || (STM8A626x) */
//#define TIM2_UPD_OVF_BRK_IRQ      /* TIM2 UPD/OVF/BRK */
//#define TIM2_CAP_COM_IRQ          /* TIM2 CAP/COM */
#endif /* (STM8S903) || (STM8AF622x) */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S105) || \
    defined(STM8S005) ||  defined (STM8AF52Ax) || defined (STM8AF62Ax) || defined (STM8AF626x)
//#define TIM3_UPD_OVF_BRK_IRQ      /* TIM3 UPD/OVF/BRK */
//#define TIM3_CAP_COM_IRQ          /* TIM3 CAP/COM */
#endif /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8AF52Ax) || (STM8AF62Ax) || (STM8A626x) */

#if defined (STM8S208) || defined(STM8S207) || defined(STM8S007) || defined(STM8S103) || \
    defined(STM8S003) ||  defined (STM8AF52Ax) || defined (STM8AF62Ax) || defined (STM8S903)
#define UART1_TX_IRQ              /* UART1 TX */
#define UART1_RX_IRQ              /* UART1 RX */
#endif /* (STM8S208) || (STM8S207) || (STM8S903) || (STM8S103) || (STM8AF52Ax) || (STM8AF62Ax) */

#if defined (STM8AF622x)
//#define UART4_TX_IRQ              /* UART4 TX */
//#define UART4_RX_IRQ              /* UART4 RX */
#endif /* (STM8AF622x) */
 
//#define I2C_IRQ                   /* I2C */

#if defined(STM8S105) || defined(STM8S005) ||  defined (STM8AF626x)
//#define UART2_RX_IRQ              /* UART2 RX */
//#define UART2_TX_IRQ              /* UART2 TX */
#endif /* (STM8S105) || (STM8AF626x) */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
//#define UART3_RX_IRQ              /* UART3 RX */
//#define UART3_TX_IRQ              /* UART3 TX */
#endif /* (STM8S207) || (STM8S208) || (STM8AF62Ax) || (STM8AF52Ax) */

#if defined(STM8S207) || defined(STM8S007) || defined(STM8S208) || defined (STM8AF52Ax) || defined (STM8AF62Ax)
//#define ADC2_IRQ                  /* ADC2 */
#else /* (STM8S105) || (STM8S103) || (STM8S903) || (STM8AF622x) */
//#define ADC1_IRQ                  /* ADC1 */
#endif /* (STM8S207) || (STM8S208) || (STM8AF62Ax) || (STM8AF52Ax) */

#if defined(STM8S903) || defined(STM8AF622x)
//#define TIM6_UPD_OVF_TRG_IRQ      /* TIM6 UPD/OVF/TRG */
#else /* (STM8S208) || (STM8S207) || (STM8S105) || (STM8S103) || (STM8AF62Ax) || (STM8AF52Ax) || (STM8AF626x) */
//#define TIM4_UPD_OVF_IRQ          /* TIM4 UPD/OVF */
#endif /* (STM8S903) || (STM8AF622x) */
//#define EEPROM_EEC_IRQ            /* EEPROM ECC CORRECTION */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line below to expanse the "assert_param" macro in the
   Standard Peripheral Library drivers code */
#define USE_FULL_ASSERT    (1) 

/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT

/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param expr: If expr is false, it calls assert_failed function
  *   which reports the name of the source file and the source
  *   line number of the call that failed.
  *   If expr is true, it returns no value.
  * @retval : None
  */
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
void assert_failed(uint8_t* file, uint32_t line);
#else
#define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif /* __STM8S_CONF_H */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
