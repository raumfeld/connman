/*
 *
 *  Connection Manager
 *
 *  Copyright (C) 2007  Intel Corporation. All rights reserved.
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/stat.h>

#include <gdbus.h>

#include "connman.h"

static GMainLoop *main_loop = NULL;

static void sig_term(int sig)
{
	g_main_loop_quit(main_loop);
}

int main(int argc, char *argv[])
{
	DBusConnection *conn;
	struct sigaction sa;

	mkdir(STATEDIR, S_IRUSR | S_IWUSR | S_IXUSR |
			S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);

	main_loop = g_main_loop_new(NULL, FALSE);

	conn = g_dbus_setup_bus(DBUS_BUS_SYSTEM, CONNMAN_SERVICE);
	if (conn == NULL) {
		fprintf(stderr, "Can't register with system bus\n");
		exit(1);
	}

	__connman_manager_init(conn);

	__connman_plugin_init();

	__connman_iface_init(conn);

	__connman_rtnl_init();

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = sig_term;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);

	g_main_loop_run(main_loop);

	__connman_rtnl_cleanup();

	__connman_iface_cleanup();

	__connman_plugin_cleanup();

	__connman_manager_cleanup();

	g_dbus_cleanup_connection(conn);

	g_main_loop_unref(main_loop);

	rmdir(STATEDIR);

	return 0;
}
