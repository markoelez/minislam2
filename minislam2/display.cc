

#include "display.h"

SDLWindow::SDLWindow(const std::string& title, int width, int height)
    : title(title), width(width), height(height){};

SDLWindow::~SDLWindow() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

void SDLWindow::init() {
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
        throw std::runtime_error("Renderer could not be created: " + std::string(SDL_GetError()));
    }
}

void SDLWindow::show(const cv::Mat& mat) {
    SDL_Texture* texture = mat_to_texture(mat, get_renderer());
    SDL_Renderer* renderer = get_renderer();
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);
}

int poll_sdl(SDL_Event e) {
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            return 1;
        }
    }
    return 0;
};

SDL_Texture* mat_to_texture(const cv::Mat& mat, SDL_Renderer* renderer) {
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_BGR24,
                                             SDL_TEXTUREACCESS_STATIC, mat.cols, mat.rows);
    SDL_UpdateTexture(texture, NULL, mat.data, mat.step);
    return texture;
};