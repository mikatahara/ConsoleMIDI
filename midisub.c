/* *************************************************************************
	midisub.cpp : MIDI入出力サブルーチン

	This software is released under the MIT License, see LICENSE.txt.
	Copyright (c) 2013 Mikata Hara
*************************************************************************	*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>

HMIDIIN		inHandle;	//MIDI入力を受信するためのMIDIデバイスインターフェイス
HMIDIOUT	outHandle;	// MIDI出力を送信するためのMIDIデバイスインターフェイス
union { unsigned long word; unsigned char data[4]; } message;	// 送信用メッセージ

// 入力デバイスの選択と初期化
int MidiInInit(DWORD midiCallback){

	MIDIINCAPS     mic;       
	unsigned long result;
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
//	result = midiInOpen(&inHandle, iDevNum, (DWORD)midiCallback, 0, CALLBACK_FUNCTION);
	result = midiInOpen(&inHandle, iDevNum, midiCallback, 0, CALLBACK_FUNCTION);
     
	if (result)
	{
		fprintf(stderr,"There was an error opening the default MIDI In device!\r\n");
		return 1;
	} else {
		fprintf(stderr,"midiInStart has been called.\n"); 
		midiInStart(inHandle);	//MIDI入力開始
	}
	return 0;
}

// 入力デバイスのクローズ
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

// 出力デバイスの選択と初期化
int MidiOutInit(){

	MIDIOUTCAPS     mic;       
	int flag;		// 関数の戻り値 
	int iNumDevs = midiOutGetNumDevs();  //MIDI出力デバイスの数
	unsigned int iDevNum=0;

	/* 出力デバイスの名前を表示する  */
	for (int i = 0; i < iNumDevs; i++)
	{
        /* 次のデバイスに関する情報を取得する */
		if (!midiOutGetDevCapsA(i, &mic, sizeof(MIDIINCAPS)))
		{
            /* デバイスIDと名前を表示します */
			fprintf(stderr,"outHandle ID #%u: %s\r\n", i, mic.szPname);
		}
	}


	/* 出力デバイスの選択 */
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

// 出力デバイスのクローズ
void MIDIOutClose()
{

	// 出力デバイスをリセットする
 	midiOutReset(outHandle);
	// MIDIデバイスのデータをすべて削除し、MIDI出力ポートを閉じる
	midiOutClose(outHandle);
}

// 3byte MIDI の出力
int MIDIOut3byte(unsigned char status,unsigned char byte1,unsigned char byte2)
{
	int flag;		// 関数の戻り値 

	message.data[0] = status;	// MIDI status
	message.data[1] = byte1;	// MIDI data1
	message.data[2] = byte2;	// MIDI data2
	message.data[3] = 0;		// 使っていない

	//MIDIデータ送信
	flag = midiOutShortMsg(outHandle, message.word);
	if (flag != MMSYSERR_NOERROR) {
		fprintf(stderr,"Warning: MIDI Output is not open.\n");
			return 1;
	}
	return 0;
}

