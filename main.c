#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"

// Initialization function for graphics
void initializeGraphics(Graphics_Context* g_sContext_p) {
    // Initialize the LCD
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    // Initialize context
    Graphics_initContext(g_sContext_p, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);

    // Set colors and fonts
    Graphics_setForegroundColor(g_sContext_p, GRAPHICS_COLOR_WHITE);
    Graphics_setBackgroundColor(g_sContext_p, GRAPHICS_COLOR_BLACK);
    Graphics_setFont(g_sContext_p, &g_sFontFixed6x8);

    // Clear the screen
    Graphics_clearDisplay(g_sContext_p);
}

unsigned colormix(unsigned r,unsigned g,unsigned b) {
    return ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
}

int main(void)
{
    // Stop watchdog timer
    MAP_WDT_A_holdTimer();

    // Initializations
    Graphics_Context g_sContext; // Create context
    initializeGraphics(&g_sContext);

    int i = 0;
    const int a = 21.3; // Division factor
    for (i = 0; i < 128; i++)
    {
        unsigned r = (i < a) ? 255 : (i < 2 * a) ? 255 - (i - a) * 12 :
                     (i < 4 * a) ? 0 : (i < 5 * a) ? (i - 4 * a) * 12 : 255;
        unsigned g = (i < a) ? i * 12 : (i < 3 * a) ? 255 :
                     (i < 4 * a) ? 255 - (i - 3 * a) * 12 : 0;
        unsigned b = (i < 2 * a) ? 0 : (i < 3 * a) ? (i - 2 * a) * 12 :
                     (i < 5 * a) ? 255 : 255 - (i - 5 * a) * 12;
        Graphics_setForegroundColor(&g_sContext, colormix(r, g, b));
        Graphics_drawLineH(&g_sContext, 0, 127, i);
    }

    Graphics_setForegroundColor(&g_sContext, GRAPHICS_COLOR_BLACK);
    Graphics_drawLineH(&g_sContext, 0, 127, 126);
    Graphics_drawLineH(&g_sContext, 0, 127, 127);
}
