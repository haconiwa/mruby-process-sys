/*
** mrb_process.c - Process class
**
** Copyright (c) Uchio Kondo 2016
**
** See Copyright Notice in LICENSE
*/

// clang-format off
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>

#include "mruby.h"
#include "mruby/array.h"
#include "mruby/string.h"
#include "mruby/error.h"
#include "mrb_process_sys.h"
// clang-format on

#pragma GCC diagnostic ignored "-Wdeclaration-after-statement"

#define DONE mrb_gc_arena_restore(mrb, 0);

#define MRB_PROCESS_SYS_DEFINE_GET_FUNC(getter)                                                                        \
  static mrb_value mrb_process_sys_##getter(mrb_state *mrb, mrb_value self)                                            \
  {                                                                                                                    \
    return mrb_fixnum_value((int)getter());                                                                            \
  }

#define MRB_PROCESS_SYS_DEFINE_SET_FUNC(setter, typename)                                                              \
  static mrb_value mrb_process_sys_##setter(mrb_state *mrb, mrb_value self)                                            \
  {                                                                                                                    \
    mrb_int __id;                                                                                                      \
    mrb_get_args(mrb, "i", &__id);                                                                                     \
    return mrb_fixnum_value((int)setter((typename)__id));                                                              \
  }

MRB_PROCESS_SYS_DEFINE_GET_FUNC(getuid)
MRB_PROCESS_SYS_DEFINE_GET_FUNC(getgid)
MRB_PROCESS_SYS_DEFINE_GET_FUNC(geteuid)
MRB_PROCESS_SYS_DEFINE_GET_FUNC(getegid)

MRB_PROCESS_SYS_DEFINE_SET_FUNC(setuid, uid_t)
MRB_PROCESS_SYS_DEFINE_SET_FUNC(setgid, gid_t)
MRB_PROCESS_SYS_DEFINE_SET_FUNC(seteuid, uid_t)
MRB_PROCESS_SYS_DEFINE_SET_FUNC(setegid, gid_t)

static mrb_value mrb_process_sys___setgroups(mrb_state *mrb, mrb_value self)
{
  mrb_value argv;
  mrb_int argc;
  int i;
  gid_t *groups;
  mrb_get_args(mrb, "A", &argv);
  argc = RARRAY_LEN(argv);
  if (argc < 1) {
    mrb_raise(mrb, E_ARGUMENT_ERROR, "setgroups must have at least 1 gid");
    return mrb_nil_value();
  }

  groups = (gid_t *)malloc(sizeof(gid_t) * (argc + 1));
  for (i = 0; i < argc; i++) {
    mrb_value gid = mrb_ary_ref(mrb, argv, i);
    if (!mrb_fixnum_p(gid)) {
      mrb_raise(mrb, E_ARGUMENT_ERROR, "must contain only integers");
    }
    *groups = (gid_t)mrb_fixnum(gid);
    groups++;
  }
  groups -= i;

  if (setgroups((size_t)argc, groups) < 0) {
    mrb_sys_fail(mrb, "setgroups failed.");
  }

  return argv;
}

/* TODO: Introduce the Passwd/Group class */
static mrb_value mrb_passwd_to_array(mrb_state *mrb, struct passwd *p)
{
  mrb_value res = mrb_ary_new_capa(mrb, 3);
  mrb_ary_push(mrb, res, mrb_fixnum_value(p->pw_uid));
  mrb_ary_push(mrb, res, mrb_fixnum_value(p->pw_gid));
  mrb_ary_push(mrb, res, mrb_str_new_cstr(mrb, p->pw_name));
  return res;
}

static mrb_value mrb_group_to_array(mrb_state *mrb, struct group *g)
{
  mrb_value res = mrb_ary_new_capa(mrb, 2);
  mrb_ary_push(mrb, res, mrb_fixnum_value(g->gr_gid));
  mrb_ary_push(mrb, res, mrb_str_new_cstr(mrb, g->gr_name));
  return res;
}

static mrb_value mrb_process_sys_getpwnam(mrb_state *mrb, mrb_value self)
{
  char *name;
  mrb_bool detailed = FALSE;
  mrb_get_args(mrb, "z|b", &name, &detailed);

  struct passwd *p = getpwnam(name);
  if (!p) {
    mrb_sys_fail(mrb, "getpwnam failed.");
  }

  if (!detailed) {
    return mrb_fixnum_value(p->pw_uid);
  } else {
    return mrb_passwd_to_array(mrb, p);
  }
}

static mrb_value mrb_process_sys_getpwuid(mrb_state *mrb, mrb_value self)
{
  mrb_int uid;
  mrb_bool detailed = FALSE;
  mrb_get_args(mrb, "i|b", &uid, &detailed);

  struct passwd *p = getpwuid((uid_t)uid);
  if (!p) {
    mrb_sys_fail(mrb, "getpwuid failed.");
  }

  if (!detailed) {
    return mrb_str_new_cstr(mrb, p->pw_name);
  } else {
    return mrb_passwd_to_array(mrb, p);
  }
}

static mrb_value mrb_process_sys_getgrnam(mrb_state *mrb, mrb_value self)
{
  char *name;
  mrb_bool detailed = FALSE;
  mrb_get_args(mrb, "z|b", &name, &detailed);

  struct group *g = getgrnam(name);
  if (!g) {
    mrb_sys_fail(mrb, "getgrnam failed.");
  }

  if (!detailed) {
    return mrb_fixnum_value(g->gr_gid);
  } else {
    return mrb_group_to_array(mrb, g);
  }
}

static mrb_value mrb_process_sys_getgrgid(mrb_state *mrb, mrb_value self)
{
  mrb_int gid;
  mrb_bool detailed = FALSE;
  mrb_get_args(mrb, "i|b", &gid, &detailed);

  struct group *g = getgrgid((gid_t)gid);
  if (!g) {
    mrb_sys_fail(mrb, "getgrgid failed.");
  }

  if (!detailed) {
    return mrb_str_new_cstr(mrb, g->gr_name);
  } else {
    return mrb_group_to_array(mrb, g);
  }
}

#define mrb_process_sys_define_get_method(mrb, sys, getter)                                                            \
  mrb_define_module_function(mrb, sys, #getter, mrb_process_sys_##getter, MRB_ARGS_NONE())

#define mrb_process_sys_define_set_method(mrb, sys, setter)                                                            \
  mrb_define_module_function(mrb, sys, #setter, mrb_process_sys_##setter, MRB_ARGS_REQ(1))

void mrb_mruby_process_sys_gem_init(mrb_state *mrb)
{
  struct RClass *process, *sys, *pwgrp;
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

  mrb_define_module_function(mrb, sys, "__setgroups", mrb_process_sys___setgroups, MRB_ARGS_REQ(1));

  pwgrp = mrb_define_module_under(mrb, process, "Pwgrp");

  mrb_define_module_function(mrb, pwgrp, "getpwnam", mrb_process_sys_getpwnam, MRB_ARGS_ARG(1, 1));
  mrb_define_module_function(mrb, pwgrp, "getpwuid", mrb_process_sys_getpwuid, MRB_ARGS_ARG(1, 1));
  mrb_define_module_function(mrb, pwgrp, "getgrnam", mrb_process_sys_getgrnam, MRB_ARGS_ARG(1, 1));
  mrb_define_module_function(mrb, pwgrp, "getgrgid", mrb_process_sys_getgrgid, MRB_ARGS_ARG(1, 1));

  DONE;
}

void mrb_mruby_process_sys_gem_final(mrb_state *mrb)
{
}
