#include <yed/plugin.h>

void
buffer_nix_fmt(int n_args, char **args);
void
ev_buffer_nix_fmt(yed_event* event);

char bufferLoc[512];

int
yed_plugin_boot(yed_plugin* self)
{
    /*Check for plug version*/
    YED_PLUG_VERSION_CHECK();

    yed_event_handler eh_nix_format;

    eh_nix_format.kind = EVENT_BUFFER_POST_WRITE;
    eh_nix_format.fn   = ev_buffer_nix_fmt;

    if (yed_get_var("nixfmt-auto") == NULL)
    {
        yed_set_var("nixfmt-auto", "yes");
    }

    if (yed_var_is_truthy("nixfmt-auto"))
    {
        yed_plugin_add_event_handler(self, eh_nix_format);
    }

    yed_plugin_set_command(self, "nix-fmt", buffer_nix_fmt);
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

    if (buffer->name && buffer->ft == yed_get_ft("Nix"))
    {
        strcpy(bufferLoc, buffer->path);
    }
    else
    {
        yed_cerr("buffer has no path or it's not a Nix file'");
    }
}

int
_nix_fmt(void)
{
    int        output_len, status;
    char       cmd_buff[1024];

    buff_path_for_fmt();
    snprintf(cmd_buff, sizeof(cmd_buff), "nixpkgs-fmt %s &> /tmp/nixlog", bufferLoc);
    yed_run_subproc(cmd_buff, &output_len, &status);

    if (status != 0)
    {
        yed_cerr("Failure to format nix buffer\n");
        return status;
    }
    else
    {

        yed_cprint("Formatted buffer\n");
        return 0;
    }
}
void
ev_buffer_nix_fmt(yed_event* event)
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

    if (frame->buffer->ft == yed_get_ft("Nix"))
    {
        if(_nix_fmt() == 0)
        {
            YEXE("buffer-reload");
            YEXE("redraw");
            yed_cprint("Reloaded buffer\n");
        }
    }
}
void
buffer_nix_fmt(int n_args, char **args)
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

    if (frame->buffer->ft == yed_get_ft("Nix"))
    {
        if(_nix_fmt() == 0)
        {
            YEXE("write-buffer");
            YEXE("buffer-reload");
            YEXE("redraw");
            yed_cprint("Reloaded buffer\n");
        }
    }
}
