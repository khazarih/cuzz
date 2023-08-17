#include <iostream>
#include "core.h"
#include "types.h"

using namespace std;

void usage()
{
    string message = "Usage: cuzz https://url/FUZZ wordlist \n";
    cout << message << endl;
}

int main(int argc, char **argv)
{
    filter filter_response;

    if (argc < 2)
    {
        usage();
        return 1;
    }
    
    if (argc <= 4) {
        string status_codes = (string)argv[3];
        int *status_codes_int = getStatusCodesFromUser(status_codes);
        
        int index = 0;
        while (status_codes_int[index] != '\0') {
            filter_response.status_codes[index] = status_codes_int[index];
            // cout << filter_response.status_codes[index] << endl;
            index += 1;
        }
    }

    string url = (string)argv[1];
    string wordlist = (string)argv[2];

    fuzz(url, wordlist, filter_response);
}
