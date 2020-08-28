/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                     BOARD SUPPORT PACKAGE (BSP)
*
*                                       IAR Development Kits
*                                              on the
*
*                                    STM32F429II-SK KICKSTART KIT
*
* Filename      : bsp.c
* Version       : V1.00
* Programmer(s) : FF
*                 YS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define   BSP_MODULE
#include  <bsp.h>
#include  <os.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/





/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             REGISTERS
*********************************************************************************************************
*/

#define  BSP_REG_DEM_CR                           (*(CPU_REG32 *)0xE000EDFC)
#define  BSP_REG_DWT_CR                           (*(CPU_REG32 *)0xE0001000)
#define  BSP_REG_DWT_CYCCNT                       (*(CPU_REG32 *)0xE0001004)
#define  BSP_REG_DBGMCU_CR                        (*(CPU_REG32 *)0xE0042004)

/* -         RCC REGISTER DEFINES                     - */
#define  BSP_REG_RCC_BASE_ADDR                    ((CPU_INT32U)(0x40023800))

#define  BSP_REG32_RCC_CR                         (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x00u ))
#define  BSP_REG32_RCC_PLLCFGR                    (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x04u ))
#define  BSP_REG32_RCC_CFGR                       (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x08u ))
#define  BSP_REG32_RCC_CIR                        (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x0Cu ))
#define  BSP_REG32_RCC_AHB1RSTR                   (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x10u ))
#define  BSP_REG32_RCC_AHB2RSTR                   (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x14u ))
#define  BSP_REG32_RCC_AHB3RSTR                   (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x18u ))
#define  BSP_REG32_RCC_APB1RSTR                   (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x20u ))
#define  BSP_REG32_RCC_APB2RSTR                   (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x24u ))
#define  BSP_REG32_RCC_AHB1ENR                    (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x30u ))
#define  BSP_REG32_RCC_AHB2ENR                    (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x34u ))
#define  BSP_REG32_RCC_AHB3ENR                    (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x38u ))
#define  BSP_REG32_RCC_APB1ENR                    (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x40u ))
#define  BSP_REG32_RCC_APB2ENR                    (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x44u ))
#define  BSP_REG32_RCC_AHB1LPENR                  (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x50u ))
#define  BSP_REG32_RCC_AHB2LPENR                  (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x54u ))
#define  BSP_REG32_RCC_AHB3LPENR                  (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x58u ))
#define  BSP_REG32_RCC_APB1LPENR                  (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x60u ))
#define  BSP_REG32_RCC_APB2LPENR                  (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x64u ))
#define  BSP_REG32_RCC_BDCR                       (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x70u ))
#define  BSP_REG32_RCC_CSR                        (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x74u ))
#define  BSP_REG32_RCC_SSCGR                      (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x80u ))
#define  BSP_REG32_RCC_PLLI2SCFGR                 (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x84u ))
#define  BSP_REG32_RCC_PLLSAICFGR                 (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x88u ))
#define  BSP_REG32_RCC_DCKCFGR                    (*(CPU_REG32 *)( BSP_REG_RCC_BASE_ADDR + 0x8Cu ))

/* -               FLASH REGISTER DEFINES             - */
#define  BSP_REG_FLASH_BASE_ADDR                  (( CPU_INT32U )(0x40023C00))

#define  BSP_REG32_FLASH_ACR                      (*(CPU_REG32 *)( BSP_REG_FLASH_BASE_ADDR + 0x00u ))
#define  BSP_REG32_FLASH_KEYR                     (*(CPU_REG32 *)( BSP_REG_FLASH_BASE_ADDR + 0x04u ))
#define  BSP_REG32_FLASH_OPTKEYR                  (*(CPU_REG32 *)( BSP_REG_FLASH_BASE_ADDR + 0x08u ))
#define  BSP_REG32_FLASH_SR                       (*(CPU_REG32 *)( BSP_REG_FLASH_BASE_ADDR + 0x0Cu ))
#define  BSP_REG32_FLASH_CR                       (*(CPU_REG32 *)( BSP_REG_FLASH_BASE_ADDR + 0x10u ))
#define  BSP_REG32_FLASH_OPTCR                    (*(CPU_REG32 *)( BSP_REG_FLASH_BASE_ADDR + 0x14u ))
#define  BSP_REG32_FLASH_OPTCR1                   (*(CPU_REG32 *)( BSP_REG_FLASH_BASE_ADDR + 0x18u ))



/*
*********************************************************************************************************
*                                            REGISTER BITS
*********************************************************************************************************
*/

#define  BSP_DBGMCU_CR_TRACE_IOEN_MASK                 0x10u
#define  BSP_DBGMCU_CR_TRACE_MODE_ASYNC                0x00u
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_01              0x40u
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_02              0x80u
#define  BSP_DBGMCU_CR_TRACE_MODE_SYNC_04              0xC0u
#define  BSP_DBGMCU_CR_TRACE_MODE_MASK                 0xC0u

#define  BSP_BIT_DEM_CR_TRCENA                    DEF_BIT_24
#define  BSP_BIT_DWT_CR_CYCCNTENA                 DEF_BIT_00


/* -                    RCC BLOCK                     - */
#define  HSE_TIMEOUT_VAL                              0x0500u
#define  BSP_MSK_HSECFG                           0x00FF0000u
#define  BSP_BIT_RCC_CR_HSION                     DEF_BIT_00
#define  BSP_BIT_RCC_CR_HSEBYP                    DEF_BIT_18
#define  BSP_BIT_RCC_CR_HSEON                     DEF_BIT_16
#define  BSP_BIT_RCC_CR_HSERDY                    DEF_BIT_17
#define  BSP_MSK_RCC_CFGR_HPRE                    0x000000F0u
#define  BSP_MSK_RCC_CFGR_SYSCLKDIV1              0x00000000u
#define  BSP_MSK_RCC_CFGR_SYSCLKDIV2              0x00000080u
#define  BSP_MSK_RCC_CFGR_SYSCLKDIV4              0x00000090u
#define  BSP_MSK_RCC_CFGR_SYSCLKDIV8              0x000000A0u
#define  BSP_MSK_RCC_CFGR_SYSCLKDIV16             0x000000B0u
#define  BSP_MSK_RCC_CFGR_SYSCLKDIV64             0x000000C0u
#define  BSP_MSK_RCC_CFGR_SYSCLKDIV128            0x000000D0u
#define  BSP_MSK_RCC_CFGR_SYSCLKDIV256            0x000000E0u
#define  BSP_MSK_RCC_CFGR_SYSCLKDIV512            0x000000F0u
#define  BSP_MSK_RCC_CFGR_PPRE1                   0x00001C00u
#define  BSP_MSK_RCC_CFGR_PPRE2                   0x0000E000u
#define  BSP_MSK_RCC_CFGR_HCLK_DIV1               0x00000000u
#define  BSP_MSK_RCC_CFGR_HCLK_DIV2               0x00001000u
#define  BSP_MSK_RCC_CFGR_HCLK_DIV4               0x00001400u
#define  BSP_MSK_RCC_CFGR_HCLK_DIV8               0x00001800u
#define  BSP_MSK_RCC_CFGR_HCLK_DIV16              0x00001C00u
#define  BSP_MSK_RCC_CFGR_SWS_HSI                 0x00000000u
#define  BSP_MSK_RCC_CFGR_SWS_HSE                 0x00000004u
#define  BSP_MSK_RCC_CFGR_SWS_PLL                 0x00000008u
#define  BSP_MSK_RCC_CFGR_SWS                     0x0000000Cu
#define  BSP_MSK_RCC_CFGR_SW_HSI                  0x00000000u
#define  BSP_MSK_RCC_CFGR_SW_HSE                  0x00000001u
#define  BSP_MSK_RCC_CFGR_SW_PLL                  0x00000002u
#define  BSP_MSK_RCC_CFGR_SW                      0x00000003u
/* -                     PLL BLOCK                    - */
#define  BSP_BIT_RCC_PLLCFGR_PLLM                          8u
#define  BSP_BIT_RCC_PLLCFGR_PLLN                        336u
#define  BSP_BIT_RCC_PLLCFGR_PLLP                          2u
#define  BSP_BIT_RCC_PLLCFGR_PLLQ                          7u
#define  BSP_BIT_RCC_CR_PLLON                     DEF_BIT_24
#define  BSP_BIT_RCC_CR_PLLRDY                    DEF_BIT_25
#define  BSP_MSK_RCC_PLLCFGR_PLLSRC_HSE           0x00400000u
#define  BSP_MSK_RCC_PLLCFGR_RST                  0x24003010u
#define  BSP_MSK_SYSCLK_SRC_PLLCLK                0x00000002u
#define  BSP_MSK_PLLCFGR_PLLSRC_HSE               0x00400000u
#define  BSP_MSK_PLLCFGR_PLLSRC_HSI               0x00000000u
/* -                    FLASH BLOCK                   - */
#define  BSP_BIT_FLASH_ACR_PRFTEN                 DEF_BIT_08
#define  BSP_BIT_FLASH_ACR_ICEN                   DEF_BIT_09
#define  BSP_BIT_FLASH_ACR_DCEN                   DEF_BIT_10
#define  BSP_MSK_FLASHLATENCY_0WS                 0x00000000u
#define  BSP_MSK_FLASHLATENCY_1WS                 0x00000001u
#define  BSP_MSK_FLASHLATENCY_2WS                 0x00000002u
#define  BSP_MSK_FLASHLATENCY_3WS                 0x00000003u
#define  BSP_MSK_FLASHLATENCY_4WS                 0x00000004u
#define  BSP_MSK_FLASHLATENCY_5WS                 0x00000005u
#define  BSP_MSK_FLASHLATENCY_6WS                 0x00000006u
#define  BSP_MSK_FLASHLATENCY_7WS                 0x00000007u
#define  BSP_MSK_FLASHLATENCY_8WS                 0x00000008u
#define  BSP_MSK_FLASHLATENCY_9WS                 0x00000009u
#define  BSP_MSK_FLASHLATENCY_10WS                0x0000000Au
#define  BSP_MSK_FLASHLATENCY_11WS                0x0000000Bu
#define  BSP_MSK_FLASHLATENCY_12WS                0x0000000Cu
#define  BSP_MSK_FLASHLATENCY_13WS                0x0000000Du
#define  BSP_MSK_FLASHLATENCY_14WS                0x0000000Eu
#define  BSP_MSK_FLASHLATENCY_15WS                0x0000000Fu





/*
*********************************************************************************************************
*                                               BSP_Init()
*
* Description : Initialize the Board Support Package (BSP).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : (1) This function SHOULD be called before any other BSP function is called.
*
*               (2) CPU instruction / data tracing requires the use of the following pins :
*                   (a) (1) Asynchronous      :  PB[3]
*                       (2) Synchronous 1-bit :  PE[3:2]
*                       (3) Synchronous 2-bit :  PE[4:2]
*                       (4) Synchronous 4-bit :  PE[6:2]
*
*                   (c) The application may wish to adjust the trace bus width depending on I/O
*                       requirements.
*********************************************************************************************************
*/

void  BSP_Init(void)
{
    Led_Init();
    delay_init(168);  //delay_init代替BSP_Tick_Init初始化。
    Usart1_Init();
    Key_Init();//初始化按键
	InitMPU6050();//初始化MPU6050
	OLED_Init();//初始化OLED
	Rtc_Init();//RTC时钟初始化
	
}


/*
*********************************************************************************************************
*                                            BSP_CPU_ClkFreq()
*
* Description : Read CPU registers to determine the CPU clock frequency of the chip.
*
* Argument(s) : none.
*
* Return(s)   : The CPU clock frequency, in Hz.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_INT32U  BSP_CPU_ClkFreq(void)
{
    RCC_ClocksTypeDef  rcc_clocks;


    RCC_GetClocksFreq(&rcc_clocks);

    return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
}


/*
*********************************************************************************************************
*                                            BSP_Tick_Init()
*
* Description : Configure and Initialize the OS Tick Services (SysTick).
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

void  BSP_Tick_Init(void)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;


    cpu_clk_freq = BSP_CPU_ClkFreq();                           /* Determine SysTick reference freq.                    */

#if (OS_VERSION >= 30000u)
    cnts = (cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz);      /* Determine nbr SysTick increments                     */
#else
    cnts = (cpu_clk_freq / (CPU_INT32U)OS_TICKS_PER_SEC);       /* Determine nbr SysTick increments.                    */
#endif

    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).              */
}














/*$PAGE*/
/*
*********************************************************************************************************
*                                          CPU_TS_TmrInit()
*
* Description : Initialize & start CPU timestamp timer.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : CPU_TS_Init().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but MUST NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrInit() is an application/BSP function that MUST be defined by the developer
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR'
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR'
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                   (c) When applicable, timer period SHOULD be less than the typical measured time
*                       but MUST be less than the maximum measured time; otherwise, timer resolution
*                       inadequate to measure desired times.
*
*                   See also 'CPU_TS_TmrRd()  Note #2'.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
void  CPU_TS_TmrInit(void)
{
    CPU_INT32U  fclk_freq;


    fclk_freq = BSP_CPU_ClkFreq();

    BSP_REG_DEM_CR     |= (CPU_INT32U)BSP_BIT_DEM_CR_TRCENA;    /* Enable Cortex-M4's DWT CYCCNT reg.                   */
    BSP_REG_DWT_CYCCNT  = (CPU_INT32U)0u;
    BSP_REG_DWT_CR     |= (CPU_INT32U)BSP_BIT_DWT_CR_CYCCNTENA;

    CPU_TS_TmrFreqSet((CPU_TS_TMR_FREQ)fclk_freq);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                           CPU_TS_TmrRd()
*
* Description : Get current CPU timestamp timer count value.
*
* Argument(s) : none.
*
* Return(s)   : Timestamp timer count (see Notes #2a & #2b).
*
* Caller(s)   : CPU_TS_Init(),
*               CPU_TS_Get32(),
*               CPU_TS_Get64(),
*               CPU_IntDisMeasStart(),
*               CPU_IntDisMeasStop().
*
*               This function is an INTERNAL CPU module function & MUST be implemented by application/
*               BSP function(s) [see Note #1] but SHOULD NOT be called by application function(s).
*
* Note(s)     : (1) CPU_TS_TmrRd() is an application/BSP function that MUST be defined by the developer
*                   if either of the following CPU features is enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) Timer count values MUST be returned via word-size-configurable 'CPU_TS_TMR'
*                       data type.
*
*                       (1) If timer has more bits, truncate timer values' higher-order bits greater
*                           than the configured 'CPU_TS_TMR' timestamp timer data type word size.
*
*                       (2) Since the timer MUST NOT have less bits than the configured 'CPU_TS_TMR'
*                           timestamp timer data type word size; 'CPU_CFG_TS_TMR_SIZE' MUST be
*                           configured so that ALL bits in 'CPU_TS_TMR' data type are significant.
*
*                           In other words, if timer size is not a binary-multiple of 8-bit octets
*                           (e.g. 20-bits or even 24-bits), then the next lower, binary-multiple
*                           octet word size SHOULD be configured (e.g. to 16-bits).  However, the
*                           minimum supported word size for CPU timestamp timers is 8-bits.
*
*                       See also 'cpu_cfg.h   CPU TIMESTAMP CONFIGURATION  Note #2'
*                              & 'cpu_core.h  CPU TIMESTAMP DATA TYPES     Note #1'.
*
*                   (b) Timer SHOULD be an 'up'  counter whose values increase with each time count.
*
*                       (1) If timer is a 'down' counter whose values decrease with each time count,
*                           then the returned timer value MUST be ones-complemented.
*
*                   (c) (1) When applicable, the amount of time measured by CPU timestamps is
*                           calculated by either of the following equations :
*
*                           (A) Time measured  =  Number timer counts  *  Timer period
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured
*                                       Timer period            Timer's period in some units of
*                                                                   (fractional) seconds
*                                       Time measured           Amount of time measured, in same
*                                                                   units of (fractional) seconds
*                                                                   as the Timer period
*
*                                                  Number timer counts
*                           (B) Time measured  =  ---------------------
*                                                    Timer frequency
*
*                                   where
*
*                                       Number timer counts     Number of timer counts measured
*                                       Timer frequency         Timer's frequency in some units
*                                                                   of counts per second
*                                       Time measured           Amount of time measured, in seconds
*
*                       (2) Timer period SHOULD be less than the typical measured time but MUST be less
*                           than the maximum measured time; otherwise, timer resolution inadequate to
*                           measure desired times.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_TMR_EN == DEF_ENABLED)
CPU_TS_TMR  CPU_TS_TmrRd(void)
{
    CPU_TS_TMR  ts_tmr_cnts;


    ts_tmr_cnts = (CPU_TS_TMR)BSP_REG_DWT_CYCCNT;

    return (ts_tmr_cnts);
}
#endif


/*$PAGE*/
/*
*********************************************************************************************************
*                                         CPU_TSxx_to_uSec()
*
* Description : Convert a 32-/64-bit CPU timestamp from timer counts to microseconds.
*
* Argument(s) : ts_cnts   CPU timestamp (in timestamp timer counts [see Note #2aA]).
*
* Return(s)   : Converted CPU timestamp (in microseconds           [see Note #2aD]).
*
* Caller(s)   : Application.
*
*               This function is an (optional) CPU module application programming interface (API)
*               function which MAY be implemented by application/BSP function(s) [see Note #1] &
*               MAY be called by application function(s).
*
* Note(s)     : (1) CPU_TS32_to_uSec()/CPU_TS64_to_uSec() are application/BSP functions that MAY be
*                   optionally defined by the developer when either of the following CPU features is
*                   enabled :
*
*                   (a) CPU timestamps
*                   (b) CPU interrupts disabled time measurements
*
*                   See 'cpu_cfg.h  CPU TIMESTAMP CONFIGURATION  Note #1'
*                     & 'cpu_cfg.h  CPU INTERRUPTS DISABLED TIME MEASUREMENT CONFIGURATION  Note #1a'.
*
*               (2) (a) The amount of time measured by CPU timestamps is calculated by either of
*                       the following equations :
*
*                                                                        10^6 microseconds
*                       (1) Time measured  =   Number timer counts   *  -------------------  *  Timer period
*                                                                            1 second
*
*                                              Number timer counts       10^6 microseconds
*                       (2) Time measured  =  ---------------------  *  -------------------
*                                                Timer frequency             1 second
*
*                               where
*
*                                   (A) Number timer counts     Number of timer counts measured
*                                   (B) Timer frequency         Timer's frequency in some units
*                                                                   of counts per second
*                                   (C) Timer period            Timer's period in some units of
*                                                                   (fractional)  seconds
*                                   (D) Time measured           Amount of time measured,
*                                                                   in microseconds
*
*                   (b) Timer period SHOULD be less than the typical measured time but MUST be less
*                       than the maximum measured time; otherwise, timer resolution inadequate to
*                       measure desired times.
*
*                   (c) Specific implementations may convert any number of CPU_TS32 or CPU_TS64 bits
*                       -- up to 32 or 64, respectively -- into microseconds.
*********************************************************************************************************
*/

#if (CPU_CFG_TS_32_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS32_to_uSec(CPU_TS32  ts_cnts)
{
    CPU_INT64U  ts_us;
    CPU_INT64U  fclk_freq;


    fclk_freq = BSP_CPU_ClkFreq();
    ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

    return (ts_us);
}
#endif


#if (CPU_CFG_TS_64_EN == DEF_ENABLED)
CPU_INT64U  CPU_TS64_to_uSec(CPU_TS64  ts_cnts)
{
    CPU_INT64U  ts_us;
    CPU_INT64U  fclk_freq;


    fclk_freq = BSP_CPU_ClkFreq();
    ts_us     = ts_cnts / (fclk_freq / DEF_TIME_NBR_uS_PER_SEC);

    return (ts_us);
}
#endif
