#include "spool/spool.h"
#include "valve/valve.h"

#include <stdio.h>

static int serve_run(const char *verb, const char *subverb, void *userdata) {
  (void)verb;
  (void)subverb;
  (void)userdata;
  puts("serving");
  return 0;
}

static const spool_route_t routes[] = {
    {.verb = "serve", .subverb = nullptr, .run = serve_run},
    {.verb = nullptr},
};

int main(int argc, char **argv) {
  static const vl_verb_t serve_cmd = {
      .name = "serve",
      .description = "Run the server.",
  };
  static const vl_verb_t *const verbs[] = {&serve_cmd, nullptr};
  const vl_executable_t settings = {
      .program_name = "spool-demo",
      .program_version = "0.1.0",
      .description = "Spool dispatch demo.",
      .usage = "spool-demo <verb>",
      .verbs = verbs,
  };

  valve_t *v = vl_create(&settings);
  if (!v)
    return 1;

  if (vl_parse(v, argc, argv) != 0) {
    vl_errors_print(v, stderr);
    vl_destroy(v);
    return 2;
  }

  int rc = 0;
  if (!vl_reserved_fired(v))
    rc = spool_dispatch(&v, routes, nullptr);
  if (v)
    vl_destroy(v);
  return rc < 0 ? 3 : rc;
}
