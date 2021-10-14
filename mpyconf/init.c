/* This file needs to interact with the yed core. */
#include <yed/plugin.h>
#include <dirent.h>
#include <errno.h>

/* TODO: Make it so its possible to read from a file list and load those plugins so my fucking yed-rc stops being crowded*/

/*
 * Below is a simple implementation of a command that we're going to use to
 * compile this file when you make changes to it.
 */
void recompile_init(int n_args, char **args);
void load_tally(int n_args, char **args);

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
    DIR* mpy_dir = opendir(fake_ypm);
    if (real_ypm_dir)
    {
        YEXE("plugin-load", "ypm");
        closedir(real_ypm_dir);
    }
    else if(mpy_dir)
    {
        YEXE("plugins-add-dir", "~/.config/yed/mpy/plugins");
        closedir(mpy_dir);
        yed_plugin_set_command(self, "load-tally", load_tally);
    }
    else
    {
        YEXE("plugin-load", "ypm");
    }

    YEXE("plugin-load", "yedrc");

    path = get_config_item_path("yedrc");

    YEXE("yedrc-load", path);

    free(path);

    return 0;
}

void recompile_init(int n_args, char **args) {
    char *build_sh_path;
    char  buff[4096];

    build_sh_path = get_config_item_path("build_init.sh");

    snprintf(buff, sizeof(buff), "%s && echo success", build_sh_path);

    free(build_sh_path);

    YEXE("sh", buff);
}
void
load_tally(int n_args, char **args)
{
    char mpyload[4096];
    memset(mpyload, 0, sizeof(mpyload));
    mpyload[0] = '\0';
    char *casa = getenv("HOME");
    strcat(mpyload, casa);
    strcat(mpyload, "/.config/yed/mpy_tally");

    FILE* mpy_tally;
    mpy_tally = fopen(mpyload, "r");
    if (mpy_tally == NULL)
    {
        yed_cerr("mpy_tally does not exist or cant be read");
        return;
    }
    char c;
    unsigned idx = 0;
    char plug[4096];

    while ((c = fgetc(mpy_tally)) != EOF)
    {
        if (c == ' ') continue;
        if (c == '\n')
        {
            plug[idx] = '\0';
            YEXE("plugin-load", plug);
            memset(plug, 0, sizeof(plug));
            idx = 0;
            continue;
        }
        plug[idx] = c;
        idx++;
    }
    fclose(mpy_tally);
}
