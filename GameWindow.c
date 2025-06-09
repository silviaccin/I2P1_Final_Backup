#include "global.h"
#include "GameWindow.h"

void game_init() {
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(10);

    display = al_create_display(WINDOW_WIDTH, WINDOW_HEIGHT);
    timer = al_create_timer(1.0 / FPS);
    event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    al_start_timer(timer);
}

void game_start() {
    while (game_running) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            game_running = false;
        }

        // rendering here
        al_clear_to_color(al_map_rgb(0, 0, 0));
        al_flip_display();
    }
}

void game_destroy() {
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);
}

