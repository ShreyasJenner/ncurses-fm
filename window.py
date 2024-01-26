from __future__ import annotations

import urwid

def menu_items(choices):
    f = open("input","r")
    for x in f:
        choices.append(x.strip())
    f.close()

def exit_on_q(key):
    if key in ('q', 'Q'):
        raise urwid.ExitMainLoop()

def menu(title, choices):
    menu_items(choices)
    body = [urwid.Text(title), urwid.Divider()]
    for c in choices:
        button = urwid.Button(c)
        urwid.connect_signal(button, 'click', item_chosen, c)
        body.append(urwid.AttrMap(button, None, focus_map='reversed'))
    return urwid.ListBox(urwid.SimpleFocusListWalker(body))


def item_chosen(button, choice):
    with open('output','w') as f:
        f.write(choice)
    #response = urwid.Text(['You chose ', choice, '\n'])
    #done = urwid.Button('Ok')
    #urwid.connect_signal(done, 'click', exit_program)
    #main.original_widget = urwid.Filler(urwid.Pile([response,
        #urwid.AttrMap(done, None, focus_map='reversed')]))


def exit_program(button):
    raise urwid.ExitMainLoop()


choices=[]
main = urwid.Padding(menu('File Manager', choices), left=2, right=2)

top = urwid.Overlay(main, urwid.SolidFill('\N{MEDIUM SHADE}'),
    align='center', width=('relative', 60),
    valign='middle', height=('relative', 60),
    min_width=20, min_height=9)

urwid.MainLoop(top, palette=[('reversed', 'standout', '')],unhandled_input=exit_on_q).run()

