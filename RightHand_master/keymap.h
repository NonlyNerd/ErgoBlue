// Some stuff to make the action maps prettier. I use Clang Format, and it
// messes up the comments if they're over 80 characters on any individual
// line...
#define LCMD MOD(LEFTCOMMAND)
#define LSHFT MOD(LEFTSHIFT)
#define LCTL MOD(LEFTCTRL)
#define LOPT MOD(LEFTOPTION)
#define RCMD MOD(RIGHTCOMMAND)
#define RSHFT MOD(LEFTSHIFT)
#define RCTL MOD(RIGHTSHIFT)
#define ROPT MOD(RIGHTOPTION)

#define LGUI MOD(LEFTGUI)
#define LALT MOD(LEFTALT)
#define RGUI MOD(RIGHTGUI)
#define RALT MOD(RIGHTALT)

#define GRV GRAVE
#define OBRC BRACKET_LEFT
#define CBRC BRACKET_RIGHT
#define BKSP BACKSPACE
#define DEL DELETE
#define PGUP PAGE_UP
#define PGDN PAGE_DOWN
#define EQ_ KEY(EQUAL)
#define SEMI_ KEY(SEMICOLON)
#define COMMA_ KEY(COMMA)
#define DOT_ KEY(PERIOD)
#define QUOTE_ KEY(APOSTROPHE)
#define ENTER_ KEY(RETURN)
#define UP_ KEY(ARROW_UP)
#define DOWN_ KEY(ARROW_DOWN)
#define LEFT_ KEY(ARROW_LEFT)
#define RIGHT_ KEY(ARROW_RIGHT)
#define SPACE_ KEY(SPACE)
#define M_MUTE KEY(M_MUTE)
#define M_VOLU KEY(M_VOLUME_UP)
#define M_VOLD KEY(M_VOLUME_DOWN)

#define M_PLAY CONS(M_PLAY)
#define M_PREV CONS(M_PREVIOUS_TRACK)
#define M_NEXT CONS(M_NEXT_TRACK)

#define SHFTC LSHFT | LCMD
#define CTLC LCTL | LCMD
#define OPTC LOPT | LCMD

// Some shortcuts for modifiers & stuff
#define CMK(a) KMOD(a, LEFTCOMMAND)
#define CM(a) MOD1(a, LEFTCOMMAND)
#define CTK(a) KMOD(a, LEFTCTRL)
#define CT(a) MOD1(a, LEFTCTRL)
#define OPK(a) KMOD(a, LEFTOPTION)
#define OP(a) MOD1(a, LEFTOPTION)
#define AL(a) MOD1(a, LEFTALT)
#define ALK(a) KMOD(a, LEFTALT)
#define GU(a) MOD1(a, LEFTGUI)
#define GUK(a) KMOD((a, LEFTGUI)
#define COSK(a) KMOD3(a, LEFTCTRL, LEFTSHIFT, LEFTOPTION)
#define COS(a) MOD3(a, LEFTCTRL, LEFTSHIFT, LEFTOPTION)
#define ALLK(a) KMOD4(a, LEFTCTRL, LEFTSHIFT, LEFTOPTION, LEFTCOMMAND)
#define ALL(a) MOD4(a, LEFTCTRL, LEFTSHIFT, LEFTOPTION, LEFTCOMMAND)

#define CM_EQ CM(EQ_)
#define CM_SEMI CM(SEMI_)
#define CM_QUOTE CM(QUOTE_)
#define CM_CMA CM(COMMA_)
#define CM_DOT CM(DOT_)
#define CM_SLSH CMK(SLASH)
#define CM_SPC CMK(SPACE)
#define CM_PUP CMK(PGUP)
#define CM_PDN CMK(PGDN)
#define CM_RET CMK(RETURN)
#define OP_LEFT OP(LEFT_)
#define OP_RIGHT OP(RIGHT_)
#define CM_OBRC CMK(OBRC)
#define CM_CBRC CMK(CBRC)
#define CM_UP CM(UP_)
#define CM_DN CM(DOWN_)
#define LAYER_MAC_BASE 0
#define LAYER_WIN_BASE 1
#define LAYER_FUNC 2
#define LAYER_MAC_CAP 3
// The next two aren't yet implemented, because they require actions that
// involve modifiers
#define LAYER_WIN_CAP 4
#define LAYER_WIN_CTL 5

#define LYR_WIN LYR_TOG(LAYER_WIN_BASE)
#define LYR_MAC LYR_TOG(LAYER_WIN_BASE)
#define LYR_FN LYR_TOG(LAYER_FUNC)
#define MAC_CAP LYR_SHIFT(LAYER_MAC_CAP)
/*
#define WIN_CAP LYR_SHIFT(LAYER_WIN_CAP)
#define WIN_CTL LYR_SHIFT(LAYER_WIN_CTL)
*/
#define WIN_CAP RCTL
#define WIN_CTL LCTL

// For the status dumper thingamajig
const char* layer_names[] = {
    "Mac(Base)", "Win", "Fn", "MacCmd", "WinCmd", "WinCtrl"};

const action_t keymap[][numcols * numrows * 2] = {
    {// LAYER_MAC_BASE (0)
     LROW1(KEY(ESCAPE), KEY(1), KEY(2), KEY(3), KEY(4), KEY(5), M_VOLD),
     LROW2(KEY(TAB), KEY(Q), KEY(W), KEY(E), KEY(R), KEY(T), M_PLAY),
     LROW3(MAC_CAP, KEY(A), KEY(S), KEY(D), KEY(F), KEY(G), KEY(GRAVE), LYR_FN),
     LROW4(LSHFT, KEY(Z), KEY(X), KEY(C), KEY(V), KEY(B), M_PREV, KEY(HOME)),
     LROW5(LCTL, LOPT, LCMD, M_MUTE, KEY(OBRC), KEY(BKSP), KEY(DEL), KEY(END)),

     RROW1(M_VOLU, KEY(6), KEY(7), KEY(8), KEY(9), KEY(0), KEY(MINUS)),
     RROW2(ROPT, KEY(Y), KEY(U), KEY(I), KEY(O), KEY(P), KEY(BACKSLASH)),
     RROW3(LYR_WIN, EQ_, KEY(H), KEY(J), KEY(K), KEY(L), SEMI_, QUOTE_),
     RROW4(KEY(PGUP), M_NEXT, KEY(N), KEY(M), COMMA_, DOT_, KEY(SLASH), RSHFT),
     RROW5(KEY(PGDN), ENTER_, SPACE_, KEY(CBRC), LEFT_, UP_, DOWN_, RIGHT_)},

    {// LAYER_WIN_BASE (1)
     // Put Alt & GUI in the right spots, Add the Insert key (instead of
     // M_PREV), Print Screen (instead of M_NEXT) and the application key
     // (instead of M_MUTE)
     LROW1(___, ___, ___, ___, ___, ___, ___),
     LROW2(___, ___, ___, ___, ___, ___, ___),
     LROW3(WIN_CAP, ___, ___, ___, ___, ___, ___, ___),
     LROW4(___, ___, ___, ___, ___, ___, KEY(INSERT), ___),
     LROW5(WIN_CTL, LGUI, LALT, KEY(APPLICATION), ___, ___, ___, ___),

     RROW1(___, ___, ___, ___, ___, ___, ___),
     RROW2(RGUI, ___, ___, ___, ___, ___, ___),
     RROW3(LYR_MAC, ___, ___, ___, ___, ___, ___, ___),
     RROW4(___, KEY(PRINT_SCREEN), ___, ___, ___, ___, ___, ___),
     RROW5(___, ___, ___, ___, ___, ___, ___, ___)},

    {// LAYER_FUNC (2)
     // Nothing too exciting here. I might perhaps go add my Rocksmith
     // keybindings, and perhaps put the function keys in a more 'debugger
     // binding friendly' order...
     LROW1(___, KEY(F1), KEY(F2), KEY(F3), KEY(F4), KEY(F5), KEY(F11)),
     LROW2(___, ___, ___, ___, ___, ___, ___),
     LROW3(___, ___, ___, ___, ___, ___, ___, ___),
     LROW4(___, ___, ___, ___, ___, ___, ___, ___),
     LROW5(___, ___, ___, ___, ___, ___, ___, ___),

     RROW1(KEY(F12), KEY(F6), KEY(F7), KEY(F8), KEY(F9), KEY(F10), ___),
     RROW2(___, ___, ___, ___, ___, ___, ___),
     RROW3(___, ___, ___, ___, ___, ___, ___, ___),
     RROW4(___, ___, ___, ___, ___, ___, ___, ___),
     RROW5(___, ___, ___, ___, ___, ___, ___, ___)},

    {// LAYER_MAC_CMD (3)
     // Mostly just cmd + key (CMK(a)), with exceptions for some window
     // manipulatiton bindings for my HammerSpoon configuration
     LROW1(CMK(ESCAPE), CMK(1), CMK(2), CMK(3), CMK(4), CMK(5), CMK(M)),
     LROW2(CMK(TAB), CMK(Q), CMK(W), CMK(E), CMK(R), CMK(T), COS(LEFT_)),
     LROW3(___, CMK(A), CMK(S), CMK(D), CMK(F), CMK(G), CMK(GRAVE), COSK(N)),
     LROW4(SHFTC, CMK(Z), CMK(X), CMK(C), CMK(V), CMK(B), COSK(H), CMK(HOME)),
     LROW5(CTLC, OPTC, LCMD, M_MUTE, CM_OBRC, OPK(BKSP), OPK(DEL), CMK(END)),

     RROW1(COS(UP_), CMK(6), CMK(7), CMK(8), CMK(9), CMK(0), CMK(MINUS)),
     RROW2(COS(RIGHT_), CMK(Y), CMK(U), CMK(I), CMK(O), CMK(P), CMK(BACKSLASH)),
     RROW3(COSK(M), CM_EQ, CMK(H), CMK(J), CMK(K), CMK(L), CM_SEMI, CM_QUOTE),
     RROW4(CM_PUP, COSK(J), CMK(N), CMK(M), CM_CMA, CM_DOT, CM_SLSH, CM_SPC),
     RROW5(CM_PDN, CM_RET, CM_SPC, CM_CBRC, OP_LEFT, CM_UP, CM_DN, OP_RIGHT)},

    /*{// LAYER_WIN_CMD (4)
        // This is magic to get a bunch of mac commands to send their Windows
        // equivalent The poster children are Caps-Q => Alt-F4 and Caps-W =>
        // Ctl-F4 Effectively making Caps-Q & Caps-W correspond to Quit and
        // Close Window
    }*/};
