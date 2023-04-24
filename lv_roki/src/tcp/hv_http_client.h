#ifndef _HV_HTTP_CLIENT_H_
#define _HV_HTTP_CLIENT_H_

#ifdef __cplusplus
extern "C"
{
#endif
    void http_async_client_get(const char *url, int (*cb)(int, const char *));

#ifdef __cplusplus
};
#endif
#endif
