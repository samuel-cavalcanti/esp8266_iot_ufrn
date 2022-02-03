#ifndef INDEX_REQUEST_HANDLE
#define INDEX_REQUEST_HANDLE

#include "Arduino.h"
#include "request.hpp"
#include "response.hpp"

const char *home_page_html()
{
    return R"(<!DOCTYPE html>
<html>

<head>
    <!-- Required meta tags -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <!-- Bootstrap CSS -->
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.1.3/dist/css/bootstrap.min.css" rel="stylesheet"
        integrity="sha384-1BmE4kWBq78iYhFldvKuhfTAU6auU8tT94WrHftjDbrCEXSU1oBoqyl2QvZ6jIW3" crossorigin="anonymous">

    <title>ESP 8266</title>
</head>

<body>

    <div style="width: 100%; height: 100%; margin-left: 1%;">
        <h1>ESP 8266</h1>
        
    <ul>
        <li>/api/leds</li>
        <li>/api/temperature</li>
    </ul>

</body>

</html>
)";
}

const Response get_index(const Request &request)
{
    String body = home_page_html();
    return Response(200, HTML_CONTENT_TYPE, body);
}

#endif