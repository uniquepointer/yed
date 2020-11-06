#include <yed/plugin.h>

static yed_frame  *frame;
static yed_buffer *list_buff;
static yed_buffer *find_buff;
static char        prompt_buff[512];

void ctags_gen(int n_args, char **args);
void ctags_find(int n_args, char **args);
void ctags_jump_to_definition(int n_args, char **args);

void ctags_find_frame_delete_handler(yed_event *event);
void ctags_find_buffer_delete_handler(yed_event *event);
void ctags_find_key_pressed_handler(yed_event *event);
void ctags_find_line_handler(yed_event *event);

int yed_plugin_boot(yed_plugin *self) {
    yed_event_handler frame_delete;
    yed_event_handler buffer_delete;
    yed_event_handler key_pressed;
    yed_event_handler line;

    frame_delete.kind  = EVENT_FRAME_PRE_DELETE;
    frame_delete.fn    = ctags_find_frame_delete_handler;
    buffer_delete.kind = EVENT_BUFFER_PRE_DELETE;
    buffer_delete.fn   = ctags_find_buffer_delete_handler;
    key_pressed.kind   = EVENT_KEY_PRESSED;
    key_pressed.fn     = ctags_find_key_pressed_handler;
    line.kind          = EVENT_LINE_PRE_DRAW;
    line.fn            = ctags_find_line_handler;
    yed_plugin_add_event_handler(self, frame_delete);
    yed_plugin_add_event_handler(self, buffer_delete);
    yed_plugin_add_event_handler(self, key_pressed);
    yed_plugin_add_event_handler(self, line);

    yed_plugin_set_command(self, "ctags-gen",  ctags_gen);
    yed_plugin_set_command(self, "ctags-find", ctags_find);
    yed_plugin_set_command(self, "ctags-jump-to-definition", ctags_jump_to_definition);

    return 0;
}

void ctags_find_frame_delete_handler(yed_event *event) {
    if (event->frame == frame) {
        if (find_buff) {
            yed_free_buffer(find_buff);
        }
        if (list_buff) {
            yed_free_buffer(list_buff);
        }

        frame = NULL;
    }
}

void ctags_find_buffer_delete_handler(yed_event *event) {
    if (event->buffer == find_buff) {
        find_buff = NULL;
    } else if (event->buffer == list_buff) {
        list_buff = NULL;
    }
}

void ctags_find_cleanup(void) {
    if (frame) {
        yed_delete_frame(frame);
    }
    if (list_buff) {
        yed_free_buffer(list_buff);
    }
    if (find_buff) {
        yed_free_buffer(find_buff);
    }
}

int ctag_parse_path_and_line(const char *text, char *path_buff, int *row) {
    char *path_start;
    char *line_start;

    if (!(path_start = strchr(text, '\t'))) {
        return 0;
    }
    path_start += 1;

    if (!(line_start = strchr(path_start, '\t'))) {
        return 0;
    }

    *line_start = 0;
    strcpy(path_buff, path_start);
    *line_start = '\t';

    if (sscanf(line_start, "%d", row) != 1) {
        return 0;
    }

    return 1;
}

void ctag_find_select(void) {
    yed_line *line;
    char      path[4096];
    int       row;

    line = yed_buff_get_line(find_buff, frame->cursor_line);
    array_zero_term(line->chars);

    if (!ctag_parse_path_and_line(line->chars.data, path, &row)) {
        ctags_find_cleanup();
        return;
    }

    ctags_find_cleanup();

    YEXE("buffer", path);
    yed_set_cursor_within_frame(ys->active_frame, 1, row);
}

void ctags_find_key_pressed_handler(yed_event *event) {
    yed_frame *eframe;

    eframe = ys->active_frame;

    if (event->key != ENTER            /* not the key we want */
    ||  ys->interactive_command        /* still typing        */
    ||  !eframe                        /* no frame            */
    ||  eframe != frame                /* not our frame       */
    ||  !eframe->buffer                /* no buffer           */
    ||  eframe->buffer != find_buff) { /* not our buffer      */
        return;
    }

    ctag_find_select();
}

void ctags_find_line_handler(yed_event *event) {
    yed_buffer *buff;
    yed_line   *line;
    yed_glyph  *git;
    int         col;
    int         width;
    int         i;
    yed_attrs   attrs[4];
    int         attr_pos;
    yed_attrs  *dst_attrs;

    if (event->frame->buffer != find_buff) {
        return;
    }

    buff = event->frame->buffer;
    line = yed_buff_get_line(buff, event->row);

    attrs[0] = yed_active_style_get_code_keyword();
    attrs[1] = yed_active_style_get_code_string();
    attrs[2] = yed_active_style_get_code_number();
    attrs[3] = yed_active_style_get_code_comment();

    attr_pos = 0;
    col      = 0;
    yed_line_glyph_traverse(*line, git) {
        if (git->c == '\t' && attr_pos < 3) {
            attr_pos += 1;
        }
        width = yed_get_glyph_width(*git);
        for (i = 0; i < width; i += 1) {
            dst_attrs = array_item(event->line_attrs, col + i);
            yed_combine_attrs(dst_attrs, attrs + attr_pos);
        }
        col += width;
    }
}

void ctags_gen(int n_args, char **args) {
    char cmd_buff[1024];
    int  i;
    int  status;

    if (n_args == 0) {
        yed_cerr("expected 1 or more arguments");
        return;
    }

    cmd_buff[0] = 0;
    strcat(cmd_buff, "ctags -n");
    for (i = 0; i < n_args; i += 1) {
        strcat(cmd_buff, " ");
        strcat(cmd_buff, args[i]);
    }
    strcat(cmd_buff, " > /dev/null 2>&1");

    status = system(cmd_buff);
    if (status) {
        yed_cerr("ctags failed with exit status %d", status);
        return;
    }
    yed_cprint("ctags has completed");
}

void ctags_find_set_prompt(char *p, char *attr) {
    prompt_buff[0] = 0;

    strcat(prompt_buff, p);

    if (attr) {
        strcat(prompt_buff, attr);
    }

    ys->cmd_prompt = prompt_buff;
}

int ctags_find_make_buffers(void) {
    list_buff = yed_get_buffer("*ctags-list");

    if (!list_buff) {
        list_buff = yed_create_buffer("*ctags-list");
        list_buff->flags |= BUFF_RD_ONLY;
    } else {
        yed_buff_clear_no_undo(list_buff);
    }

    if (yed_fill_buff_from_file(list_buff, "tags")) {
        ctags_find_cleanup();
        return 0;
    }

    if (!find_buff) {
        find_buff = yed_create_buffer("*ctags-find-list");
        find_buff->flags |= BUFF_RD_ONLY;
    } else {
        yed_buff_clear_no_undo(find_buff);
    }

    return 1;
}

void ctags_find_make_frame(void) {
    frame = yed_add_new_frame(0.15, 0.15, 0.7, 0.7);
    yed_clear_frame(frame);
    yed_activate_frame(frame);
}

void ctags_find_filter(void) {
    char      *tag_start;
    int        len;
    yed_line  *line;
    yed_glyph *git;
    int        max_tag_len;
    int        tag_len;
    int        row;
    yed_line  *new_line;
    int        col;
    int        i;

    array_zero_term(ys->cmd_buff);
    tag_start = array_data(ys->cmd_buff);
    len       = strlen(tag_start);

    yed_buff_clear_no_undo(find_buff);
    yed_buff_delete_line_no_undo(find_buff, 1);

    max_tag_len = 0;
    bucket_array_traverse(list_buff->lines, line) {
        tag_len = 0;
        yed_line_glyph_traverse(*line, git) {
            if (git->c == '\t') { break; }
            tag_len += 1;
        }
        if (tag_len > max_tag_len) {
            max_tag_len = tag_len;
        }
    }

    bucket_array_traverse(list_buff->lines, line) {
        if (array_len(line->chars) > 0
        &&  yed_line_col_to_glyph(line, 1)->c == '!') {
            continue;
        }

        array_zero_term(line->chars);

        if (strncmp(line->chars.data, tag_start, len) == 0) {
            yed_buffer_add_line_no_undo(find_buff);
            row = yed_buff_n_lines(find_buff);
            yed_buff_set_line_no_undo(find_buff, row, line);
            new_line = yed_buff_get_line(find_buff, row);
            for (col = 1; col <= new_line->visual_width;) {
                git = yed_line_col_to_glyph(new_line, col);
                if (git->c == '\t') {
                    for (i = 0; i < max_tag_len - col + 1; i += 1) {
                        yed_insert_into_line_no_undo(find_buff, row, col, G(' '));
                    }
                    break;
                }
                col += yed_get_glyph_width(*git);
            }
        }
    }

    if (yed_buff_n_lines(find_buff) == 0) {
        yed_buffer_add_line_no_undo(find_buff);
    }
}

int ctags_find_start(void) {
    ys->interactive_command = "ctags-find";
    ctags_find_set_prompt("(ctags-find) ", NULL);
    ctags_find_make_frame();
    if (!ctags_find_make_buffers()) {
        return 0;
    }
    yed_frame_set_buff(frame, find_buff);
    yed_clear_cmd_buff();
    ctags_find_filter();

    return 1;
}

void ctags_find_take_key(int key) {
    if (key == CTRL_C) {
        ys->interactive_command = NULL;
        ys->current_search      = NULL;
        yed_clear_cmd_buff();
        ctags_find_cleanup();
    } else if (key == ENTER) {
        ys->interactive_command = NULL;
        ys->current_search      = NULL;
        frame->dirty            = 1;
        yed_clear_cmd_buff();
        if (yed_buff_n_lines(find_buff) == 1) {
            ctag_find_select();
        }
    } else {
        if (key == BACKSPACE) {
            if (array_len(ys->cmd_buff)) {
                yed_cmd_buff_pop();
            }
        } else if (!iscntrl(key)) {
            yed_cmd_buff_push(key);
        }

        ctags_find_filter();
    }
}

void ctags_find(int n_args, char **args) {
    int key;

    if (!ys->interactive_command) {
        if (!ctags_find_start()) {
            ys->interactive_command = NULL;
            yed_cerr("error opening tags file (have you run 'ctags-gen'?)");
        }
    } else {
        sscanf(args[0], "%d", &key);
        ctags_find_take_key(key);
    }
}

void ctags_jump_to_definition(int n_args, char **args) {
    char       *word;
    int         word_len;
    yed_line   *line;
    char       *text;
    char        path[4096];
    int         row;

    if (!ctags_find_make_buffers()) {
        yed_cerr("error opening tags file (have you run 'ctags-gen'?)");
        goto out;
    }

    if (!(word = yed_word_under_cursor())) {
        yed_cerr("cursor is not on a word");
        goto out;
    }

    word_len = strlen(word);

    bucket_array_traverse(list_buff->lines, line) {
        array_zero_term(line->chars);
        text = array_data(line->chars);

        if (array_len(line->chars) <= word_len) { continue; }
        if (strncmp(text, word, word_len))      { continue; }
        if (*(char*)(text + word_len) != '\t')  { continue; }

        if (!ctag_parse_path_and_line(text, path, &row)) {
            yed_cerr("error parsing location from tag line");
            goto out;
        }

        YEXE("buffer", path);
        yed_set_cursor_within_frame(ys->active_frame, 1, row);

        goto out;
    }

    yed_cerr("not found");

out:
    ctags_find_cleanup();
}