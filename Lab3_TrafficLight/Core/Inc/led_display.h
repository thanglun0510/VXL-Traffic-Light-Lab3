#ifndef INC_LED_DISPLAY_H_
#define INC_LED_DISPLAY_H_

extern int led_buffer[4];

void display7SEG(int num);
void update7SEG(int index);
void clearAllTrafficLights(void);
void setTrafficLight1(int color); // 0: Đỏ, 1: Vàng, 2: Xanh
void setTrafficLight2(int color);

#endif /* INC_LED_DISPLAY_H_ */
