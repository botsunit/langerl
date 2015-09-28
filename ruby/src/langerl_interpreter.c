#undef RUBY_EXPORT
#include <ruby/ruby.h>
#include <unistd.h>
#include <ei.h>
#include "langerl_interpreter.h"

#define ROOT_RB "./ruby/tests/sum.rb"

int start_interpreter(void **interpreter) {
  *interpreter = NULL;
  LANGERL_LOG("start interpreter for ruby!");
  const char *argv[] = {"node.ruby"};
  int argc = 1;

  ruby_sysinit(&argc, (char***)&argv); 
  RUBY_INIT_STACK;
  ruby_init();
  ruby_init_loadpath();

  rb_funcall(rb_mKernel, rb_intern("require"), 1, rb_str_new2(ROOT_RB));

  ID sym_mymodule = rb_intern("Summer");
  VALUE mymodule = rb_const_get(rb_cObject, sym_mymodule);
  VALUE result = rb_funcall(mymodule, rb_intern("sum"), 1, INT2FIX(10));

  LANGERL_LOG("Result = %d\n", NUM2INT(result));

  return 1;
}

int stop_interpreter(void *interpreter) {
  LANGERL_LOG("stop interpreter for ruby! %s", interpreter);
  ruby_cleanup(0);
  return 1;
}

int test_interpreter(int value) {
  LANGERL_LOG("test interpreter for ruby!");
  return value + value;
}

int load_file_interpreter(char *file) {
  if(access(file, F_OK) == 0) {
    VALUE result = rb_funcall(rb_mKernel, rb_intern("require"), 1, rb_str_new2(file));
    if(result == Qtrue) {
      LANGERL_LOG("Load file %s: OK", file);
      return LOAD_OK;
    } else {
      LANGERL_LOG("Load file %s: ERROR", file);
      return LOAD_ALREADY;
    }
  } else {
    LANGERL_LOG("Load file %s: MISSING", file);
    return LOAD_MISSING_FILE;
  }
}

void * exec_interpreter(char *code, int *rcod) {
  int state;
  VALUE result = rb_eval_string_protect(code, &state);
  if(0 == state) {
    *rcod = EXEC_OK;
    return (void*)result;
  }
  *rcod = EXEC_ERROR;
  return NULL;
}

void to_erlang(ei_x_buff *x_out, void *data) {
  char *str = NULL;
  VALUE rdata = (VALUE)data;
  LANGERL_LOG("VALUE!!!");
  switch (TYPE(rdata)) {
    case T_NIL:
      LANGERL_LOG("===> T_NIL");
      ei_x_encode_atom(x_out, "nil");
      break;
    case T_OBJECT:
      LANGERL_LOG("===> T_OBJECT");
      // TODO
      ei_x_encode_atom(x_out, "undefined_t_object");
      break;
    case T_CLASS:
      LANGERL_LOG("===> T_CLASS");
      // TODO
      ei_x_encode_atom(x_out, "undefined_t_class");
      break;
    case T_MODULE:
      LANGERL_LOG("===> T_MODULE");
      // TODO
      ei_x_encode_atom(x_out, "undefined_t_module");
      break;
    case T_FLOAT:
      LANGERL_LOG("===> T_FLOAT");
      ei_x_encode_double(x_out, NUM2DBL(rdata));
      break;
    case T_STRING:
      LANGERL_LOG("===> T_STRING");
      str = StringValueCStr(rdata);
      ei_x_encode_binary(x_out, str, strlen(str));
      break;
    case T_REGEXP:
      LANGERL_LOG("===> T_REGEXP");
      // TODO
      ei_x_encode_atom(x_out, "undefined_t_regexp");
      break;
    case T_ARRAY:
      LANGERL_LOG("===> T_ARRAY");
      // TODO
      ei_x_encode_atom(x_out, "undefined_t_array");
      break;
    case T_HASH:
      LANGERL_LOG("===> T_HASH");
      // TODO
      ei_x_encode_atom(x_out, "undefined_t_hash");
      break;
    case T_STRUCT:
      LANGERL_LOG("===> T_STRUCT");
      // TODO
      ei_x_encode_atom(x_out, "undefined_t_struct");
      break;
    case T_BIGNUM:
      LANGERL_LOG("===> T_BIGNUM");
      ei_x_encode_longlong(x_out, NUM2LL(rdata));
      break;
    case T_FIXNUM:
      LANGERL_LOG("===> T_FIXNUM");
      ei_x_encode_long(x_out, NUM2LONG(rdata));
      break;
    case T_COMPLEX:
      LANGERL_LOG("===> T_COMPLEX");
      // TODO
      ei_x_encode_atom(x_out, "undefined_t_complex");
      break;
    case T_RATIONAL:
      LANGERL_LOG("===> T_RATIONAL");
      // TODO
      ei_x_encode_atom(x_out, "undefined_t_rational");
      break;
    case T_FILE:
      LANGERL_LOG("===> T_FILE");
      // TODO
      ei_x_encode_atom(x_out, "undefined_t_file");
      break;
    case T_TRUE:
      LANGERL_LOG("===> T_TRUE");
      ei_x_encode_boolean(x_out, 1);
      break;
    case T_FALSE:
      LANGERL_LOG("===> T_FALSE");
      ei_x_encode_boolean(x_out, 0);
      break;
    case T_DATA:
      LANGERL_LOG("===> T_DATA");
      // TODO
      ei_x_encode_atom(x_out, "undefined_t_data");
      break;
    case T_SYMBOL :
      LANGERL_LOG("===> T_SYMBOL");
      VALUE sym = rb_funcall(rdata, rb_intern("to_s"), 0);
      str = StringValueCStr(sym);
      ei_x_encode_atom(x_out, str);
      break;
    default:
      LANGERL_LOG("===> UNDEFINED: %d", TYPE(rdata));
      ei_x_encode_atom(x_out, "undefined");
      break;
  }
  return;
}

