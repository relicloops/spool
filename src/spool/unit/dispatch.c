#include "spool/spool.h"

#include "../spool_private.h"

#include <stdio.h>
#include <string.h>

int spool_dispatch(valve_t **v, const spool_route_t *routes, void *userdata) {
  if (!v || !*v || !routes)
    return -1;

  const char *verb = vl_verb_get(*v);
  const char *subverb = vl_subverb_get(*v);

  char verb_buf[256];
  char subverb_buf[256];
  verb_buf[0] = '\0';
  subverb_buf[0] = '\0';

  if (verb && verb[0] != '\0')
    snprintf(verb_buf, sizeof(verb_buf), "%s", verb);
  if (subverb && subverb[0] != '\0')
    snprintf(subverb_buf, sizeof(subverb_buf), "%s", subverb);

  vl_destroy(*v);
  *v = nullptr;

  return spool_run_routes_(verb_buf[0] ? verb_buf : nullptr,
                           subverb_buf[0] ? subverb_buf : nullptr, routes,
                           userdata);
}
