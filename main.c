#include <SDL2/SDL.h>
#include "pedestrians.h"

const int WIDTH = 640;
const int HEIGHT = 480;

const int n_peds = 400;

const double UPPER_WALL = HEIGHT/2-0.5*HEIGHT/10;
const double LOWER_WALL = HEIGHT/2+HEIGHT/10;

int main(int argc, char *argv[])
{
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
      printf("SDL_Init Error\n");
      return 1;
  }
  SDL_Window *window = SDL_CreateWindow("Pedestrians!", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL) {
      printf("SDL_CreateWindow Error\n");
      return 1;
  }
  SDL_Renderer *render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (render == NULL) {
      printf("SDL_CreateRenderer Error\n");
      return 1;
  }


  pedestrian* peds = malloc(sizeof(pedestrian) * n_peds);
  for(int i = 0; i<n_peds/2; i++){
      pedestrian jerry;
      jerry.r.x = WIDTH/2 - WIDTH/10;
      jerry.r.y = HEIGHT/2 + uniform01()*HEIGHT/10;
      jerry.v.x = 0.0;
      jerry.v.y = 0.0;
      jerry.a_p.x = 0.0;
      jerry.a_p.y = 0.0;
      jerry.a_f.x = 0.0;
      jerry.a_f.y = 0.0;
      jerry.desired_v.x = 1.0* uniform01() + 1.0;
      jerry.desired_v.y = 0.0;
      jerry.d0 = 10.0 * uniform01();
      jerry.m = 1.0;
      jerry.idx = i;
      jerry.red = 255;
      jerry.grn = 0;
      jerry.blu = 0;
      peds[i] = jerry;
  }
  for(int i = n_peds/2; i<n_peds; i++){
      pedestrian jerry;
      jerry.r.x = WIDTH/2+WIDTH/10;
      jerry.r.y = HEIGHT/2 + uniform01()*HEIGHT/10;
      jerry.v.x = 0.0;
      jerry.v.y = 0.0;
      jerry.a_p.x = 0.0;
      jerry.a_p.y = 0.0;
      jerry.a_f.x = 0.0;
      jerry.a_f.y = 0.0;
      jerry.desired_v.x = -1.0 * uniform01() - 1.0;
      jerry.desired_v.y = 0.0;
      jerry.d0 = 10.0 * uniform01();
      jerry.m = 1.0;
      jerry.idx = i;
      jerry.red = 0;
      jerry.grn = 255;
      jerry.blu = 0;
      peds[i] = jerry;
  }
  //pedestrian berry = {{0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {0.0, 0.0}, {1.0, 1.0}, 1.0, 1.0, 0};
  /*printf("%lf\n", berry.r.y);
  for(int i = 0; i < 10000; i++){
      update_pedestrian(&berry, peds, 0.1);
      //printf("pos %lf %lf\n", jerry.r.x, jerry.r.y);
      printf("vel %lf %lf\n", berry.v.x, berry.v.y);
  }*/

  int close = 0;
  while (!close) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
         close = 1;
         break;
      default:
             break;
      }
    }
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderClear(render);
    SDL_SetRenderDrawColor(render, 200, 200, 200, 255);
    SDL_RenderDrawLine(render, 0, (int)UPPER_WALL, WIDTH, (int)UPPER_WALL);
    SDL_RenderDrawLine(render, 0, (int)LOWER_WALL, WIDTH, (int)LOWER_WALL);
    for(int i = 0; i< n_peds; i++){
      SDL_SetRenderDrawColor(render, peds[i].red, peds[i].grn, peds[i].blu, 255);
      update_pedestrian(&peds[i], peds, n_peds, UPPER_WALL, LOWER_WALL, 0.1);
      SDL_RenderDrawPoint(render, (int)peds[i].r.x, (int)peds[i].r.y);
    }
    SDL_RenderPresent(render);

    SDL_Delay(1000 / 120);
  }

  SDL_DestroyRenderer(render);

  SDL_DestroyWindow(window);

  SDL_Quit();

  return 0;
}
