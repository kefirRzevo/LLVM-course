#include <stdio.h>
#include <assert.h>

static FILE* log = NULL;
static int is_opened = 0;

__attribute__((constructor))
static void open() {
  log = fopen("runtime.json", "w+");
  fprintf(log, "[");
  is_opened = 1;
}

__attribute__((destructor))
static void close() {
  is_opened = 0;
  fprintf(log, "\"\"]\n");
  fclose(log);
}

void callOptLogger(char *callerName, char *calleeName) {
  if (is_opened)
    fprintf(log, "\"call %s->%s\",\n", callerName, calleeName);
}

void optLogger(char *opName) {
  if (is_opened)
    fprintf(log, "\"%s\",\n", opName);
}
