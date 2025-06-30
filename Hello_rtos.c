#include <stdio.h>
#include "Board_LED.h"
#include "Board_Buttons.h"
#include "Board_GLCD.h"
#include "GLCD_Config.h"
#include "cmsis_compiler.h"  // 適用於大多數 CMSIS 平台（推薦）
#include "DS_CM3.h"

extern GLCD_FONT GLCD_Font_16x24;

volatile int32_t delay_val = 500;

void delay_ms(uint32_t ms) {
    // 假設系統有1ms延遲函數。若無需精準，可用for-loop模擬
    for (volatile uint32_t i = 0; i < ms * 4000; i++) __NOP();
}

void BUT_OnLCD(uint32_t butNum) {
#ifdef __USE_LCD
    GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
    GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
    GLCD_DrawChar((9+butNum)*16, 7*24, 0x80+1);
#endif
}

void BUT_OffLCD(uint32_t butNum) {
#ifdef __USE_LCD
    GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
    GLCD_SetForegroundColor(GLCD_COLOR_LIGHT_GREY);
    GLCD_DrawChar((9+butNum)*16, 7*24, 0x80+0);
#endif
}

void LED_OnLCD(uint32_t ledNum) {
#ifdef __USE_LCD
    GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
    GLCD_SetForegroundColor(GLCD_COLOR_GREEN);
    GLCD_DrawChar((9+ledNum)*16, 6*24, 0x80+1);
#endif
}

void LED_OffLCD(uint32_t ledNum) {
#ifdef __USE_LCD
    GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
    GLCD_SetForegroundColor(GLCD_COLOR_LIGHT_GREY);
    GLCD_DrawChar((9+ledNum)*16, 6*24, 0x80+0);
#endif
}

int main(void) {
    uint32_t led_max = LED_GetCount();
    uint32_t led_num = 0;
    uint32_t button_max = Buttons_GetCount();
    uint32_t button_state;
    uint32_t button_num;
    uint32_t button_msk = (1U << button_max) - 1;

    SystemCoreClockUpdate();
    LED_Initialize();
    Buttons_Initialize();

#ifdef __USE_LCD
    GLCD_Initialize();
    GLCD_SetFont(&GLCD_Font_16x24);
    GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
    GLCD_ClearScreen();
    GLCD_SetBackgroundColor(GLCD_COLOR_BLUE);
    GLCD_SetForegroundColor(GLCD_COLOR_RED);
    GLCD_DrawString(0*16, 0*24, "   BareMetal Blinky ");
    GLCD_DrawString(0*16, 1*24, "     No RTOS        ");
    GLCD_SetBackgroundColor(GLCD_COLOR_WHITE);
    GLCD_SetForegroundColor(GLCD_COLOR_BLACK);
    GLCD_DrawString(2*16, 6*24, "LED:");
    GLCD_DrawString(2*16, 7*24, "KEY:");

    for (int i = 0; i < led_max; i++) {
        GLCD_DrawChar((9+i)*16, 6*24, 0x80+0);
    }
    for (int i = 0; i < button_max; i++) {
        GLCD_DrawChar((9+i)*16, 7*24, 0x80+0);
    }
#endif

    while (1) {
        // ====== Button polling ======
        button_state = Buttons_GetState();
        for (button_num = 0; button_num < button_max; button_num++) {
            if (button_state & (1U << button_num)) {
                BUT_OnLCD(button_num);
            } else {
                BUT_OffLCD(button_num);
            }
        }

        // Only blink if any button is pressed
        if (button_state & button_msk) {
            LED_On(led_num);
            LED_OnLCD(led_num);
            printf("LED %d : ON\n", led_num);

            delay_ms(delay_val);

            LED_Off(led_num);
            LED_OffLCD(led_num);
            printf("LED %d : OFF\n", led_num);

            led_num++;
            if (led_num >= led_max) led_num = 0;
        }

        delay_ms(delay_val);
    }
}