/*
name is cmu800.c

Copyright (c) 2024 Windy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "types.h"


/*
	CMU-800 ��I/O �|�[�g

�@�@���@��W�J����Ă����֌W��A�W�����p�[�؂�ւ��ŁAI/O�|�[�g�̊J�n��ݒ�ł����悤�ł��B
�@�@P6�ł́A10h ����n�܂�܂��B
�@�@���@��Ŏg���ꍇ�́A�K���A��������������肵�Ă��������B


�@�@8253(1)
10h  �`�����l���P�@�i�����f�B�j�̉��K�ݒ�
11h  �`�����l���Q�@�i�x�[�X�j�̉��K�ݒ�
12h  �`�����l���R�@�i�R�[�h�P�j�̉��K�ݒ�
13h�@8253(1)�̐ݒ�

�@�@8253(2)
14h  �`�����l���S�@�i�R�[�h�Q�j�̉��K�ݒ�
15h  �`�����l���T�@�i�R�[�h�R�j�̉��K�ݒ�
16h  �`�����l���U�@�i�R�[�h�S�j�̉��K�ݒ�
17h�@8253(2)�̐ݒ�

	8255
18h  CV-OUT, TUNE-GATE ,KEY ON/OFF �̐ݒ�
19h  �������Y���̔���
1Ah	 �`�����l���Z���N�g�A�ݒ芮��
1Bh  8255�̐ݒ�


	�`�����l���P�̖炵���@�̑傫�ȗ���

1. 18H: KEY ON �ɂ��� 
2. 1AH: LSB��1�ɂ��� �@�@(b3-b1���O�ɂ���i�`�����l���P�j)
3. 1AH: LSB��0�ɂ��Đݒ蔽�f���[�h�ɂ��� �@�@(b3-b1���O�ɂ���i�`�����l���P�j)
4. 10H: �`�����l���P�̎��g�����o�͂���(���ʁj
5. 10H: �`�����l���P�̎��g�����o�͂���(��ʁj
6. 1AH: LSB��1�ɂ��Ĕ��f���[�h�I�� �@�@(b3-b1���O�ɂ���i�`�����l���P�j)

7. ���̒����ɂ��A�K�v�ȉ񐔁A1-6�܂ŌJ��Ԃ����s����

18H �ŏI�I�ɁAKEY OFF����


����

	���̍����͐ݒ�ł��邪�A���̒����͐ݒ�ł��Ȃ��̂��s�v�c���������A
	�ǂ����A�����݂ɐ�؂肳�ꂽ�����Ȃ��Ă������ƂŁA���̒������o���炵��
	���̒����������ƁA�Z�����Ɣ�ׂāA�����́AKEY ON �̐ݒ�ƁA���g���̐ݒ肪�s����


KEY ON/OFF�̐ݒ�

	18H: KEY ON�̂Ƃ��́AMSB��0�ɂ���@OFF�̂Ƃ���1�ɂ���
	���́A���g���̐ݒ�ƁA�Z�b�g�Őݒ肪�s����悤���B

���g���̐ݒ�

	�`�����l���ɂ���āA����I/O�|�[�g���قȂ��Ă���B
	�`�����l��1:10H �`�����l��2:11H  �`�����l��3:12H  �`�����l��4:14H  �`�����l��5:15H  �`�����l��6:16H
	cmu800_freq[�I�N�^�[�u][����]�ɓ����Ă���f�[�^�����ʁA��ʂ̏��Ԃɏo�͂��܂��B
	���t�v���O����������A���ĂȂ��`�����l���́A���g���f�[�^�Ƃ��āA0001H���n�����B�i�Ӗ��Ȃ��f�[�^�H�j

�ݒ�̔��f

	1AH��LSB��1��0�ɂ��邱�ƂŁA�ݒ�𔽉f�����郂�[�h�ɂȂ�炵��
	1AH��b3-b1���A�`�����l���ԍ�-1 �ł��B�I�������ALSB���A1�ɖ߂��܂��B

�`�����l���P����U�܂Ŏ��g����ݒ肵����A���Y���̔������������炵��


�e���|�̑���

	1AH ��b7-b4 �����͂ɂȂ��Ă��āA�e���|�ɍ��킹�āA0000��1111�Ɏ����I�ɕω����܂�
	�����炭�ACMU-800 �̃e���|�܂݂��񂷂��ƂŁA�ݒ�ł���Ǝv���܂��B
�@�@���t�v���O�������ł�����݂Ă��āA�E�G�C�g������悤��

*/

byte port18 = 0x00;				//I/O [18]  CMU-800
byte port19 = 0x00;				//I/O [19]  CMU-800
byte port1A = 0x00;				//I/O [1A]  CMU-800
int  cmu800_isKeyOn;			//�ݒ蒆�̃L�[�̏�ԁ@TRUE:key on FALSE:key off
//int  cmu800_freq;				//�ݒ蒆�̎��g���f�[�^

// CMU-800 �̎��g�� [�I�N�^�[�u][���K]
word cmu800_freqTable[][12] ={
	{0x9741,0x8EBE, 0x86BB, 0x7F2E, 0x780B, 0x714E, 0x6AED, 0x64EC, 0x5F41, 0x59E8, 0x54D9, 0x5015},
	{0x4B95,0x4754, 0x4353, 0x3F8D, 0x3BFC, 0x389E, 0x356E, 0x326E, 0x2F99, 0x2CED, 0x2A66, 0x2805},
	{0x25C5,0x23A5, 0x21A5, 0x1FC3, 0x1DFB, 0x1C4C, 0x1AB4, 0x1934, 0x17CA, 0x1674, 0x1531, 0x1401},
	{0x12E1,0x11D1, 0x10D1, 0x0FE1, 0x0EFD, 0x0E26, 0x0D59, 0x0C99, 0x0BE4, 0x0B39, 0x0A98, 0x0A00},
	{0x0970,0x08E8, 0x0868, 0x07F0, 0x077E, 0x0712, 0x06AC, 0x064C, 0x05F2, 0x059C, 0x054C, 0x0500},
	{0x04B8,0x0474, 0x0434, 0x03F8, 0x03BF, 0x0389, 0x0356, 0x0326, 0x02F9, 0x02CE, 0x02A6, 0x0280},
	{0x025C,0x023A, 0x021A, 0x01FC, 0x01DF, 0x01C4, 0x01AB, 0x0193, 0x017C, 0x0167, 0x0153, 0x0140},
	{0x012E,0x011D, 0x010D, 0x00FE, 0x00F0, 0x00E2, 0x00D5, 0x00C9, 0x00BE, 0x00B3, 0x00A9, 0x00A0},
	{0x0097,0x008E, 0x0086, 0x007F, 0x0078, 0x0071, 0x006A, 0x0064, 0x005F, 0x0059, 0x0054, 0x0050},
	{0x004B,0x0047, 0x0043, 0x003F, 0x003C, },
};

// PSG�����̎��g��
word psg_freq[8][12] = {
	{0xee8, 0x0e12, 0x0d48 ,0x0c88 ,0x0bd4 ,0x0b2a ,0x0a8a ,0x09f2 ,0x964 , 0x8dc , 0x085e , 0x07e6}, 
};

// ����
char *toneName[] = {"C","C#","D","D#","E","F","F#","G","G#","A","A#","B"};

// ******************************************************
//		CMU800_init
//		������
// ******************************************************

void CMU800_init(void)
{
	// PSG�����̎��g���́A�P�I�N�^�[�u�ڂ����Z�b�g����Ă���B
	// �I�N�^�[�u�������邲�ƂɁA�����ɂȂ�悤�ɁA�����ݒ�

	for (int octave = 0; octave < 7; octave++) {
		for (int tone = 0; tone < 12; tone++) {
			psg_freq[octave+1][tone] = psg_freq[octave][tone]/2;
		}
	}
}

// ******************************************************
//			CMU800_DoOut
//		����: Port: �|�[�g�ԍ�
//			  Value:�l
// ******************************************************
void CMU800_DoOut(byte Port, byte Value)
{
	switch( Port) {
	// ---------- ���g���̐ݒ� ----------
	// ���ʁ���ʂ̏��ԂɁA�A�����ďo�͂���
	  case 0x10:
	  case 0x11:
	  case 0x12:
	  case 0x14:
	  case 0x15:
	  case 0x16:

		{
		int channel;
		switch (Port) {
			case 0x10: channel = 0; break;
			case 0x11: channel = 1; break;
			case 0x12: channel = 2; break;
			case 0x14: channel = 3; break;
			case 0x15: channel = 4; break;
			case 0x16: channel = 5; break;
		}
		static byte preValue;
		static flag = 0;
		PRINTDEBUG2(DEV_LOG, "OUT &H%02X,&H%02X:  ", Port, Value);
		if (flag == 0) {
			preValue = Value;
			flag++;
			PRINTDEBUG(DEV_LOG, "(���ʃf�[�^)\n");

		}else {
			flag = 0;
			PRINTDEBUG(DEV_LOG, "(��ʃf�[�^)");
			int octave=0,toneNo=0;
			if (cmu800_isKeyOn ) {
				int v = Value * 256 + preValue;
				if (cnv(v, &octave, &toneNo)) {
					PRINTDEBUG2(DEV_LOG, "(octave=%d toneName=%s)\n", octave, toneName[toneNo]);
					PSGOut(channel * 2 + 0, psg_freq[octave][toneNo] & 0xff);			// *** TEST CODE ****
					PSGOut(channel * 2 + 1, ((psg_freq[octave][toneNo] & 0xff00) >> 8) & 0xff);	// *** TEST CODE ****
					PSGOut(7, 0x38);
					PSGOut(channel + 8, 14);
				}else {
					PRINTDEBUG(DEV_LOG, "(NO DATA)\n");
					PSGOut(channel + 8, 0);
				}
			}else {
				PSGOut(channel + 8, 0);
			}
		}
		break;

	// -------- CV-OUT TUNE-GATE KEY ON/OFF ---------------
	//     b0-b5: CV-OUT �f�[�^�@(�O���[�q�ɏo�͂����)
	//     b6:    TUNE-GATE
	//     b7:    GATE-DATA 0�Ŕ��� 1�ŏ���
	  case 0x18:
			port18 = Value;
			cmu800_isKeyOn = (port18 & 0x80) ? FALSE : TRUE;
			PRINTDEBUG2(DEV_LOG, "OUT &H%02X,&h%02X  ", Port, Value);
			PRINTDEBUG3(DEV_LOG, "(CV-OUT:%X TUNE-GATE:%X GATE-DATA:%X)\n",Value & 0x1F,(Value>>6)&1,cmu800_isKeyOn);
			break;

      // ------- ���Y������ --------
	  // 0�ɂȂ����Ƃ��ɔ�������@������P�ɖ߂����
	  case 0x19:
			port19 = Value;					// TO FIX: �����������Ă܂���
			PRINTDEBUG2(DEV_LOG, "OUT &H%02X,&h%02X  ���Y��\n", Port, Value);
			break;

	  // -------- �`�����l���̐ݒ�𔽉f���� ---------------
	  // b0: 1->0 �ɂȂ����Ƃ���bit 1-3 �̃`�����l���̐ݒ�𔽉f������B���̌�P�ɖ߂��Ȃ��Ƃ����Ȃ�
	  // b1-3: �ݒ肷��`�����l���@0����V
	  // �i�G�~�����[�^�ł́A���g�����o�͂��ꂽ���ɁAPSG�̎��g����ς��Ă݂�j
	  case 0x1A:
			{
			int channel = (Value >>1)& 7;
			//if( channel >2) break;			// TO FIX: �Ƃ肠�����R�`�����l�������i����PSG�����Ŗ炷�̂�
			port1A = Value;
			PRINTDEBUG2(DEV_LOG, "OUT &H%02X,&H%02X  ", Port, Value);
			PRINTDEBUG1(DEV_LOG, "(Channel= %02X", channel);
			if ((Value & 1) == 0) {
				PRINTDEBUG(DEV_LOG, "���f\n");
			}else {
				PRINTDEBUG(DEV_LOG, "���ɖ߂�\n");
			}
			}
			break;
	  // --------------- 8255 �R���g���[���f�[�^ -----------
	  case 0x1B:
			PRINTDEBUG2(DEV_LOG, "OUT &H%02X,&h%02X (8255 Control word)\n", Port, Value);
			break;
		}
	}
}

// ******************************************************
//       CMU800_DoIn
//      ����: Port  �|�[�g�ԍ�
//            Value �l
// ******************************************************
void CMU800_DoIn(byte Port,byte *Value)
{
	switch( Port) {
	// -------  ------
	// ���4bit ���́F�e���|�ɂ����1111 or 0000 �ɂȂ�
		case 0x1A:
		{
			static int cnt=0;
			static byte tmp = 0xF0;
			cnt++;
			if(( cnt % 200)==0) {				// TO FIX:CMU-800�̂܂݂̃e���|�w��ɂ���āA�ς��^�C�~���O��ς���
				tmp = (tmp == 0xF0) ? 0 : 0xF0;
				*Value = tmp;
				cnt=0;
				}
			break;
		}
	}
}

// ******************************************************
//     CMU-800�ɓn�������g���f�[�^����A�I�N�^�[�u�ƁA�����ɕϊ�����
//			����:Value: ���g���f�[�^
//				 octave:�I�N�^�[�u
//				 toneNo:�g�[���ԍ��i�h���~�t�@�\���V�h�j
// ******************************************************
static int cnv(word Value , int *octave , int *toneNo)
{
	int found = FALSE;
	for (int oct = 0; oct < 10; oct++) {
		for (int no = 0; no < 12; no++) {
			if (oct==9 && no==5) goto exit;	// DATA�̏I�[�ɒB�����甲����
			if (Value == cmu800_freqTable[oct][no]) {
				found =TRUE;
				*octave = oct;
				*toneNo = no;
				goto exit;
			}
		}
	}
exit:
	return found;
}
