/*
** mrb_process.c - Process class
**
** Copyright (c) Uchio Kondo 2016
**
** See Copyright Notice in LICENSE
*/

#include <unistd.h>
#include <sys/types.h>

#include "mruby.h"
#include "mruby/data.h"
#include "mruby/class.h"
#include "mrb_process_sys.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

#define MRB_PROCESS_SYS_DEFINE_GET_FUNC(getter)                         \
  static mrb_value mrb_process_sys_ ## getter                           \
    (mrb_state *mrb, mrb_value self)                                    \
  {                                                                     \
    return mrb_fixnum_value((int)getter());                             \
  }

#define MRB_PROCESS_SYS_DEFINE_SET_FUNC(setter, typename)               \
  static mrb_value mrb_process_sys_ ## setter                           \
    (mrb_state *mrb, mrb_value self)                                    \
  {                                                                     \
    int __id;                                                           \
    mrb_get_args(mrb, "i", &__id);                                      \
    return mrb_fixnum_value((int)setter((typename)__id));               \
  }

MRB_PROCESS_SYS_DEFINE_GET_FUNC(getuid)
MRB_PROCESS_SYS_DEFINE_GET_FUNC(getgid)
MRB_PROCESS_SYS_DEFINE_GET_FUNC(geteuid)
MRB_PROCESS_SYS_DEFINE_GET_FUNC(getegid)

MRB_PROCESS_SYS_DEFINE_SET_FUNC(setuid,  uid_t)
MRB_PROCESS_SYS_DEFINE_SET_FUNC(setgid,  gid_t)
MRB_PROCESS_SYS_DEFINE_SET_FUNC(seteuid, uid_t)
MRB_PROCESS_SYS_DEFINE_SET_FUNC(setegid, gid_t)

#define mrb_process_sys_define_get_method(mrb, sys, getter) \
  mrb_define_module_function(mrb, sys, #getter, mrb_process_sys_ ## getter, MRB_ARGS_NONE())

#define mrb_process_sys_define_set_method(mrb, sys, setter) \
  mrb_define_module_function(mrb, sys, #setter, mrb_process_sys_ ## setter, MRB_ARGS_REQ(1))

void mrb_mruby_process_sys_gem_init(mrb_state *mrb)
{
  struct RClass *process, *sys;
  if (mrb_class_defined(mrb, "Process")) {
    process = mrb_module_get(mrb, "Process");
  } else {
    process = mrb_define_module(mrb, "Process");
  }
  sys = mrb_define_module_under(mrb, process, "Sys");

  mrb_process_sys_define_get_method(mrb, sys, getuid);
  mrb_process_sys_define_get_method(mrb, sys, getgid);
  mrb_process_sys_define_get_method(mrb, sys, geteuid);
  mrb_process_sys_define_get_method(mrb, sys, getegid);

  mrb_process_sys_define_set_method(mrb, sys, setuid);
  mrb_process_sys_define_set_method(mrb, sys, setgid);
  mrb_process_sys_define_set_method(mrb, sys, seteuid);
  mrb_process_sys_define_set_method(mrb, sys, setegid);

  DONE;
}

void mrb_mruby_process_sys_gem_final(mrb_state *mrb)
{
}
