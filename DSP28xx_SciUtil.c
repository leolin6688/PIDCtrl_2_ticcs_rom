/*
 * File: DSP28xx_SciUtil.c
 *
 * Code generated for Simulink model 'PIDCtrl_2'.
 *
 * Model version                  : 1.208
 * Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
 * C/C++ source code generated on : Sat Jan 23 09:45:26 2016
 *
 * Target selection: idelink_ert.tlc
 * Embedded hardware selection: Texas Instruments->C2000
 * Code generation objectives:
 *    1. ROM efficiency
 *    2. RAM efficiency
 *    3. Execution efficiency
 *    4. Debugging
 *    5. Safety precaution
 * Validation result: Not run
 */

#include "DSP28xx_SciUtil.h"

/*
 * Receive character(s) from the SCIb
 * Received character(s) will be write to rcvBuff.
 * Return 0 if characters are received with no error.
 * Return 1 if waiting timeout.
 * Return 2 if data error.(receiving timout or checksum error)
 * Return 3 if a parity error occured.
 * Return 4 if a frame error occured.
 */
int scib_rcv(char *rcvBuff, int buffLen, int loopMode)
{
  int i;
  unsigned int cnt = 0;
  unsigned int maxcnt;
  if (loopMode == LONGLOOP) {
    maxcnt = RCVMAXCNTL;
  } else {
    maxcnt = RCVMAXCNTS;
  }

  for (i = 0; i<buffLen; i++) {
    cnt = 0;
    while (ScibRegs.SCIFFRX.bit.RXFFST == 0) {/* wait until data received */
      if (i == 0) {
        if (cnt++ > maxcnt)
          return TIMEOUT;
      } else {
        if (cnt++ > RCVMAXCNTL)
          return TIMEOUT;
      }
    }

    rcvBuff[i] = ScibRegs.SCIRXBUF.all;
    if (ScibRegs.SCIFFRX.bit.RXFFOVF == 1)/* detect FIFO overflow*/
    {
      ScibRegs.SCIFFRX.bit.RXFFOVRCLR = 1;

      /*ScibRegs.SCIFFRX.bit.RXFIFORESET = 0;    Reset the FIFO pointer to zero.
       * ScibRegs.SCIFFRX.bit.RXFIFORESET = 1;   Re-enable receive FIFO operation.
       */
    }

    if (ScibRegs.SCIRXST.bit.FE)
      return FRAMERR;
    if (ScibRegs.SCIRXST.bit.PE)
      return PRTYERR;
    if ((ScibRegs.SCIRXST.bit.RXWAKE ==1) && (ScibRegs.SCIRXST.bit.RXERROR == 1)
        ) {
      ScibRegs.SCICTL1.bit.SWRESET = 1;
      ScibRegs.SCICTL1.bit.SWRESET = 0;
      ScibRegs.SCICTL1.bit.SWRESET = 1;
    }
  }

  return NOERROR;
}

int byteswap_L8cmp(unsigned int* outdata, char* recdata, int inportWidth, int
                   typeLen)
{
  int i, j;
  int numWrd = (inportWidth * typeLen)/2;/* number of words (16 bit length) to receive*/

  /* Little Endian, 8bit swap */
  for (i = 0; i < numWrd; i++) {
    outdata[i] = 0;
    for (j = 0; j<2; j++) {
      outdata[i] += recdata[i*2+j] <<(8*j);
    }
  }

  return 0;
}

/* Transmit character(s) from the SCIc*/
void scic_xmit(char* pmsg, int msglen)
{
  int i;
  for (i = 0; i < msglen; i++) {
    while (ScicRegs.SCIFFTX.bit.TXFFST == 16) {
    }                                  /* The buffer is full;*/

    ScicRegs.SCITXBUF= pmsg[i];
  }

  while (ScicRegs.SCIFFTX.bit.TXFFST != 0) {
  }
}

int byteswap_L8exp(char* outdata, char* inport ,int inportWidth, int typeLen)
{
  int i,j;
  int k = 0;
  int numWrd = (inportWidth * typeLen)/2;/* number of words (16 bit length) to send*/

  /* Little Endian, 8bit swap */
  for (i = 0; i < numWrd; i++) {
    for (j = 0; j < 2; j++) {
      outdata[k++] = inport[i] >>(8*j);
    }
  }

  return 0;
}

/*
 * Receive character(s) from the SCIc
 * Received character(s) will be write to rcvBuff.
 * Return 0 if characters are received with no error.
 * Return 1 if waiting timeout.
 * Return 2 if data error.(receiving timout or checksum error)
 * Return 3 if a parity error occured.
 * Return 4 if a frame error occured.
 */
int scic_rcv(char *rcvBuff, int buffLen, int loopMode)
{
  int i;
  unsigned int cnt = 0;
  unsigned int maxcnt;
  if (loopMode == LONGLOOP) {
    maxcnt = RCVMAXCNTL;
  } else {
    maxcnt = RCVMAXCNTS;
  }

  for (i = 0; i<buffLen; i++) {
    cnt = 0;
    while (ScicRegs.SCIFFRX.bit.RXFFST == 0) {/* wait until data received */
      if (i == 0) {
        if (cnt++ > maxcnt)
          return TIMEOUT;
      } else {
        if (cnt++ > RCVMAXCNTL)
          return TIMEOUT;
      }
    }

    rcvBuff[i] = ScicRegs.SCIRXBUF.all;
    if (ScicRegs.SCIFFRX.bit.RXFFOVF == 1)/* detect FIFO overflow*/
    {
      ScicRegs.SCIFFRX.bit.RXFFOVRCLR = 1;

      /*ScicRegs.SCIFFRX.bit.RXFIFORESET = 0;    Reset the FIFO pointer to zero.
       * ScicRegs.SCIFFRX.bit.RXFIFORESET = 1;   Re-enable receive FIFO operation.
       */
    }

    if (ScicRegs.SCIRXST.bit.FE)
      return FRAMERR;
    if (ScicRegs.SCIRXST.bit.PE)
      return PRTYERR;
    if ((ScicRegs.SCIRXST.bit.RXWAKE ==1) && (ScicRegs.SCIRXST.bit.RXERROR == 1)
        ) {
      ScicRegs.SCICTL1.bit.SWRESET = 1;
      ScicRegs.SCICTL1.bit.SWRESET = 0;
      ScicRegs.SCICTL1.bit.SWRESET = 1;
    }
  }

  return NOERROR;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
