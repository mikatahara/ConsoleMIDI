/* *************************************************************************
	midiintest.cpp : MIDI入力テスト用のコンソールアプリケーション

	This software is released under the MIT License, see LICENSE.txt.
	Copyright (c) 2013 Mikata Hara
*************************************************************************	*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>

/*	MIDIを受信した時のコールバック関数			*/
/*	HMIDIIN inHandle	;	MIDI受信のハンドル	*/
/*	UINT uMsg		: メッセージ				*/
/*	DWORD dwParam1	: MIDI Data					*/
/*	DWORD dwParam2	: Time Stamp				*/

HMIDIIN      inHandle;	//MIDI入力を受信するためのMIDIデバイスインターフェイス

void CALLBACK midiCallback(HMIDIIN handle, UINT uMsg, DWORD dwInstance,
	DWORD dwParam1, DWORD dwParam2)
{
	switch ( uMsg ) {
		case MM_MIM_DATA:	/* MIDI Short Message */
			fprintf(stderr,"-----APPARENTLY THERE IS DATA.-----\n");

		/* MIDIチャンネルメッセージの処理プログラムはここに記載する	*/
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
	int ckey;	// 現在押されているキーボードキーのストレージ
	unsigned int iNumDevs= midiInGetNumDevs();  // MIDI入力デバイスの数
	unsigned int iDevNum=0; 

    /* 入力デバイスの名前を表示する */
	for (int i = 0; i < iNumDevs; i++)
	{
		/* 次のデバイスに関する情報を取得する */
		if (!midiInGetDevCaps(i, &mic, sizeof(MIDIINCAPS)))
		{
			/* デバイスIDと名前を表示します */
			fprintf(stderr,"Device ID #%u: %s\r\n", i, mic.szPname);
		}
	}
 
	/* 入力デバイスの選択 */
	fprintf(stderr,"These are the only available devices...?\n"); 
	fprintf(stderr,"Device No.=? ");
	scanf("%u",&iDevNum);
 
    /* 入力デバイスを開く 		*/
	/* コールバック関数を登録 	*/
	result = midiInOpen(&inHandle, iDevNum, (DWORD)midiCallback, 0, CALLBACK_FUNCTION);
     
	if (result)
	{
		fprintf(stderr,"There was an error opening the default MIDI In device!\r\n");
	} else {
		fprintf(stderr,"midiInStart has been called.\n"); 
		midiInStart(inHandle);	//MIDI入力開始
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
	while(!kbhit());	//何かキーが押されるまで続ける
	MIDIInClose();
	return EXIT_SUCCESS;
}
