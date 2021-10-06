#ifndef __VAR_H__
#define __VAR_H__


void yed_init_vars(void);

void yed_set_default_vars(void);

void yed_set_var(char *var, char *val);
char *yed_get_var(char *var);
void yed_unset_var(char *var);

#define DEFAULT_TABW 4
int yed_get_tab_width(void);
#define DEFAULT_SCROLL_OFF 5
int yed_get_default_scroll_offset(void);

#define DEFAULT_FILL_STRING "~"

#define DEFAULT_STATUS_LINE_LEFT   " %f %b"
#define DEFAULT_STATUS_LINE_CENTER ""
#define DEFAULT_STATUS_LINE_RIGHT  "(%p%%)  %l :: %c  %t "

int yed_var_is_truthy(char *var);
int yed_get_var_as_int(char *var, int *out);

#endif
