/*******************************************************************************
  Clock PLIB
 
  Company:
    Microchip Technology Inc.
 
  File Name:
    plib_clk.c
 
  Summary:
    Clock PLIB Source File
 
  Description:
    None
 
*******************************************************************************/
 
/*******************************************************************************
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/

// Section: Includes
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "device.h"
#include "interrupts.h"
#include "plib_clk.h"
#include "plib_clk_common.h"

// Section: Macro Definitions

//CLOCK CLKxCON NOSC options
#define CLK1CON_NOSC_PGC          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(0) << _CLK1CON_NOSC_POSITION))) 
#define CLK1CON_NOSC_FRC          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(1) << _CLK1CON_NOSC_POSITION))) 
#define CLK1CON_NOSC_BFRC          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(2) << _CLK1CON_NOSC_POSITION))) 
#define CLK1CON_NOSC_POSC          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(3) << _CLK1CON_NOSC_POSITION))) 
#define CLK1CON_NOSC_LPRC          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(4) << _CLK1CON_NOSC_POSITION))) 
#define CLK1CON_NOSC_PLL1_FOUT          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(5) << _CLK1CON_NOSC_POSITION))) 
#define CLK1CON_NOSC_PLL2_FOUT          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(6) << _CLK1CON_NOSC_POSITION))) 
#define CLK1CON_NOSC_PLL1_VCO          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(7) << _CLK1CON_NOSC_POSITION))) 
#define CLK1CON_NOSC_PLL2_VCO          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(8) << _CLK1CON_NOSC_POSITION))) 
#define CLK1CON_NOSC_REFI1          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(9) << _CLK1CON_NOSC_POSITION))) 
#define CLK1CON_NOSC_REFI2          ((uint32_t)(_CLK1CON_NOSC_MASK & ((uint32_t)(10) << _CLK1CON_NOSC_POSITION))) 

//CLOCK CLKxCON BOSC options
#define CLK1CON_BOSC_PGC          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(0) << _CLK1CON_BOSC_POSITION))) 
#define CLK1CON_BOSC_FRC          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(1) << _CLK1CON_BOSC_POSITION))) 
#define CLK1CON_BOSC_BFRC          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(2) << _CLK1CON_BOSC_POSITION))) 
#define CLK1CON_BOSC_POSC          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(3) << _CLK1CON_BOSC_POSITION))) 
#define CLK1CON_BOSC_LPRC          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(4) << _CLK1CON_BOSC_POSITION))) 
#define CLK1CON_BOSC_PLL1_FOUT          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(5) << _CLK1CON_BOSC_POSITION))) 
#define CLK1CON_BOSC_PLL2_FOUT          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(6) << _CLK1CON_BOSC_POSITION))) 
#define CLK1CON_BOSC_PLL1_VCO          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(7) << _CLK1CON_BOSC_POSITION))) 
#define CLK1CON_BOSC_PLL2_VCO          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(8) << _CLK1CON_BOSC_POSITION))) 
#define CLK1CON_BOSC_REFI1          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(9) << _CLK1CON_BOSC_POSITION))) 
#define CLK1CON_BOSC_REFI2          ((uint32_t)(_CLK1CON_BOSC_MASK & ((uint32_t)(10) << _CLK1CON_BOSC_POSITION))) 

//CLOCK CLKxCON INTDIV set
#define CLK1DIV_INTDIV_SET(value)          ((uint32_t)(_CLK1DIV_INTDIV_MASK & ((uint32_t)(value) << _CLK1DIV_INTDIV_POSITION))) 
//CLOCK CLKONxCON FRACDIV set
#define CLK1DIV_FRACDIV_SET(value)          ((uint32_t)(_CLK1DIV_FRACDIV_MASK & ((uint32_t)(value) << _CLK1DIV_FRACDIV_POSITION))) 

//CLOCK CLKxCON NOSC options
#define CLK2CON_NOSC_PGC          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(0) << _CLK2CON_NOSC_POSITION))) 
#define CLK2CON_NOSC_FRC          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(1) << _CLK2CON_NOSC_POSITION))) 
#define CLK2CON_NOSC_BFRC          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(2) << _CLK2CON_NOSC_POSITION))) 
#define CLK2CON_NOSC_POSC          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(3) << _CLK2CON_NOSC_POSITION))) 
#define CLK2CON_NOSC_LPRC          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(4) << _CLK2CON_NOSC_POSITION))) 
#define CLK2CON_NOSC_PLL1_FOUT          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(5) << _CLK2CON_NOSC_POSITION))) 
#define CLK2CON_NOSC_PLL2_FOUT          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(6) << _CLK2CON_NOSC_POSITION))) 
#define CLK2CON_NOSC_PLL1_VCO          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(7) << _CLK2CON_NOSC_POSITION))) 
#define CLK2CON_NOSC_PLL2_VCO          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(8) << _CLK2CON_NOSC_POSITION))) 
#define CLK2CON_NOSC_REFI1          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(9) << _CLK2CON_NOSC_POSITION))) 
#define CLK2CON_NOSC_REFI2          ((uint32_t)(_CLK2CON_NOSC_MASK & ((uint32_t)(10) << _CLK2CON_NOSC_POSITION))) 

//CLOCK CLKxCON BOSC options
#define CLK2CON_BOSC_PGC          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(0) << _CLK2CON_BOSC_POSITION))) 
#define CLK2CON_BOSC_FRC          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(1) << _CLK2CON_BOSC_POSITION))) 
#define CLK2CON_BOSC_BFRC          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(2) << _CLK2CON_BOSC_POSITION))) 
#define CLK2CON_BOSC_POSC          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(3) << _CLK2CON_BOSC_POSITION))) 
#define CLK2CON_BOSC_LPRC          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(4) << _CLK2CON_BOSC_POSITION))) 
#define CLK2CON_BOSC_PLL1_FOUT          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(5) << _CLK2CON_BOSC_POSITION))) 
#define CLK2CON_BOSC_PLL2_FOUT          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(6) << _CLK2CON_BOSC_POSITION))) 
#define CLK2CON_BOSC_PLL1_VCO          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(7) << _CLK2CON_BOSC_POSITION))) 
#define CLK2CON_BOSC_PLL2_VCO          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(8) << _CLK2CON_BOSC_POSITION))) 
#define CLK2CON_BOSC_REFI1          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(9) << _CLK2CON_BOSC_POSITION))) 
#define CLK2CON_BOSC_REFI2          ((uint32_t)(_CLK2CON_BOSC_MASK & ((uint32_t)(10) << _CLK2CON_BOSC_POSITION))) 

//CLOCK CLKxCON INTDIV set
#define CLK2DIV_INTDIV_SET(value)          ((uint32_t)(_CLK2DIV_INTDIV_MASK & ((uint32_t)(value) << _CLK2DIV_INTDIV_POSITION))) 
//CLOCK CLKONxCON FRACDIV set
#define CLK2DIV_FRACDIV_SET(value)          ((uint32_t)(_CLK2DIV_FRACDIV_MASK & ((uint32_t)(value) << _CLK2DIV_FRACDIV_POSITION))) 

//CLOCK CLKxCON NOSC options
#define CLK3CON_NOSC_PGC          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(0) << _CLK3CON_NOSC_POSITION))) 
#define CLK3CON_NOSC_FRC          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(1) << _CLK3CON_NOSC_POSITION))) 
#define CLK3CON_NOSC_BFRC          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(2) << _CLK3CON_NOSC_POSITION))) 
#define CLK3CON_NOSC_POSC          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(3) << _CLK3CON_NOSC_POSITION))) 
#define CLK3CON_NOSC_LPRC          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(4) << _CLK3CON_NOSC_POSITION))) 
#define CLK3CON_NOSC_PLL1_FOUT          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(5) << _CLK3CON_NOSC_POSITION))) 
#define CLK3CON_NOSC_PLL2_FOUT          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(6) << _CLK3CON_NOSC_POSITION))) 
#define CLK3CON_NOSC_PLL1_VCO          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(7) << _CLK3CON_NOSC_POSITION))) 
#define CLK3CON_NOSC_PLL2_VCO          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(8) << _CLK3CON_NOSC_POSITION))) 
#define CLK3CON_NOSC_REFI1          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(9) << _CLK3CON_NOSC_POSITION))) 
#define CLK3CON_NOSC_REFI2          ((uint32_t)(_CLK3CON_NOSC_MASK & ((uint32_t)(10) << _CLK3CON_NOSC_POSITION))) 

//CLOCK CLKxCON BOSC options
#define CLK3CON_BOSC_PGC          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(0) << _CLK3CON_BOSC_POSITION))) 
#define CLK3CON_BOSC_FRC          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(1) << _CLK3CON_BOSC_POSITION))) 
#define CLK3CON_BOSC_BFRC          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(2) << _CLK3CON_BOSC_POSITION))) 
#define CLK3CON_BOSC_POSC          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(3) << _CLK3CON_BOSC_POSITION))) 
#define CLK3CON_BOSC_LPRC          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(4) << _CLK3CON_BOSC_POSITION))) 
#define CLK3CON_BOSC_PLL1_FOUT          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(5) << _CLK3CON_BOSC_POSITION))) 
#define CLK3CON_BOSC_PLL2_FOUT          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(6) << _CLK3CON_BOSC_POSITION))) 
#define CLK3CON_BOSC_PLL1_VCO          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(7) << _CLK3CON_BOSC_POSITION))) 
#define CLK3CON_BOSC_PLL2_VCO          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(8) << _CLK3CON_BOSC_POSITION))) 
#define CLK3CON_BOSC_REFI1          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(9) << _CLK3CON_BOSC_POSITION))) 
#define CLK3CON_BOSC_REFI2          ((uint32_t)(_CLK3CON_BOSC_MASK & ((uint32_t)(10) << _CLK3CON_BOSC_POSITION))) 

//CLOCK CLKxCON INTDIV set
#define CLK3DIV_INTDIV_SET(value)          ((uint32_t)(_CLK3DIV_INTDIV_MASK & ((uint32_t)(value) << _CLK3DIV_INTDIV_POSITION))) 
//CLOCK CLKONxCON FRACDIV set
#define CLK3DIV_FRACDIV_SET(value)          ((uint32_t)(_CLK3DIV_FRACDIV_MASK & ((uint32_t)(value) << _CLK3DIV_FRACDIV_POSITION))) 

//CLOCK CLKxCON NOSC options
#define CLK6CON_NOSC_PGC          ((uint32_t)(_CLK6CON_NOSC_MASK & ((uint32_t)(0) << _CLK6CON_NOSC_POSITION))) 
#define CLK6CON_NOSC_FRC          ((uint32_t)(_CLK6CON_NOSC_MASK & ((uint32_t)(1) << _CLK6CON_NOSC_POSITION))) 
#define CLK6CON_NOSC_BFRC          ((uint32_t)(_CLK6CON_NOSC_MASK & ((uint32_t)(2) << _CLK6CON_NOSC_POSITION))) 
#define CLK6CON_NOSC_POSC          ((uint32_t)(_CLK6CON_NOSC_MASK & ((uint32_t)(3) << _CLK6CON_NOSC_POSITION))) 
#define CLK6CON_NOSC_LPRC          ((uint32_t)(_CLK6CON_NOSC_MASK & ((uint32_t)(4) << _CLK6CON_NOSC_POSITION))) 
#define CLK6CON_NOSC_PLL1_FOUT          ((uint32_t)(_CLK6CON_NOSC_MASK & ((uint32_t)(5) << _CLK6CON_NOSC_POSITION))) 
#define CLK6CON_NOSC_PLL2_FOUT          ((uint32_t)(_CLK6CON_NOSC_MASK & ((uint32_t)(6) << _CLK6CON_NOSC_POSITION))) 
#define CLK6CON_NOSC_PLL1_VCO          ((uint32_t)(_CLK6CON_NOSC_MASK & ((uint32_t)(7) << _CLK6CON_NOSC_POSITION))) 
#define CLK6CON_NOSC_PLL2_VCO          ((uint32_t)(_CLK6CON_NOSC_MASK & ((uint32_t)(8) << _CLK6CON_NOSC_POSITION))) 
#define CLK6CON_NOSC_REFI1          ((uint32_t)(_CLK6CON_NOSC_MASK & ((uint32_t)(9) << _CLK6CON_NOSC_POSITION))) 
#define CLK6CON_NOSC_REFI2          ((uint32_t)(_CLK6CON_NOSC_MASK & ((uint32_t)(10) << _CLK6CON_NOSC_POSITION))) 

//CLOCK CLKxCON BOSC options
#define CLK6CON_BOSC_PGC          ((uint32_t)(_CLK6CON_BOSC_MASK & ((uint32_t)(0) << _CLK6CON_BOSC_POSITION))) 
#define CLK6CON_BOSC_FRC          ((uint32_t)(_CLK6CON_BOSC_MASK & ((uint32_t)(1) << _CLK6CON_BOSC_POSITION))) 
#define CLK6CON_BOSC_BFRC          ((uint32_t)(_CLK6CON_BOSC_MASK & ((uint32_t)(2) << _CLK6CON_BOSC_POSITION))) 
#define CLK6CON_BOSC_POSC          ((uint32_t)(_CLK6CON_BOSC_MASK & ((uint32_t)(3) << _CLK6CON_BOSC_POSITION))) 
#define CLK6CON_BOSC_LPRC          ((uint32_t)(_CLK6CON_BOSC_MASK & ((uint32_t)(4) << _CLK6CON_BOSC_POSITION))) 
#define CLK6CON_BOSC_PLL1_FOUT          ((uint32_t)(_CLK6CON_BOSC_MASK & ((uint32_t)(5) << _CLK6CON_BOSC_POSITION))) 
#define CLK6CON_BOSC_PLL2_FOUT          ((uint32_t)(_CLK6CON_BOSC_MASK & ((uint32_t)(6) << _CLK6CON_BOSC_POSITION))) 
#define CLK6CON_BOSC_PLL1_VCO          ((uint32_t)(_CLK6CON_BOSC_MASK & ((uint32_t)(7) << _CLK6CON_BOSC_POSITION))) 
#define CLK6CON_BOSC_PLL2_VCO          ((uint32_t)(_CLK6CON_BOSC_MASK & ((uint32_t)(8) << _CLK6CON_BOSC_POSITION))) 
#define CLK6CON_BOSC_REFI1          ((uint32_t)(_CLK6CON_BOSC_MASK & ((uint32_t)(9) << _CLK6CON_BOSC_POSITION))) 
#define CLK6CON_BOSC_REFI2          ((uint32_t)(_CLK6CON_BOSC_MASK & ((uint32_t)(10) << _CLK6CON_BOSC_POSITION))) 

//CLOCK CLKxCON INTDIV set
#define CLK6DIV_INTDIV_SET(value)          ((uint32_t)(_CLK6DIV_INTDIV_MASK & ((uint32_t)(value) << _CLK6DIV_INTDIV_POSITION))) 
//CLOCK CLKONxCON FRACDIV set
#define CLK6DIV_FRACDIV_SET(value)          ((uint32_t)(_CLK6DIV_FRACDIV_MASK & ((uint32_t)(value) << _CLK6DIV_FRACDIV_POSITION))) 


#define PLL1FOUT_SOURCE         5U
#define PLL2VCODIV_SOURCE       8U 

// Section: Static Variables



void CLOCK_Initialize(void)
{
    /*  
        System Clock Source                             :  FRC
        System/Generator 1 frequency (Fosc)             :  8.0 MHz
        
        Clock Generator 2 frequency                     : 8.0 MHz
        Clock Generator 3 frequency                     : 8.0 MHz
        Clock Generator 6 frequency                     : 8.0 MHz
        

    */
    //Primary oscillator settings 
    OSCCFGbits.POSCMD = 3U;
    
    //If CLK GEN 1 (system clock) is using a PLL, switch to FRC to avoid risk of over-clocking the CPU while changing PLL settings
    uint32_t currentSysClock = CLK1CONbits.COSC;
    if((currentSysClock >= PLL1FOUT_SOURCE) && (currentSysClock <= PLL2VCODIV_SOURCE))
    {
        CLK1CONbits.NOSC = 1U; //FRC as source 
        CLK1CONbits.OSWEN = 1U;
        while(CLK1CONbits.OSWEN == 1U){};
    }
    
    //Clock Generator 1 settings
    CLK1CON = (_CLK1CON_ON_MASK
                |_CLK1CON_OE_MASK
                |CLK1CON_NOSC_FRC
                |CLK1CON_BOSC_BFRC);
    //Enable clock switching
    CLK1CONbits.OSWEN = 1U;
#ifndef __MPLAB_DEBUGGER_SIMULATOR    
    //Wait for clock switching complete
    while(CLK1CONbits.OSWEN == 1U){};
#endif

    //Clock Generator 2 settings
    CLK2CON = (_CLK2CON_ON_MASK
                |_CLK2CON_OE_MASK
                |CLK2CON_NOSC_FRC
                |CLK2CON_BOSC_BFRC);
    //Enable clock switching
    CLK2CONbits.OSWEN = 1U;
#ifndef __MPLAB_DEBUGGER_SIMULATOR    
    //Wait for clock switching complete
    while(CLK2CONbits.OSWEN == 1U){};
#endif

    //Clock Generator 3 settings
    CLK3CON = (_CLK3CON_ON_MASK
                |_CLK3CON_OE_MASK
                |CLK3CON_NOSC_BFRC
                |CLK3CON_BOSC_FRC);
    //Enable clock switching
    CLK3CONbits.OSWEN = 1U;
#ifndef __MPLAB_DEBUGGER_SIMULATOR    
    //Wait for clock switching complete
    while(CLK3CONbits.OSWEN == 1U){};
#endif

    //Clock Generator 6 settings
    CLK6CON = (_CLK6CON_ON_MASK
                |_CLK6CON_OE_MASK
                |CLK6CON_NOSC_FRC
                |CLK6CON_BOSC_BFRC);
    //Enable clock switching
    CLK6CONbits.OSWEN = 1U;
#ifndef __MPLAB_DEBUGGER_SIMULATOR    
    //Wait for clock switching complete
    while(CLK6CONbits.OSWEN == 1U){};
#endif

}

 
