#include "dolphin/dolphin.h"
#include <furi.h>
#include <gui/gui.h>
#include <stdlib.h>

int SCORE = 0;
int LEVEL = 1;
int score_tracker = 0;

char score_str[16];

int player_x = 6;
int player_y = 28;
int hatch_x = 1;
int hatch_y = 1;

bool is_flapping = false;

// Specials:
int max_flap_x = 1;
int max_flap_y = 2;
int duck_count = 1;
int duck_swarm[9];

// Sprite coordinates for drawing
int player[][2] = {{6, 3}, {7, 3}, {5, 4}, {8, 4}, {5, 5}, {8, 5}, {2, 6}, {3, 6}, {4, 6}, {8, 6}, {9, 6}, {2, 7}, {7, 7}, {3, 8}, {7, 8}, {4, 9}, {5, 9}, {6, 9}};
int portal[][2] = {{6, 2}, {7, 2}, {8, 2}, {9, 2}, {10, 2}, {11, 2}, {12, 2}, {13, 2}, {5, 3}, {6, 3}, {7, 3}, {8, 3}, {9, 3}, {10, 3}, {11, 3}, {12, 3}, {13, 3},{14, 3}, {4, 4}, {5, 4}, {6, 4}, {13, 4}, {14, 4}, {15, 4}, {3, 5}, {4, 5}, {5, 5}, {14, 5}, {15, 5}, {16, 5}, {2, 6}, {3, 6}, {4, 6}, {15, 6}, {16, 6}, {17, 6}, {2, 7}, {3, 7}, {16, 7}, {17, 7}, {2, 8}, {3, 8}, {16, 8}, {17, 8}, {2, 9}, {3, 9}, {16, 9}, {17, 9}, {2, 10}, {3, 10}, {16, 10}, {17, 10}, {2, 11}, {3, 11}, {16, 11}, {17, 11}, {2, 12}, {3, 12}, {16, 12}, {17, 12}, {2, 13}, {3, 13}, {16, 13}, {17, 13}, {2, 14}, {3, 14}, {16, 14}, {17, 14}, {2, 15}, {3, 15}, {16, 15}, {17, 15}, {2, 16}, {3, 16}, {16, 16}, {17, 16}, {2, 17}, {3, 17}, {16, 17}, {17, 17}, {2, 18}, {3, 18}, {16, 18}, {17, 18}, {2, 19}, {3, 19}, {16, 19}, {17, 19}, {2, 20}, {3, 20}, {16, 20}, {17, 20}, {2, 21}, {3, 21}, {16, 21}, {17, 21}, {2, 22}, {3, 22}, {16, 22}, {17, 22}, {2, 23}, {3, 23}, {16, 23}, {17, 23}, {2, 24}, {3, 24}, {16, 24}, {17, 24}, {2, 25}, {3, 25}, {16, 25}, {17, 25}, {2, 26}, {3, 26}, {16, 26}, {17, 26}, {2, 27}, {3, 27}, {16, 27}, {17, 27}, {2, 28}, {3, 28}, {16, 28}, {17, 28}, {2, 29}, {3, 29}, {4, 29}, {15, 29}, {16, 29}, {17, 29}, {3, 30}, {4, 30}, {5, 30}, {14, 30}, {15, 30}, {16, 30}, {4, 31}, {5, 31}, {6, 31}, {13, 31}, {14, 31}, {15, 31}, {5, 32}, {6, 32}, {7, 32}, {8, 32}, {9, 32}, {10, 32}, {11, 32}, {12, 32}, {13, 32}, {14, 32}, {6, 33}, {7, 33}, {8, 33}, {9, 33}, {10, 33}, {11, 33}, {12, 33}, {13, 33}};

void collide_rect()
{
    //int player_left = player_x;
    //int player_top = player_y - 1;
    //int player_right = player_x + 8;
    //int player_bottom = player_y + 7;
}

void draw_portal(Canvas * canvas)
{
    int array_size = sizeof(portal) / sizeof(portal[0]);
    for (int i = 0; i < array_size; i++)
    {
        int x = portal[i][0];
        int y = portal[i][1];
        
        if(x != 0 && y != 0)
        {
            canvas_draw_dot(canvas, x + 104, y + 16);
        }
    }
}

void draw_player(Canvas * canvas)
{
    if (is_flapping)
    {
        player_y -= max_flap_y;
    }
    
    else if (!is_flapping)
    {
        player_y += max_flap_y;
    }

    player_x += max_flap_x;
    
    int array_size = sizeof(player) / sizeof(player[0]);
    for (int j = 0; j < 9; j++)
    {
        if (duck_swarm[j] == 1)
        {
            int row = j % 3 + 1;
            int column = j <= 2 ? 1 : j < 6 ? 2 : 3;

            for (int i = 0; i < array_size; i++)
            {
                int x = player[i][0] + (8 * row);
                int y = player[i][1] + (8 * column);
                if(x != 0 && y != 0)
                {
                    canvas_draw_dot(canvas, x + player_x, y + player_y);
                }
            }
        }
    }
}

static void draw_callback(Canvas * canvas, void * context)
{
    UNUSED(context);
    canvas_clear(canvas);
    furi_delay_us(40000);
    
    score_tracker += 1;
    // Increment score every 1 second
    if (score_tracker == 25)
    {
        SCORE += 1;
        score_tracker = 0;

        duck_swarm[duck_count] = 1;
        duck_count += 1;
        
    }
    
    collide_rect();
    draw_player(canvas);
    draw_portal(canvas);

    snprintf(score_str, sizeof(score_str), "%d", SCORE);
    canvas_draw_str(canvas, 2, 8, score_str);

    canvas_commit(canvas);
}

static void input_callback(InputEvent * event, void * context)
{
    FuriMessageQueue * queue = (FuriMessageQueue *)context;
    if(event->type == InputTypeShort || event->type == InputTypeRepeat || event->type == InputTypePress)
    {
        if (event->key == InputKeyOk)
        {
            is_flapping = true;
        }
    }

    if(event->type == InputTypeRelease)
    {
        if (event->key == InputKeyOk)
        {
            is_flapping = false;
        }
    }

    furi_message_queue_put(queue, event, FuriWaitForever);
}

int main()
{
    duck_swarm[0] = 1;
    for (int i = 1; i < 9; i++)
    {
        duck_swarm[i] = 0;
    }

    FuriMessageQueue * queue = furi_message_queue_alloc(8, sizeof(InputEvent));
    ViewPort * view_port = view_port_alloc();
    view_port_draw_callback_set(view_port, draw_callback, NULL);
    view_port_input_callback_set(view_port, input_callback, queue);
    Gui* gui = (Gui *)furi_record_open("gui");
    gui_add_view_port(gui, view_port, GuiLayerFullscreen);
    dolphin_deed(DolphinDeedPluginGameStart);
    
    InputEvent event;
    bool running = true;
    while(running)
    {
        if(furi_message_queue_get(queue, &event, FuriWaitForever) == FuriStatusOk)
        {
            if(event.type == InputTypeShort && event.key == InputKeyBack)
            {
                running = false;
            }
        }
        view_port_update(view_port);
    }

    view_port_enabled_set(view_port, false);
    furi_message_queue_free(queue);
    gui_remove_view_port(gui, view_port);
    view_port_free(view_port);
    furi_record_close(RECORD_GUI);
    
    return 0;
}
