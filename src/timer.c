#include "timer.h"

void
timer_init(PomodoroTimer *timer, int work_seconds, int break_seconds)
{
    timer->work_seconds = work_seconds;
    timer->break_seconds = break_seconds;
    timer->remaining_seconds = work_seconds;
    timer->is_running = false;
    timer->mode = TIMER_MODE_WORK;
}

void
timer_start(PomodoroTimer *timer)
{
    timer->is_running = true;
}

void
timer_pause(PomodoroTimer *timer)
{
    timer->is_running = false;
}

void
timer_reset(PomodoroTimer *timer)
{
    timer->is_running = false;
    timer->mode = TIMER_MODE_WORK;
    timer->remaining_seconds = timer->work_seconds;
}

void
timer_tick(PomodoroTimer *timer)
{
    if (!timer->is_running) {
        return;
    }

    if (timer->remaining_seconds > 0) {
        timer->remaining_seconds--;
    }

        if (timer->remaining_seconds > 0) {
            return;
        }
    }

    timer->mode = timer->mode == TIMER_MODE_WORK ? TIMER_MODE_BREAK : TIMER_MODE_WORK;
    timer->remaining_seconds = timer->mode == TIMER_MODE_WORK
        ? timer->work_seconds
        : timer->break_seconds;
}

bool
timer_is_running(const PomodoroTimer *timer)
{
    return timer->is_running;
}

const char *
timer_mode_text(const PomodoroTimer *timer)
{
    if (timer->mode == TIMER_MODE_WORK) {
        return "Focus";
    }

    return "Break";
}
