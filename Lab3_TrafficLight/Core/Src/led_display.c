#include "main.h"
#include "led_display.h"

int led_buffer[4] = {0, 0, 0, 0};
uint8_t segmentMap[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void display7SEG(int num) {
    if (num < 0 || num > 9) return;
    uint8_t segments = segmentMap[num];
    // Dữ liệu a-g vẫn ở PB0-PB6
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, (segments & 0x01) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, (segments & 0x02) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, (segments & 0x04) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, (segments & 0x08) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, (segments & 0x10) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, (segments & 0x20) ? GPIO_PIN_RESET : GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, (segments & 0x40) ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void update7SEG(int index) {
    // 1. Tắt hết 4 LED chống lem (Nối trực tiếp Anode -> Kéo xuống THẤP để tắt)
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);

    // 2. Xuất dữ liệu số mới
    display7SEG(led_buffer[index]);

    // 3. Kích hoạt đúng LED cần sáng (Kéo lên mức CAO)
    if (index == 0) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12, GPIO_PIN_SET);
    if (index == 1) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_13, GPIO_PIN_SET);
    if (index == 2) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_14, GPIO_PIN_SET);
    if (index == 3) HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
}

void clearAllTrafficLights() {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 |
                             GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15, GPIO_PIN_RESET);
}

void setTrafficLight1(int color) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10 | GPIO_PIN_12 | GPIO_PIN_14, GPIO_PIN_RESET);
    if (color == 0) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET); // Đỏ 1
    if (color == 1) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET); // Vàng 1
    if (color == 2) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); // Xanh 1
}

void setTrafficLight2(int color) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11 | GPIO_PIN_13 | GPIO_PIN_15, GPIO_PIN_RESET);
    if (color == 0) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET); // Đỏ 2
    if (color == 1) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET); // Vàng 2
    if (color == 2) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET); // Xanh 2
}
