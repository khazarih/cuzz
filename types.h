#pragma once

using namespace std;

typedef struct {
    int status_codes[10]; 
    int content_length[10];
} filter;

struct args {
    const char* url;
    filter filter_response;
    bool follow_redirect;
};