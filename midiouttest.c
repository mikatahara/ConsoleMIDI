/* *************************************************************************
	midiouttest.cpp : MIDI�o�̓e�X�g�p�̃R���\�[���A�v���P�[�V����

	This software is released under the MIT License, see LICENSE.txt.
	Copyright (c) 2013 Mikata Hara
*************************************************************************	*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>

HMIDIOUT outHandle;    // MIDI�o�͂𑗐M���邽�߂�MIDI�f�o�C�X�C���^�[�t�F�C�X

int MidiOutInit(){

	MIDIOUTCAPS     mic;       
	int flag;		// �֐��̖߂�l 
	int iNumDevs = midiOutGetNumDevs();  //MIDI�o�̓f�o�C�X�̐�
	unsigned int iDevNum=0;

	/* �o�̓f�o�C�X�̖��O��\������  */
	for (int i = 0; i < iNumDevs; i++)
	{
        /* ���̃f�o�C�X�Ɋւ�������擾���� */
		if (!midiOutGetDevCapsA(i, &mic, sizeof(MIDIINCAPS)))
		{
            /* �f�o�C�XID�Ɩ��O��\�����܂� */
			fprintf(stderr,"outHandle ID #%u: %s\r\n", i, mic.szPname);
		}
	}


	/* �o�̓f�o�C�X�̑I�� */
	fprintf(stderr,"These are the only available devices...?\n"); 
	fprintf(stderr,"Device No.=? ");
	scanf("%u",&iDevNum);

	// Open the MIDI output port
	flag = midiOutOpen(&outHandle, iDevNum, 0, 0, CALLBACK_NULL);

	if (flag != MMSYSERR_NOERROR) {
		fprintf(stderr,"Error opening MIDI Output.\n");
		return 1;
	}
	return 0;
}

int main(int argc, char** argv) {

	int ckey;           // ���݉�����Ă���L�[�{�[�h�L�[
	int flag;           // �G���[�t���O
	int notestate=0;	// ����ON��OFF���H
	unsigned char notenum;	//Note Number
	unsigned char Note[8]={48,50,52,53,55,57,59,60};
	int i;

	// ���M�p���b�Z�[�W
	union { unsigned long word; unsigned char data[4]; } message;
	// Note On���Ƃ���ƈȉ��̂悤�ɐݒ肷��
	message.data[0] = 0x90;  // MIDI note-on message (requires to data bytes)
	message.data[1] = 60;    // MIDI note-on message: Key number (60 = middle C)
	message.data[2] = 100;   // MIDI note-on message: Key velocity (100 = loud)
	message.data[3] = 0;     // �g���Ă��Ȃ�

	if(MidiOutInit()) return EXIT_FAILURE;	// MIDI�o�̓f�o�C�X������

	// "q"���������܂ő�����
	fprintf(stderr,"Press \"1\", \"2\", \"2\", ..., \"8\" to send MIDI\n");
	fprintf(stderr,"Press \"q\" to quit.\n");

	while (1) {
		if (kbhit()) {	// �R���s���[�^�[�̃L�[�{�[�h�̃L�[�������ꂽ�ꍇ 
			ckey = getch();
			if (ckey == 'q') break;
			if(ckey<'1' || ckey>'8') continue;
			notenum = Note[ckey-'1'];

			if (notestate == 0) {
			// �m�[�g�͌��݃I�t�B�m�[�g���I���ɂ���B
				message.data[2] = 100;	//velocity=100
				message.data[1] = notenum;
				notestate = 1;
				printf("Note turned ON.\r");
			} else {
			//  �m�[�g�͌��݃I���B�m�[�g���I�t�ɂ���B
				message.data[2] = 0;  //velocity = note off
				message.data[1] = notenum;
				notestate = 0;
				printf("Note turned OFF.\r");
			}

		//MIDI�f�[�^���M
			flag = midiOutShortMsg(outHandle, message.word);
			if (flag != MMSYSERR_NOERROR) {
				printf("Warning: MIDI Output is not open.\n");
				break;
			}
		}
	}

	// �o�̓f�o�C�X�����Z�b�g����
 	midiOutReset(outHandle);

	// MIDI�f�o�C�X�̃f�[�^�����ׂč폜���AMIDI�o�̓|�[�g�����
	midiOutClose(outHandle);

	return EXIT_SUCCESS;
}
