/**
* Copyright (C) 2021 WEEDO3D Perron
*/

#include <libmaple/dma.h>
#include <libmaple/usart.h>
#include <libmaple/gpio.h>
#include <usart_private.h>
#include <wirish.h>
#include "../HAL/STM32F1/HAL.h"
#include "../../Configuration.h"
#include "WTHardware.h"

#define DMA_BUF_SIZE 256

dma_tube_config host_tube_config;
dma_tube_config lcd_tube_config;

HardwareSerial *host_serial = &MSerial1;
HardwareSerial *lcd_serial = &MSerial2;

char host_rx_buf[DMA_BUF_SIZE];
char lcd_rx_buf[DMA_BUF_SIZE];

void setup_host_tube_config(void) 
{
    host_tube_config.tube_src = &host_serial->c_dev()->regs->DR;
    host_tube_config.tube_src_size = DMA_SIZE_8BITS;
    host_tube_config.tube_dst = host_rx_buf;
    host_tube_config.tube_dst_size = DMA_SIZE_8BITS;
    host_tube_config.tube_nr_xfers = DMA_BUF_SIZE;
    host_tube_config.tube_flags = DMA_CFG_DST_INC | DMA_CFG_CIRC;
    host_tube_config.target_data = NULL;
    host_tube_config.tube_req_src = DMA_REQ_SRC_USART1_RX;
}

void setup_lcd_tube_config(void) 
{
    lcd_tube_config.tube_src = &lcd_serial->c_dev()->regs->DR;
    lcd_tube_config.tube_src_size = DMA_SIZE_8BITS;
    lcd_tube_config.tube_dst = lcd_rx_buf;
    lcd_tube_config.tube_dst_size = DMA_SIZE_8BITS;
    lcd_tube_config.tube_nr_xfers = DMA_BUF_SIZE;
    lcd_tube_config.tube_flags = DMA_CFG_DST_INC | DMA_CFG_CIRC;
    lcd_tube_config.target_data = NULL;
    lcd_tube_config.tube_req_src = DMA_REQ_SRC_USART2_RX;
}

void setup_dma_ctl(void) 
{
    dma_init(DMA1);

    // uart1 dma init
    int status = dma_tube_cfg(DMA1, DMA_CH5, &host_tube_config);
    ASSERT(status == DMA_TUBE_CFG_SUCCESS);
    dma_enable(DMA1, DMA_CH5);

    //uart2 dma init
    status = dma_tube_cfg(DMA1, DMA_CH6, &lcd_tube_config);
    ASSERT(status == DMA_TUBE_CFG_SUCCESS);
    dma_set_priority(DMA1, DMA_CH6, DMA_PRIORITY_HIGH);
    dma_enable(DMA1, DMA_CH6);
       
}

void host_uart_init(void)
{
    host_serial->begin(HOST_BAUDRATE);
    usart_dev *serial_dev = host_serial->c_dev();
    serial_dev->regs->CR3 = USART_CR3_DMAR;     // dma enable receive
}

void lcd_uart_init(void)
{
    lcd_serial->begin(LCD_BAUDRATE);
    usart_dev *serial_dev = lcd_serial->c_dev();
    serial_dev->regs->CR3 = USART_CR3_DMAR;     // dma enable receive
}

void SerialInit(void)
{
    setup_host_tube_config();
    setup_lcd_tube_config();
    setup_dma_ctl();
    host_uart_init();
    lcd_uart_init();
}

#ifdef DMA_RX_SUPPORT

extern "C" void __irq_usart1(void) 
{
    uint8_t clear; 

    if ((USART1_BASE->CR1 & USART_CR1_IDLEIE) && (USART1_BASE->SR & USART_SR_IDLE))
    {
        USART1_BASE->CR1 &= ~USART_CR1_IDLEIE;                           

        uint16_t _len = DMA_BUF_SIZE - DMA1_BASE->CNDTR5;                
        for (uint16_t i = 0; i < _len; i++)
            rb_push_insert(USART1->rb, host_rx_buf[i]); 

        clear = USART1_BASE->SR;       
        clear  =USART1_BASE->DR;       
        clear = clear;                 

        dma_disable(DMA1, DMA_CH5);            
        DMA1_BASE->CNDTR5 = DMA_BUF_SIZE;
        dma_enable(DMA1, DMA_CH5);

        USART1_BASE->CR1 |= USART_CR1_IDLEIE;                         
    }

    /* TXE signifies readiness to send a byte to DR. */
    if ((USART1_BASE->CR1 & USART_CR1_TXEIE) && (USART1_BASE->SR & USART_SR_TXE)) 
    {
        usart_irq(USART1->rb, USART1->wb, USART1_BASE);  
    }
}

extern "C" void __irq_usart2(void) 
{
    uint8_t clear;  

    if ((USART2_BASE->CR1 & USART_CR1_IDLEIE) && (USART2_BASE->SR & USART_SR_IDLE))
    {
        USART2_BASE->CR1 &= ~USART_CR1_IDLEIE;                         

        uint16_t _len = DMA_BUF_SIZE - DMA1_BASE->CNDTR6;               
        for (uint16_t i = 0; i < _len; i++)
            rb_push_insert(USART2->rb, lcd_rx_buf[i]); 

        clear = USART2_BASE->SR;      
        clear  =USART2_BASE->DR;        
        clear = clear;                 

        dma_disable(DMA1, DMA_CH6);          
        DMA1_BASE->CNDTR6 = DMA_BUF_SIZE;
        dma_enable(DMA1, DMA_CH6);

        USART2_BASE->CR1 |= USART_CR1_IDLEIE;                        
    }

    if ((USART2_BASE->CR1 & USART_CR1_TXEIE) && (USART2_BASE->SR & USART_SR_TXE)) 
    {
        usart_irq(USART2->rb, USART2->wb, USART2_BASE);  
    }
}


#endif