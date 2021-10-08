/* *************************************************************************
	midiouttest.cpp : MIDI出力テスト用のコンソールアプリケーション

	This software is released under the MIT License, see LICENSE.txt.
	Copyright (c) 2013 Mikata Hara
*************************************************************************	*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>

HMIDIOUT outHandle;    // MIDI出力を送信するためのMIDIデバイスインターフェイス

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

int main(int argc, char** argv) {

	int ckey;           // 現在押されているキーボードキー
	int flag;           // エラーフラグ
	int notestate=0;	// 現在ONかOFFか？
	unsigned char notenum;	//Note Number
	unsigned char Note[8]={48,50,52,53,55,57,59,60};
	int i;

	// 送信用メッセージ
	union { unsigned long word; unsigned char data[4]; } message;
	// Note Onを例とすると以下のように設定する
	message.data[0] = 0x90;  // MIDI note-on message (requires to data bytes)
	message.data[1] = 60;    // MIDI note-on message: Key number (60 = middle C)
	message.data[2] = 100;   // MIDI note-on message: Key velocity (100 = loud)
	message.data[3] = 0;     // 使っていない

	if(MidiOutInit()) return EXIT_FAILURE;	// MIDI出力デバイス初期化

	// "q"が押されるまで続ける
	fprintf(stderr,"Press \"1\", \"2\", \"2\", ..., \"8\" to send MIDI\n");
	fprintf(stderr,"Press \"q\" to quit.\n");

	while (1) {
		if (kbhit()) {	// コンピューターのキーボードのキーが押された場合 
			ckey = getch();
			if (ckey == 'q') break;
			if(ckey<'1' || ckey>'8') continue;
			notenum = Note[ckey-'1'];

			if (notestate == 0) {
			// ノートは現在オフ。ノートをオンにする。
				message.data[2] = 100;	//velocity=100
				message.data[1] = notenum;
				notestate = 1;
				printf("Note turned ON.\r");
			} else {
			//  ノートは現在オン。ノートをオフにする。
				message.data[2] = 0;  //velocity = note off
				message.data[1] = notenum;
				notestate = 0;
				printf("Note turned OFF.\r");
			}

		//MIDIデータ送信
			flag = midiOutShortMsg(outHandle, message.word);
			if (flag != MMSYSERR_NOERROR) {
				printf("Warning: MIDI Output is not open.\n");
				break;
			}
		}
	}

	// 出力デバイスをリセットする
 	midiOutReset(outHandle);

	// MIDIデバイスのデータをすべて削除し、MIDI出力ポートを閉じる
	midiOutClose(outHandle);

	return EXIT_SUCCESS;
}
