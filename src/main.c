/*
 * apXterm - A terminal emulator with SSH support
 * Copyright (C) 2025 arian-press2015 <arian.press2015@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "app.h"

int main(int argc, char **argv)
{
    GtkApplication *app = init_app();
    int status = run_app(app, argc, argv);
    clear_app(app);

    return status;
}