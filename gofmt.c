#include <yed/plugin.h>

void
buffer_go_fmt();
void
     ev_buffer_go_fmt(yed_event* event);

char bufferLoc[512];

int
yed_plugin_boot(yed_plugin* self)
{
    /*Check for plug version*/
    YED_PLUG_VERSION_CHECK();

    yed_event_handler gofmt;

    gofmt.kind = EVENT_BUFFER_PRE_WRITE;
    gofmt.fn   = ev_buffer_go_fmt;

    if (!!(yed_var_is_truthy("gofmt-auto")))
    {
        yed_plugin_add_event_handler(self, gofmt);
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

    if (buffer->name)
    {
        strcpy(bufferLoc, buffer->path);
    }
    else
    {
        yed_cerr("buffer has no path");
    }
}

void
go_fmt(void)
{
    int        output_len, status;
    char       cmd_buff[1024];

    buff_path_for_fmt();
    snprintf(cmd_buff, sizeof(cmd_buff), "gofmt -w %s", bufferLoc);

    yed_run_subproc(cmd_buff, &output_len, &status);

    if (status != 0)
    {
        yed_cerr("Failure to format golang\n");
    }
    else
    {
        yed_cprint("Formatted buffer\n");
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
        go_fmt();
        YEXE("buffer-reload");
        yed_cprint("Buffer reloaded");
    }
}
void
buffer_go_fmt()
{
    go_fmt();
    YEXE("buffer-reload");
    yed_cprint("Buffer reloaded");
}
