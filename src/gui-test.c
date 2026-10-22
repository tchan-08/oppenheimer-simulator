#include "gui.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

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
