/*
� [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#include <xc.h>
#include "led_red.h"
#include "../../src/config/default/peripheral/pwm/plib_pwm.h"

static bool issccp1Enabled = false;

void LED_RED_Initialize(void)
{
    
}

void LED_RED_On(void)
{
    PWM_GeneratorEnable(PWM_GENERATOR_1);
    issccp1Enabled = true;
}

void LED_RED_Off(void)
{
    PWM_GeneratorDisable(PWM_GENERATOR_1);
    issccp1Enabled = false;
}

void LED_RED_Toggle(void)
{
    if (issccp1Enabled == true) 
    {
        LED_RED_Off();
    } 
    else 
    {
        LED_RED_On();
    }
}

void LED_RED_Set(bool on) 
{
    
}

void LED_RED_SetIntensity(uint16_t request)
{  
    PWM_DutyCycleSet(PWM_GENERATOR_1, request);
} 

const struct LED_DIMMABLE ledRed = 
{
    &LED_RED_Initialize,
    &LED_RED_On,
    &LED_RED_Off,
    &LED_RED_Toggle,
    &LED_RED_Set,
    &LED_RED_SetIntensity
};