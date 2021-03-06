/*
 * Copyright (C) 2019 Unwired Devices LLC <info@unwds.com>

 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software
 * is furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
 * PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
 * FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/**
 * @defgroup
 * @ingroup
 * @brief
 * @{
 * @file        
 * @brief       
 * @author      Mikhail Perkov
 */
 
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#include "board.h"
#include "periph/gpio.h"
#include "periph/nfc.h"

#define ENABLE_DEBUG        (0)
#include "debug.h"


void isr_nfct(void)
{   
    if(NRF_NFCT->EVENTS_READY) {
        NRF_NFCT->EVENTS_READY = 0UL;
        __DSB();          
        puts("EVENTS_READY");
        
        NRF_NFCT->INTENSET |= NFCT_INTENSET_TXFRAMESTART_Msk;
          NRF_NFCT->TASKS_STARTTX = 1;
    }
    if(NRF_NFCT->EVENTS_FIELDDETECTED) {
        NRF_NFCT->EVENTS_FIELDDETECTED = 0UL;
        __DSB();
        // NRF_NFCT->TASKS_ACTIVATE = 1;
        // NRF_NFCT->TASKS_STARTTX = 1;
        puts("EVENTS_FIELDDETECTED");
    }
    if(NRF_NFCT->EVENTS_FIELDLOST) {    
        NRF_NFCT->EVENTS_FIELDLOST  = 0UL;
        __DSB();
        puts("EVENTS_FIELDLOST"); 
        NRF_NFCT->TASKS_SENSE = 1;
    }
    if(NRF_NFCT->EVENTS_TXFRAMESTART) {
        NRF_NFCT->EVENTS_TXFRAMESTART  = 0UL;
        __DSB();
        puts("EVENTS_TXFRAMESTART");
    }
    if(NRF_NFCT->EVENTS_TXFRAMEEND) {
        NRF_NFCT->EVENTS_TXFRAMEEND  = 0UL;
        __DSB();
        puts("EVENTS_TXFRAMEEND");
    }
    if(NRF_NFCT->EVENTS_RXFRAMESTART) {
        NRF_NFCT->EVENTS_RXFRAMESTART  = 0UL;
        __DSB();
        puts("EVENTS_RXFRAMESTART");
    }
    if(NRF_NFCT->EVENTS_RXFRAMEEND) {       
        NRF_NFCT->EVENTS_RXFRAMEEND  = 0UL;
        __DSB();
        puts("EVENTS_RXFRAMEEND"); 
    }
    if(NRF_NFCT->EVENTS_ERROR) {        
        NRF_NFCT->EVENTS_ERROR  = 0UL;
        __DSB();
        NRF_NFCT->TASKS_DISABLE = 1;
        puts("EVENTS_ERROR");
    }
    if(NRF_NFCT->EVENTS_RXERROR) {      
        NRF_NFCT->EVENTS_RXERROR  = 0UL;
        __DSB();
        NRF_NFCT->TASKS_DISABLE = 1;
        puts("EVENTS_RXERROR");  
    }
    if(NRF_NFCT->EVENTS_ENDRX) {        
        NRF_NFCT->EVENTS_ENDRX  = 0UL;
        __DSB();
        puts("EVENTS_ENDRX");
    }
    if(NRF_NFCT->EVENTS_ENDTX) {        
        NRF_NFCT->EVENTS_ENDTX  = 0UL;
        __DSB();
        puts("EVENTS_ENDTX");
    }
    if(NRF_NFCT->EVENTS_AUTOCOLRESSTARTED) {       
        NRF_NFCT->EVENTS_AUTOCOLRESSTARTED  = 0UL;
        __DSB();
        puts("EVENTS_AUTOCOLRESSTARTED"); 
    }
    if(NRF_NFCT->EVENTS_COLLISION) {       
        NRF_NFCT->EVENTS_COLLISION  = 0UL;
        __DSB();
        puts("EVENTS_COLLISION"); 
    }
    if(NRF_NFCT->EVENTS_SELECTED) {
        NRF_NFCT->EVENTS_SELECTED  = 0UL;
        __DSB();
        puts("EVENTS_SELECTED");
    }
    if(NRF_NFCT->EVENTS_STARTED) {       
        NRF_NFCT->EVENTS_STARTED  = 0UL;
        __DSB();
        puts("EVENTS_STARTED"); 
    }

     puts(">>> [IRQ END] <<<\n");
    
    cortexm_isr_end();
}


void nfc_init(void)
{
    puts("NFC init...");
       
     /* Checking setting of pins dedicated to NFC functionality */
    if((NRF_UICR->NFCPINS & UICR_NFCPINS_PROTECT_Msk) != UICR_NFCPINS_PROTECT_NFC) {
        // DEBUG("[NRF_UICR->NFCPINS]: ERROR %08lX\n", NRF_UICR->NFCPINS);
         /* Setting of pins dedicated to NFC functionality */
        NRF_UICR->NFCPINS |= UICR_NFCPINS_PROTECT_Msk;
        // DEBUG("[NRF_UICR->NFCPINS]: OK %08lX\n", NRF_UICR->NFCPINS);
    }
  
    /* TODO: Frame delay mode */
        /* Minimum frame delay */
    // NRF_NFCT->FRAMEDELAYMIN = ;
        /* Maximum frame delay */
    // NRF_NFCT->FRAMEDELAYMAX = ;
        /*  Frame is transmitted between FRAMEDELAYMIN and FRAMEDELAYMAX */
    // NRF_NFCT->FRAMEDELAYMODE = NFCT_FRAMEDELAYMODE_FRAMEDELAYMODE_Window;

    
        /*  No timeout */
    NRF_NFCT->FRAMEDELAYMODE = NFCT_FRAMEDELAYMODE_FRAMEDELAYMODE_FreeRun;
    
    
    /* SENSRES SDD */
    // TODO: NRF_NFCT->SENSRES |= ;
    
    // NRF_NFCT->NFCID1_LAST = 0xAABBCCDD;
    
        /* Clear error status */
    NRF_NFCT->ERRORSTATUS = NFCT_ERRORSTATUS_FRAMEDELAYTIMEOUT_Msk |
                            NFCT_ERRORSTATUS_NFCFIELDTOOSTRONG_Msk |
                            NFCT_ERRORSTATUS_NFCFIELDTOOWEAK_Msk;
                            
        /*  Enable NFC sense field mode, change state to sense mode */
    // NRF_NFCT->TASKS_SENSE = 1;
  
    NRF_NFCT->TASKS_ACTIVATE = 1;
  
        /* Enable NFCT interrupts */
    NRF_NFCT->INTENSET =    NFCT_INTENSET_FIELDDETECTED_Msk | 
                            NFCT_INTENSET_FIELDLOST_Msk |
                            NFCT_INTENSET_ERROR_Msk |
                            NFCT_INTENSET_READY_Msk;


    // NRF_NFCT->SHORTS |= 0x3;   
    
        /* Enable interrupts */
    NVIC_EnableIRQ(NFCT_IRQn);

}