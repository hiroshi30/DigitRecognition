// ESC - quit app
// MLB - draw
// MRB - erase
// 0 - save canvas as 0
// 1 - save canvas as 1
// w - clear canvas
// a - save image set

#ifdef Debug
#include <stdio.h>
#endif // Debug

#include <kazamori.h>
#ifdef Debug
#include <print.h>
#endif // Debug

#include <SDL2/SDL.h>


#define window_title "DigitRecognition"
#define window_width (16 * 65)
#define window_height (9 * 65)

#define pixel_width (16 * 5)
#define pixel_height (9 * 5)


int init();
void quit();
void set_color(SDL_Color* color);
void clear();
void add(ImageSet* image_set, int index);


SDL_Window* window;
SDL_Renderer* renderer;
int surface[window_height / pixel_height][window_width / pixel_width];


int main(int argc, char* argv[]) {
    if (init() != 0) {
        return 1;
    }


    clear();


    SDL_Color background_color = {0, 0, 0, 255};
    SDL_Color color = {255, 255, 255, 255};


    const char* image_set_file_name = "ImageSet.txt";
    ImageSet image_set;
    ImageSet_load(&image_set, image_set_file_name);
    if (image_set.__constructed__ != 1) {
        ImageSet_construct(&image_set, 0, 5, 3, window_height / pixel_height, window_width / pixel_width, 2);
    }
    #ifdef Debug
    ImageSet_print_output(&image_set);
    #endif // Debug


    int click_left = 0;
    int click_right = 0;

    bool run = true;
    bool action = true;
    SDL_Event event;
    while (run) {
        while(SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                run = false;
                break;
            }

            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    run = false;
                    break;
                }

                if (event.key.keysym.sym == SDLK_0) {
                    add(&image_set, 0);
                    #ifdef Debug
                    ImageSet_print_output(&image_set);
                    #endif // Debug
                }

                if (event.key.keysym.sym == SDLK_1) {
                    add(&image_set, 1);
                    #ifdef Debug
                    ImageSet_print_output(&image_set);
                    #endif // Debug
                }

                if (event.key.keysym.sym == SDLK_w) {
                    action = true;
                    clear();
                }

                if (event.key.keysym.sym == SDLK_a) {
                    ImageSet_save(&image_set, image_set_file_name);
                }
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    click_left = 1;
                }

                if (event.button.button == SDL_BUTTON_RIGHT) {
                    click_right = 1;
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    click_left = 0;
                }

                if (event.button.button == SDL_BUTTON_RIGHT) {
                    click_right = 0;
                }
            }
        }

        if (click_left == 1 || click_right == 1) {
            int mouse_x, mouse_y;
            SDL_GetMouseState(&mouse_x, &mouse_y);
            if (mouse_x > 0 && mouse_x < window_width && mouse_y > 0 && mouse_y < window_height) {
                action = true;
                surface[mouse_y / pixel_height][mouse_x / pixel_width] = click_left * (1 - click_right);
            }
        }

        if (action) {
            action = false;

            set_color(&background_color);
            SDL_RenderClear(renderer);
            
            set_color(&color);
            for (int x = 0; x <= window_width; x += pixel_width) {
                SDL_RenderDrawLine(renderer, x, 0, x, window_height);
            }
            for (int y = 0; y <= window_height; y += pixel_height) {
                SDL_RenderDrawLine(renderer, 0, y, window_width, y);
            }

            for (int y = 0; y < window_height / pixel_height; ++y) {
                for (int x = 0; x < window_width / pixel_width; ++x) {
                    if (surface[y][x] == 1) {
                        SDL_RenderFillRect(renderer, &(SDL_Rect){
                            x * pixel_width,
                            y * pixel_height,
                            pixel_width,
                            pixel_height
                        });
                    }
                }
            }

            SDL_RenderPresent(renderer);
        }
    }

    ImageSet_destruct(&image_set);

    quit();

    return 0;
}


int init() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        #ifdef Debug
        printf("!!! ERROR in SDL_Init(): %s !!!\n", SDL_GetError());
        #endif // Debug

        return 1;
    }

    window = SDL_CreateWindow(window_title, 100, 100, window_width, window_height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        #ifdef Debug
        printf("!!! ERROR in SDL_CreateWindow(): %s !!!\n", SDL_GetError());
        #endif // Debug
        
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        #ifdef Debug
        printf("!!! ERROR in SDL_CreateRenderer(): %s !!!\n", SDL_GetError());
        #endif // Debug
        
        return 1;
    }

    return 0;
}


void quit() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


void set_color(SDL_Color* color) {
    SDL_SetRenderDrawColor(renderer, color->r, color->g, color->b, color->a);
}


void clear() {
    for (int y = 0; y < window_height / pixel_height; ++y) {
        for (int x = 0; x < window_width / pixel_width; ++x) {
            surface[y][x] = 0;
        }
    }
}


void add(ImageSet* image_set, int index) {
    double input[image_set->channels * image_set->height * image_set->width];
    for (int c = 0; c < image_set->channels; ++c) {
        for (int h = 0; h < image_set->height; ++h) {
            for (int w = 0; w < image_set->width; ++w) {
                input[c * image_set->height * image_set->width + h * image_set->width + w] = surface[h][w];
            }
        }
    }
    
    double output[image_set->output_length];
    for (int i = 0; i < image_set->output_length; ++i) {
        output[i] = 0;
    }
    output[index] = 1;

    ImageSet_add(image_set, input, output); 
}
