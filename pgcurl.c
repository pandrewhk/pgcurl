#include "postgres.h"
#include "lib/stringinfo.h"
#include "utils/memutils.h"
#include "fmgr.h"
#include "utils/builtins.h"
#include <stdio.h>
#include <curl/curl.h>

PG_MODULE_MAGIC;

int get(StringInfo url, StringInfo buf);

PG_FUNCTION_INFO_V1(curl);

static size_t
write_cb(void *buf, size_t size, size_t nmemb, void *userp)
{
    int        s = size*nmemb;

    appendBinaryStringInfo((StringInfo)userp, buf, s);
    return s;
}

Datum
curl(PG_FUNCTION_ARGS)
{
	int res;
	StringInfo url = makeStringInfo();
	StringInfo buf = makeStringInfo();
	text *out;

	res = get(url, buf);
	out = (text *) buf->data;
	SET_VARSIZE(out, buf->len);

	PG_RETURN_TEXT_P(out);
//	PG_RETURN_TEXT_P(cstring_to_text("Hello, World!"));
}

int get(StringInfo url, StringInfo buf)
{
  CURL *curl;
  CURLcode res;

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://httpbin.org/get");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
    initStringInfo(buf);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, buf);

    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));

    curl_easy_cleanup(curl);
  }
  return 0;
}
