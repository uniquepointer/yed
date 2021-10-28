#include <yed/plugin.h>

void
a_cmd(int n_args, char** args);

char bufferLoc[512];
int
yed_plugin_boot(yed_plugin* self)
{
    /*Check for matching YED and plugin version*/
    YED_PLUG_VERSION_CHECK();
    yed_plugin_set_command(self, "A", a_cmd);
    return 0;
}

void
_buff_path_for_fmt()
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
    const char* ext = get_path_ext(buffer->path);

    if (
                buffer->name         &&
                (strcmp(ext, "c")    ||
                 strcmp(ext, "cxx")   ||
                 strcmp(ext, "cpp")   ||
                 strcmp(ext, "h")     ||
                 strcmp(ext, "hpp")   ||
                 strcmp(ext, "hxx"))  &&
                (buffer->ft == yed_get_ft("C") ||
                 buffer->ft == yed_get_ft("C++"))
    )
    {
        memset(bufferLoc, 0, sizeof(bufferLoc));
        strcpy(bufferLoc, buffer->path);
    }
    else
    {
        yed_cerr("buffer has no path or it's not a C/C++ file'");
    }
}

void
a_cmd(int n_args, char** args)
{
    yed_frame* frame;
    yed_buffer* buffer;
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
    if (!buffer->name && (buffer->ft != yed_get_ft("C") ||
                          buffer->ft != yed_get_ft("C++")))
    {
        yed_cerr("Not a C or C++ file.");
    }
    _buff_path_for_fmt();
    const char* ext = get_path_ext(buffer->path);
    char newpath[512];
    memset(newpath, 0, sizeof(newpath));
    strcpy(newpath, bufferLoc);
    size_t idx = strlen(newpath) - 1;
    size_t erase = strlen(ext) == 0 ? 0 : (strlen(ext) - 1);

    if (frame->buffer->ft == yed_get_ft("C") &&
            (strcmp(ext, "c") == 0 || strcmp(ext, "h") == 0))
    {
        if (strcmp(ext, "c") == 0)
        {
            newpath[idx - erase] = 'h';
        }
        else
        {
            newpath[idx - erase] = 'c';
        }
    }
    else if (frame->buffer->ft == yed_get_ft("C++") &&
             (strcmp(ext, "cpp") == 0 || strcmp(ext, "hpp") == 0))
    {
        if (strcmp(ext, "cpp") == 0)
        {
            newpath[idx - erase] = 'h';
            newpath[idx - (erase - 1)] = 'p';
            newpath[idx - (erase - 2)] = 'p';
        }
        else
        {
            newpath[idx - erase] = 'c';
            newpath[idx - (erase - 1)] = 'p';
            newpath[idx - (erase - 2)] = 'p';
        }
    }
    else if (frame->buffer->ft == yed_get_ft("C++") &&
             (strcmp(ext, "cxx") == 0 || strcmp(ext, "hxx") == 0))
    {
        if (strcmp(ext, "cxx") == 0)
        {
            newpath[idx - erase] = 'h';
            newpath[idx - (erase - 1)] = 'x';
            newpath[idx - (erase - 2)] = 'x';
        }
        else
        {
            newpath[idx - erase] = 'c';
            newpath[idx - (erase - 1)] = 'x';
            newpath[idx - (erase - 2)] = 'x';
        }
    }
    YEXE("buffer", newpath);
}
