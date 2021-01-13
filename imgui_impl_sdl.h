#include <SDL2/SDL.h>

bool ImGui_ImplSDL2_InitForOpenGL(SDL_Window* window, void* sdl_gl_context);
void ImGui_ImplSDL2_Shutdown();
void ImGui_ImplSDL2_NewFrame(SDL_Window* window);
bool ImGui_ImplSDL2_ProcessEvent(const SDL_Event* event);
