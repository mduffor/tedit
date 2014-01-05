//
// VKeys.hpp
// Copyright 2013  Michael T. Duffy.  
// contact:  mduffor@gmail.com

// This file is part of TEdit.
//
// TEdit is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 3 as
// published by the Free Software Foundation.
//
// TEdit is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with TEdit.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef VKEYS_HPP
#define VKEYS_HPP

#define VKEY_SPACE            0x0020
#define VKEY_ESC              0x0100
#define VKEY_BACKSPACE        0x0101
#define VKEY_TAB              0x0102
#define VKEY_BACKTAB          0x0103
#define VKEY_PAUSE            0x0104
#define VKEY_PAGEUP           0x0105
#define VKEY_PAGEDOWN         0x0106
#define VKEY_END              0x0107
#define VKEY_HOME             0x0108
#define VKEY_LEFT             0x0109
#define VKEY_UP               0x010a
#define VKEY_RIGHT            0x010b
#define VKEY_DOWN             0x010c
#define VKEY_INSERT           0x010d
#define VKEY_DELETE           0x010e
#define VKEY_ENTER            0x010f
#define VKEY_F1               0x0110
#define VKEY_F2               0x0111
#define VKEY_F3               0x0112
#define VKEY_F4               0x0113
#define VKEY_F5               0x0114
#define VKEY_F6               0x0115
#define VKEY_F7               0x0116
#define VKEY_F8               0x0117
#define VKEY_F9               0x0118
#define VKEY_F10              0x0119
#define VKEY_F11              0x011a
#define VKEY_F12              0x011b
#define VKEY_F13              0x011c 
#define VKEY_F14              0x011d 
#define VKEY_F15              0x011e 
#define VKEY_CONTROL          0x011f 
#define VKEY_SHIFT            0x0120 
#define VKEY_ALT              0x0121
#define VKEY_META             0x0122

#define VKEY_MENU             0x0123
#define VKEY_CAPITAL          0x0124
#define VKEY_HELP             0x0125
#define VKEY_LBUTTON          0x0126
#define VKEY_RBUTTON          0x0127
#define VKEY_MBUTTON          0x0128
#define VKEY_NUMPAD0          0x0129
#define VKEY_NUMPAD1          0x012a
#define VKEY_NUMPAD2          0x012b
#define VKEY_NUMPAD3          0x012c
#define VKEY_NUMPAD4          0x012d
#define VKEY_NUMPAD5          0x012f
#define VKEY_NUMPAD6          0x0130
#define VKEY_NUMPAD7          0x0131
#define VKEY_NUMPAD8          0x0132
#define VKEY_NUMPAD9          0x0133
#define VKEY_NUMPAD_SPACE     0x0134
#define VKEY_NUMPAD_TAB       0x0135
#define VKEY_NUMPAD_ENTER     0x0136
#define VKEY_NUMPAD_F1        0x0137
#define VKEY_NUMPAD_F2        0x0138
#define VKEY_NUMPAD_F3        0x0139
#define VKEY_NUMPAD_F4        0x013a
#define VKEY_NUMPAD_HOME      0x013b
#define VKEY_NUMPAD_LEFT      0x013c
#define VKEY_NUMPAD_UP        0x013d
#define VKEY_NUMPAD_RIGHT     0x013e
#define VKEY_NUMPAD_DOWN      0x013f

//#define VKEY_NUMPAD_PRIOR     0x0140
#define VKEY_NUMPAD_PAGEUP     0x0141 
//#define VKEY_NUMPAD_NEXT      0x0142
#define VKEY_NUMPAD_PAGEDOWN   0x0143
#define VKEY_NUMPAD_END        0x0144
#define VKEY_NUMPAD_BEGIN      0x0145
#define VKEY_NUMPAD_INSERT     0x0146
#define VKEY_NUMPAD_DELETE     0x0147
#define VKEY_NUMPAD_EQUAL      0x0148
#define VKEY_NUMPAD_MULTIPLY   0x0149
#define VKEY_NUMPAD_ADD        0x014a
#define VKEY_NUMPAD_SEPARATOR  0x014b
#define VKEY_NUMPAD_SUBTRACT   0x014c
#define VKEY_NUMPAD_DECIMAL    0x014d
#define VKEY_NUMPAD_DIVIDE     0x014e

#define VKEY_MULTIPLY          0x014f
#define VKEY_ADD               0x0150
#define VKEY_SUBTRACT          0x0151
#define VKEY_DECIMAL           0x0152
#define VKEY_DIVIDE            0x0153
#define VKEY_NUMLOCK           0x0154
#define VKEY_SCROLL            0x0155
#define VKEY_SEPARATOR         0x0156


#define VKFLG_SHIFT          0x8000
#define VKFLG_CONTROL        0x4000
#define VKFLG_ALT            0x2000
#define VKFLG_META           0x1000
#define VKFLG_MASK           0xf000

#define CTRL_A              (0x0001 | VKFLG_CONTROL)
#define CTRL_B              (0x0002 | VKFLG_CONTROL)
#define CTRL_C              (0x0003 | VKFLG_CONTROL)
#define CTRL_D              (0x0004 | VKFLG_CONTROL)
#define CTRL_E              (0x0005 | VKFLG_CONTROL)
#define CTRL_F              (0x0006 | VKFLG_CONTROL)
#define CTRL_G              (0x0007 | VKFLG_CONTROL)
#define CTRL_H              (0x0008 | VKFLG_CONTROL)
#define CTRL_I              (0x0009 | VKFLG_CONTROL)
#define CTRL_J              (0x000a | VKFLG_CONTROL)
#define CTRL_K              (0x000b | VKFLG_CONTROL)
#define CTRL_L              (0x000c | VKFLG_CONTROL)
#define CTRL_M              (0x000d | VKFLG_CONTROL)
#define CTRL_N              (0x000e | VKFLG_CONTROL)
#define CTRL_O              (0x000f | VKFLG_CONTROL)
#define CTRL_P              (0x0010 | VKFLG_CONTROL)
#define CTRL_Q              (0x0011 | VKFLG_CONTROL)
#define CTRL_R              (0x0012 | VKFLG_CONTROL)
#define CTRL_S              (0x0013 | VKFLG_CONTROL)
#define CTRL_T              (0x0014 | VKFLG_CONTROL)
#define CTRL_U              (0x0015 | VKFLG_CONTROL)
#define CTRL_V              (0x0016 | VKFLG_CONTROL)
#define CTRL_W              (0x0017 | VKFLG_CONTROL)
#define CTRL_X              (0x0018 | VKFLG_CONTROL)
#define CTRL_Y              (0x0019 | VKFLG_CONTROL)
#define CTRL_Z              (0x001a | VKFLG_CONTROL)

#endif // VKEYS_HPP
