#ifndef __CMAP_PARSER_UTIL_DEFINE_H__
#define __CMAP_PARSER_UTIL_DEFINE_H__

#define APPEND_INSTRUCTION_ARGS(txt, args...) \
  cmap_parser_util_public.append_instruction_args(txt, args)

#define APPEND_VARIABLE_ARGS(txt, args...) \
  cmap_parser_util_public.append_variable_args(txt, args)

#define PREPEND_INSTRUCTION_ARGS(txt, args...) \
  cmap_parser_util_public.prepend_instruction_args(txt, args)

#define APPEND_INSTRUCTION(txt) \
  cmap_parser_util_public.append_instruction(txt)

#define APPEND_LF() cmap_parser_util_public.append_lf()

#define ADD_ARGS(instruction, args) \
  cmap_parser_util_public.add_args(instruction, args)

#define APPEND_INSTRUCTION_ARGS_ARGS(args, txt, others...) \
  cmap_parser_util_public.append_instruction_args_args(args, txt, others)

#define ADD_DEFINITIONS() cmap_parser_util_public.add_definitions()
#define ADD_GLOBAL_ENV() cmap_parser_util_public.add_global_env()

#define PREPEND_MAP_VAR(map_name) \
  cmap_parser_util_public.prepend_map_var(map_name)

#define NEXT_NAME(what) cmap_parser_util_public.next_name(what)

#endif
