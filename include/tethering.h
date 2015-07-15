/*
 *
 *  Connection Manager
 *
 *  Copyright (C) 2007-2013  Intel Corporation. All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef __CONNMAN_TETHERING_H
#define __CONNMAN_TETHERING_H

#include <connman/device.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * SECTION:tethering
 * @title: tethering primitives
 * @short_description: Functions for handling tethering details
 */

int connman_tethering_get_target_index_for_device(struct connman_device *device);
bool connman_tethering_is_bridged_ap_mode_active();

#ifdef __cplusplus
}
#endif

#endif /* __CONNMAN_TETHERING_H */
