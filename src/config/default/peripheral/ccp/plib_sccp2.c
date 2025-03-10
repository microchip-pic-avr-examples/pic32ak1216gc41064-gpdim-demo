/*******************************************************************************
  Capture/Compare/PWM/Timer Module SCCP2 Peripheral Library (PLIB)

  Company:
    Microchip Technology Inc.

  File Name:
    plib_sccp2.c

  Summary:
    SCCP2 Source File

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
#include "plib_sccp2.h"
#include "interrupts.h"
// *****************************************************************************

// *****************************************************************************
// Section: SCCP2 Implementation
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************

static volatile CCP_TIMER_OBJECT sccp2TimerObj;
void SCCP2_CompareInitialize (void)
{
    /* Disable Timer */
    CCP2CON1 &= ~_CCP2CON1_ON_MASK;

    CCP2CON1 = 0x5;

    CCP2CON2 = 0x1000000;

    CCP2CON3 = 0x0;

    CCP2PR = 4000;
    CCP2RA = 0;
    CCP2RB = 4095;

    IEC1 |= _IEC1_CCT2IE_MASK;
}

void SCCP2_CompareStart (void)
{
    CCP2CON1 |= _CCP2CON1_ON_MASK;
}

void SCCP2_CompareStop (void)
{
    CCP2CON1 &= ~_CCP2CON1_ON_MASK;
}

void SCCP2_CompareAutoShutdownClear (void)
{
    CCP2STAT &= ~_CCP2STAT_ASEVT_MASK;
}

void SCCP2_CompareAutoShutdownSet (void)
{
    CCP2CON2 |= _CCP2CON2_SSDG_MASK;
}


  

void SCCP2_Compare16bitPeriodValueSet (uint16_t value)
{
    CCP2PR = value;
}

uint16_t SCCP2_Compare16bitPeriodValueGet (void)
{
    return (uint16_t)CCP2PR;
}


void SCCP2_Compare16bitRAValueSet (uint16_t value)
{
    CCP2RA = value;
}

uint16_t SCCP2_Compare16bitRAValueGet (void)
{
    return (uint16_t)CCP2RA;
}

void SCCP2_Compare16bitRBValueSet (uint16_t value)
{
    CCP2RB = value;
}

uint16_t SCCP2_Compare16bitRBValueGet (void)
{
    return (uint16_t)CCP2RB;
}


void SCCP2_TimerCallbackRegister(CCP_TIMER_CALLBACK callback, uintptr_t context)
{
    sccp2TimerObj.callback_fn = callback;

    sccp2TimerObj.context = context;
}

void __attribute__((used)) CCT2_InterruptHandler (void)
{
    /* Additional local variable to prevent MISRA C violations (Rule 13.x) */
    uintptr_t context = sccp2TimerObj.context;
    uint32_t status = IFS1bits.CCT2IF;
    IFS1 &= ~_IFS1_CCT2IF_MASK;    //Clear IRQ flag

    if( (sccp2TimerObj.callback_fn != NULL))
    {
        sccp2TimerObj.callback_fn(status, context);
    }
}

