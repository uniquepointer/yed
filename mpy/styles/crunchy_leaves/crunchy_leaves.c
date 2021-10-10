#include <yed/plugin.h>

#define MAYBE_CONVERT(rgb) (tc ? (rgb) : rgb_to_256(rgb))


#define white      MAYBE_CONVERT(0xFFF3F0)
#define black      MAYBE_CONVERT(0x0A0A0A)
#define grey        MAYBE_CONVERT(0x151414)
#define light_grey MAYBE_CONVERT(0x494646)
#define selection_grey MAYBE_CONVERT(0x393F46)
#define search_blue   MAYBE_CONVERT(0x99ffe6)
#define search_selection_bg MAYBE_CONVERT(0x170326)
#define yellow     MAYBE_CONVERT(0xF9DA77)
#define red        MAYBE_CONVERT(0xD37387)
#define orange     MAYBE_CONVERT(0xF7803B)
#define status_line_fg       MAYBE_CONVERT(0x252730)
#define associate_fg  MAYBE_CONVERT(0xFFF3F0)
#define green MAYBE_CONVERT(0xaafac8)
#define text      MAYBE_CONVERT(0xf9b79f)
#define cyan        MAYBE_CONVERT(0x170326)

#define background      grey
#define foreground      text
#define in_bg           black
#define in_fg           text
#define srch_fg         search_blue
#define srch_bg         search_selection_bg
#define srch_cursor_bg  red
#define srch_cursor_fg  search_selection_bg
#define attn            MAYBE_CONVERT(0xAF0000)
#define assoc_bg        orange
#define assoc_fg        associate_fg
#define status_bg       orange
#define status_fg       status_line_fg
#define sel_color_fg    text
#define sel_color_bg    light_grey
#define comment         MAYBE_CONVERT(0x555760)
#define keyword         white
#define pp_keyword      white
#define control_flow    green
#define typename        white
#define call            green
#define constant        white
#define number          white
#define string          yellow
#define character       yellow

PACKABLE_STYLE(crunchy_leaves) {
    yed_style s;
    int       tc,
              attr_kind;

    YED_PLUG_VERSION_CHECK();

    tc        = !!yed_get_var("truecolor");
    attr_kind = tc ? ATTR_RGB : ATTR_256;

    memset(&s, 0, sizeof(s));

    s.active.flags            = attr_kind;
    s.active.fg               = foreground;
    s.active.bg               = background;

    s.inactive.flags          = attr_kind;
    s.inactive.fg             = in_fg;
    s.inactive.bg             = in_bg;

    s.active_border           = s.active;
    s.inactive_border         = s.inactive;

    s.cursor_line.flags       = attr_kind;
    s.cursor_line.fg          = text;
    s.cursor_line.bg          = light_grey;

    s.search.flags            = attr_kind | ATTR_INVERSE;
    s.search.fg               = srch_fg;
    s.search.bg               = srch_bg;

    s.search_cursor.flags     = attr_kind | ATTR_INVERSE;
    s.search_cursor.fg        = srch_cursor_fg;
    s.search_cursor.bg        = srch_cursor_bg;

    s.selection.flags         = attr_kind;
    s.selection.fg            = text;
    s.selection.bg            = light_grey;

    s.attention.flags         = attr_kind | ATTR_BOLD;
    s.attention.fg            = attn;

    s.associate.flags         = attr_kind | ATTR_BOLD;
    s.associate.bg            = assoc_bg;
    s.associate.fg            = assoc_fg;

    s.command_line            = s.active;

    s.status_line.flags       = attr_kind | ATTR_BOLD;
    s.status_line.fg          = status_fg;
    s.status_line.bg          = status_bg;

    s.active_gutter           = s.active;
    s.inactive_gutter         = s.inactive;

    s.code_comment.flags      = attr_kind | ATTR_BOLD;
    s.code_comment.fg         = comment;

    s.code_keyword.flags      = attr_kind | ATTR_BOLD;
    s.code_keyword.fg         = keyword;

    s.code_preprocessor.flags = attr_kind | ATTR_BOLD;
    s.code_preprocessor.fg    = pp_keyword;

    s.code_control_flow.flags = attr_kind | ATTR_BOLD;
    s.code_control_flow.fg    = control_flow;

    s.code_typename.flags     = attr_kind | ATTR_BOLD;
    s.code_typename.fg        = typename;

    s.code_fn_call.flags      = attr_kind;
    s.code_fn_call.fg         = call;

    s.code_number.flags       = attr_kind;
    s.code_number.fg          = number;

    s.code_constant.flags     = attr_kind;
    s.code_constant.fg        = constant;

    s.code_string.flags       = attr_kind;
    s.code_string.fg          = string;

    s.code_character.flags    = attr_kind;
    s.code_character.fg       = character;

    yed_plugin_set_style(self, "crunchy_leaves", &s);

    return 0;
}
