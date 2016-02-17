/*
 * File: DSP28xx_SciUtil.h
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

#ifndef RTW_HEADER_DSP28xx_SciUtil_h_
#define RTW_HEADER_DSP28xx_SciUtil_h_
#include "DSP2833x_Device.h"
#include <string.h>
#define NOERROR                        0                         /* no error*/
#define TIMEOUT                        1                         /* waiting timeout*/
#define DATAERR                        2                         /* data error (checksum error)*/
#define PRTYERR                        3                         /* parity error*/
#define FRAMERR                        4                         /* frame error*/
#define RCVMAXRETRY                    10
#define RCVMAXCNTS                     1000
#define RCVMAXCNTL                     50000
#define SHORTLOOP                      0
#define LONGLOOP                       1

int scib_rcv(char *rcvBuff, int buffLen, int loopMode);
int byteswap_L8cmp(unsigned int* outdata, char* recdata, int inportWidth, int
                   typeLen);
void scic_xmit(char* pmsg, int msglen);
int byteswap_L8exp(char* outdata, char* inport ,int inportWidth, int typeLen);
int scic_rcv(char *rcvBuff, int buffLen, int loopMode);

#endif                                 /* RTW_HEADER_DSP28xx_SciUtil_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
