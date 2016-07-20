/*
** mrb_process.c - Process class
**
** Copyright (c) Uchio Kondo 2016
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/data.h"
#include "mruby/class.h"
#include "mrb_process_sys.h"

#define DONE mrb_gc_arena_restore(mrb, 0);


static mrb_value mrb_process_sys_hello(mrb_state *mrb, mrb_value self)
{
  return mrb_str_new_lit(mrb, "Process::Sys !!1");
}

void mrb_mruby_process_sys_gem_init(mrb_state *mrb)
{
  struct RClass *process, *sys;
  if (mrb_class_defined(mrb, "Process")) {
    process = mrb_module_get(mrb, "Process");
  } else {
    process = mrb_define_module(mrb, "Process");
  }
  sys = mrb_define_module_under(mrb, process, "Sys");

  mrb_define_module_function(mrb, sys, "hello", mrb_process_sys_hello, MRB_ARGS_NONE());
  DONE;
}

void mrb_mruby_process_sys_gem_final(mrb_state *mrb)
{
}
