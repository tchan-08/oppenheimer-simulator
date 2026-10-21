#include "gui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_render.h>
#include <stdbool.h>
#include <stddef.h>

void guiStateInit(struct GuiState *state) {
  state->texture = NULL;
}

enum GuiLoadImageResult guiStateLoadImage(struct GuiState *state,
                                          SDL_Renderer *renderer, char *path) {
  state->texture = IMG_LoadTexture(renderer, path);
  if (state->texture == NULL) {
    return GUI_LOAD_IMAGE_ERROR;
  }

  SDL_QueryTexture(state->texture, NULL, NULL, &state->width, &state->height);

  return GUI_LOAD_IMAGE_SUCCESS;
}

enum GuiInitSdlResult guiSdlInit(void) {
  int err = SDL_Init(SDL_INIT_VIDEO);
  if (err < 0) {
    return GUI_INIT_SDL_SDL_ERROR;
  }

  err = IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP);
  if (err == 0) {
    return GUI_INIT_SDL_IMAGE_ERROR;
  }

  return GUI_INIT_SDL_SUCCESS;
}

enum GuiInitWindowResult guiWindowInit(struct GuiWindow *window) {
  window->window =
      SDL_CreateWindow("Oppenheimer Simulator", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
  if (window->window == NULL) {
    return GUI_INIT_WINDOW_WINDOW_ERROR;
  }

  window->renderer =
      SDL_CreateRenderer(window->window, -1, 
        SDL_RENDERER_ACCELERATED // SDL_RENDERER_SOFTWARE
      );
  if (window->window == NULL) {
    return GUI_INIT_WINDOW_RENDERER_ERROR;
  }

  SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, 255);
  SDL_RenderClear(window->renderer);

  return GUI_INIT_WINDOW_SUCCESS;
}

#define GUI_MIN(a, b) ((a < b) ? (a) : (b))
#define GUI_WINDOW_PADDING 50

void guiWindowShowImage(struct GuiWindow *window,
                        const struct GuiState *state) {
  int window_width;
  int window_height;
  SDL_GetWindowSize(window->window, &window_width, &window_height);

  int viewport_width = window_width - GUI_WINDOW_PADDING;
  int viewport_height = window_height - GUI_WINDOW_PADDING;

  double scale = GUI_MIN((double)viewport_width / (double)state->width,
                         (double)viewport_height / (double)state->height);

  int scaled_width = (int)((double)state->width * scale);
  int scaled_height = (int)((double)state->height * scale);

  SDL_Rect imageRect;
  imageRect.x = (window_width - scaled_width) / 2;
  imageRect.y = (window_height - scaled_height) / 2;
  imageRect.w = scaled_width;
  imageRect.h = scaled_height;

  SDL_RenderClear(window->renderer);
  SDL_RenderCopy(window->renderer, state->texture, NULL, &imageRect);
  SDL_RenderPresent(window->renderer);
}

// TODO: put this in the main main

int _main(void) {
  struct GuiState state;
  struct GuiWindow window;

  if (guiSdlInit() != GUI_INIT_SDL_SUCCESS) {
    return 1;
  }

  guiStateInit(&state);

  if (guiWindowInit(&window) != GUI_INIT_WINDOW_SUCCESS) {
    return 1;
  }

  if (guiStateLoadImage(&state, window.renderer, "assets/oppenheimer.jpg") !=
      GUI_LOAD_IMAGE_SUCCESS) {
    return 1;
  }

  guiWindowShowImage(&window, &state);

  bool quit = false;
  SDL_Event e;
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        quit = true;
      if (e.type == SDL_WINDOWEVENT &&
          e.window.event == SDL_WINDOWEVENT_RESIZED)
        guiWindowShowImage(&window, &state);
    }
  }

  SDL_DestroyWindow(window.window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
