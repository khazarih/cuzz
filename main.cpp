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
    if (argc < 3)
    {
        usage();
        return 1;
    }

    filter filter_response;
    filter_response.status_codes[0] = 404;
    filter_response.status_codes[1] = 302;
    filter_response.status_codes[2] = '\n';

    string url = (string)argv[1];
    string wordlist = (string)argv[2];


    fuzz(url, wordlist, filter_response);
}
