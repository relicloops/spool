#ifndef SPOOL_PRIVATE_H
#define SPOOL_PRIVATE_H

#ifndef SPOOL_VERSION
#define SPOOL_VERSION "1.0.0-000"
#endif

#include "spool/spool.h"

#include "valve/valve.h"

#include <stddef.h>

const spool_route_t *spool_route_find_(const char *verb, const char *subverb,
                                       const spool_route_t *routes);

int spool_run_routes_(const char *verb, const char *subverb,
                      const spool_route_t *routes, void *userdata);

#endif /* SPOOL_PRIVATE_H */
