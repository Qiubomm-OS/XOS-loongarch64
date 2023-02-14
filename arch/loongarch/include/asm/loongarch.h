#ifndef _ASM_LOONGARCH_H
#define _ASM_LOONGARCH_H

/* LoongArch Registers */
#define REG_ZERO	0x0
#define REG_RA		0x1
#define REG_TP		0x2
#define REG_SP		0x3
#define REG_A0		0x4 /* Reused as V0 for return value */
#define REG_A1		0x5 /* Reused as V1 for return value */
#define REG_A2		0x6
#define REG_A3		0x7
#define REG_A4		0x8
#define REG_A5		0x9
#define REG_A6		0xa
#define REG_A7		0xb
#define REG_T0		0xc
#define REG_T1		0xd
#define REG_T2		0xe
#define REG_T3		0xf
#define REG_T4		0x10
#define REG_T5		0x11
#define REG_T6		0x12
#define REG_T7		0x13
#define REG_T8		0x14
#define REG_U0		0x15 /* Kernel uses it as percpu base */
#define REG_FP		0x16
#define REG_S0		0x17
#define REG_S1		0x18
#define REG_S2		0x19
#define REG_S3		0x1a
#define REG_S4		0x1b
#define REG_S5		0x1c
#define REG_S6		0x1d
#define REG_S7		0x1e
#define REG_S8		0x1f

/* Bit fields for CPUCFG registers */
#define LOONGARCH_CPUCFG0		0x0
#define  CPUCFG0_PRID			GENMASK(31, 0)

#define LOONGARCH_CPUCFG1		0x1
#define  CPUCFG1_ISGR32			BIT(0)
#define  CPUCFG1_ISGR64			BIT(1)
#define  CPUCFG1_PAGING			BIT(2)
#define  CPUCFG1_IOCSR			BIT(3)
#define  CPUCFG1_PABITS			GENMASK(11, 4)
#define  CPUCFG1_VABITS			GENMASK(19, 12)
#define  CPUCFG1_UAL			BIT(20)
#define  CPUCFG1_RI			BIT(21)
#define  CPUCFG1_EP			BIT(22)
#define  CPUCFG1_RPLV			BIT(23)
#define  CPUCFG1_HUGEPG			BIT(24)
#define  CPUCFG1_IOCSRBRD		BIT(25)
#define  CPUCFG1_MSGINT			BIT(26)

#define LOONGARCH_CPUCFG2		0x2
#define  CPUCFG2_FP			BIT(0)
#define  CPUCFG2_FPSP			BIT(1)
#define  CPUCFG2_FPDP			BIT(2)
#define  CPUCFG2_FPVERS			GENMASK(5, 3)
#define  CPUCFG2_LSX			BIT(6)
#define  CPUCFG2_LASX			BIT(7)
#define  CPUCFG2_COMPLEX		BIT(8)
#define  CPUCFG2_CRYPTO			BIT(9)
#define  CPUCFG2_LVZP			BIT(10)
#define  CPUCFG2_LVZVER			GENMASK(13, 11)
#define  CPUCFG2_LLFTP			BIT(14)
#define  CPUCFG2_LLFTPREV		GENMASK(17, 15)
#define  CPUCFG2_X86BT			BIT(18)
#define  CPUCFG2_ARMBT			BIT(19)
#define  CPUCFG2_MIPSBT			BIT(20)
#define  CPUCFG2_LSPW			BIT(21)
#define  CPUCFG2_LAM			BIT(22)

#define LOONGARCH_CPUCFG3		0x3
#define  CPUCFG3_CCDMA			BIT(0)
#define  CPUCFG3_SFB			BIT(1)
#define  CPUCFG3_UCACC			BIT(2)
#define  CPUCFG3_LLEXC			BIT(3)
#define  CPUCFG3_SCDLY			BIT(4)
#define  CPUCFG3_LLDBAR			BIT(5)
#define  CPUCFG3_ITLBT			BIT(6)
#define  CPUCFG3_ICACHET		BIT(7)
#define  CPUCFG3_SPW_LVL		GENMASK(10, 8)
#define  CPUCFG3_SPW_HG_HF		BIT(11)
#define  CPUCFG3_RVA			BIT(12)
#define  CPUCFG3_RVAMAX			GENMASK(16, 13)

#define LOONGARCH_CPUCFG4		0x4
#define  CPUCFG4_CCFREQ			GENMASK(31, 0)

#define LOONGARCH_CPUCFG5		0x5
#define  CPUCFG5_CCMUL			GENMASK(15, 0)
#define  CPUCFG5_CCDIV			GENMASK(31, 16)

#define LOONGARCH_CPUCFG6		0x6
#define  CPUCFG6_PMP			BIT(0)
#define  CPUCFG6_PAMVER			GENMASK(3, 1)
#define  CPUCFG6_PMNUM			GENMASK(7, 4)
#define  CPUCFG6_PMBITS			GENMASK(13, 8)
#define  CPUCFG6_UPM			BIT(14)

#define LOONGARCH_CPUCFG16		0x10
#define  CPUCFG16_L1_IUPRE		BIT(0)
#define  CPUCFG16_L1_IUUNIFY		BIT(1)
#define  CPUCFG16_L1_DPRE		BIT(2)
#define  CPUCFG16_L2_IUPRE		BIT(3)
#define  CPUCFG16_L2_IUUNIFY		BIT(4)
#define  CPUCFG16_L2_IUPRIV		BIT(5)
#define  CPUCFG16_L2_IUINCL		BIT(6)
#define  CPUCFG16_L2_DPRE		BIT(7)
#define  CPUCFG16_L2_DPRIV		BIT(8)
#define  CPUCFG16_L2_DINCL		BIT(9)
#define  CPUCFG16_L3_IUPRE		BIT(10)
#define  CPUCFG16_L3_IUUNIFY		BIT(11)
#define  CPUCFG16_L3_IUPRIV		BIT(12)
#define  CPUCFG16_L3_IUINCL		BIT(13)
#define  CPUCFG16_L3_DPRE		BIT(14)
#define  CPUCFG16_L3_DPRIV		BIT(15)
#define  CPUCFG16_L3_DINCL		BIT(16)

#define LOONGARCH_CPUCFG17		0x11
#define LOONGARCH_CPUCFG18		0x12
#define LOONGARCH_CPUCFG19		0x13
#define LOONGARCH_CPUCFG20		0x14
#define  CPUCFG_CACHE_WAYS_M		GENMASK(15, 0)
#define  CPUCFG_CACHE_SETS_M		GENMASK(23, 16)
#define  CPUCFG_CACHE_LSIZE_M		GENMASK(30, 24)
#define  CPUCFG_CACHE_WAYS	 	0
#define  CPUCFG_CACHE_SETS		16
#define  CPUCFG_CACHE_LSIZE		24

#define LOONGARCH_CPUCFG48		0x30
#define  CPUCFG48_MCSR_LCK		BIT(0)
#define  CPUCFG48_NAP_EN		BIT(1)
#define  CPUCFG48_VFPU_CG		BIT(2)
#define  CPUCFG48_RAM_CG		BIT(3)



#endif
