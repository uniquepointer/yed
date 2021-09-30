# yed (Your Editor) AIO Edition

Check out our web page for more info!
[your-editor.org](https://your-editor.org)

# TL;DR
**Yed but with my super defaults.**
```
git clone https://git.sr.ht/~uniquepointer/yed && cd yed
make
```
Running ```make``` will put it on your .local/bin, so if you want a specific place youc an do ```make LOC=/your/full/path/here```

If you got your own config just make a folder inside this fork called ```userconf``` and copy your ```yedrc``` and ```ypm_list``` and you can carry it with you.
Once you are done installing and run yed, you can just run ```ypm-update```, you don't need an internet connection to compile and load your plugins!
![yed](screenshots/1.png)

# Introduction
`yed` is a small and simple terminal editor core that is meant to be extended through a powerful plugin architecture.
The editor base is command driven, lightweight, and fast.
It makes no assumptions about a user's desired editing style and leaves most functionality up to implementation and configuration plugins.
## Core Editor Features
- _FAST_
- Dependency free
- Layered frame management
- 24-bit truecolor support
- Dynamic key bindings and key sequences
- Undo/redo
- Live find and replace
- Customization/extension via plugins

![yed](screenshots/2.png)

## Plugins
`yed` plugins are shared libraries (typically written in C) that use the various facilities provided by the core editor to implement customization and add additional functionality.
A plugin may:
- Add commands
- Set/unset variables
- Define key bindings
- Manipulate buffers and frames
- Define styles
- Register event handlers, allowing it to:
    - Control how text is drawn
    - Intercept keystrokes
    - Programmatically send keystrokes
    - Perform actions on save
    - More!
