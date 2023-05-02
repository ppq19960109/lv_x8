#ifndef _HTTP_CURL_H_
#define _HTTP_CURL_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "curl/curl.h"
    int http_getRequest(const char *url, curl_write_callback write_callback);
    int http_postRequest(const char *url, const char *body, curl_write_callback write_callback);
    int http_download(const char *url, const char *path, curl_xferinfo_callback progress_callback);

#ifdef __cplusplus
};
#endif
#endif
