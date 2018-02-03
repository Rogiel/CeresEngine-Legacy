//
// Created by Rogiel Sulzbach on 10/20/17.
//

#pragma once

#include <OpenGL/gl3.h>

#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

NVGcontext* nvgCreateGL3_swift(int flags);