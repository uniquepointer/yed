#include <yed/plugin.h>

void
buffer_go_fmt(int n_args, char **args);
void
ev_buffer_go_fmt(yed_event* event);

char bufferLoc[512];

int
yed_plugin_boot(yed_plugin* self)
{
    /*Check for plug version*/
    YED_PLUG_VERSION_CHECK();

    yed_event_handler eh_go_format;

    eh_go_format.kind = EVENT_BUFFER_POST_WRITE;
    eh_go_format.fn   = ev_buffer_go_fmt;

    if (yed_get_var("gofmt-auto") == NULL)
    {
        yed_set_var("gofmt-auto", "yes");
    }

    if (yed_var_is_truthy("gofmt-auto"))
    {
        yed_plugin_add_event_handler(self, eh_go_format);
    }

    yed_plugin_set_command(self, "go-fmt", buffer_go_fmt);
    return 0;
}

void
buff_path_for_fmt()
{
    yed_buffer* buffer;
    yed_frame*  frame;

    if (!ys->active_frame)
    {
        yed_cerr("no active frame");
        return;
    }

    frame = ys->active_frame;

    if (!frame->buffer)
    {
        yed_cerr("active frame has no buffer");
        return;
    }

    buffer = frame->buffer;

    if (buffer->name && buffer->ft == yed_get_ft("Golang"))
    {
        strcpy(bufferLoc, buffer->path);
    }
    else
    {
        yed_cerr("buffer has no path or it's not a Golang file'");
    }
}

int
_go_fmt(void)
{
    int        output_len, status;
    char       cmd_buff[1024];

    buff_path_for_fmt();
    snprintf(cmd_buff, sizeof(cmd_buff), "gofmt -w %s &> /tmp/golog", bufferLoc);
    yed_run_subproc(cmd_buff, &output_len, &status);

    if (status != 0)
    {
        yed_cerr("Failure to format golang\n");
        return status;
    }
    else
    {

        yed_cprint("Formatted buffer\n");
        return 0;
    }
}
void
ev_buffer_go_fmt(yed_event* event)
{
    yed_frame* frame;

    if (!ys->active_frame)
    {
        yed_cerr("no active frame");
        return;
    }

    frame = ys->active_frame;

    if (!frame->buffer)
    {
        yed_cerr("active frame has no buffer");
        return;
    }

    if (frame->buffer->ft == yed_get_ft("Golang"))
    {
        if(_go_fmt() == 0)
        {
            YEXE("buffer-reload");
            YEXE("redraw");
            yed_cprint("Reloaded buffer\n");
        }
    }
}
void
buffer_go_fmt(int n_args, char **args)
{
    yed_frame* frame;

    if (!ys->active_frame)
    {
        yed_cerr("no active frame");
        return;
    }

    frame = ys->active_frame;

    if (!frame->buffer)
    {
        yed_cerr("active frame has no buffer");
        return;
    }

    if (frame->buffer->ft == yed_get_ft("Golang"))
    {
        if(_go_fmt() == 0)
        {
            YEXE("write-buffer");
            YEXE("buffer-reload");
            YEXE("redraw");
            yed_cprint("Reloaded buffer\n");
        }
    }
}
