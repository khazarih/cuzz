#pragma once

#include <fstream>
#include "request.h"
#include "types.h"


using namespace std;

void fuzz(string base, string wordlist, filter filter_response) {
    string text;
    ifstream file(wordlist);
    string url;

    while (getline (file, text)) {
        url = base + "/" + text;
        request(url, filter_response);
    }

    file.close();
}