/* *************************************************************************
	midisub.cpp : MIDI���o�̓T�u���[�`��

	This software is released under the MIT License, see LICENSE.txt.
	Copyright (c) 2013 Mikata Hara
*************************************************************************	*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>

HMIDIIN		inHandle;	//MIDI���͂���M���邽�߂�MIDI�f�o�C�X�C���^�[�t�F�C�X
HMIDIOUT	outHandle;	// MIDI�o�͂𑗐M���邽�߂�MIDI�f�o�C�X�C���^�[�t�F�C�X
union { unsigned long word; unsigned char data[4]; } message;	// ���M�p���b�Z�[�W

// ���̓f�o�C�X�̑I���Ə�����
int MidiInInit(DWORD midiCallback){

	MIDIINCAPS     mic;       
	unsigned long result;
	unsigned int iNumDevs= midiInGetNumDevs();  // MIDI���̓f�o�C�X�̐�
	unsigned int iDevNum=0; 

    /* ���̓f�o�C�X�̖��O��\������ */
	for (int i = 0; i < iNumDevs; i++)
	{
		/* ���̃f�o�C�X�Ɋւ�������擾���� */
		if (!midiInGetDevCaps(i, &mic, sizeof(MIDIINCAPS)))
		{
			/* �f�o�C�XID�Ɩ��O��\�����܂� */
			fprintf(stderr,"Device ID #%u: %s\r\n", i, mic.szPname);
		}
	}
 
	/* ���̓f�o�C�X�̑I�� */
	fprintf(stderr,"These are the only available devices...?\n"); 
	fprintf(stderr,"Device No.=? ");
	scanf("%u",&iDevNum);
 
    /* ���̓f�o�C�X���J�� 		*/
	/* �R�[���o�b�N�֐���o�^ 	*/
//	result = midiInOpen(&inHandle, iDevNum, (DWORD)midiCallback, 0, CALLBACK_FUNCTION);
	result = midiInOpen(&inHandle, iDevNum, midiCallback, 0, CALLBACK_FUNCTION);
     
	if (result)
	{
		fprintf(stderr,"There was an error opening the default MIDI In device!\r\n");
		return 1;
	} else {
		fprintf(stderr,"midiInStart has been called.\n"); 
		midiInStart(inHandle);	//MIDI���͊J�n
	}
	return 0;
}

// ���̓f�o�C�X�̃N���[�Y
void MIDIInClose()
{
	midiInStop(inHandle);
//	midiInReset(inHandle);
	midiInClose(inHandle);
/* 
	fprintf(stderr,"Lines are done twice because midiCallback\n");
	fprintf(stderr,"is called when midiInClose is called...?\n");
	fprintf(stderr,"%d was the MIDIIN handle.\n",inHandle); 
	fprintf(stderr,"Stuff's closed now.\n");     
*/
}

// �o�̓f�o�C�X�̑I���Ə�����
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

// �o�̓f�o�C�X�̃N���[�Y
void MIDIOutClose()
{

	// �o�̓f�o�C�X�����Z�b�g����
 	midiOutReset(outHandle);
	// MIDI�f�o�C�X�̃f�[�^�����ׂč폜���AMIDI�o�̓|�[�g�����
	midiOutClose(outHandle);
}

// 3byte MIDI �̏o��
int MIDIOut3byte(unsigned char status,unsigned char byte1,unsigned char byte2)
{
	int flag;		// �֐��̖߂�l 

	message.data[0] = status;	// MIDI status
	message.data[1] = byte1;	// MIDI data1
	message.data[2] = byte2;	// MIDI data2
	message.data[3] = 0;		// �g���Ă��Ȃ�

	//MIDI�f�[�^���M
	flag = midiOutShortMsg(outHandle, message.word);
	if (flag != MMSYSERR_NOERROR) {
		fprintf(stderr,"Warning: MIDI Output is not open.\n");
			return 1;
	}
	return 0;
}

