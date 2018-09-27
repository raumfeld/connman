/*
 *
 *  Connection Manager
 *
 *  Copyright (C) 2007-2013  Intel Corporation. All rights reserved.
 *  Copyright (C) 2016  Yann E. MORIN <yann.morin.1998@free.fr>. All rights reserved.
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <execinfo.h>
#include <dlfcn.h>

#include "connman.h"

#define UNW_LOCAL_ONLY
#include <libunwind.h>

static const char* get_location(void* addr)
{
	Dl_info info;

	if (dladdr(addr, &info))
	{
		if (info.dli_fname && *info.dli_fname)
		return info.dli_fname;
	}

	return NULL;
}

void print_backtrace(const char* program_path, const char* program_exec, unsigned int offset)
{
	unw_cursor_t  cursor;
	unw_context_t context;
	unw_word_t    pc;

	char   buffer[256];
	size_t buffer_size = sizeof(buffer);

	if (unw_getcontext(&context) != 0 || unw_init_local(&cursor, &context) != 0)
		return;

	int max_depth = 50;

	connman_error("+++++++++++++++++++++++++++");

	while (max_depth-- != 0 && unw_step(&cursor) > 0 && unw_get_reg(&cursor, UNW_REG_IP, &pc) == 0) {
		unw_word_t offset;
		int result = unw_get_proc_name(&cursor, buffer, buffer_size, &offset);

		if (-result == UNW_ENOMEM)
			result = 0;
		else if (result != 0)
			offset = 0;

		{
			const char* symbol = result == 0 ? buffer : "???";
			const char* location = get_location((void*) (pc - offset));
			connman_error("  %s +%d from %s\n", symbol, offset, location ?: "??");
		}
	}

	connman_error("+++++++++++++++++++++++++++");
}
