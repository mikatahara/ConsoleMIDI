# ConsoleMIDI
MIDI App for Windows Console

- midsub.c : Library for MIDI Device Open and Close
- midiintest.c : MIDI IN test program
- midiouttest.c : MIDI OUT test program

```
gcc -o midiintest midiintest.c c:\MinGW\Lib\libwinmm.a
gcc -o midiouttest midiouttest.c c:\MinGW\Lib\libwinmm.a
gcc -o miditest miditest.c midisub.c c:\MinGW\Lib\libwinmm.a
```

[MIDI IN/OUT Console Application](https://www.webmidiaudio.com/npage512.html)
