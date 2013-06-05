#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mruby.h>
#include <mruby/proc.h>
#include <mruby/string.h>

int main(int argc, char** argv)
{
  const char rite[] = "app.mrb";

  mrb_state* mrb = mrb_open();

  FILE* fp = fopen(rite, "r");
  if (fp == NULL) {
    fprintf(stderr, "%s not found.\n", rite);
    fprintf(stderr, "`mruby/bin/mrbc app.rb` to create app.mrb file.\n");
    exit(EXIT_FAILURE);
  }

  int n = mrb_read_irep_file(mrb, fp);
  fclose(fp);
  if (n < 0) {
    fprintf(stderr, "%s - irep load error.\n", rite);
    exit(EXIT_FAILURE);
  }
  mrb_run(mrb, mrb_proc_new(mrb, mrb->irep[n]), mrb_top_self(mrb));

  if (argc < 2) {
    printf("Usage %s <environment>\n", argv[0]);
    return 0;
  }

  // AppSetting
  struct RClass* clz = mrb_class_get(mrb, "AppSetting");
  mrb_value setting = mrb_obj_value(clz);
  // AppSetting.load(*targets) -> AppConfig
  mrb_value target = mrb_str_new(mrb, argv[1], strlen(argv[1]));
  mrb_value conf = mrb_funcall(mrb, setting, "load", 1, target);
  // AppConfig.#debug -> bool
  mrb_value debug = mrb_funcall(mrb, conf, "debug", 0);
  printf("debug: %d\n", mrb_type(debug) == MRB_TT_TRUE);
  // AppConfig.#timeout -> Integer
  mrb_value timeout = mrb_funcall(mrb, conf, "timeout", 0);
  printf("timeout: %d\n", timeout.value.i);
  // AppConfig.#title -> String
  mrb_value title = mrb_funcall(mrb, conf, "title", 0);
  printf("title: %s\n", mrb_string_value_ptr(mrb, title));
  // AppConfig.#messages -> Array
  mrb_value messages = mrb_funcall(mrb, conf, "messages", 0);
  {
    // Array.#size -> Integer
    mrb_value len = mrb_funcall(mrb, messages, "size", 0);
    int i;
    for (i = 0; i < len.value.i; i++) {
      // Array.#at(nth) -> object
      mrb_value message = mrb_funcall(mrb, messages, "at", 1, mrb_fixnum_value(i));
      printf("%s\n", mrb_string_value_ptr(mrb, message));
    }
  }

  mrb_close(mrb);

  return 0;
}
