/* *************************************************************************
	midisub.h : 

	This software is released under the MIT License, see LICENSE.txt.
	Copyright (c) 2013 Mikata Hara
*************************************************************************	*/

extern HMIDIOUT	outHandle;	// MIDI入力を受信するためのMIDIデバイスインターフェイス
extern HMIDIOUT	inHandle;	// MIDI出力を送信するためのMIDIデバイスインターフェイス

extern int MidiInInit(DWORD midiCallback);
extern int MidiOutInit();
extern void MIDIInClose();
extern void MIDIOutClose();
extern int MIDIOut3byte(unsigned char status,unsigned char byte1,unsigned char byte2);
