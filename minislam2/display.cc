

#include "display.h"

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