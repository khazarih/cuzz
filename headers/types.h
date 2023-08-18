#pragma once

using namespace std;

typedef struct {
    int status_codes[10]; 
    int content_length[10];
} filter;

struct RequestArgs {
    string url;
    filter filter_response;
    bool follow_redirect;
};