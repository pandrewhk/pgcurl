MODULE_big = pgcurl

EXTENSION = pgcurl
DATA = pgcurl--0.0.1.sql
PGFILEDESC = "pgcurl - use curl from PostgreSQL"
SHLIB_LINK += -lcurl
OBJS = pgcurl.o

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
