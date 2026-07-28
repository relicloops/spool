#ifndef SPOOL_H
#define SPOOL_H

/* Opaque Valve object. Defined by Valve; spool does not include valve/valve.h
 * here so the public surface stays a companion, not a Valve header dump. */
typedef struct valve valve_t;

typedef int (*spool_handler_fn)(const char *verb, const char *subverb,
                                void *userdata);

typedef struct spool_route {
  const char *verb;
  const char *subverb; /* NULL = match any subverb (including none) */
  spool_handler_fn run;
} spool_route_t;

/**
 * Takes ownership of `*v`: snapshot verb/subverb, destroy the valve, set
 * `*v` to NULL, then dispatch. `routes` is NULL-terminated (last entry has
 * `.verb == NULL`). Returns the handler return code, or -1 on no route /
 * bad args.
 */
int spool_dispatch(valve_t **v, const spool_route_t *routes, void *userdata);

const char *spool_version_get(void);
const char *spool_version_string(void);

#endif /* SPOOL_H */
