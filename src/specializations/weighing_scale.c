/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */
/**
 * \file weighing_scale.c
 * \brief Weighing Scale Specialization implementation
 *
 * Copyright (C) 2010 Signove Tecnologia Corporation.
 * All rights reserved.
 * Contact: Signove Tecnologia Corporation (contact@signove.com)
 *
 * $LICENSE_TEXT:BEGIN$
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation and appearing
 * in the file LICENSE included in the packaging of this file; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 * $LICENSE_TEXT:END$
 *
 * \date Jun 14, 2010
 * \author Diego Bezerra
 */

#include <stdio.h>
#include "weighing_scale.h"
#include "src/util/bytelib.h"
#include "src/dim/nomenclature.h"
#include "src/dim/mds.h"
#include "src/util/log.h"

/** Measurement 1: Weight */
#define HANDLE_MEASUREMENT_1 1

/**
 * \defgroup WeighingScale Weighing Scale
 * \ingroup DeviceSpecializations
 * \brief Weighing Scale IEEE specialization
 *
 * In the context of personal health devices in this family of standards,
 * a weighing scale is a device that measures the body weight of a person
 * and, optionally, determines other physiological quantities (e.g., the
 * body mass index or the height of a person). Weighing scale devices
 * considered in this standard are typically placed on the floor with a person
 * stepping on the device to perform a weight measurement, with the result
 * being converted into mass internally of the device.
 *
 * In the personal health context, the body weight of a person is typically
 * not measured more frequently than twice a day.
 *
 * Weighing scale devices may use a variety of techniques for measuring
 * body weight. One typical method is to place several strain-gauge load
 * cells under the measurement plane to convert deformation into weight.
 *
 * The scale receives the user's weight. The measures are read from a Numeric
 * object and data are saved at numeric->compound_basic_nu_observed_value->value field.
 * The following example reads weight and simply prints it.
 *
 * \code
 *
 * struct MDS_object* obj1 = mds_get_object_by_handle(HANDLE_MEASUREMENT_1);
 *
 * if (obj1 != NULL && obj1->choice == MDS_OBJ_METRIC) {
 *
 *		if (obj1->u.metric.choice == METRIC_NUMERIC){
 *			struct Numeric* numeric = &obj1->u.metric.u.numeric;
 *
 *			if (numeric->simple_nu_observed_value > 0){
 *				unsigned char *unit_label = mds_get_unit_code_string(numeric->metric.unit_code);
 *				sprintf(str, "%s Weight (%s): %.2f,", str, unit_label, numeric->simple_nu_observed_value);
 *
 *				sprintf(str, "%s Measurement time: %2.X/%.2X/%.2X %.2X:%.2X:%.2X\n", str,
 *							numeric->metric.absolute_time_stamp.day,
 *							numeric->metric.absolute_time_stamp.month,
 *							numeric->metric.absolute_time_stamp.year,
 *							numeric->metric.absolute_time_stamp.hour,
 *							numeric->metric.absolute_time_stamp.minute,
 *							numeric->metric.absolute_time_stamp.second);
 *			}
 *		}
 * }
 *
 * \endcode
 *
 * @{
 */

/**
 *  Returns the standard configuration for <em>Weighing Scale</em> specialization (05DC).
 *  For more information about <em>Weighing Scale</em> specialization, see IEEE 11073-10415
 *  Standard (Section 6.4, page 7).
 *
 *  \return an StdConfiguration struct that represents the standard configuration (a StdConfiguration instance)
 *  for <em>Weighing Scale</em> specialization.
 */
static ConfigObjectList *weighting_scale_get_config_ID05DC()
{
	ConfigObjectList *std_object_list = malloc(sizeof(ConfigObjectList));
	std_object_list->count = 1;
	std_object_list->length = 44;
	std_object_list->value = malloc(sizeof(ConfigObject) * std_object_list->count);
	std_object_list->value[0].obj_class = MDC_MOC_VMO_METRIC_NU;
	std_object_list->value[0].obj_handle = 1;

	AttributeList *attr_list1 = malloc(sizeof(AttributeList));
	attr_list1->count = 4;
	attr_list1->length = 36;
	attr_list1->value = malloc(attr_list1->count * sizeof(AVA_Type));

	attr_list1->value[0].attribute_id = MDC_ATTR_ID_TYPE;
	attr_list1->value[0].attribute_value.length = 4;
	ByteStreamWriter *bsw = byte_stream_writer_instance(4);
	write_intu16(bsw, MDC_PART_SCADA);
	write_intu16(bsw, MDC_MASS_BODY_ACTUAL);
	attr_list1->value[0].attribute_value.value = bsw->buffer;

	attr_list1->value[1].attribute_id = MDC_ATTR_METRIC_SPEC_SMALL;
	attr_list1->value[1].attribute_value.length = 2;
	free(bsw);
	bsw = byte_stream_writer_instance(2);
	write_intu16(bsw, 0xF040); // 0xF0 0x40
	attr_list1->value[1].attribute_value.value = bsw->buffer;

	attr_list1->value[2].attribute_id = MDC_ATTR_UNIT_CODE;
	attr_list1->value[2].attribute_value.length = 2;
	free(bsw);
	bsw = byte_stream_writer_instance(2);
	write_intu16(bsw, MDC_DIM_KILO_G);
	attr_list1->value[2].attribute_value.value = bsw->buffer;

	attr_list1->value[3].attribute_id = MDC_ATTR_ATTRIBUTE_VAL_MAP;
	attr_list1->value[3].attribute_value.length = 12;
	free(bsw);
	bsw = byte_stream_writer_instance(12);
	write_intu16(bsw, 0x0002); // MetricIdList.count = 3
	write_intu16(bsw, 0x0008); // MetricIdList.length = 6
	write_intu16(bsw, MDC_ATTR_NU_VAL_OBS_SIMP);
	write_intu16(bsw, 0x0004); // value length = 4
	write_intu16(bsw, MDC_ATTR_TIME_STAMP_ABS);
	write_intu16(bsw, 0x0008); // value length = 8
	attr_list1->value[3].attribute_value.value = bsw->buffer;

	std_object_list->value[0].attributes = *attr_list1;

	free(attr_list1);
	free(bsw);

	return std_object_list;
}

/**
 *  Creates the standard configuration for <em>Weighing Scale</em> specialization (05DC).
 *  For more information about <em>Weighing Scale</em> specialization, see IEEE 11073-10415
 *  Standard (Section 6.4, page 7).
 *
 *  \return an StdConfiguration struct that represents the standard configuration (a StdConfiguration instance)
 *  for <em>Weighing Scale</em> specialization just created.
 */
struct StdConfiguration *weighting_scale_create_std_config_ID05DC()
{
	struct StdConfiguration *result = malloc(
			sizeof(struct StdConfiguration));
	result->dev_config_id = 0x05DC;
	result->configure_action = &weighting_scale_get_config_ID05DC;
	result->event_report = NULL;
	return result;
}

/** @} */
