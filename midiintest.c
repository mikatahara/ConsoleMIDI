/* *************************************************************************
	midiintest.cpp : MIDI���̓e�X�g�p�̃R���\�[���A�v���P�[�V����

	This software is released under the MIT License, see LICENSE.txt.
	Copyright (c) 2013 Mikata Hara
*************************************************************************	*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>

/*	MIDI����M�������̃R�[���o�b�N�֐�			*/
/*	HMIDIIN inHandle	;	MIDI��M�̃n���h��	*/
/*	UINT uMsg		: ���b�Z�[�W				*/
/*	DWORD dwParam1	: MIDI Data					*/
/*	DWORD dwParam2	: Time Stamp				*/

HMIDIIN      inHandle;	//MIDI���͂���M���邽�߂�MIDI�f�o�C�X�C���^�[�t�F�C�X

void CALLBACK midiCallback(HMIDIIN handle, UINT uMsg, DWORD dwInstance,
	DWORD dwParam1, DWORD dwParam2)
{
	switch ( uMsg ) {
		case MM_MIM_DATA:	/* MIDI Short Message */
			fprintf(stderr,"-----APPARENTLY THERE IS DATA.-----\n");

		/* MIDI�`�����l�����b�Z�[�W�̏����v���O�����͂����ɋL�ڂ���	*/
			if( dwParam1!=0xF8 && dwParam1!= 0xFE ){
				fprintf(stderr,"dwInstance is %08lx\n", dwInstance);  
				fprintf(stderr,"Handle is %08lx\n", handle);
				fprintf(stderr,"uMsg is %08lx\n", uMsg);
				fprintf(stderr,"dwParam1 is %08x\n",dwParam1);
				fprintf(stderr,"dwParam2 is %08x\n",dwParam2);
	 		}
			break;


		case MM_MIM_OPEN:
			fprintf(stderr,"-----OPENED.-----\n");
			break;

		case MM_MIM_CLOSE:
			fprintf(stderr,"-----EVERYTHING IS CLOSING.-----\n");
			break;
		
		case MM_MIM_LONGDATA:
			fprintf(stderr,"-----LONGDATA'D.-----\n");
			break;

		case MM_MIM_ERROR:
			fprintf(stderr,"-----ERROR.-----\n");
			break;

		case MM_MIM_LONGERROR:
			fprintf(stderr,"-----LONGERROR.  EVEN WORSE.-----\n");
			break;
    }
}

void MidiInInit(){

	MIDIINCAPS     mic;       
	unsigned long result;
	int ckey;	// ���݉�����Ă���L�[�{�[�h�L�[�̃X�g���[�W
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
	result = midiInOpen(&inHandle, iDevNum, (DWORD)midiCallback, 0, CALLBACK_FUNCTION);
     
	if (result)
	{
		fprintf(stderr,"There was an error opening the default MIDI In device!\r\n");
	} else {
		fprintf(stderr,"midiInStart has been called.\n"); 
		midiInStart(inHandle);	//MIDI���͊J�n
	}
}

void MIDIInClose()
{
	midiInStop(inHandle);
//	midiInReset(inHandle);
	midiInClose(inHandle);
 
	fprintf(stderr,"Lines are done twice because midiCallback\n");
	fprintf(stderr,"is called when midiInClose is called...?\n");
	fprintf(stderr,"%d was the MIDIIN handle.\n",inHandle); 
	fprintf(stderr,"Stuff's closed now.\n");     
}

int main(int argc, char *argv[])
{
	MidiInInit();
	while(!kbhit());	//�����L�[���������܂ő�����
	MIDIInClose();
	return EXIT_SUCCESS;
}
