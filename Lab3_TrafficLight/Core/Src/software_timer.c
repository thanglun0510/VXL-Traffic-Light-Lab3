#include "software_timer.h"

int timer_counter[10] = {0};
int timer_flag[10] = {0};
int TIMER_CYCLE = 10; // Chu kỳ ngắt của Timer 2 là 10ms

void setTimer(int index, int duration) {
    timer_counter[index] = duration / TIMER_CYCLE;
    timer_flag[index] = 0;
}

void timer_run(void) {
    for (int i = 0; i < 10; i++) {
        if (timer_counter[i] > 0) {
            timer_counter[i]--;
            if (timer_counter[i] <= 0) {
                timer_flag[i] = 1;
            }
        }
    }
}
