#include "requests.h"
#include "hthread.h"
#include "hv_http_client.h"

void http_async_client_get(const char *url, int (*cb)(int, const char *))
{
    printf("test_http_async_client request thread tid=%ld\n", hv_gettid());
    HttpRequestPtr req(new HttpRequest);
    req->method = HTTP_GET;
    req->url = url;
    // req->headers["Connection"] = "keep-alive";
    // req->body = "this is an async request.";
    req->timeout = 5;
    http_client_send_async(req, [=](const HttpResponsePtr &resp)
                           {
        printf("test_http_async_client response thread tid=%ld\n", hv_gettid());
        if (resp == NULL) {
            printf("request failed!\n");
            if(cb!=NULL)
                cb(-1,NULL);
        } else {
            printf("status_code:%d %s\r\n", resp->status_code, resp->status_message());
            const char* body=resp->body.c_str();
            printf("body:%s\n", body);
            if(cb!=NULL)
                cb(resp->status_code,body);
        } });
}