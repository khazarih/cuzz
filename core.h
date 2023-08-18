#pragma once

#include <fstream>
#include "types.h"
#include <regex>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <pthread.h>
#include <curl/curl.h>
#include "types.h"

using namespace std;

void usage()
{
    string message = "Usage: \ncuzz help\n\n";
    message += "cuzz https://url wordlist \n\n";
    message += "Filter by status codes. Comma seperated \n";
    message += "cuzz https://url wordlist 404,403 \n\n";
    message += "Follow redirects? Just add 'yes' as 4th option \n";
    message += "cuzz https://url wordlist 404,403 yes \n\n";
    cout << message << endl;
}

string extractDomain(string url)
{
    regex domainRegex(R"(https?://([^/]+))");

    smatch matches;
    if (regex_search(url, matches, domainRegex))
    {
        if (matches.size() >= 2)
        {
            return matches[1].str();
        }
    }

    return "";
}

bool httpPrefixCheck(string url)
{
    string result = url;

    if (result.compare(0, 7, "http://") == 0)
    {
        return true;
    }
    else if (result.compare(0, 8, "https://") == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool isValidUrl(string url)
{
    string hostname;

    if (!httpPrefixCheck(url))
    {
        cout << "Please type url with HTTP scheme (http:// or https://)\n";
        return false;
    }

    string extracted = extractDomain(url);

    if (extracted.length() > 0)
    {
        hostname = extractDomain(url);
    }
    else
    {
        hostname = url;
    }

    struct hostent *host = gethostbyname(hostname.c_str());
    if (!host)
    {
        cout << "Could not resolve the hostname: " << hostname << endl;
        return false;
    }

    return true;
}

int *getStatusCodesFromUser(string codes)
{
    string arr_status_codes[10];

    string status_codes = codes;

    int seperator_index = 0;
    int index = 0;
    int status_code_index = 0;
    while (status_codes[index] != '\0')
    {
        if (status_codes[index] == ',' && status_code_index != 10)
        {
            seperator_index = index;
            status_code_index += 1;
            index += 1;
        }
        arr_status_codes[status_code_index] += status_codes[index];
        index += 1;
    }

    arr_status_codes[status_code_index + 1] = "STOP";

    int *int_arr_status_codes = new int[10];

    for (int i = 0; i < sizeof(arr_status_codes) / sizeof(arr_status_codes[0]); i++)
    {
        if (arr_status_codes[i] == "STOP")
        {
            break;
        }
        int_arr_status_codes[i] = stoi(arr_status_codes[i]);
    }

    return int_arr_status_codes;
}

bool file_exists(string name)
{
    ifstream f(name.c_str());
    return f.good();
}

bool filterByStatusCode(int status_code, int *status_codes)
{
    bool result = false;

    int index = 0;
    while (status_codes[index] != '\0')
    {
        if (status_codes[index] == status_code)
        {
            result = true;
        }
        index += 1;
    }

    return result;
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t totalSize = size * nmemb;
    auto *response = static_cast<string *>(userp);
    response->append(static_cast<char *>(contents), totalSize);
    return totalSize;
}

long getStatusCode(CURL *curl)
{
    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    return http_code;
}

void request(string url, filter filter_response, bool follow_redirect = false)
{
    CURL *curl = curl_easy_init();
    if (curl)
    {
        string response;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        if (follow_redirect)
        {
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        }
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            cerr << "cURL error: " << curl_easy_strerror(res) << endl;
            curl_easy_cleanup(curl);
            return;
        }
        int status_code = getStatusCode(curl);

        if (filterByStatusCode(status_code, filter_response.status_codes) == true)
        {
            return;
        }

        cout << "Status code: " << status_code << " URL: " << url << endl;
    }
}

void fuzz(string base_url, string wordlist, filter filter_response, bool follow_redirect)
{
    string text;
    ifstream file(wordlist);
    string url;

    while (getline(file, text))
    {
        url = base_url + "/" + text;
        request(url, filter_response, follow_redirect);
    }

    file.close();
}
