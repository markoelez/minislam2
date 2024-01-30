

#include <SDL.h>

#include <opencv2/imgproc.hpp>
#include <stdexcept>
#include <string>

class SDLWindow {
   public:
    SDLWindow(const std::string& title, int width, int height) {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw std::runtime_error("SDL could not initialize: " + std::string(SDL_GetError()));
        }

        window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  width, height, SDL_WINDOW_SHOWN);
        if (window == nullptr) {
            throw std::runtime_error("Window could not be created: " + std::string(SDL_GetError()));
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if (renderer == nullptr) {
            throw std::runtime_error("Renderer could not be created: " +
                                     std::string(SDL_GetError()));
        }
    }

    ~SDLWindow() {
        if (renderer) SDL_DestroyRenderer(renderer);
        if (window) SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void show(const cv::Mat& mat);

    SDL_Renderer* get_renderer() const { return renderer; }

   private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
};

int poll_sdl(SDL_Event e);

SDL_Texture* mat_to_texture(const cv::Mat& mat, SDL_Renderer* renderer);