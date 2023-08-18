#include <curl/curl.h>
#include "core.h"
#include "types.h"
#include <algorithm>
#include <iterator>


using namespace std;


bool filterByStatusCode(int status_code, int *status_codes) {
    bool result = false;

    int index = 0;
    while (status_codes[index] != '\0') {
        if (status_codes[index] == status_code) {
            result = true;
        }
        index += 1;
    }

    return result;
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    auto* response = static_cast<string*>(userp);
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

long getStatusCode(CURL* curl) {
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    return http_code;
}

void request(string url, filter filter_response, bool follow_redirect=false) {
    CURL* curl = curl_easy_init();
    if (curl) {
        string response;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        if (follow_redirect) {
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        }
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            cerr << "cURL error: " << curl_easy_strerror(res) << endl;
            curl_easy_cleanup(curl);
            return;
        }
        int status_code = getStatusCode(curl);

        if (filterByStatusCode(status_code, filter_response.status_codes) == true) {
            return;
        }

        cout << "Status code: " << status_code << " URL: " << url << endl;

    }
}