#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_acodec.h>
#include "constants.h"
#include "heli.h"
#include "obstacles.h"
#include <time.h>

ALLEGRO_SAMPLE_INSTANCE *instance1 = NULL, *instance2 = NULL;

int main()  {

    bool redraw = false, gameOver = false, menu =true;
    int x=0, y=0, lives, score, multiple = 1;

    ///GAME VARIABLES
    struct heli copter;
    struct obstacle meteors[NUM_METEORS];
    struct obstacle bullet[NUM_BULLETS];
    struct explode explosion[NUM_EXPLOSION];

    ///ALLEGRO VARIABLES
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT event;
    ALLEGRO_TIMER *timer = NULL, *timer1 = NULL;
    ALLEGRO_EVENT_QUEUE *equeue = NULL;
    ALLEGRO_KEYBOARD_STATE kstate;
    ALLEGRO_BITMAP *imagebg = NULL, *menuimage = NULL;
    ALLEGRO_FONT *font = NULL;
    ALLEGRO_SAMPLE *sample1 = NULL, *sample2 = NULL, *sample3 = NULL, *sample4 = NULL;
    ALLEGRO_SAMPLE_INSTANCE *instance3 = NULL, *instance4 = NULL;

    ///INITIALIZATION FUNCTION
    if(!al_init())
        return -1;
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_acodec_addon();
    al_install_audio();
    al_reserve_samples(10);
    al_install_keyboard();
    al_install_mouse();
    reset(&copter, &meteors[0], &bullet[0], &explosion[0]);

    srand(time(0));

    al_set_new_display_flags(ALLEGRO_RESIZABLE);
    al_set_new_window_position(50, 50);
    display = al_create_display(WIDTH, HEIGHT);
    if(!display)
        return -1;
    al_set_window_title(display, "SHOOTING GAME");

    equeue = al_create_event_queue();
    timer = al_create_timer(1.0/FPS);
    timer1 = al_create_timer(1.0/FPS1);
    imagebg = al_load_bitmap("Resources\\bitmaps\\starBG.png");
    menuimage = al_load_bitmap("Resources\\bitmaps\\menu.png");
    font = al_load_ttf_font("Resources\\fonts\\momshand.ttf", 35, 0);

    sample1 = al_load_sample("Resources\\sounds\\helicopter_loop.wav");
    sample2 = al_load_sample("Resources\\sounds\\boom.wav");
    sample3 = al_load_sample("Resources\\sounds\\bullet.wav");
    sample4 = al_load_sample("Resources\\sounds\\game_over.wav");

    instance1 = al_create_sample_instance(sample1);
    instance2 = al_create_sample_instance(sample2);
    instance3 = al_create_sample_instance(sample3);
    instance4 = al_create_sample_instance(sample4);

    al_set_sample_instance_speed(instance2, 3.0);
    al_set_sample_instance_gain(instance2, 5.0);
    al_set_sample_instance_gain(instance4, 3.0);
    al_set_sample_instance_speed(instance3, 5.0);

    al_attach_sample_instance_to_mixer(instance1, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(instance2, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(instance3, al_get_default_mixer());
    al_attach_sample_instance_to_mixer(instance4, al_get_default_mixer());

    al_register_event_source(equeue, al_get_keyboard_event_source());
    al_register_event_source(equeue, al_get_timer_event_source(timer));
    al_register_event_source(equeue, al_get_timer_event_source(timer1));
    al_register_event_source(equeue, al_get_display_event_source(display));
    al_register_event_source(equeue, al_get_mouse_event_source());

    al_start_timer(timer);
    al_start_timer(timer1);

    al_hide_mouse_cursor(display);

    while(1){

        al_wait_for_event(equeue, &event);
        al_get_keyboard_state(&kstate);

        if(event.type == ALLEGRO_EVENT_TIMER){

            if(event.timer.source == timer){

                if(!menu){

                    if(al_key_down(&kstate, ALLEGRO_KEY_UP) || al_key_down(&kstate, ALLEGRO_KEY_W) )
                        moveHeliUp(&copter);

                    else if(al_key_down(&kstate, ALLEGRO_KEY_DOWN) || al_key_down(&kstate, ALLEGRO_KEY_S))
                        moveHeliDown(&copter);

                    else if(al_key_down(&kstate, ALLEGRO_KEY_RIGHT) || al_key_down(&kstate, ALLEGRO_KEY_D))
                        moveHeliRight(&copter);

                    else if(al_key_down(&kstate, ALLEGRO_KEY_LEFT) || al_key_down(&kstate, ALLEGRO_KEY_A))
                        moveHeliLeft(&copter);

                    startMeteor(&meteors[0], NUM_METEORS);
                    updateMeteor(&meteors[0], NUM_METEORS, &lives);
                    collideMeteor(&meteors[0], NUM_METEORS, &copter, &explosion[0], &lives);
                    updateBullet(&bullet[0], NUM_BULLETS);
                    collideBullet(&bullet[0], NUM_BULLETS, &meteors[0], NUM_METEORS, &explosion[0], &score);
                    updateExplosion(&explosion[0], NUM_EXPLOSION, sample2);

                    if(lives == 0){
                        al_play_sample_instance(instance4);
                        gameOver = true;
                    }
                    if(score == 30 * multiple){

                        for(int j=0; j<NUM_METEORS; j++)
                            meteors[j].speed += 2;

                        copter.speed += 2;
                        multiple++;
                    }

                }
            }

            else if(event.timer.source == timer1){

                    redraw = true;
            }

        }else if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE){

            break;

        }else if(event.type == ALLEGRO_EVENT_KEY_DOWN){

            if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                break;

            else if(event.keyboard.keycode == ALLEGRO_KEY_SPACE && !menu){
                al_play_sample_instance(instance3);
                startBullet(&bullet[0], NUM_BULLETS, &copter);
            }

            else if(event.keyboard.keycode == ALLEGRO_KEY_ENTER && menu){

                menu = false;
                score = 0;
                lives = 3;
                gameOver = false;
                reset(&copter, &meteors[0], &bullet[0], &explosion[0]);
            }
        }

        if(al_event_queue_is_empty(equeue) && redraw){

            redraw = false;

            if(!menu && !gameOver){

                drawHeli(&copter);
                drawMeteor(&meteors[0], NUM_METEORS);
                drawBullet(&bullet[0], NUM_BULLETS);
                drawExplosion(&explosion[0], NUM_EXPLOSION);
                al_draw_textf(font, al_map_rgb(60, 212, 217), 0, 0, 0, "SCORE : %d  LIVES : %d", score, lives);

                ///BACKGROUND
                al_draw_bitmap(imagebg, x, y, 0);
                if(x + WIDTH< WIDTH)
                    al_draw_bitmap(imagebg, x+WIDTH, y, 0);
                x--;

                if(x + WIDTH < 0)
                    x=0;

            }else if(menu || gameOver){

                menu = true;
                al_draw_bitmap(menuimage, 0, 0, 0);
            }

            al_flip_display();
            al_clear_to_color(al_map_rgb(0, 0, 0));

        }
    }

    al_destroy_display(display);
    al_destroy_bitmap(imagebg);
    al_destroy_bitmap(menuimage);
    for(int i=0; i<5; i++){
        al_destroy_bitmap(copter.image[i]);
    }
    for(int i=0; i<NUM_METEORS; i++){
        al_destroy_bitmap(meteors[i].image);
    }
    for(int i=0; i<NUM_BULLETS; i++){
        al_destroy_bitmap(bullet[i].image);
    }
    for(int i=0; i<NUM_EXPLOSION; i++){
        al_destroy_bitmap(explosion[i].image);
    }
    al_destroy_font(font);
    al_destroy_event_queue(equeue);
    al_destroy_timer(timer);
    al_destroy_timer(timer1);
    al_destroy_sample(sample1);
    al_destroy_sample(sample2);
    al_destroy_sample(sample3);
    al_destroy_sample(sample4);
    al_destroy_sample_instance(instance1);
    al_destroy_sample_instance(instance2);
    al_destroy_sample_instance(instance3);
    al_destroy_sample_instance(instance4);

    return 0;
}
