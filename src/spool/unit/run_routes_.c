#include "../spool_private.h"

int spool_run_routes_(const char *verb, const char *subverb,
                      const spool_route_t *routes, void *userdata) {
  const spool_route_t *route = spool_route_find_(verb, subverb, routes);
  if (!route)
    return -1;
  return route->run(verb, subverb, userdata);
}
