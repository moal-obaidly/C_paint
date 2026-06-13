#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>

#define HEIGHT 1920
#define WIDTH 1080
#define FPS 240
#define RADIUS 20
#define PALLETE_SIZE 50

uint32_t color_choice = 0x00FF0000;
uint32_t color_options [] = {0x00FF0000,0x00FFFFFF,0x000000FF,0x0000FF00,0x00000000,0x00FFFF00,0x00800080};
size_t num_of_colors = sizeof(color_options) / sizeof(color_options[0]);

// pick color from the options in the color_options array
bool choose_color(int x,int y){
    int choice;
    if (x < (PALLETE_SIZE*num_of_colors) && y<PALLETE_SIZE){
        choice = x/PALLETE_SIZE;
        color_choice = color_options[choice];
        return true;
    } 
    return false;
}


void draw_circle(SDL_Surface *surface, int x_center, int y_center, int radius, uint32_t color){
    int distance;
    for (int x = (x_center - radius); x<(x_center + radius); x++){
        for (int y = (y_center - radius); y<(y_center + radius); y++){

            distance = sqrt(pow(x-x_center,2) + pow(y-y_center,2));
            if (distance < radius ){
                SDL_Rect rect = {x,y,1,1};
                SDL_FillRect(surface, &rect,color);
            }
        }

    }

}

// presents all the color options in a nice pallete
void draw_color_picker(SDL_Surface *surface){

    for (int i = 0; i<num_of_colors; i++){
        SDL_Rect rect = {i*PALLETE_SIZE,0,PALLETE_SIZE,PALLETE_SIZE};
        SDL_FillRect(surface, &rect,color_options[i]);
    }
}

int main(int argc, char* argv[]){
    bool done = false;
    bool draw = false;
    SDL_Window *window = SDL_CreateWindow("Paint", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,HEIGHT,WIDTH,0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    
    //load background
    SDL_UpdateWindowSurface(window);
    float milli_delay = (1.0/FPS)*1000;






    if (argc == 2){
        // code to allow a background option, must be in the form of a ppm image
        FILE *fp = fopen(argv[1], "r");
        
        char magic[3];
        fscanf(fp, "%2s", magic);
        int width,height;
        fscanf(fp, "%d %d", &width, &height);
        int max_brightness;
        fscanf(fp, "%d", &max_brightness);

        int r,g,b;
        int brightness;
        int index;

        
        for (int i = 0; i<height; i++){

            for (int j = 0; j<width; j++){
                
                fscanf(fp, "%d %d %d", &r,&g,&b);
                
                unsigned int hexColor = (g << 16) | (b << 8) | r; 
                uint32_t color = hexColor;
                SDL_Rect rect = {i,j,1,1};
                SDL_FillRect(surface, &rect,color);
                
                

                
            }
        }
    }
    else if(argc > 2){
        printf("usage is either: %s or %s background_filename\n",argv[0],argv[0]);
    }

    int x,y;
    while(!done){
        SDL_Event event;

        while(SDL_PollEvent(&event)){
            switch (event.type){

                case SDL_QUIT:
                    done = true;
                    break;
                case SDL_MOUSEMOTION:
                    x = event.motion.x ; 
                    y = event.motion.y ;
                    break;
                    
                case SDL_MOUSEBUTTONDOWN:
                    
                    
                    x = event.motion.x ; 
                    y = event.motion.y ;
                    if (choose_color(x,y)){
                        break;
                    }
                    draw = true;
                    break;
                case SDL_MOUSEBUTTONUP:
                    draw = false;
                    break;
                }
            }
               
        
    draw_color_picker(surface);
    SDL_UpdateWindowSurface(window);
    if (draw){
        
        draw_circle(surface,x,y,RADIUS,color_choice);
        // SDL_Rect rect = {x,y,50,50};
        // SDL_FillRect(surface, &rect,0x00FF0000);
        SDL_UpdateWindowSurface(window);
    }
    

    SDL_Delay(milli_delay);
    }
    
    return 0;


}