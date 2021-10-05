#include <yed/plugin.h>

void
buffer_go_fmt(void);
void
ev_buffer_go_fmt(yed_event* event);

char bufferLoc[512];

int
yed_plugin_boot(yed_plugin* self)
{
    /*Check for plug version*/
    YED_PLUG_VERSION_CHECK();

    yed_event_handler goFmt;
    yed_event_handler buffReload;

    goFmt.kind = EVENT_BUFFER_POST_WRITE;
    goFmt.fn   = ev_buffer_go_fmt;

    if (yed_get_var("go-fmt-auto") == NULL)
    {
        yed_set_var("go-fmt-auto", "yes");
    }

    if (yed_var_is_truthy("go-fmt-auto"))
    {
        yed_plugin_add_event_handler(self, goFmt);
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

int
go_fmt(void)
{
    int        output_len, status;
    char       cmd_buff[1024];

    buff_path_for_fmt();
    snprintf(cmd_buff, sizeof(cmd_buff), "gofmt -w %s &> /tmp/golog", bufferLoc);
    LOG_CMD_ENTER("gofmt");
    yed_cerr("%s", yed_run_subproc(cmd_buff, &output_len, &status));
    LOG_EXIT();

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
        if(go_fmt() == 0)
        {
            YEXE("buffer-reload");
            yed_cprint("Reloaded buffer\n");
        }
    }
}
void
buffer_go_fmt()
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
        if(go_fmt() == 0)
        {
            YEXE("buffer-reload");
            yed_cprint("Reloaded buffer\n");
        }
    }
}
