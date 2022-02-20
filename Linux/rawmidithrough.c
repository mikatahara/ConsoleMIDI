//
// Description:	  Read ALSA Raw MIDI input using threads.
//

#include <alsa/asoundlib.h> /* for alsa interface   */
#include <unistd.h>			/* for sleep() function */
#include <sys/types.h>
#include <sys/select.h>
#include <sys/stat.h>

// function declarations:
//void errormessage(const char *format, ...);
void midiportfunction();
void midi_anlz(unsigned char status, unsigned char data1, unsigned char data2);

static unsigned char mStatus = 0, mData1 = 0, mData2 = 0;
static unsigned char mSysEx[1024];
static int mIcount = 0;
static int mSysexflag = 0;
static int mChannel = 0;

int npfds;
struct pollfd *pfds;
struct pollfd qfds;
snd_rawmidi_t *midiinport = NULL;
snd_rawmidi_t *midioutport = NULL;

int main()
{
	int status;
	int mode = SND_RAWMIDI_SYNC;
	const char *inportname = "hw:2,0,0";
	const char *outportname = "hw:3,0,0";

	if ((status = snd_rawmidi_open(&midiinport, NULL, inportname, mode)) < 0)
	{
		fprintf(stderr, "Problem opening MIDI input: %s", snd_strerror(status));
		exit(1);
	}
	if ((status = snd_rawmidi_open(NULL, &midioutport, outportname, mode)) < 0)
	{
		fprintf(stderr, "Problem opening MIDI input: %s", snd_strerror(status));
		exit(1);
	}

	snd_rawmidi_nonblock(midiinport, 1);
	npfds = snd_rawmidi_poll_descriptors_count(midiinport);
	pfds = (struct pollfd *)alloca(npfds * sizeof(struct pollfd));
	status = snd_rawmidi_poll_descriptors(midiinport, pfds, POLLIN);

	fprintf(stderr, "poll_descriptors_count=%d\n", npfds);
	fprintf(stderr, "poll_descriptors=%d\n", status);

	while (1)
	{
		midiportfunction();
		//		fprintf(stderr,"**");
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////

//////////////////////////////
//
// midiportfunction -- Thread function which waits around until a MIDI
//	  input byte arrives and then it prints the byte to the terminal.
//	  This thread function does not end gracefully when the program
//	  stops.

// void *midiportfunction(void *arg) {
void midiportfunction()
{

	unsigned char buffer[16];
	unsigned short reevents;
	int err, i;

	while (1)
	{

		/* ポーリング記述子から返されたイベントを取得します */
		/* 成功した場合はゼロ、負の場合はエラーコード */
		err = snd_rawmidi_poll_descriptors_revents(midiinport, &qfds, npfds, &reevents);
		if (err < 0)
		{
			fprintf(stderr, "polling error =%d\n", err);
			exit(1);
		}

		/* MIDIストリームからMIDIバイトを読み取る */
		/* 成功した場合はゼロ、負の場合はエラーコード */
		if ((err = snd_rawmidi_read(midiinport, buffer, 1)) < 0)
		{
			return;
		}
		if (buffer[0] & 0x80)
		{
			mStatus = buffer[0];
			mIcount = 0;
			if (mStatus == 0xF0)
			{
				mSysexflag = 1;
				mSysEx[mIcount] = mStatus;
				mIcount++;
			}
			else if (mStatus == 0xF7)
			{
				mSysEx[mIcount] = mStatus;
				mIcount++;
				for (i = 0; i < mIcount; i++)
				{
					fprintf(stderr, "%02x ", mSysEx[i]);
				}
				fprintf(stderr, "\n");
				mSysexflag = 0;
				mIcount = 0;
			}
		} else {
			if (mSysexflag)
			{
				mSysEx[mIcount] = buffer[0];
				mIcount++;
			}
			else
				switch (mStatus & 0xF0)
				{
				case 0x80: // note off
				case 0x90: // note on
				case 0xB0: // cc
				case 0xE0: // bend
				case 0xA0: // poly after
					if (mIcount == 0)
					{
						mData1 = buffer[0];
						mIcount++;
					}
					else if (mIcount == 1)
					{
						mData2 = buffer[0];
						mIcount = 0;
						midi_anlz(mStatus, mData1, mData2);
					}
					break;
				case 0xC0: // program change
				case 0xD0: // pressur
					if (mIcount == 0)
					{
						mData1 = buffer[0];
						mIcount = 0;
						snd_rawmidi_close(midiinport);
						exit(1);
					}
					break;
				}
		}
		fflush(stdout);
	}
	return;
}

void midi_anlz(unsigned char status, unsigned char data1, unsigned char data2)
{
	fprintf(stderr, "%02x %02x %02x\n", status, data1, data2);
	unsigned char buffer[3]={status,data1,data2};	

	switch (mStatus & 0xF0)
	{
		case 0x80: // note off
		case 0x90: // note on
		case 0xB0: // cc
		case 0xE0: // bend
		case 0xA0: // poly after
			snd_rawmidi_write(midioutport,buffer,3);
			snd_rawmidi_drain(midioutport);
			break;
		case 0xC0: // program change
		case 0xD0: // pressur
			snd_rawmidi_write(midioutport,buffer,2);
			snd_rawmidi_drain(midioutport);
			break;
	}
}
