#include <yed/plugin.h>
#include "bridge.h"

void formatter_fmt(yed_event* event);
void run_formatter(int nargs, char** args);
int  auto_formatter(char *path);

int yed_plugin_boot(yed_plugin *self) {
    yed_event_handler auto_formatter_eh;

    YED_PLUG_VERSION_CHECK();

    auto_formatter_eh.kind = EVENT_BUFFER_POST_WRITE;
    auto_formatter_eh.fn   = formatter_fmt;

    if (yed_get_var("formatter-auto") == NULL) {
        yed_set_var("formatter-auto", "yes");
    }

    yed_plugin_add_event_handler(self, auto_formatter_eh);

    yed_plugin_set_command(self, "run-formatter", run_formatter);
    return 0;
}

void formatter_fmt(yed_event* event) {
    if (!yed_var_is_truthy("formatter-auto")) {
        return;
    }

    if(!event->buffer
            || event->buffer->kind != BUFF_KIND_FILE) {
        return;
    }

    LOG_FN_ENTER();
    if (event->buffer->ft == yed_get_ft("C")
            ||  event->buffer->ft == yed_get_ft("C++")) {
        if(auto_formatter(event->buffer->path) == 0) {
            YEXE("buffer-reload");
        }
    }

    yed_cerr("saved file");
    LOG_EXIT();
}

void run_formatter(int nargs, char** args) {
    LOG_FN_ENTER();

    yed_frame *frame;
    frame = ys->active_frame;

    if(!frame
            || !frame->buffer
            || frame->buffer->kind != BUFF_KIND_FILE) {
        return;
    }

    if (frame->buffer->ft == yed_get_ft("C")
            ||  frame->buffer->ft == yed_get_ft("C++")) {
        yed_cerr("fmt command");
        if(auto_formatter(frame->buffer->path) == 0) {
            YEXE("buffer-reload");
        }
    } else {
        yed_cerr("File type not C or C++");
    }
    LOG_EXIT();
}

int auto_formatter(char *path) {
    int    status;
    int    argc;
    char **argv;

    LOG_FN_ENTER();
    yed_cerr("formatting\n");

    argc = 2;
    argv = (char **) malloc(2*sizeof(char*));
    argv[0] = strdup("astyle");
    argv[1] = strdup(path);

    yed_cerr("%s\n", (argv[0]));
    yed_cerr("%s\n", (argv[1]));
    status = run_astyle(argc, argv);
    yed_cerr("%d", status);

    LOG_EXIT();
    return status;
}
