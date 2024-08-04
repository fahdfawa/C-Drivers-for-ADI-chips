/******************************************************************************
Copyright (c) 2022-2023 Analog Devices, Inc. All Rights Reserved. This software
is proprietary and confidential to Analog Devices, Inc. and its licensors.

By using this software you agree to the terms of the associated
Analog Devices Software License Agreement.
******************************************************************************/

/* MAX25432B IC1101_v0.1 */

#ifndef LIBTCPM_MAX25432_H_
#define LIBTCPM_MAX25432_H_

#include <stdbool.h>
#include <stdint.h>

#define TCPM_PART_NUM_LEN   6u

struct adi_tcpm_callbacks
{
    void (*i2c_read_block)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t data_size);
    void (*i2c_write_block)(uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint8_t data_size);
    bool (*gpio_read)(uint8_t pin_number);
};

struct adi_tcpm_version
{
    char part_number[TCPM_PART_NUM_LEN];
    uint8_t ver_major;
    uint8_t ver_minor;
};

struct adi_tcpm_src_caps
{
    uint16_t max_mv;
    uint16_t max_ma;
    bool pdo_type; /* FPDO or APDO */
};

typedef enum
{
    ADI_TCPM_STAT_OK,
    ADI_TCPM_STAT_CRC_ERR,
    ADI_TCPM_STAT_INVALID_CMD,
    ADI_TCPM_STAT_INVALID_LEN,
    ADI_TCPM_STAT_INVALID_PARAMS,
    ADI_TCPM_STAT_NO_AVAIL_DATA,
    ADI_TCPM_STAT_INVALID_PDOS,
} adi_tcpm_status_t;

typedef enum
{
    ADI_TCPM_DFP_VDO_HUB,
    ADI_TCPM_DFP_VDO_HOST,
    ADI_TCPM_DFP_VDO_BRICK,
} adi_tcpm_dfp_vdo_t;

void adi_tcpm_get_tcpm_version(struct adi_tcpm_version *tcpm_ver);

void adi_tcpm_set_callbacks(struct adi_tcpm_callbacks *tcpm_cb);
void adi_tcpm_set_module_ids(uint16_t vid, uint32_t xid, uint16_t pid, uint16_t bcd);
void adi_tcpm_set_port(uint8_t port_index, uint8_t tcpc_address, uint8_t alert_gpio, uint32_t max_pdp_mw, bool port_managed);
void adi_tcpm_set_port_power_gang(uint8_t port_index, uint8_t gang_index, bool master_port);
void adi_tcpm_timer_ms_irq(void);
void adi_tcpm_state_manager(void);

void adi_tcpm_set_fault_status_callback(void (*fault_status_callback)(uint8_t port_index, uint8_t reg));
void adi_tcpm_set_auto_shield_status_callback(void (*auto_shield_status_callback)(uint8_t port_index, uint16_t reg));
void adi_tcpm_set_vconn_stg_callback(void (*vconn_stg_callback)(uint8_t port_index));
void adi_tcpm_set_vbus_transition_callback(void (*vbus_transition_callback)(uint8_t port_index, uint16_t vbus_mv, bool trans_done));

adi_tcpm_status_t adi_tcpm_set_rp_current(uint8_t port_index, bool rp_3a);
adi_tcpm_status_t adi_tcpm_set_ilim_margin(uint8_t port_index, uint16_t ilim_margin_ma);
adi_tcpm_status_t adi_tcpm_set_ilim_offset(uint8_t port_index, uint16_t ilim_offset_ma);
adi_tcpm_status_t adi_tcpm_set_vppsshutdown_threshold(uint8_t port_index, uint16_t v_pps_shutdown_thresh_mv, uint8_t ir_drop_offset_mohm);
adi_tcpm_status_t adi_tcpm_set_cable_comp_gain(uint8_t port_index, uint8_t gain);
adi_tcpm_status_t adi_tcpm_set_slope_compensation(uint8_t port_index, uint8_t slp_0v_6v, uint8_t slp_6v_10v, uint8_t slp_10v_16v, uint8_t slp_16v);
adi_tcpm_status_t adi_tcpm_set_switching_frequency(uint8_t port_index, uint8_t switching_freq);
adi_tcpm_status_t adi_tcpm_set_sync_dir_out(uint8_t port_index, bool sync_output);
adi_tcpm_status_t adi_tcpm_set_spread_spectrum(uint8_t port_index, uint8_t spread_spectrum);
adi_tcpm_status_t adi_tcpm_set_auto_cdp_dcp_mode(uint8_t port_index, uint8_t mode);
adi_tcpm_status_t adi_tcpm_set_input_threshold(uint8_t port_index, uint8_t vin_thresh);
adi_tcpm_status_t adi_tcpm_set_vconn_threshold(uint8_t port_index, uint8_t oc_thresh, uint8_t uv_thresh);
adi_tcpm_status_t adi_tcpm_set_vbus_threshold(uint8_t port_index, uint8_t ov_thresh, uint8_t uv_thresh);
adi_tcpm_status_t adi_tcpm_set_fault_detection(uint8_t port_index, bool vbus_discharge_det_en, bool vbus_ocp_det_en, bool vbus_ovp_det_en, bool vconn_ocp_det_en);
adi_tcpm_status_t adi_tcpm_set_fault_retry_timer(uint8_t port_index, uint8_t retry_timer);
adi_tcpm_status_t adi_tcpm_set_vdm_disc_id_response(uint8_t port_index, bool disc_id_en, adi_tcpm_dfp_vdo_t dfp_type);
adi_tcpm_status_t adi_tcpm_set_sink_info_request(uint8_t port_index, bool sink_ids_en, bool sink_max_power_en);

adi_tcpm_status_t adi_tcpm_build_pdos(uint32_t *pdo, uint8_t *nr_pdo, uint32_t port_power_mw, bool usb_comm_cap, bool pps_en);
adi_tcpm_status_t adi_tcpm_build_pdos_all_5a(uint32_t *pdo, uint8_t *nr_pdo, uint32_t port_power_mw, bool usb_comm_cap, bool pps_en);
adi_tcpm_status_t adi_tcpm_build_pdos_custom(uint32_t *pdo, uint8_t *nr_pdo, uint32_t port_power_mw, bool usb_comm_cap, const struct adi_tcpm_src_caps src_caps[7], uint8_t src_caps_size);

adi_tcpm_status_t adi_tcpm_update_port_power(uint8_t port_index, uint32_t port_power_mw, bool apdos_en);
adi_tcpm_status_t adi_tcpm_update_source_caps(uint8_t port_index, uint32_t pdo[7], uint8_t nr_pdo);
adi_tcpm_status_t adi_tcpm_update_source_caps_info(uint8_t port_index, bool usb_comm_cap);
adi_tcpm_status_t adi_tcpm_get_source_caps(uint8_t port_index, uint32_t *pdo, uint8_t *nr_pdo);

adi_tcpm_status_t adi_tcpm_update_sync_dir_out(uint8_t port_index, bool sync_output);
adi_tcpm_status_t adi_tcpm_update_spread_spectrum(uint8_t port_index, uint8_t spread_spectrum);
adi_tcpm_status_t adi_tcpm_update_cdp_dcp_mode(uint8_t port_index, uint8_t mode);

adi_tcpm_status_t adi_tcpm_get_negotiated_voltage_mv(uint8_t port_index, uint16_t *volt_mv);
adi_tcpm_status_t adi_tcpm_get_negotiated_current_ma(uint8_t port_index, uint16_t *curr_ma);
adi_tcpm_status_t adi_tcpm_get_adc_voltage_mv(uint8_t port_index, uint16_t *volt_mv);
adi_tcpm_status_t adi_tcpm_get_adc_current_ma(uint8_t port_index, uint16_t *curr_ma);
adi_tcpm_status_t adi_tcpm_get_pdo_max_voltage_mv(uint8_t port_index, uint16_t *volt_mv);
adi_tcpm_status_t adi_tcpm_get_pdo_max_current_ma(uint8_t port_index, uint16_t *curr_ma);

adi_tcpm_status_t adi_tcpm_get_disabled_status(uint8_t port_index, bool *disabled_status);
adi_tcpm_status_t adi_tcpm_disable(uint8_t port_index);
adi_tcpm_status_t adi_tcpm_undisable(uint8_t port_index);

adi_tcpm_status_t adi_tcpm_get_typec_only_status(uint8_t port_index, bool *typec_only_status);
adi_tcpm_status_t adi_tcpm_set_typec_only(uint8_t port_index, bool typec_en);

adi_tcpm_status_t adi_tcpm_pd_hard_reset(uint8_t port_index);

adi_tcpm_status_t adi_tcpm_get_cc_attach_status(uint8_t port_index, bool *attach_status);
adi_tcpm_status_t adi_tcpm_get_cc_orientation(uint8_t port_index, bool *cc_flipped);
adi_tcpm_status_t adi_tcpm_has_explicit_contract(uint8_t port_index, bool *has_contract);
adi_tcpm_status_t adi_tcpm_get_pdo_index(uint8_t port_index, uint8_t *pdo_index);
adi_tcpm_status_t adi_tcpm_get_cap_mismatch_status(uint8_t port_index, bool *cap_mismatch_status);

adi_tcpm_status_t adi_tcpm_get_cable_emarker_status(uint8_t port_index, bool *emarker_status);
adi_tcpm_status_t adi_tcpm_get_cable_ids(uint8_t port_index, uint16_t *vid, uint16_t *pid);

adi_tcpm_status_t adi_tcpm_get_sink_ids(uint8_t port_index, uint16_t *vid, uint16_t *pid);
adi_tcpm_status_t adi_tcpm_get_sink_max_power(uint8_t port_index, uint32_t *sink_max_power);

#endif /* LIBTCPM_MAX25432_H_ */
