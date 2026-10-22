#include "main.h"
#include "input_reading.h"
#include "input_processing.h"
#include "software_timer.h"
#include "led_display.h"


// Thời gian mặc định
int red_time = 5;
int amber_time = 2;
int green_time = 3;

int system_mode = 1;
int temp_value = 0;

// Các cờ chống dội/đè nút
int button1_flag = 0;
int button2_flag = 0;
int button3_flag = 0;

// Các biến cho Mode 1 (Đếm ngược)
int traffic_state = 1; // 1: Đỏ-Xanh, 2: Đỏ-Vàng, 3: Xanh-Đỏ, 4: Vàng-Đỏ
int countdown1 = 5;
int countdown2 = 3;

// Hàm hỗ trợ tách số để hiển thị lên 4 LED 7 đoạn
void update_led_buffer(int val1, int val2) {
    led_buffer[0] = val1 / 10;
    led_buffer[1] = val1 % 10;
    led_buffer[2] = val2 / 10;
    led_buffer[3] = val2 % 10;
}

void fsm_for_input_processing(void) {
    // ==========================================
    // 1. XỬ LÝ NÚT 1: CHUYỂN MODE
    // ==========================================
    if (is_button_pressed(0)) {
        if (button1_flag == 0) {
            button1_flag = 1;
            system_mode++;
            if (system_mode > 4) {
                system_mode = 1;
                // Khởi tạo lại Mode 1 khi thoát cài đặt
                traffic_state = 1;
                countdown1 = red_time;
                countdown2 = green_time;
                setTrafficLight1(0); // Trụ 1 Đỏ
                setTrafficLight2(2); // Trụ 2 Xanh
                setTimer(0, 1000);   // Bật lại đếm lùi 1 giây
            } else {
                // Khi vừa sang Mode 2, 3, 4
                clearAllTrafficLights();
                setTimer(1, 250); // Bật nhấp nháy 2Hz

                if (system_mode == 2) temp_value = red_time;
                if (system_mode == 3) temp_value = amber_time;
                if (system_mode == 4) temp_value = green_time;
            }
        }
    } else {
        button1_flag = 0;
    }

    // ==========================================
    // 2. XỬ LÝ NÚT 2: TĂNG GIÁ TRỊ THỜI GIAN
    // ==========================================
    if (is_button_pressed(1)) {
        if (button2_flag == 0) {
            button2_flag = 1;
            if (system_mode >= 2 && system_mode <= 4) {
                temp_value++;
                if (temp_value > 99) temp_value = 1;
            }
        }
    } else {
        button2_flag = 0;
    }

    // ==========================================
    // 3. XỬ LÝ NÚT 3: XÁC NHẬN LƯU (SET)
    // ==========================================
    if (is_button_pressed(2)) {
        if (button3_flag == 0) {
            button3_flag = 1;
            if (system_mode == 2) red_time = temp_value;
            if (system_mode == 3) amber_time = temp_value;
            if (system_mode == 4) green_time = temp_value;
        }
    } else {
        button3_flag = 0;
    }

    // ==========================================
    // 4. MÁY TRẠNG THÁI HIỂN THỊ CHÍNH
    // ==========================================
    switch (system_mode) {
        case 1:
            update_led_buffer(countdown1, countdown2);
            if (timer_flag[0] == 1) {
                setTimer(0, 1000);
                countdown1--;
                countdown2--;

                if (traffic_state == 1) { // Đỏ 1 - Xanh 2
                    if (countdown2 <= 0) {
                        traffic_state = 2; countdown2 = amber_time;
                        setTrafficLight2(1);
                    }
                }
                else if (traffic_state == 2) { // Đỏ 1 - Vàng 2
                    if (countdown1 <= 0) {
                        traffic_state = 3; countdown1 = green_time; countdown2 = red_time;
                        setTrafficLight1(2); setTrafficLight2(0);
                    }
                }
                else if (traffic_state == 3) { // Xanh 1 - Đỏ 2
                    if (countdown1 <= 0) {
                        traffic_state = 4; countdown1 = amber_time;
                        setTrafficLight1(1);
                    }
                }
                else if (traffic_state == 4) { // Vàng 1 - Đỏ 2
                    if (countdown2 <= 0) {
                        traffic_state = 1; countdown1 = red_time; countdown2 = green_time;
                        setTrafficLight1(0); setTrafficLight2(2);
                    }
                }
            }
            break;

        case 2: // Cài đặt đèn Đỏ
            update_led_buffer(temp_value, 2);
            if (timer_flag[1] == 1) {
                setTimer(1, 250);
                HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10 | GPIO_PIN_11); // Đảo chân Đỏ 1, Đỏ 2
            }
            break;

        case 3: // Cài đặt đèn Vàng
            update_led_buffer(temp_value, 3);
            if (timer_flag[1] == 1) {
                setTimer(1, 250);
                HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_12 | GPIO_PIN_13); // Đảo chân Vàng 1, Vàng 2
            }
            break;

        case 4: // Cài đặt đèn Xanh
            update_led_buffer(temp_value, 4);
            if (timer_flag[1] == 1) {
                setTimer(1, 250);
                HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14 | GPIO_PIN_15); // Đảo chân Xanh 1, Xanh 2
            }
            break;
    }
} // <--- DẤU ĐÓNG NGOẶC HÀM BỊ THIẾU ĐÃ ĐƯỢC THÊM VÀO ĐÂY
