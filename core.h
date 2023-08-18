#pragma once

#include <fstream>
#include "request.h"
#include "types.h"


using namespace std;


int* getStatusCodesFromUser(string codes) {
    string arr_status_codes[10];

    string status_codes = codes;
    
    int seperator_index = 0;
    int index = 0;
    int status_code_index = 0;
    while (status_codes[index] != '\0') {
        if (status_codes[index] == ',' && status_code_index != 10) {
            seperator_index = index;
            status_code_index += 1;
            index += 1;
        }
        arr_status_codes[status_code_index] += status_codes[index];
        index += 1;
    }

    arr_status_codes[status_code_index + 1] = "STOP";

    int *int_arr_status_codes = new int[10];

    for (int i = 0; i < sizeof(arr_status_codes)/sizeof(arr_status_codes[0]); i++) {
        if (arr_status_codes[i] == "STOP") {
            break;
        }
        int_arr_status_codes[i] = stoi(arr_status_codes[i]);
    }

    return int_arr_status_codes;
}

void fuzz(string base_url, string wordlist, filter filter_response) {
    string text;
    ifstream file(wordlist);
    string url;

    while (getline (file, text)) {
        url = base_url + "/" + text;
        request(url, filter_response);
    }

    file.close();
}