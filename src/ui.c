#include "ui.h"

#include <stdio.h>

#include "timer.h"

typedef struct {
    PomodoroTimer timer;
    GtkWidget *mode_label;
    GtkWidget *time_label;
    GtkWidget *status_label;
    GtkWidget *start_button;
    GtkWidget *pause_button;
    guint tick_id;
} AppState;

static void load_css(void);
static void update_timer_labels(AppState *state);

static gboolean
on_timer_tick(gpointer user_data)
{
    AppState *state = user_data;

    timer_tick(&state->timer);
    update_timer_labels(state);

    return G_SOURCE_CONTINUE;
}

static void
on_start_clicked(GtkButton *button, gpointer user_data)
{
    AppState *state = user_data;

    (void)button;

    timer_start(&state->timer);
    update_timer_labels(state);
}

static void
on_pause_clicked(GtkButton *button, gpointer user_data)
{
    AppState *state = user_data;

    (void)button;

    timer_pause(&state->timer);
    update_timer_labels(state);
}

static void
on_reset_clicked(GtkButton *button, gpointer user_data)
{
    AppState *state = user_data;

    (void)button;

    timer_reset(&state->timer);
    update_timer_labels(state);
}

static gboolean
on_window_close(GtkWindow *window, gpointer user_data)
{
    AppState *state = user_data;

    (void)window;

    if (state->tick_id != 0) {
        g_source_remove(state->tick_id);
    }

    g_free(state);

    return FALSE;
}

static void
load_css(void)
{
    GtkCssProvider *provider;
    GdkDisplay *display;
    const char *css =
        "window { background: #f6f2ea; }"
        ".app-shell { background: #ffffff; border-radius: 8px; }"
        ".timer-mode { color: #3d405b; font-size: 20px; font-weight: 700; }"
        ".timer-time { color: #1f2933; font-size: 56px; font-weight: 800; }"
        ".timer-status { color: #6b7280; font-size: 13px; }"
        ".primary-action { background: #2f855a; color: #ffffff; }";

    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider, css, -1);

    display = gdk_display_get_default();
    gtk_style_context_add_provider_for_display(
        display,
        GTK_STYLE_PROVIDER(provider),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

    g_object_unref(provider);
}

static void
update_timer_labels(AppState *state)
{
    int minutes;
    int seconds;
    char time_text[16];

    minutes = state->timer.remaining_seconds / 60;
    seconds = state->timer.remaining_seconds % 60;

    snprintf(time_text, sizeof(time_text), "%02d:%02d", minutes, seconds);

    gtk_label_set_text(GTK_LABEL(state->mode_label), timer_mode_text(&state->timer));
    gtk_label_set_text(GTK_LABEL(state->time_label), time_text);
    gtk_label_set_text(
        GTK_LABEL(state->status_label),
        timer_is_running(&state->timer) ? "Running" : "Paused");

    gtk_widget_set_sensitive(state->start_button, !timer_is_running(&state->timer));
    gtk_widget_set_sensitive(state->pause_button, timer_is_running(&state->timer));
}

GtkWidget *
ui_create_main_window(GtkApplication *app)
{
    AppState *state;
    GtkWidget *window;
    GtkWidget *box;
    GtkWidget *button_box;
    GtkWidget *reset_button;

    load_css();

    state = g_new0(AppState, 1);
    timer_init(&state->timer, 25 * 60, 5 * 60);

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Pomodoro Timer");
    gtk_window_set_default_size(GTK_WINDOW(window), 320, 240);

    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 16);
    gtk_widget_set_margin_top(box, 32);
    gtk_widget_set_margin_bottom(box, 32);
    gtk_widget_set_margin_start(box, 32);
    gtk_widget_set_margin_end(box, 32);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(box, "app-shell");

    state->mode_label = gtk_label_new(NULL);
    gtk_widget_add_css_class(state->mode_label, "timer-mode");

    state->time_label = gtk_label_new(NULL);
    gtk_widget_add_css_class(state->time_label, "timer-time");

    state->status_label = gtk_label_new(NULL);
    gtk_widget_add_css_class(state->status_label, "timer-status");

    button_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_halign(button_box, GTK_ALIGN_CENTER);

    state->start_button = gtk_button_new_with_label("Start");
    state->pause_button = gtk_button_new_with_label("Pause");
    reset_button = gtk_button_new_with_label("Reset");
    gtk_widget_add_css_class(state->start_button, "primary-action");

    gtk_box_append(GTK_BOX(button_box), state->start_button);
    gtk_box_append(GTK_BOX(button_box), state->pause_button);
    gtk_box_append(GTK_BOX(button_box), reset_button);

    gtk_box_append(GTK_BOX(box), state->mode_label);
    gtk_box_append(GTK_BOX(box), state->time_label);
    gtk_box_append(GTK_BOX(box), state->status_label);
    gtk_box_append(GTK_BOX(box), button_box);

    gtk_window_set_child(GTK_WINDOW(window), box);

    g_signal_connect(state->start_button, "clicked", G_CALLBACK(on_start_clicked), state);
    g_signal_connect(state->pause_button, "clicked", G_CALLBACK(on_pause_clicked), state);
    g_signal_connect(reset_button, "clicked", G_CALLBACK(on_reset_clicked), state);
    g_signal_connect(window, "close-request", G_CALLBACK(on_window_close), state);

    state->tick_id = g_timeout_add_seconds(1, on_timer_tick, state);
    update_timer_labels(state);

    return window;
}
