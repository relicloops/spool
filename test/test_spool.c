#include "unity.h"

#include "spool/spool.h"

#include "valve/valve.h"
#include "spool_private.h"

#include <stdio.h>
#include <string.h>

/* Force-link Valve + Spool units (Ceedling deep deps only find valve.c otherwise). */
TEST_SOURCE_FILE("dispatch.c")
TEST_SOURCE_FILE("route_find_.c")
TEST_SOURCE_FILE("run_routes_.c")
TEST_SOURCE_FILE("version_get.c")
TEST_SOURCE_FILE("version_string.c")
TEST_SOURCE_FILE("valve_version_stub.c")
TEST_SOURCE_FILE("annotations_print_.c")
TEST_SOURCE_FILE("array_free_.c")
TEST_SOURCE_FILE("at.c")
TEST_SOURCE_FILE("clear.c")
TEST_SOURCE_FILE("collides_with_globals_.c")
TEST_SOURCE_FILE("color.c")
TEST_SOURCE_FILE("conflict_count_.c")
TEST_SOURCE_FILE("conflicts_valid_.c")
TEST_SOURCE_FILE("count.c")
TEST_SOURCE_FILE("destroy.c")
TEST_SOURCE_FILE("duration_parse_.c")
TEST_SOURCE_FILE("error_add_.c")
TEST_SOURCE_FILE("error_at.c")
TEST_SOURCE_FILE("error_count.c")
TEST_SOURCE_FILE("errors_foreach.c")
TEST_SOURCE_FILE("errors_print.c")
TEST_SOURCE_FILE("find_.c")
TEST_SOURCE_FILE("find_option_.c")
TEST_SOURCE_FILE("forward_seen_.c")
TEST_SOURCE_FILE("has.c")
TEST_SOURCE_FILE("have_duplicate_.c")
TEST_SOURCE_FILE("have_duplicate_long_.c")
TEST_SOURCE_FILE("have_duplicate_short_.c")
TEST_SOURCE_FILE("help_print_.c")
TEST_SOURCE_FILE("help_resolve.c")
TEST_SOURCE_FILE("help_resolve_internal_.c")
TEST_SOURCE_FILE("help_target.c")
TEST_SOURCE_FILE("in_.c")
TEST_SOURCE_FILE("label_.c")
TEST_SOURCE_FILE("label_print_.c")
TEST_SOURCE_FILE("mapped_.c")
TEST_SOURCE_FILE("option_count_.c")
TEST_SOURCE_FILE("option_name_valid_.c")
TEST_SOURCE_FILE("options_copy_.c")
TEST_SOURCE_FILE("options_have_invalid_.c")
TEST_SOURCE_FILE("options_use_reserved_.c")
TEST_SOURCE_FILE("other_.c")
TEST_SOURCE_FILE("parse.c")
TEST_SOURCE_FILE("path_split.c")
TEST_SOURCE_FILE("repeat_valid_.c")
TEST_SOURCE_FILE("required_check_.c")
TEST_SOURCE_FILE("requirement_count_.c")
TEST_SOURCE_FILE("requirements_valid_.c")
TEST_SOURCE_FILE("reserved.c")
TEST_SOURCE_FILE("reserved_fired.c")
TEST_SOURCE_FILE("reverse_seen_.c")
TEST_SOURCE_FILE("seen_.c")
TEST_SOURCE_FILE("set_.c")
TEST_SOURCE_FILE("subverb_get.c")
TEST_SOURCE_FILE("table_count_.c")
TEST_SOURCE_FILE("targets_clear.c")
TEST_SOURCE_FILE("type_valid_.c")
TEST_SOURCE_FILE("usage_print_.c")
TEST_SOURCE_FILE("validate_.c")
TEST_SOURCE_FILE("value_label_.c")
TEST_SOURCE_FILE("value_valid_.c")
TEST_SOURCE_FILE("valve.c")
TEST_SOURCE_FILE("valve_print_.c")
TEST_SOURCE_FILE("verb_array_clear_.c")
TEST_SOURCE_FILE("verb_copy_one_.c")
TEST_SOURCE_FILE("verb_count_.c")
TEST_SOURCE_FILE("verb_name_valid_.c")
TEST_SOURCE_FILE("verbs_have_invalid_.c")
TEST_SOURCE_FILE("verbs_use_reserved_.c")
TEST_SOURCE_FILE("version_print_.c")
TEST_SOURCE_FILE("visible_.c")
TEST_SOURCE_FILE("vl_conflict_active_.c")
TEST_SOURCE_FILE("vl_conflict_add_.c")
TEST_SOURCE_FILE("vl_conflict_error_.c")
TEST_SOURCE_FILE("vl_conflict_exists_.c")
TEST_SOURCE_FILE("vl_conflicts_check_.c")
TEST_SOURCE_FILE("vl_conflicts_clear_.c")
TEST_SOURCE_FILE("vl_conflicts_copy_.c")
TEST_SOURCE_FILE("vl_conflicts_copy_table_.c")
TEST_SOURCE_FILE("vl_get.c")
TEST_SOURCE_FILE("vl_options_clear_.c")
TEST_SOURCE_FILE("vl_requirement_active_.c")
TEST_SOURCE_FILE("vl_requirement_add_.c")
TEST_SOURCE_FILE("vl_requirement_error_.c")
TEST_SOURCE_FILE("vl_requirement_exists_.c")
TEST_SOURCE_FILE("vl_requirements_check_.c")
TEST_SOURCE_FILE("vl_requirements_clear_.c")
TEST_SOURCE_FILE("vl_requirements_copy_.c")
TEST_SOURCE_FILE("vl_requirements_copy_table_.c")
TEST_SOURCE_FILE("vl_results_clear_.c")
TEST_SOURCE_FILE("vl_verb_get.c")


static int g_handler_rc;
static char g_seen_verb[64];
static char g_seen_subverb[64];
static int g_userdata_flag;

static int on_serve_(const char *verb, const char *subverb, void *userdata) {
  snprintf(g_seen_verb, sizeof(g_seen_verb), "%s", verb ? verb : "");
  snprintf(g_seen_subverb, sizeof(g_seen_subverb), "%s", subverb ? subverb : "");
  if (userdata)
    *(int *)userdata = 1;
  return g_handler_rc;
}

static int on_language_list_(const char *verb, const char *subverb,
                             void *userdata) {
  (void)userdata;
  snprintf(g_seen_verb, sizeof(g_seen_verb), "%s", verb ? verb : "");
  snprintf(g_seen_subverb, sizeof(g_seen_subverb), "%s",
           subverb ? subverb : "");
  return 7;
}

void setUp(void) {
  g_handler_rc = 42;
  g_seen_verb[0] = '\0';
  g_seen_subverb[0] = '\0';
  g_userdata_flag = 0;
}

void tearDown(void) {}

void test_spool_version_get_non_null(void) {
  const char *v = spool_version_get();
  TEST_ASSERT_NOT_NULL(v);
  TEST_ASSERT_TRUE(strstr(v, "1.0.0") != nullptr);
}

void test_spool_version_string_contains_spool(void) {
  const char *s = spool_version_string();
  TEST_ASSERT_NOT_NULL(s);
  TEST_ASSERT_TRUE(strstr(s, "spool") != nullptr);
}

void test_run_routes_matches_verb(void) {
  const spool_route_t routes[] = {
      {.verb = "serve", .subverb = nullptr, .run = on_serve_},
      {.verb = nullptr},
  };
  TEST_ASSERT_EQUAL_INT(42, spool_run_routes_("serve", nullptr, routes, nullptr));
  TEST_ASSERT_EQUAL_STRING("serve", g_seen_verb);
}

void test_run_routes_matches_subverb(void) {
  const spool_route_t routes[] = {
      {.verb = "language", .subverb = "list", .run = on_language_list_},
      {.verb = "language", .subverb = nullptr, .run = on_serve_},
      {.verb = nullptr},
  };
  TEST_ASSERT_EQUAL_INT(7, spool_run_routes_("language", "list", routes, nullptr));
  TEST_ASSERT_EQUAL_STRING("language", g_seen_verb);
  TEST_ASSERT_EQUAL_STRING("list", g_seen_subverb);
}

void test_run_routes_null_subverb_matches_any(void) {
  const spool_route_t routes[] = {
      {.verb = "serve", .subverb = nullptr, .run = on_serve_},
      {.verb = nullptr},
  };
  TEST_ASSERT_EQUAL_INT(42, spool_run_routes_("serve", "extra", routes, nullptr));
}

void test_run_routes_unknown_verb_returns_minus_one(void) {
  const spool_route_t routes[] = {
      {.verb = "serve", .subverb = nullptr, .run = on_serve_},
      {.verb = nullptr},
  };
  TEST_ASSERT_EQUAL_INT(-1, spool_run_routes_("nope", nullptr, routes, nullptr));
}

void test_run_routes_null_args_return_minus_one(void) {
  const spool_route_t routes[] = {
      {.verb = "serve", .subverb = nullptr, .run = on_serve_},
      {.verb = nullptr},
  };
  TEST_ASSERT_EQUAL_INT(-1, spool_run_routes_(nullptr, nullptr, routes, nullptr));
  TEST_ASSERT_EQUAL_INT(-1, spool_run_routes_("serve", nullptr, nullptr, nullptr));
}

void test_spool_dispatch_serve_hits_handler_and_destroys(void) {
  static const vl_verb_t serve_cmd = {
      .name = "serve",
      .description = "Run the server.",
  };
  static const vl_verb_t *const verbs[] = {&serve_cmd, nullptr};
  const vl_executable_t settings = {
      .program_name = "spool-test",
      .program_version = "0",
      .description = "test",
      .usage = "spool-test <verb>",
      .assign = VL_ASSIGN_INLINE,
      .verbs = verbs,
  };

  valve_t *v = vl_create(&settings);
  TEST_ASSERT_NOT_NULL(v);

  char *argv[] = {(char *)"spool-test", (char *)"serve"};
  TEST_ASSERT_EQUAL_INT(0, vl_parse(v, 2, argv));
  TEST_ASSERT_FALSE(vl_reserved_fired(v));

  const spool_route_t routes[] = {
      {.verb = "serve", .subverb = nullptr, .run = on_serve_},
      {.verb = nullptr},
  };

  int flag = 0;
  TEST_ASSERT_EQUAL_INT(42, spool_dispatch(&v, routes, &flag));
  TEST_ASSERT_NULL(v);
  TEST_ASSERT_EQUAL_INT(1, flag);
  TEST_ASSERT_EQUAL_STRING("serve", g_seen_verb);
}

void test_spool_dispatch_unknown_verb_returns_minus_one(void) {
  static const vl_verb_t serve_cmd = {
      .name = "serve",
      .description = "Run the server.",
  };
  static const vl_verb_t *const verbs[] = {&serve_cmd, nullptr};
  const vl_executable_t settings = {
      .program_name = "spool-test",
      .program_version = "0",
      .assign = VL_ASSIGN_INLINE,
      .verbs = verbs,
  };

  valve_t *v = vl_create(&settings);
  TEST_ASSERT_NOT_NULL(v);

  char *argv[] = {(char *)"spool-test", (char *)"serve"};
  TEST_ASSERT_EQUAL_INT(0, vl_parse(v, 2, argv));

  const spool_route_t routes[] = {
      {.verb = "quota", .subverb = nullptr, .run = on_serve_},
      {.verb = nullptr},
  };

  TEST_ASSERT_EQUAL_INT(-1, spool_dispatch(&v, routes, nullptr));
  TEST_ASSERT_NULL(v);
}

void test_spool_dispatch_null_args_return_minus_one(void) {
  const spool_route_t routes[] = {
      {.verb = "serve", .subverb = nullptr, .run = on_serve_},
      {.verb = nullptr},
  };
  valve_t *v = nullptr;
  TEST_ASSERT_EQUAL_INT(-1, spool_dispatch(nullptr, routes, nullptr));
  TEST_ASSERT_EQUAL_INT(-1, spool_dispatch(&v, routes, nullptr));
  TEST_ASSERT_EQUAL_INT(-1, spool_dispatch(&v, nullptr, nullptr));
}
