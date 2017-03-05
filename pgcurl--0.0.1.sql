-- complain if script is sourced in psql, rather than via CREATE EXTENSION
\echo Use "CREATE EXTENSION pgcurl" to load this file. \quit

CREATE OR REPLACE FUNCTION curl(text)
    RETURNS text
    AS 'MODULE_PATHNAME'
    LANGUAGE C;
