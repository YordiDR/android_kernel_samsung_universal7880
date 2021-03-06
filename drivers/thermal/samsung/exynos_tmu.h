/*
 * exynos_tmu.h - Samsung EXYNOS TMU (Thermal Management Unit)
 *
 *  Copyright (C) 2011 Samsung Electronics
 *  Donggeun Kim <dg77.kim@samsung.com>
 *  Amit Daniel Kachhap <amit.daniel@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _EXYNOS_TMU_H
#define _EXYNOS_TMU_H
#include <linux/cpu_cooling.h>

#include "exynos_thermal_common.h"

#define FIRST_SENSOR		0

enum calibration_type {
	TYPE_ONE_POINT_TRIMMING,
	TYPE_ONE_POINT_TRIMMING_25,
	TYPE_ONE_POINT_TRIMMING_85,
	TYPE_TWO_POINT_TRIMMING,
	TYPE_NONE,
};

enum calibration_mode {
	SW_MODE,
	HW_MODE,
};

enum soc_type {
	SOC_ARCH_EXYNOS3250 = 1,
	SOC_ARCH_EXYNOS4210,
	SOC_ARCH_EXYNOS4412,
	SOC_ARCH_EXYNOS5250,
	SOC_ARCH_EXYNOS5260,
	SOC_ARCH_EXYNOS5420_TRIMINFO,
	SOC_ARCH_EXYNOS5440,
	SOC_ARCH_EXYNOS7580,
	SOC_ARCH_EXYNOS7870,
	SOC_ARCH_EXYNOS7880,
	SOC_ARCH_EXYNOS8890,
};

/**
 * EXYNOS TMU supported features.
 * TMU_SUPPORT_EMULATION - This features is used to set user defined
 *			temperature to the TMU controller.
 * TMU_SUPPORT_MULTI_INST - This features denotes that the soc
 *			has many instances of TMU.
 * TMU_SUPPORT_TRIM_RELOAD - This features shows that trimming can
 *			be reloaded.
 * TMU_SUPPORT_FALLING_TRIP - This features shows that interrupt can
 *			be registered for falling trips also.
 * TMU_SUPPORT_READY_STATUS - This feature tells that the TMU current
 *			state(active/idle) can be checked.
 * TMU_SUPPORT_EMUL_TIME - This features allows to set next temp emulation
 *			sample time.
 * TMU_SUPPORT_ADDRESS_MULTIPLE - This feature tells that the different TMU
 *			sensors shares some common registers.
 * TMU_SUPPORT - macro to compare the above features with the supplied.
 */
#define TMU_SUPPORT_EMULATION			BIT(0)
#define TMU_SUPPORT_MULTI_INST			BIT(1)
#define TMU_SUPPORT_TRIM_RELOAD			BIT(2)
#define TMU_SUPPORT_FALLING_TRIP		BIT(3)
#define TMU_SUPPORT_READY_STATUS		BIT(4)
#define TMU_SUPPORT_EMUL_TIME			BIT(5)
#define TMU_SUPPORT_ADDRESS_MULTIPLE		BIT(6)

#define TMU_SUPPORTS(a, b)	(a->features & TMU_SUPPORT_ ## b)

/**
 * struct exynos_tmu_register - register descriptors to access registers and
 * bitfields. The register validity, offsets and bitfield values may vary
 * slightly across different exynos SOC's.
 * @triminfo_data: register containing 2 pont trimming data
 * @triminfo_ctrl: trim info controller register.
 * @triminfo_ctrl_count: the number of trim info controller register.
 * @triminfo_85_shift: shift bit of the 85 C trim value in triminfo_data reg.
 * @tmu_ctrl: TMU main controller register.
 * @tmu_ctrl1: TMU main controller register.
 * @test_mux_addr_shift: shift bits of test mux address.
 * @therm_trip_mode_shift: shift bits of tripping mode in tmu_ctrl register.
 * @therm_trip_mode_mask: mask bits of tripping mode in tmu_ctrl register.
 * @therm_trip_en_shift: shift bits of tripping enable in tmu_ctrl register.
 * @lpi0_mode_en_shift: shift bits of LIPI0 mode enable bit in tmu_ctrl1 register.
 * @tmu_status: register drescribing the TMU status.
 * @tmu_cur_temp: register containing the current temperature of the TMU.
 * @threshold_temp: register containing the base threshold level.
 * @threshold_th0: Register containing first set of rising levels.
 * @threshold_th1: Register containing second set of rising levels.
 * @threshold_th2: Register containing third set of rising levels.
 * @threshold_th3: Register containing fourth set of rising levels.
 * @threshold_th3_l0_shift: shift bits of level0 threshold temperature.
 * @tmu_inten: register containing the different threshold interrupt
	enable bits.
 * @inten_rise0_shift: shift bits of rising 0 interrupt bits.
 * @inten_rise1_shift: shift bits of rising 1 interrupt bits.
 * @inten_rise2_shift: shift bits of rising 2 interrupt bits.
 * @inten_rise3_shift: shift bits of rising 3 interrupt bits.
 * @inten_rise4_shift: shift bits of rising 4 interrupt bits.
 * @inten_rise5_shift: shift bits of rising 5 interrupt bits.
 * @inten_rise6_shift: shift bits of rising 6 interrupt bits.
 * @inten_rise7_shift: shift bits of rising 7 interrupt bits.
 * @inten_fall0_shift: shift bits of falling 0 interrupt bits.
 * @inten_fall1_shift: shift bits of falling 1 interrupt bits.
 * @inten_fall2_shift: shift bits of falling 2 interrupt bits.
 * @inten_fall3_shift: shift bits of falling 3 interrupt bits.
 * @inten_fall4_shift: shift bits of falling 4 interrupt bits.
 * @inten_fall5_shift: shift bits of falling 5 interrupt bits.
 * @inten_fall6_shift: shift bits of falling 6 interrupt bits.
 * @inten_fall7_shift: shift bits of falling 7 interrupt bits.
 * @tmu_intstat: Register containing the interrupt status values.
 * @tmu_intclear: Register for clearing the raised interrupt status.
 * @emul_con: TMU emulation controller register.
 * @emul_temp_shift: shift bits of emulation temperature.
 * @emul_time_shift: shift bits of emulation time.
 * @tmu_irqstatus: register to find which TMU generated interrupts.
 * @tmu_pmin: register to get/set the Pmin value.
 */
struct exynos_tmu_registers {
	u32	triminfo_data;
	u32	calib_sel_shift;
	u32	calib_sel_mask;

	u32	triminfo_ctrl[MAX_TRIMINFO_CTRL_REG];
	u32	triminfo_ctrl_count;

	u32	triminfo_85_shift;

	u32	tmu_ctrl;
	u32	tmu_ctrl1;
	u32     test_mux_addr_shift;
	/* vref and slope otp value */
	u32	buf_vref_otp_reg;
	u32	buf_vref_otp_shift;
	u32	buf_vref_otp_mask;
	u32	buf_slope_otp_reg;
	u32	buf_slope_otp_shift;
	u32	buf_slope_otp_mask;
	u32	therm_trip_mode_shift;
	u32	therm_trip_mode_mask;
	u32	therm_trip_en_shift;
	u32	lpi0_mode_en_shift;

	u32	tmu_status;
	u32	valid_mask;
	u32	valid_p0_shift;
	u32	valid_p1_shift;
	u32	valid_p2_shift;
	u32	valid_p3_shift;
	u32	valid_p4_shift;

	u32	tmu_cur_temp;

	u32	threshold_temp;

	u32	threshold_th0;
	u32	threshold_th1;
	u32	threshold_th2;
	u32	threshold_th3;
	u32	threshold_th3_l0_shift;

	u32	threshold_th4;
	u32	threshold_th5;
	u32	threshold_th6;
	u32	threshold_th7;

	u32	tmu_inten;

	u32	tmu_intstat;
	u32     inten_rise0_shift;
	u32     inten_rise1_shift;
	u32     inten_rise2_shift;
	u32     inten_rise3_shift;
	u32     inten_rise4_shift;
	u32     inten_rise5_shift;
	u32     inten_rise6_shift;
	u32     inten_rise7_shift;
	u32     inten_fall0_shift;
	u32     inten_fall1_shift;
	u32     inten_fall2_shift;
	u32     inten_fall3_shift;
	u32     inten_fall4_shift;
	u32     inten_fall5_shift;
	u32     inten_fall6_shift;
	u32     inten_fall7_shift;

	u32	tmu_intclear;

	u32	emul_con;
	u32	emul_temp_shift;
	u32	emul_time_shift;

	u32	tmu_irqstatus;
	u32	tmu_pmin;
};

/**
 * struct exynos_tmu_platform_data
 * @threshold: basic temperature for generating interrupt
 *	       25 <= threshold <= 125 [unit: degree Celsius]
 * @threshold_falling: differntial value for setting threshold
 *		       of temperature falling interrupt.
 * @trigger_levels: array for each interrupt levels
 *	[unit: degree Celsius]
 *	0: temperature for trigger_level0 interrupt
 *	   condition for trigger_level0 interrupt:
 *		current temperature > threshold + trigger_levels[0]
 *	1: temperature for trigger_level1 interrupt
 *	   condition for trigger_level1 interrupt:
 *		current temperature > threshold + trigger_levels[1]
 *	2: temperature for trigger_level2 interrupt
 *	   condition for trigger_level2 interrupt:
 *		current temperature > threshold + trigger_levels[2]
 *	3: temperature for trigger_level3 interrupt
 *	   condition for trigger_level3 interrupt:
 *		current temperature > threshold + trigger_levels[3]
 * @trigger_type: defines the type of trigger. Possible values are,
 *	THROTTLE_ACTIVE trigger type
 *	THROTTLE_PASSIVE trigger type
 *	SW_TRIP trigger type
 *	HW_TRIP
 * @trigger_enable[]: array to denote which trigger levels are enabled.
 *	1 = enable trigger_level[] interrupt,
 *	0 = disable trigger_level[] interrupt
 * @max_trigger_level: max trigger level supported by the TMU
 * @non_hw_trigger_levels: number of defined non-hardware trigger levels
 * @gain: gain of amplifier in the positive-TC generator block
 *	0 < gain <= 15
 * @reference_voltage: reference voltage of amplifier
 *	in the positive-TC generator block
 *	0 < reference_voltage <= 31
 * @noise_cancel_mode: noise cancellation mode
 *	000, 100, 101, 110 and 111 can be different modes
 * @type: determines the type of SOC
 * @efuse_value: platform defined fuse value
 * @min_efuse_value: minimum valid trimming data
 * @max_efuse_value: maximum valid trimming data
 * @first_point_trim: temp value of the first point trimming
 * @second_point_trim: temp value of the second point trimming
 * @default_temp_offset: default temperature offset in case of no trimming
 * @test_mux; information if SoC supports test MUX
 * @triminfo_reload: reload value to read TRIMINFO register
 * @cal_type: calibration type for temperature
 * @freq_clip_table: Table representing frequency reduction percentage.
 * @freq_tab_count: Count of the above table as frequency reduction may
 *	applicable to only some of the trigger levels.
 * @registers: Pointer to structure containing all the TMU controller registers
 *	and bitfields shifts and masks.
 * @features: a bitfield value indicating the features supported in SOC like
 *	emulation, multi instance etc
 *
 * This structure is required for configuration of exynos_tmu driver.
 */
struct exynos_tmu_platform_data {
	char *tmu_name;
	int ect_hotplug_flag;
	int ect_hotplug_interval;

	u32 temp_mask;
	u8 threshold;
	u8 threshold_falling;
	u8 trigger_levels[MAX_TRIP_COUNT];
	enum trigger_type trigger_type[MAX_TRIP_COUNT];
	bool trigger_enable[MAX_TRIP_COUNT];
	u8 max_trigger_level;
	u8 non_hw_trigger_levels;
	u8 gain;
	u8 reference_voltage;
	u8 noise_cancel_mode;

	u32 efuse_value;
	u32 min_efuse_value;
	u32 max_efuse_value;
	u8 first_point_trim;
	u8 second_point_trim;
	u8 default_temp_offset;
	u8 test_mux;
	u8 triminfo_reload[MAX_TRIMINFO_CTRL_REG];
	bool hotplug_enable;
	u32 hotplug_in_threshold;
	u32 hotplug_out_threshold;

	enum calibration_type cal_type;
	enum soc_type type;
	enum dev_type d_type;
	struct freq_clip_table freq_tab[MAX_TRIP_COUNT];
	unsigned int freq_tab_count;
	const struct exynos_tmu_registers *registers;
	unsigned int features;

	u8 sensor_type;
};

/**
 * struct exynos_tmu_init_data
 * @tmu_count: number of TMU instances.
 * @tmu_data: platform data of all TMU instances.
 * This structure is required to store data for multi-instance exynos tmu
 * driver.
 */
struct exynos_tmu_init_data {
	int tmu_count;
	struct exynos_tmu_platform_data tmu_data[];
};

#endif /* _EXYNOS_TMU_H */
