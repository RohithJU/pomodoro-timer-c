#ifndef TIMER_H
#define TIMER_H

#include <stdbool.h>

typedef enum {
    TIMER_MODE_WORK,
    TIMER_MODE_BREAK
} TimerMode;

typedef struct {
    int work_seconds;
    int break_seconds;
    int remaining_seconds;
    bool is_running;
    TimerMode mode;
} PomodoroTimer;

void timer_init(PomodoroTimer *timer, int work_seconds, int break_seconds);
void timer_start(PomodoroTimer *timer);
void timer_pause(PomodoroTimer *timer);
void timer_reset(PomodoroTimer *timer);
void timer_tick(PomodoroTimer *timer);
bool timer_is_running(const PomodoroTimer *timer);
const char *timer_mode_text(const PomodoroTimer *timer);

#endif
