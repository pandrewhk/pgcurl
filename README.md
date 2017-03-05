# pgcurl
Call curl from PostgreSQL.

## Installation
```
sh% make install
psql# create extension pgcurl;
psql# select curl('http://httpbin.org/get')::jsonb->'url';
```

## Limitations
* Only text at the moment (gzip/deflate uncompressed by libcurl)
* Quite slow: fetches one url at a time
* No options
