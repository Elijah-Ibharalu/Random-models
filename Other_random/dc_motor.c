/*
 * dc_motor.c
 *
 * Code generation for model "dc_motor".
 *
 * Model version              : 1.2
 * Simulink Coder version : 24.1 (R2024a) 19-Nov-2023
 * C source code generated on : Sun Mar  8 14:51:23 2026
 *
 * Target selection: grt.tlc
 * Note: GRT includes extra infrastructure and instrumentation for prototyping
 * Embedded hardware selection: Intel->x86-64 (Windows64)
 * Code generation objectives: Unspecified
 * Validation result: Not run
 */

#include "dc_motor.h"
#include "rtwtypes.h"
#include "dc_motor_private.h"
#include "rt_nonfinite.h"

/* Block signals (default storage) */
B_dc_motor_T dc_motor_B;

/* Continuous states */
X_dc_motor_T dc_motor_X;

/* Disabled State Vector */
XDis_dc_motor_T dc_motor_XDis;

/* Real-time model */
static RT_MODEL_dc_motor_T dc_motor_M_;
RT_MODEL_dc_motor_T *const dc_motor_M = &dc_motor_M_;

/*
 * This function updates continuous states using the ODE3 fixed-step
 * solver algorithm
 */
static void rt_ertODEUpdateContinuousStates(RTWSolverInfo *si )
{
  /* Solver Matrices */
  static const real_T rt_ODE3_A[3] = {
    1.0/2.0, 3.0/4.0, 1.0
  };

  static const real_T rt_ODE3_B[3][3] = {
    { 1.0/2.0, 0.0, 0.0 },

    { 0.0, 3.0/4.0, 0.0 },

    { 2.0/9.0, 1.0/3.0, 4.0/9.0 }
  };

  time_T t = rtsiGetT(si);
  time_T tnew = rtsiGetSolverStopTime(si);
  time_T h = rtsiGetStepSize(si);
  real_T *x = rtsiGetContStates(si);
  ODE3_IntgData *id = (ODE3_IntgData *)rtsiGetSolverData(si);
  real_T *y = id->y;
  real_T *f0 = id->f[0];
  real_T *f1 = id->f[1];
  real_T *f2 = id->f[2];
  real_T hB[3];
  int_T i;
  int_T nXc = 3;
  rtsiSetSimTimeStep(si,MINOR_TIME_STEP);

  /* Save the state values at time t in y, we'll use x as ynew. */
  (void) memcpy(y, x,
                (uint_T)nXc*sizeof(real_T));

  /* Assumes that rtsiSetT and ModelOutputs are up-to-date */
  /* f0 = f(t,y) */
  rtsiSetdX(si, f0);
  dc_motor_derivatives();

  /* f(:,2) = feval(odefile, t + hA(1), y + f*hB(:,1), args(:)(*)); */
  hB[0] = h * rt_ODE3_B[0][0];
  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[0]);
  rtsiSetdX(si, f1);
  dc_motor_step();
  dc_motor_derivatives();

  /* f(:,3) = feval(odefile, t + hA(2), y + f*hB(:,2), args(:)(*)); */
  for (i = 0; i <= 1; i++) {
    hB[i] = h * rt_ODE3_B[1][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1]);
  }

  rtsiSetT(si, t + h*rt_ODE3_A[1]);
  rtsiSetdX(si, f2);
  dc_motor_step();
  dc_motor_derivatives();

  /* tnew = t + hA(3);
     ynew = y + f*hB(:,3); */
  for (i = 0; i <= 2; i++) {
    hB[i] = h * rt_ODE3_B[2][i];
  }

  for (i = 0; i < nXc; i++) {
    x[i] = y[i] + (f0[i]*hB[0] + f1[i]*hB[1] + f2[i]*hB[2]);
  }

  rtsiSetT(si, tnew);
  rtsiSetSimTimeStep(si,MAJOR_TIME_STEP);
}

/* Model step function */
void dc_motor_step(void)
{
  real_T tmp;
  if (rtmIsMajorTimeStep(dc_motor_M)) {
    /* set solver stop time */
    if (!(dc_motor_M->Timing.clockTick0+1)) {
      rtsiSetSolverStopTime(&dc_motor_M->solverInfo,
                            ((dc_motor_M->Timing.clockTickH0 + 1) *
        dc_motor_M->Timing.stepSize0 * 4294967296.0));
    } else {
      rtsiSetSolverStopTime(&dc_motor_M->solverInfo,
                            ((dc_motor_M->Timing.clockTick0 + 1) *
        dc_motor_M->Timing.stepSize0 + dc_motor_M->Timing.clockTickH0 *
        dc_motor_M->Timing.stepSize0 * 4294967296.0));
    }
  }                                    /* end MajorTimeStep */

  /* Update absolute time of base rate at minor time step */
  if (rtmIsMinorTimeStep(dc_motor_M)) {
    dc_motor_M->Timing.t[0] = rtsiGetT(&dc_motor_M->solverInfo);
  }

  /* Integrator: '<Root>/Integrator' */
  dc_motor_B.i = dc_motor_X.Integrator_CSTATE;

  /* Integrator: '<Root>/Integrator1' */
  dc_motor_B.dtheta = dc_motor_X.Integrator1_CSTATE;

  /* Gain: '<Root>/Gain6' */
  dc_motor_B.RPM = dc_motor_P.Gain6_Gain * dc_motor_B.dtheta;

  /* Integrator: '<Root>/Integrator2' */
  dc_motor_B.theta = dc_motor_X.Integrator2_CSTATE;
  if (rtmIsMajorTimeStep(dc_motor_M)) {
  }

  /* Step: '<Root>/Step' */
  if (dc_motor_M->Timing.t[0] < dc_motor_P.Step_Time) {
    tmp = dc_motor_P.Step_Y0;
  } else {
    tmp = dc_motor_P.V;
  }

  /* Gain: '<Root>/Gain2' incorporates:
   *  Gain: '<Root>/Gain'
   *  Gain: '<Root>/Gain1'
   *  Step: '<Root>/Step'
   *  Sum: '<Root>/Add'
   */
  dc_motor_B.di = ((tmp - dc_motor_P.R * dc_motor_B.i) - dc_motor_P.K *
                   dc_motor_B.dtheta) * (1.0 / dc_motor_P.L);

  /* Gain: '<Root>/Gain5' incorporates:
   *  Gain: '<Root>/Gain3'
   *  Gain: '<Root>/Gain4'
   *  Sum: '<Root>/Add1'
   */
  dc_motor_B.ddtheta = (dc_motor_P.K * dc_motor_B.i - dc_motor_P.b *
                        dc_motor_B.dtheta) * (1.0 / dc_motor_P.J);
  if (rtmIsMajorTimeStep(dc_motor_M)) {
    /* Matfile logging */
    rt_UpdateTXYLogVars(dc_motor_M->rtwLogInfo, (dc_motor_M->Timing.t));
  }                                    /* end MajorTimeStep */

  if (rtmIsMajorTimeStep(dc_motor_M)) {
    /* signal main to stop simulation */
    {                                  /* Sample time: [0.0s, 0.0s] */
      if ((rtmGetTFinal(dc_motor_M)!=-1) &&
          !((rtmGetTFinal(dc_motor_M)-(((dc_motor_M->Timing.clockTick1+
               dc_motor_M->Timing.clockTickH1* 4294967296.0)) * 0.2)) >
            (((dc_motor_M->Timing.clockTick1+dc_motor_M->Timing.clockTickH1*
               4294967296.0)) * 0.2) * (DBL_EPSILON))) {
        rtmSetErrorStatus(dc_motor_M, "Simulation finished");
      }
    }

    rt_ertODEUpdateContinuousStates(&dc_motor_M->solverInfo);

    /* Update absolute time for base rate */
    /* The "clockTick0" counts the number of times the code of this task has
     * been executed. The absolute time is the multiplication of "clockTick0"
     * and "Timing.stepSize0". Size of "clockTick0" ensures timer will not
     * overflow during the application lifespan selected.
     * Timer of this task consists of two 32 bit unsigned integers.
     * The two integers represent the low bits Timing.clockTick0 and the high bits
     * Timing.clockTickH0. When the low bit overflows to 0, the high bits increment.
     */
    if (!(++dc_motor_M->Timing.clockTick0)) {
      ++dc_motor_M->Timing.clockTickH0;
    }

    dc_motor_M->Timing.t[0] = rtsiGetSolverStopTime(&dc_motor_M->solverInfo);

    {
      /* Update absolute timer for sample time: [0.2s, 0.0s] */
      /* The "clockTick1" counts the number of times the code of this task has
       * been executed. The resolution of this integer timer is 0.2, which is the step size
       * of the task. Size of "clockTick1" ensures timer will not overflow during the
       * application lifespan selected.
       * Timer of this task consists of two 32 bit unsigned integers.
       * The two integers represent the low bits Timing.clockTick1 and the high bits
       * Timing.clockTickH1. When the low bit overflows to 0, the high bits increment.
       */
      dc_motor_M->Timing.clockTick1++;
      if (!dc_motor_M->Timing.clockTick1) {
        dc_motor_M->Timing.clockTickH1++;
      }
    }
  }                                    /* end MajorTimeStep */
}

/* Derivatives for root system: '<Root>' */
void dc_motor_derivatives(void)
{
  XDot_dc_motor_T *_rtXdot;
  _rtXdot = ((XDot_dc_motor_T *) dc_motor_M->derivs);

  /* Derivatives for Integrator: '<Root>/Integrator' */
  _rtXdot->Integrator_CSTATE = dc_motor_B.di;

  /* Derivatives for Integrator: '<Root>/Integrator1' */
  _rtXdot->Integrator1_CSTATE = dc_motor_B.ddtheta;

  /* Derivatives for Integrator: '<Root>/Integrator2' */
  _rtXdot->Integrator2_CSTATE = dc_motor_B.dtheta;
}

/* Model initialize function */
void dc_motor_initialize(void)
{
  /* Registration code */

  /* initialize non-finites */
  rt_InitInfAndNaN(sizeof(real_T));

  /* initialize real-time model */
  (void) memset((void *)dc_motor_M, 0,
                sizeof(RT_MODEL_dc_motor_T));

  {
    /* Setup solver object */
    rtsiSetSimTimeStepPtr(&dc_motor_M->solverInfo,
                          &dc_motor_M->Timing.simTimeStep);
    rtsiSetTPtr(&dc_motor_M->solverInfo, &rtmGetTPtr(dc_motor_M));
    rtsiSetStepSizePtr(&dc_motor_M->solverInfo, &dc_motor_M->Timing.stepSize0);
    rtsiSetdXPtr(&dc_motor_M->solverInfo, &dc_motor_M->derivs);
    rtsiSetContStatesPtr(&dc_motor_M->solverInfo, (real_T **)
                         &dc_motor_M->contStates);
    rtsiSetNumContStatesPtr(&dc_motor_M->solverInfo,
      &dc_motor_M->Sizes.numContStates);
    rtsiSetNumPeriodicContStatesPtr(&dc_motor_M->solverInfo,
      &dc_motor_M->Sizes.numPeriodicContStates);
    rtsiSetPeriodicContStateIndicesPtr(&dc_motor_M->solverInfo,
      &dc_motor_M->periodicContStateIndices);
    rtsiSetPeriodicContStateRangesPtr(&dc_motor_M->solverInfo,
      &dc_motor_M->periodicContStateRanges);
    rtsiSetContStateDisabledPtr(&dc_motor_M->solverInfo, (boolean_T**)
      &dc_motor_M->contStateDisabled);
    rtsiSetErrorStatusPtr(&dc_motor_M->solverInfo, (&rtmGetErrorStatus
      (dc_motor_M)));
    rtsiSetRTModelPtr(&dc_motor_M->solverInfo, dc_motor_M);
  }

  rtsiSetSimTimeStep(&dc_motor_M->solverInfo, MAJOR_TIME_STEP);
  rtsiSetIsMinorTimeStepWithModeChange(&dc_motor_M->solverInfo, false);
  rtsiSetIsContModeFrozen(&dc_motor_M->solverInfo, false);
  dc_motor_M->intgData.y = dc_motor_M->odeY;
  dc_motor_M->intgData.f[0] = dc_motor_M->odeF[0];
  dc_motor_M->intgData.f[1] = dc_motor_M->odeF[1];
  dc_motor_M->intgData.f[2] = dc_motor_M->odeF[2];
  dc_motor_M->contStates = ((X_dc_motor_T *) &dc_motor_X);
  dc_motor_M->contStateDisabled = ((XDis_dc_motor_T *) &dc_motor_XDis);
  dc_motor_M->Timing.tStart = (0.0);
  rtsiSetSolverData(&dc_motor_M->solverInfo, (void *)&dc_motor_M->intgData);
  rtsiSetSolverName(&dc_motor_M->solverInfo,"ode3");
  rtmSetTPtr(dc_motor_M, &dc_motor_M->Timing.tArray[0]);
  rtmSetTFinal(dc_motor_M, 10.0);
  dc_motor_M->Timing.stepSize0 = 0.2;

  /* Setup for data logging */
  {
    static RTWLogInfo rt_DataLoggingInfo;
    rt_DataLoggingInfo.loggingInterval = (NULL);
    dc_motor_M->rtwLogInfo = &rt_DataLoggingInfo;
  }

  /* Setup for data logging */
  {
    rtliSetLogXSignalInfo(dc_motor_M->rtwLogInfo, (NULL));
    rtliSetLogXSignalPtrs(dc_motor_M->rtwLogInfo, (NULL));
    rtliSetLogT(dc_motor_M->rtwLogInfo, "tout");
    rtliSetLogX(dc_motor_M->rtwLogInfo, "");
    rtliSetLogXFinal(dc_motor_M->rtwLogInfo, "");
    rtliSetLogVarNameModifier(dc_motor_M->rtwLogInfo, "rt_");
    rtliSetLogFormat(dc_motor_M->rtwLogInfo, 4);
    rtliSetLogMaxRows(dc_motor_M->rtwLogInfo, 0);
    rtliSetLogDecimation(dc_motor_M->rtwLogInfo, 1);
    rtliSetLogY(dc_motor_M->rtwLogInfo, "");
    rtliSetLogYSignalInfo(dc_motor_M->rtwLogInfo, (NULL));
    rtliSetLogYSignalPtrs(dc_motor_M->rtwLogInfo, (NULL));
  }

  /* block I/O */
  (void) memset(((void *) &dc_motor_B), 0,
                sizeof(B_dc_motor_T));

  /* states (continuous) */
  {
    (void) memset((void *)&dc_motor_X, 0,
                  sizeof(X_dc_motor_T));
  }

  /* disabled states */
  {
    (void) memset((void *)&dc_motor_XDis, 0,
                  sizeof(XDis_dc_motor_T));
  }

  /* Matfile logging */
  rt_StartDataLoggingWithStartTime(dc_motor_M->rtwLogInfo, 0.0, rtmGetTFinal
    (dc_motor_M), dc_motor_M->Timing.stepSize0, (&rtmGetErrorStatus(dc_motor_M)));

  /* InitializeConditions for Integrator: '<Root>/Integrator' */
  dc_motor_X.Integrator_CSTATE = dc_motor_P.Integrator_IC;

  /* InitializeConditions for Integrator: '<Root>/Integrator1' */
  dc_motor_X.Integrator1_CSTATE = dc_motor_P.Integrator1_IC;

  /* InitializeConditions for Integrator: '<Root>/Integrator2' */
  dc_motor_X.Integrator2_CSTATE = dc_motor_P.Integrator2_IC;
}

/* Model terminate function */
void dc_motor_terminate(void)
{
  /* (no terminate code required) */
}
