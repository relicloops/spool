#include "../spool_private.h"

#include <string.h>

const spool_route_t *spool_route_find_(const char *verb, const char *subverb,
                                       const spool_route_t *routes) {
  if (!verb || !verb[0] || !routes)
    return nullptr;

  for (const spool_route_t *route = routes; route->verb != nullptr; ++route) {
    if (!route->run)
      continue;
    if (strcmp(route->verb, verb) != 0)
      continue;
    if (route->subverb == nullptr)
      return route;
    if (subverb != nullptr && strcmp(route->subverb, subverb) == 0)
      return route;
  }

  return nullptr;
}
