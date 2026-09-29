#include "stm32f4xx.h"

int main(void)
{
    /* GPIOB is clocked from AHB1. */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    /*
     * Read back the enable bit before touching GPIOB.
     * Besides checking the write path, the volatile read provides the small
     * synchronization delay required after enabling an RCC peripheral clock.
     */
    (void)(RCC->AHB1ENR & RCC_AHB1ENR_GPIOBEN);

    /* PB0: general-purpose output (MODER0 = 01). */
    GPIOB->MODER &= ~(3U << (0U * 2U));
    GPIOB->MODER |=  (1U << (0U * 2U));

    /* PB0: push-pull (OT0 = 0). */
    GPIOB->OTYPER &= ~(1U << 0U);

    /* PB0: low output speed (OSPEEDR0 = 00). */
    GPIOB->OSPEEDR &= ~(3U << (0U * 2U));

    /* PB0: no pull-up / no pull-down (PUPDR0 = 00). */
    GPIOB->PUPDR &= ~(3U << (0U * 2U));

    /* NUCLEO-F446ZE LD1 is active high on PB0. */
    GPIOB->ODR |= (1U << 0U);

    while (1)
    {
    }
}
