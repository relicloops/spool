#include "valve/valve.h"

const char *vl_version_get(void) {
  return VALVE_VERSION;
}

const char *vl_version_string(void) {
  return "valve v" VALVE_VERSION;
}
