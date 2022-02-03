#ifndef ROUTES_H
#define ROUTES_H

#include "request.hpp"
#include "response.hpp"

#define LEDS_END_POINT "/api/leds"
#define TEMPERATURE_END_POINT "/api/temperature"
#define INDEX_END_POINT "/"

#include "api/leds/leds_request_handle.cpp"
#include "api/temperature/temperature_request_handle.cpp"
#include "index/index_request_handle.h"

#endif //ROUTES_H