

#include <SDL.h>

#include <opencv2/imgproc.hpp>
#include <stdexcept>
#include <string>

class SDLWindow {
   public:
    SDLWindow(const std::string& title, int width, int height);

    ~SDLWindow();

    void init();

    void show(const cv::Mat& mat);

    SDL_Renderer* get_renderer() const { return renderer; }

   private:
    std::string title;
    int width, height;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
};

int poll_sdl(SDL_Event e);

SDL_Texture* mat_to_texture(const cv::Mat& mat, SDL_Renderer* renderer);