/*
 * File: PIDCtrl_2_main.c
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

#include "PIDCtrl_2.h"
#include "rtwtypes.h"
#include "PIDCtrl_2_private.h"
#include "c2000_main.h"
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include <stdlib.h>
#include <stdio.h>

void init_board(void);
void enable_interrupts(void);
void config_schedulerTimer(void);
void disable_interrupts(void);
volatile int IsrOverrun = 0;
static boolean_T OverrunFlag = 0;

/* Function: rt_OneStep -------------------------------------------
 *
 * Abstract:
 *      Perform one step of the model.  Single-tasking implementation.
 */
void rt_OneStep(void)
{
  /* Check for overrun. Protect OverrunFlag against
   * preemption.
   */
  if (OverrunFlag++) {
    IsrOverrun = 1;
    OverrunFlag--;
    return;
  }

  asm(" SETC INTM");
  PieCtrlRegs.PIEIER1.all |= (1 << 6);
  asm(" CLRC INTM");
  PIDCtrl_2_step();

  /* Get model outputs here */
  asm(" SETC INTM");
  PieCtrlRegs.PIEIER1.all &= ~(1 << 6);
  asm(" RPT #5 || NOP");
  IFR &= 0xFFFE;
  PieCtrlRegs.PIEACK.all = 0x1;
  asm(" CLRC INTM");
  OverrunFlag--;
}

/* Function: main -------------------------------------------
 *
 * Abstract:
 *      Entry point into the code.
 */
void main(void)
{
  volatile boolean_T noErr;
  init_board();
  ((void) 0);
  PIDCtrl_2_initialize();
  config_schedulerTimer();
  noErr =
    ((void*) 0) == (NULL);
  enable_interrupts();
  while (noErr ) {
    noErr =
      ((void*) 0) == (NULL);
  }

  /* Disable rt_OneStep() here */
  disable_interrupts();
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
