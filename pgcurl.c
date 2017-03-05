#include "postgres.h"
#include "lib/stringinfo.h"
#include "utils/memutils.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include <stdio.h>
#include <curl/curl.h>

PG_MODULE_MAGIC;

void get(char *url, StringInfo buf);

PG_FUNCTION_INFO_V1(curl);

static size_t
write_cb(void *buf, size_t size, size_t nmemb, void *userp)
{
    int s = size*nmemb;

    appendBinaryStringInfo((StringInfo)userp, buf, s);
    return s;
}

Datum
curl(PG_FUNCTION_ARGS)
{
    StringInfo buf = makeStringInfo();
    text *url = PG_GETARG_TEXT_P(0);

    get(text_to_cstring(url), buf);

    PG_RETURN_TEXT_P(cstring_to_text_with_len(buf->data, buf->len));
}

void
get(char *url, StringInfo buf)
{
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_ENCODING, "");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    initStringInfo(buf);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);

    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
        appendStringInfo(buf, "curl_easy_perform() failed: %s\n",
                         curl_easy_strerror(res));

    curl_easy_cleanup(curl);
  }
}
