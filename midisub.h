/* *************************************************************************
	midisub.h : 

	This software is released under the MIT License, see LICENSE.txt.
	Copyright (c) 2013 Mikata Hara
*************************************************************************	*/

extern HMIDIOUT	outHandle;	// MIDI���͂���M���邽�߂�MIDI�f�o�C�X�C���^�[�t�F�C�X
extern HMIDIOUT	inHandle;	// MIDI�o�͂𑗐M���邽�߂�MIDI�f�o�C�X�C���^�[�t�F�C�X

extern int MidiInInit(DWORD midiCallback);
extern int MidiOutInit();
extern void MIDIInClose();
extern void MIDIOutClose();
extern int MIDIOut3byte(unsigned char status,unsigned char byte1,unsigned char byte2);
