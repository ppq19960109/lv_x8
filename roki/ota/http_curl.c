#include <stdio.h>
#include <unistd.h>
#include "http_curl.h"

int http_getRequest(const char *url, curl_write_callback write_callback)
{
    CURLcode ret = -1;

    CURL *curl = curl_easy_init();
    if (!curl)
    {
        printf("curl_easy_init failed.");
        goto fail;
    }
    /*
     *   构建http报文头
     */
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0); // 是否验证cURL对等证书
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0); // 0：不检查证书 2：既检查证书又检查域名
    curl_easy_setopt(curl, CURLOPT_URL, url);          // url地址

    // curl_easy_setopt(curl, CURLOPT_WRITEDATA, args);//传递给 write_callback() 回调函数的第四个参数
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback); // 接受数据回调
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);                    // 是否打印调试信息
    curl_easy_setopt(curl, CURLOPT_HEADER, 0);                     // 是否输出响应头
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);                   // 设置整个cURL函数执行过程的最长等待时间，单位：秒
    ret = curl_easy_perform(curl);
    if (ret != CURLE_OK)
    {
        printf("%s,curl error code: %d\n", __func__, ret);
    }

fail:
    if (curl)
    {
        curl_easy_cleanup(curl);
    }

    return ret;
}

int http_postRequest(const char *url, const char *body, curl_write_callback write_callback)
{
    CURLcode ret = -1;
    struct curl_slist *headers = NULL;

    CURL *curl = curl_easy_init();
    if (!curl)
    {
        printf("curl_easy_init failed.");
        goto fail;
    }
    /*
     *   构建http报文头
     */
    headers = curl_slist_append(headers, "Content-Type:application/json; charset=UTF-8");
    if (headers == NULL)
    {
        printf("curl_slist_append failed.\n");
        goto fail;
    }
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0); // 是否验证cURL对等证书
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0); // 0：不检查证书 2：既检查证书又检查域名
    curl_easy_setopt(curl, CURLOPT_URL, url);          // url地址
    curl_easy_setopt(curl, CURLOPT_POST, 1);           // post请求
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);  // post数据
    // curl_easy_setopt(curl, CURLOPT_WRITEDATA, args);//传递给 write_callback() 回调函数的第四个参数
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback); // 接受数据回调
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0);                    // 是否打印调试信息
    curl_easy_setopt(curl, CURLOPT_HEADER, 0);                     // 是否输出响应头
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);                   // 设置整个cURL函数执行过程的最长等待时间，单位：秒
    ret = curl_easy_perform(curl);
    if (ret != CURLE_OK)
    {
        printf("%s,curl error code: %d\n", __func__, ret);
    }

fail:
    if (headers)
    {
        curl_slist_free_all(headers);
    }
    if (curl)
    {
        curl_easy_cleanup(curl);
    }

    return ret;
}

int curl_download_progress_callback(void *clientp, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
    if (dltotal != 0)
    {
        printf("Download progress: %3.0f%%\n", (dlnow / (dltotal * 1.00)) * 100);
    }
    return 0;
}
size_t curl_download_write_callback(char *buffer, size_t size, size_t nitems, void *outstream)
{
    FILE *fp = (FILE *)outstream;
    size_t ret = fwrite(buffer, size, nitems, fp);
    printf("recv:%s size:%zu nitems:%zu ret:%zu\n", buffer, size, nitems, ret);
    return ret;
}

int http_download(const char *url, const char *path, curl_xferinfo_callback progress_callback)
{
    CURLcode ret = -1;
    FILE *fp = NULL;
    CURL *curl = NULL;

    fp = fopen(path, "wb");
    if (!fp)
    {
        printf("fopen failed.");
        goto fail;
    }
    curl = curl_easy_init();
    if (!curl)
    {
        printf("curl_easy_init failed.");
        goto fail;
    }
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 0); // 是否打印调试信息
    // curl_easy_setopt(curl, CURLOPT_HEADER, 0);  // 是否输出响应头
    /*
     *   CURLOPT_WRITEFUNCTION 可以不用设置，curl会调用默认的回调函数，
     *   把数据写入fp所指向的文件中。
     */
    // curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
    // curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, args); //传递给 progress_callback() 回调函数的第一个参数
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, progress_callback);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 60 * 10);
    ret = curl_easy_perform(curl);
    if (ret != CURLE_OK)
    {
        printf("%s,curl error code: %d\n", __func__, ret);
    }

fail:
    if (curl)
    {
        curl_easy_cleanup(curl);
    }
    if (fp)
    {
        fclose(fp);
    }
    return ret;
}

int http_curl_init(void)
{
    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK)
    {
        printf("curl_global_init failed.");
        return -1;
    }
    return 0;
}
void http_curl_deinit(void)
{
    curl_global_cleanup();
}
