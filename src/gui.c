#include "gui.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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

enum GuiInitSdlResult guiSdlInit() {
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
      SDL_CreateWindow("Hello SDL2", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN);
  if (window->window == NULL) {
    return GUI_INIT_WINDOW_WINDOW_ERROR;
  }

  window->renderer =
      SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED);
  if (window->window == NULL) {
    return GUI_INIT_WINDOW_RENDERER_ERROR;
  }

  SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, 255);
  SDL_RenderClear(window->renderer);

  return GUI_INIT_WINDOW_SUCCESS;
}

void guiWindowShowImage(struct GuiWindow *window,
                        const struct GuiState *state) {
  // TODO: get width and height of window
  // TODO: centre image

  int window_width;
  int window_height;
  SDL_GetWindowSize(window->window, &window_width, &window_height);

  SDL_Rect imageRect;
  imageRect.x = (window_width - state->width) / 2;
  imageRect.y = (window_height - state->height) / 2;
  imageRect.w = state->width;
  imageRect.h = state->height;

  SDL_RenderClear(window->renderer);
  SDL_RenderCopy(window->renderer, state->texture, NULL, &imageRect);
  SDL_RenderPresent(window->renderer);
}

// TODO: put this in the main main
int main(void) {
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

  int quit = 0;
  SDL_Event e;
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT)
        quit = 1;
    }
  }

  SDL_DestroyWindow(window.window);
  SDL_Quit();

  return EXIT_SUCCESS;
}
