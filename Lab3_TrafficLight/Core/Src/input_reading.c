#include "main.h"
#include "input_reading.h"

#define NO_OF_BUTTONS 3
#define BUTTON_IS_PRESSED GPIO_PIN_RESET
#define BUTTON_IS_RELEASED GPIO_PIN_SET

static GPIO_PinState buttonBuffer[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTONS];

void button_reading(void) {
    for(int i = 0; i < NO_OF_BUTTONS; i++) {
        debounceButtonBuffer2[i] = debounceButtonBuffer1[i];

        // Đã đổi sang PA1, PA2, PA3
        if (i == 0) debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
        if (i == 1) debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2);
        if (i == 2) debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);

        if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
            buttonBuffer[i] = debounceButtonBuffer1[i];
        }
    }
}

unsigned char is_button_pressed(unsigned char index) {
    if (index >= NO_OF_BUTTONS) return 0;
    return (buttonBuffer[index] == BUTTON_IS_PRESSED);
}
