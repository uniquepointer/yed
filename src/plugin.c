#include "internal.h"

void yed_init_plugins(void) {
    char  buff[256];
    char *home;
    char *init_plug;
    int   err;

    ys->plugins = tree_make_c(yed_plugin_name_t, yed_plugin_ptr_t, strcmp);

    buff[0]   = 0;
    home      = getenv("HOME");
    init_plug = "/.yed/init.so";

    if (!home)    { goto not_found; }

    strcat(buff, home);
    strcat(buff, init_plug);

    if (access(buff, F_OK) != -1) {
        err = yed_load_plugin(buff);

        switch (err) {
            case YED_PLUG_SUCCESS:
                ys->small_message = "loaded init.so";
                break;
            case YED_PLUG_NO_BOOT:
                ys->small_message = "!! init missing 'yed_plugin_boot' !!";
                break;
            case YED_PLUG_BOOT_FAIL:
                ys->small_message = "!! init 'yed_plugin_boot' failed !!";
                break;
        }
    } else {
not_found:
        ys->small_message = "no init plugin found";
    }
}

void yed_plugin_force_lib_unload(const char *name, yed_plugin *plug) {
    void *try_handle;

    while ((try_handle = dlopen(name, RTLD_NOW | RTLD_NOLOAD))) {
        dlclose(try_handle);
        dlclose(plug->handle);
    }
}

int yed_load_plugin(char *plug_name) {
    int                         err;
    yed_plugin                 *plug;
    tree_it(yed_plugin_name_t,
            yed_plugin_ptr_t)   it;

    it = tree_lookup(ys->plugins, plug_name);

    if (tree_it_good(it)) {
        yed_unload_plugin(tree_it_key(it));
    }

    plug = malloc(sizeof(*plug));

    plug->handle = dlopen(plug_name, RTLD_NOW | RTLD_LOCAL);
    if (!plug->handle) {
        free(plug);
        return YED_PLUG_NOT_FOUND;
    }

    plug->added_cmds     = array_make(char*);
    plug->added_bindings = array_make(int);

    plug->boot = dlsym(plug->handle, "yed_plugin_boot");
    if (!plug->boot) {
        dlclose(plug->handle);
        array_free(plug->added_bindings);
        array_free(plug->added_cmds);
        free(plug);
        return YED_PLUG_NO_BOOT;
    }

    err = plug->boot(plug);
    if (err) {
        dlclose(plug->handle);
        array_free(plug->added_bindings);
        array_free(plug->added_cmds);
        free(plug);
        return YED_PLUG_BOOT_FAIL;
    }

    tree_insert(ys->plugins, strdup(plug_name), plug);

    return YED_PLUG_SUCCESS;
}

int yed_unload_plugin(char *plug_name) {
    tree_it(yed_plugin_name_t,
            yed_plugin_ptr_t)     it;
    tree_it(yed_command_name_t,
            yed_command)          cmd_it;
    char                        **cmd_name_it,
                                 *old_key;
    yed_plugin                   *old_plug;
    int                          *key_it;

    it = tree_lookup(ys->plugins, plug_name);

    if (!tree_it_good(it))    { return 1; }

    old_key  = tree_it_key(it);
    old_plug = tree_it_val(it);

    tree_delete(ys->plugins, old_key);

    if (old_plug) {
        yed_plugin_force_lib_unload(old_key, old_plug);

        array_traverse(old_plug->added_cmds, cmd_name_it) {
            yed_unset_command(*cmd_name_it);

            /* If this is a default command, restore it. */
            cmd_it = tree_lookup(ys->default_commands, *cmd_name_it);
            if (tree_it_good(cmd_it)) {
                yed_set_command(tree_it_key(cmd_it), tree_it_val(cmd_it));
            }
        }
        array_free(old_plug->added_cmds);

        array_traverse(old_plug->added_bindings, key_it) {
            yed_set_default_key_binding(*key_it);
        }
        array_free(old_plug->added_bindings);

        free(old_plug);
    }

    free(old_key);

    return 0;
}

int yed_unload_plugins(void) {
    tree_it(yed_plugin_name_t,
            yed_plugin_ptr_t)  it;

    while (tree_len(ys->plugins)) {
        it = tree_begin(ys->plugins);
        yed_unload_plugin(tree_it_key(it));
    }

    return 0;
}

int yed_unload_plugin_libs(void) {
    tree_it(yed_plugin_name_t,
            yed_plugin_ptr_t)  it;

    tree_traverse(ys->plugins, it) {
        yed_plugin_force_lib_unload(tree_it_key(it), tree_it_val(it));
    }

    return 0;
}

int yed_reload_plugins(void) {
    array_t                      plugs;
    tree_it(yed_plugin_name_t,
            yed_plugin_ptr_t)    it;
    char                        *name_dup,
                               **name_it;

    plugs = array_make(char*);

    tree_traverse(ys->plugins, it) {
        name_dup = strdup(tree_it_key(it));
        array_push(plugs, name_dup);
    }

    yed_unload_plugins();

    array_traverse(plugs, name_it) {
        yed_load_plugin(*name_it);
        free(*name_it);
    }

    array_free(plugs);

    return 0;
}

void yed_plugin_set_command(yed_plugin *plug, char *name, yed_command command) {
    char *name_dup;

    name_dup = strdup(name);
    tree_insert(ys->commands, strdup(name), command);
    array_push(plug->added_cmds, name_dup);
}

void yed_plugin_bind_key(yed_plugin *plug, int key, char *cmd_name, int takes_key_as_arg) {
    yed_key_binding binding;

    binding.is_bound         = 1;
    binding.key              = key;
    binding.cmd              = cmd_name;
    binding.takes_key_as_arg = takes_key_as_arg;

    yed_bind_key(binding);
    array_push(plug->added_bindings, key);
}
