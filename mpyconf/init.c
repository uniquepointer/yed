/* This file needs to interact with the yed core. */
#include <yed/plugin.h>
#include <dirent.h>
#include <errno.h>

/*
 * Below is a simple implementation of a command that we're going to use to
 * compile this file when you make changes to it.
 */
void recompile_init(int n_args, char **args);

/* This is the entry point for this file when yed loads it. */
int yed_plugin_boot(yed_plugin *self) {
    char *path;

    /*
     * This macro ensures that our init plugin isn't loaded into an
     * incompatible version of yed.
     * All it does is return an error code back to yed if the versions
     * don't look right.
     */
    YED_PLUG_VERSION_CHECK();

    /* This makes the recompile_init function available as a command. */
    yed_plugin_set_command(self, "recompile-init", recompile_init);

    char fake_ypm[4096];
    fake_ypm[0] = '\0';
    char real_ypm[4096];
    real_ypm[0] = '\0';
    char *casa = getenv("HOME");
    strcat(fake_ypm, casa);
    strcat(real_ypm, casa);
    strcat(fake_ypm, "/.config/yed/mpy");
    strcat(real_ypm, "/.config/yed/ypm");

    DIR* real_ypm_dir = opendir(real_ypm);
    if (real_ypm_dir)
    {
        YEXE("plugin-load", "ypm");
        closedir(real_ypm_dir);
    }
    else
    {
        YEXE("plugins-add-dir", "~/.config/yed/mpy/plugins");
    }

    YEXE("plugin-load", "yedrc");

    path = get_config_item_path("yedrc");

    YEXE("yedrc-load", path);

    free(path);

    return 0;
}

void recompile_init(int n_args, char **args) {
    const char *config_path;
    char        buff[4096];

    config_path = get_config_path();

    snprintf(buff, sizeof(buff),
             "gcc -o %s/init.so %s/init.c $(yed --print-cflags --print-ldflags) && echo success",
             config_path, config_path);

    YEXE("sh", buff);
}
