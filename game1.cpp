#include "function.h"
#include "baseObject.h"
#include "map.h"
#include "mainObject.h"
#include "timer_game.h"
#include "ThreatsObject.h"

baseObject  g_background;

bool init_data() {
    bool success = true;
    int ret = SDL_Init(SDL_INIT_VIDEO);
    if (ret < 0) return false;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); //set image quality
    g_window = SDL_CreateWindow("Game SDL 2.0: By Thanh Trung", 
                                SDL_WINDOWPOS_UNDEFINED, 
                                SDL_WINDOWPOS_UNDEFINED,
                                SCREEN_WIDTH,SCREEN_HEIGHT,
                                SDL_WINDOW_SHOWN);

    if (g_window == NULL) success = false;
    else {
        g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
        if (g_screen == NULL) return success = false;
        else {
            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            int imgFlags = IMG_INIT_PNG;
            if (!(IMG_Init(imgFlags) & imgFlags)) {
                printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                success = false;
            }
           }
        }
    return success;
}

bool load_background() {
    bool ret = g_background.LoadImg("image//background.png", g_screen);
    if (ret == false) return false;
    return true;
}

void close() {
    g_background.Free();
    SDL_DestroyRenderer(g_screen);
    g_screen = NULL;
    SDL_DestroyWindow(g_window);
    g_window = NULL;
    IMG_Quit();
    SDL_Quit();
}

std::vector<threats_object*> MakeThreatList() {
    std::vector<threats_object*> list_threats;

    //// Threat 0
    for (int i = 0; i < 5; i++) {
        threats_object* p_threat = new threats_object();  
        p_threat->load_img_threat("image/threat/wolf_white_walk_left.png", g_screen);
        p_threat->threat_type = 0;
        p_threat->set_clip();
        p_threat->set_type_move(threats_object::MOVE_IN_SPACE_THREAT);
        if(i<2) p_threat->set_real_x(7350 + i * 730);
        else p_threat->set_real_x(17000 + i * 800);

        p_threat->set_real_y(200);
        p_threat->set_animation_real(p_threat->get_real_x() - 20, p_threat->get_real_x() + 20);
        p_threat->set_input_left(1);
        list_threats.push_back(p_threat);
    }

    // Threat 1
    for (int i = 0; i < 10; i++) {
        threats_object* p_threat = new threats_object(); 
        p_threat->load_img_threat("image/threat/wolf_black_walk_left.png", g_screen);
        p_threat->threat_type = 1;
        p_threat->set_clip();
        p_threat->set_type_move(threats_object::MOVE_IN_SPACE_THREAT);
        if (i < 7)
            p_threat->set_real_x(1400 + i * 1150);
        else p_threat->set_real_x(13800 + i * 800);

        p_threat->set_real_y(200);
        p_threat->set_animation_real(p_threat->get_real_x() - 50, p_threat->get_real_x() + 50);
        p_threat->set_input_left(1);
        list_threats.push_back(p_threat);
    }

    // Threat 2
    for (int i = 0; i < 15; i++) {
        threats_object* p_threat = new threats_object();  
        p_threat->load_img_threat("image/threat/snake_walk.png", g_screen);
        p_threat->threat_type = 2;
        p_threat->set_clip();
        p_threat->set_type_move(threats_object::MOVE_IN_SPACE_THREAT);
        p_threat->set_real_x(9280 + i * 650);
        p_threat->set_real_y(200);
        p_threat->set_animation_real(p_threat->get_real_x() - 50, p_threat->get_real_x() + 50);
        p_threat->set_input_left(1);
        list_threats.push_back(p_threat);
    }

    ////// Threat 3
    //for (int i = 0; i < 10; i++) {
    //    threats_object* p_threat = new threats_object();  // Tạo đối tượng trên heap
    //    p_threat->load_img_threat("image/threat/Flame.png", g_screen);
    //    p_threat->set_clip();
    //    p_threat->set_type_move(threats_object::STATIC_THREAT);
    //    p_threat->set_real_x(550 + i * 2400);
    //    p_threat->set_real_y(250);
    //    p_threat->set_input_left(0);
    //    list_threats.push_back(p_threat);
    //}

    return list_threats;
}

int main(int argc, char* argv[]) {
    
   timer_game fps_timer;

   bool is_quit = false;
   bool is_game_over = false;

    if (init_data() == false) return -1;
    if (load_background() == false) return -1;

    SDL_Rect destinationRect;
    destinationRect.x = 0;
    destinationRect.y = 0;
    destinationRect.w = SCREEN_WIDTH;   
    destinationRect.h = SCREEN_HEIGHT;  

    // map
    game_map gameMap;
    gameMap.load_map("image//map//map01.dat");
    gameMap.load_tile(g_screen);
    
    //character
    main_object p_player;
    p_player.loadImg("image//Shinobi//Run.png", g_screen);
    p_player.set_clip();

    //threat
    std::vector<threats_object*> threats_list = MakeThreatList();

    while (!is_quit) {
        fps_timer.start();
        while (SDL_PollEvent(&g_event) != 0) {
            if (g_event.type == SDL_QUIT) is_quit = true;
                p_player.handel_input_action(g_event, g_screen);
        }

            SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
            g_background.Render(g_screen, NULL, &destinationRect);

            //charater
            map map_data = gameMap.getmap();

            p_player.set_map_xy(map_data.start_x, map_data.start_y);
            p_player.do_player(map_data, threats_list);
            p_player.show(threats_list, g_screen);

            // update map
            gameMap.set_map(map_data);
            gameMap.draw_map(g_screen);

            // threat
            for (int i = 0; i < threats_list.size(); i++) {
                threats_object* p_threat = threats_list.at(i);
                if (p_threat != NULL) {

                    SDL_Rect player_rect = p_player.get_rect();
                    SDL_Rect threat_rect = p_threat->get_rect();

                    if (p_player.check_collision(player_rect, threat_rect)) {
                        if (p_player.isAttacking()) {
                            threats_list.erase(threats_list.begin() + i);
                            i--;
                        }                  
                    }
                    if (p_player.set_dead() == true) {
                        if (MessageBox(NULL, L"GAME OVER", L"Infor", MB_OK | MB_ICONSTOP) == IDOK) {
                            p_threat->Free();
                            close();
                            SDL_Quit();
                            return 0;
                        }
                    }
                    p_threat->set_map_xy(map_data.start_x, map_data.start_y);
                    p_threat->do_player(map_data);
                    p_threat->show(g_screen, &p_player);
                    p_threat->img_move_type(g_screen);
                }
            }
    
            SDL_RenderPresent(g_screen);
            ////fps
            int real_time = fps_timer.get_ticks();
            int time_one_frame = 1000 / FRAME_PER_SECOND; //ms
            if (real_time < time_one_frame) {
                int delay_time = time_one_frame - real_time;
                if (delay_time > 0) SDL_Delay(delay_time);
            }
    }
    close();

    return 0;
}
