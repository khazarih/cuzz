#include <iostream>
#include "core.h"
#include "types.h"
#include <netdb.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char **argv)
{
    filter filter_response;

    if (argc < 3)
    {
        usage();
        return 1;
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

    string url = (string)argv[1];
    string wordlist = (string)argv[2];

    string hostname = removeHttpPrefix(url);
    struct hostent* host = gethostbyname(hostname.c_str());
    if (!host) {
        cout << "Could not resolve the hostname: " << hostname << endl;
        return 1;
    }

    if (!file_exists(wordlist)) {
        cout << "File does not exist: " << wordlist << endl;
        return 1;
    }


    fuzz(url, wordlist, filter_response);
}
