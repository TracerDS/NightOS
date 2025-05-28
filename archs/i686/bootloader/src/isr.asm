extern ISR_Handler

global __kernel_isr_stub__:function (__kernel_isr_stub__.end - __kernel_isr_stub__.start)
__kernel_isr_stub__:
    .start:
        pusha

        xor eax, eax
        xor ecx, ecx
        xor edx, edx

        mov ax, ds
        mov cx, es
        mov dx, fs
        
        push eax ; push ds
        push ecx ; push es
        push edx ; push fs

        mov ax, gs
        mov cx, ss

        push eax  ; push gs
        push ecx  ; push ss

        mov ax, 0x10
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        mov ss, ax

        lea eax, dword [esp]

        push eax
        call ISR_Handler
        pop eax

        pop ecx ; pop ss
        pop eax ; pop gs

        mov ss, cx
        mov gs, ax

        pop edx ; pop fs
        pop ecx ; pop es
        pop eax ; pop ds

        mov fs, dx
        mov es, cx
        mov ds, ax

        popa
        add esp, 8
        iret
    .end:

; esp - 0x207fcc
; ebp - 0x208000
; 0x207fc4 0x207fcc
%macro ISR_NoError 1
global ISR%1
ISR%1:
    push 0xFFFFFFFF
    push %1
    jmp __kernel_isr_stub__
%endmacro

%macro ISR_Error 1
global ISR%1
ISR%1:
    push %1
    jmp __kernel_isr_stub__
%endmacro

ISR_NoError 0
ISR_NoError 1
ISR_NoError 2
ISR_NoError 3
ISR_NoError 4
ISR_NoError 5
ISR_NoError 6
ISR_NoError 7
ISR_Error 8
ISR_NoError 9
ISR_Error 10
ISR_Error 11
ISR_Error 12
ISR_Error 13
ISR_Error 14
ISR_NoError 15
ISR_NoError 16
ISR_Error 17
ISR_NoError 18
ISR_NoError 19
ISR_NoError 20
ISR_Error 21
ISR_NoError 22
ISR_NoError 23
ISR_NoError 24
ISR_NoError 25
ISR_NoError 26
ISR_NoError 27
ISR_NoError 28
ISR_NoError 29
ISR_NoError 30
ISR_NoError 31
ISR_NoError 32
ISR_NoError 33
ISR_NoError 34
ISR_NoError 35
ISR_NoError 36
ISR_NoError 37
ISR_NoError 38
ISR_NoError 39
ISR_NoError 40
ISR_NoError 41
ISR_NoError 42
ISR_NoError 43
ISR_NoError 44
ISR_NoError 45
ISR_NoError 46
ISR_NoError 47
ISR_NoError 48
ISR_NoError 49
ISR_NoError 50
ISR_NoError 51
ISR_NoError 52
ISR_NoError 53
ISR_NoError 54
ISR_NoError 55
ISR_NoError 56
ISR_NoError 57
ISR_NoError 58
ISR_NoError 59
ISR_NoError 60
ISR_NoError 61
ISR_NoError 62
ISR_NoError 63
ISR_NoError 64
ISR_NoError 65
ISR_NoError 66
ISR_NoError 67
ISR_NoError 68
ISR_NoError 69
ISR_NoError 70
ISR_NoError 71
ISR_NoError 72
ISR_NoError 73
ISR_NoError 74
ISR_NoError 75
ISR_NoError 76
ISR_NoError 77
ISR_NoError 78
ISR_NoError 79
ISR_NoError 80
ISR_NoError 81
ISR_NoError 82
ISR_NoError 83
ISR_NoError 84
ISR_NoError 85
ISR_NoError 86
ISR_NoError 87
ISR_NoError 88
ISR_NoError 89
ISR_NoError 90
ISR_NoError 91
ISR_NoError 92
ISR_NoError 93
ISR_NoError 94
ISR_NoError 95
ISR_NoError 96
ISR_NoError 97
ISR_NoError 98
ISR_NoError 99
ISR_NoError 100
ISR_NoError 101
ISR_NoError 102
ISR_NoError 103
ISR_NoError 104
ISR_NoError 105
ISR_NoError 106
ISR_NoError 107
ISR_NoError 108
ISR_NoError 109
ISR_NoError 110
ISR_NoError 111
ISR_NoError 112
ISR_NoError 113
ISR_NoError 114
ISR_NoError 115
ISR_NoError 116
ISR_NoError 117
ISR_NoError 118
ISR_NoError 119
ISR_NoError 120
ISR_NoError 121
ISR_NoError 122
ISR_NoError 123
ISR_NoError 124
ISR_NoError 125
ISR_NoError 126
ISR_NoError 127
ISR_NoError 128
ISR_NoError 129
ISR_NoError 130
ISR_NoError 131
ISR_NoError 132
ISR_NoError 133
ISR_NoError 134
ISR_NoError 135
ISR_NoError 136
ISR_NoError 137
ISR_NoError 138
ISR_NoError 139
ISR_NoError 140
ISR_NoError 141
ISR_NoError 142
ISR_NoError 143
ISR_NoError 144
ISR_NoError 145
ISR_NoError 146
ISR_NoError 147
ISR_NoError 148
ISR_NoError 149
ISR_NoError 150
ISR_NoError 151
ISR_NoError 152
ISR_NoError 153
ISR_NoError 154
ISR_NoError 155
ISR_NoError 156
ISR_NoError 157
ISR_NoError 158
ISR_NoError 159
ISR_NoError 160
ISR_NoError 161
ISR_NoError 162
ISR_NoError 163
ISR_NoError 164
ISR_NoError 165
ISR_NoError 166
ISR_NoError 167
ISR_NoError 168
ISR_NoError 169
ISR_NoError 170
ISR_NoError 171
ISR_NoError 172
ISR_NoError 173
ISR_NoError 174
ISR_NoError 175
ISR_NoError 176
ISR_NoError 177
ISR_NoError 178
ISR_NoError 179
ISR_NoError 180
ISR_NoError 181
ISR_NoError 182
ISR_NoError 183
ISR_NoError 184
ISR_NoError 185
ISR_NoError 186
ISR_NoError 187
ISR_NoError 188
ISR_NoError 189
ISR_NoError 190
ISR_NoError 191
ISR_NoError 192
ISR_NoError 193
ISR_NoError 194
ISR_NoError 195
ISR_NoError 196
ISR_NoError 197
ISR_NoError 198
ISR_NoError 199
ISR_NoError 200
ISR_NoError 201
ISR_NoError 202
ISR_NoError 203
ISR_NoError 204
ISR_NoError 205
ISR_NoError 206
ISR_NoError 207
ISR_NoError 208
ISR_NoError 209
ISR_NoError 210
ISR_NoError 211
ISR_NoError 212
ISR_NoError 213
ISR_NoError 214
ISR_NoError 215
ISR_NoError 216
ISR_NoError 217
ISR_NoError 218
ISR_NoError 219
ISR_NoError 220
ISR_NoError 221
ISR_NoError 222
ISR_NoError 223
ISR_NoError 224
ISR_NoError 225
ISR_NoError 226
ISR_NoError 227
ISR_NoError 228
ISR_NoError 229
ISR_NoError 230
ISR_NoError 231
ISR_NoError 232
ISR_NoError 233
ISR_NoError 234
ISR_NoError 235
ISR_NoError 236
ISR_NoError 237
ISR_NoError 238
ISR_NoError 239
ISR_NoError 240
ISR_NoError 241
ISR_NoError 242
ISR_NoError 243
ISR_NoError 244
ISR_NoError 245
ISR_NoError 246
ISR_NoError 247
ISR_NoError 248
ISR_NoError 249
ISR_NoError 250
ISR_NoError 251
ISR_NoError 252
ISR_NoError 253
ISR_NoError 254
ISR_NoError 255
