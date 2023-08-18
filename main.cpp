#include <iostream>
#include "headers/core.h"
#include "headers/types.h"
#include <thread>
#include <vector>


using namespace std;

int main(int argc, char **argv)
{
    filter filter_response;
    bool follow_redirect = false;
    
    if (argc < 3)
    {
        usage();
        return 1;
    }
    
    if (argv[1] == "help") {
        usage();
        return 0;
    }

    if (argc == 4) {
        string status_codes = (string)argv[3];
        int *status_codes_int = getStatusCodesFromUser(status_codes);
        
        int index = 0;
        while (status_codes_int[index] != '\0') {
            filter_response.status_codes[index] = status_codes_int[index];
            index += 1;
        }
    }

    if (argc == 5) {
        string redirect = (string)argv[4];
        if (redirect == "yes") {
            follow_redirect = true;
        }
    }

    string base_url = (string)argv[1];
    string wordlist = (string)argv[2];

    if (!isValidUrl(base_url)) {
        return 1;
    }

    if (!file_exists(wordlist)) {
        cout << "File does not exist: " << wordlist << endl;
        return 1;
    }

    vector<thread> threads;

    string text;
    ifstream file(wordlist);
    string url;

    while (getline(file, text))
    {
        string url = base_url + "/" + text;
        RequestArgs args{url, filter_response, follow_redirect};
        threads.emplace_back(request, args);
    }

    for (thread& thread : threads) {
        thread.join();
    }

    file.close();
}
