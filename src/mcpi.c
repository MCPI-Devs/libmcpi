/*
 * mcpi.c
 * 
 * Copyright 2020 Alvarito050506 <donfrutosgomez@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; version 3 of the License.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * Part of this code is based in code generated by re2c 1.1.1.
 * 
 */

#include <stdio.h>
#include <mcpi.h>

mcpi_command_t parse_command(char* string, mcpi_err_t* err)
{
	char y;
	int i = 0, j = 0, k = 0;
	int arg_start = 0;
	int arg_end = 0;
	mcpi_command_t command;
	y = *string;
	switch (y)
	{
		case '(':
		case ')':
		case '.':
		case '{':
		case '}':
		case '[':
		case ']':
		case '=':
		case ',':
			goto error;
		default:
			goto package;
	}

package:
	y = *++string;
	i++;
	switch (y)
	{
		case '(':
		case ')':
		case '{':
		case '}':
		case '[':
		case ']':
		case '=':
		case ',':
			goto error;
		case '.':
			command.package[i] = '\x00';
			i = 0;
			goto func;
		default:
			command.package[i - 1] = *(string - 1);
			command.package[i] = y;
			goto package;
	}

func:
	y = *++string;
	i++;
	switch (y)
	{
		case '(':
			arg_start = i + 1;
			arg_end = i + 1;
			command.name[i - 1] = '\x00';
			goto arg;
		default:
			command.name[i - 1] = y;
			goto func;
	}

arg:
	y = *++string;
	switch (y)
	{
		case '(':
		case '{':
		case '}':
		case '[':
		case ']':
		case '=':
			goto error;
		case ')':
			command.args[j][k] = '\x00';
			goto final;
		case ',':
			goto comma;
		default:
			arg_end++;
			command.args[j][k] = y;
			k++;
			goto arg;
	}

comma:
	y = *++string;
	switch (y)
	{
		case '(':
		case ')':
		case '.':
		case '{':
		case '}':
		case '[':
		case ']':
		case '=':
		case ',':
			goto error;
		default:
			y = *--string;
			command.args[j][k] = '\x00';
			j++;
			k = 0;
			goto arg;
	}

final:
	command.argc = j + 1;
	return command;

error:
	err->code = 1;
	err->pos = i;
	err->chr = y;
	return command;
}
