#ifndef MINI_FACTORY_MINI_FACTORY_H_
#define MINI_FACTORY_MINI_FACTORY_H_

#include <SDL.h>
#include <SDL_image.h>

#include <iostream>
#include <memory>
#include <cmath>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

template<class T> using uPtr = std::unique_ptr<T>;
template<class T> using sPtr = std::shared_ptr<T>;

#endif
