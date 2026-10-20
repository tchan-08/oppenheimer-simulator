#include <SDL2/SDL.h>

struct GuiState {
  struct SDL_Texture *texture;
  int width;
  int height;
};

struct GuiWindow {
  SDL_Window *window;
  SDL_Renderer *renderer;
};

enum GuiInitSdlResult {
  GUI_INIT_SDL_IMAGE_ERROR,
  GUI_INIT_SDL_SDL_ERROR,
  GUI_INIT_SDL_SUCCESS,
};

enum GuiInitWindowResult {
  GUI_INIT_WINDOW_WINDOW_ERROR,
  GUI_INIT_WINDOW_RENDERER_ERROR,
  GUI_INIT_WINDOW_SUCCESS,
};

enum GuiLoadImageResult {
  GUI_LOAD_IMAGE_ERROR,
  GUI_LOAD_IMAGE_SUCCESS,
};

enum GuiInitSdlResult guiSdlInit(void);

void guiStateInit(struct GuiState *state);
enum GuiLoadImageResult guiStateLoadImage(struct GuiState *state,
                                          SDL_Renderer *renderer, char *path);

enum GuiInitWindowResult guiWindowInit(struct GuiWindow *window);
