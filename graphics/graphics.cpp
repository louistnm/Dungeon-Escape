//
// Created by Louis Tennier on 1/19/26.
//

#include "graphics.h"

Graphics::Graphics(const std::string& title, int window_width, int window_height)
    : title{title}, width{window_width}, height{window_height} {
    SDL_SetAppMetadata(title.data(), "1.0", NULL); //char* references the address. Strings do not exist in C.
    if (!SDL_CreateWindowAndRenderer(title.data(), width, height, SDL_WINDOW_RESIZABLE, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    }

    SDL_SetRenderLogicalPresentation(renderer, width, height, SDL_LOGICAL_PRESENTATION_LETTERBOX);
}

void Graphics::clear() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Graphics::draw(const SDL_FRect& rect, const Color& color, bool filled) {
    auto [red,green,blue,alpha] = color;
    SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
    if (filled) {
        SDL_RenderFillRect(renderer, &rect);
    } else {
        SDL_RenderRect(renderer, &rect);
    }
}

void Graphics::update() {
    SDL_RenderPresent(renderer);
}