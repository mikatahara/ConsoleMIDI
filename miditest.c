/* *************************************************************************
	miditest.cpp : MIDI入力テスト用のコンソールアプリケーション

	This software is released under the MIT License, see LICENSE.txt.
	Copyright (c) 2013 Mikata Hara

	> gcc -o miditest miditest.c midisub.c c:\MinGW\Lib\libwinmm.a
*************************************************************************	*/

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>
#include "midisub.h"

// MIDI 受信コールバック関数
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


int main(int argc, char** argv) {

	int ckey;           // 現在押されているキーボードキー
	int notestate=0;	// 現在ONかOFFか？
	unsigned char notenum;	//Note Number
	unsigned char Note[8]={48,50,52,53,55,57,59,60};
	int i;

	if(MidiOutInit()) return EXIT_FAILURE;	// MIDI出力デバイス初期化
	if(MidiInInit((DWORD)midiCallback)) return EXIT_FAILURE;	// MIDI入力デバイス初期化

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
				MIDIOut3byte(0x90,notenum,100);
				notestate = 1;
				printf("Note turned ON.\r");
			} else {
			//  ノートは現在オン。ノートをオフにする。
				MIDIOut3byte(0x80,notenum,0);
				notestate = 0;
				printf("Note turned OFF.\r");
			}
		}
	}

	MIDIInClose();
	MIDIOutClose();

	return EXIT_SUCCESS;
}
