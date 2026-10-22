#include "gui.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

int main(void) {
  struct GuiImage state;
  struct GuiWindow window;

  if (guiSdlInit() != GUI_INIT_SDL_SUCCESS) {
    perror("failed to init SDL");
    return EXIT_FAILURE;
  }

  if (guiWindowInit(&window) != GUI_INIT_WINDOW_SUCCESS) {
    perror("failed to init window");
    return EXIT_FAILURE;
  }

  guiImageInit(&state);
  if (guiImageLoad(&state, window.renderer, "assets/oppenheimer.jpg") !=
      GUI_LOAD_IMAGE_SUCCESS) {
    perror("failed to load image");
    return EXIT_FAILURE;
  }

  guiWindowShowImage(&window, &state);

  bool quit = false;
  SDL_Event e;
  while (!quit) {
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        quit = true;
      }

      if (e.type == SDL_WINDOWEVENT &&
          e.window.event == SDL_WINDOWEVENT_RESIZED) {
        guiWindowShowImage(&window, &state);
      }

      if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_q) {
        quit = true;
      }
    }
  }

  guiImageDeinit(&state);
  guiWindowDeinit(&window);
  guiSdlDeinit();

  return EXIT_SUCCESS;
}
