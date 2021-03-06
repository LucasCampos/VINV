/*
 *     This file is part of VINV.
 *
 *     VINV is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program; if not, write to the Free Software
 *     Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 * */
#ifndef COLOR_PICKER_H
#define COLOR_PICKER_H
struct Color {
	int red, blue, green;
	Color(): red(0), blue(0), green(0) {};
	Color(int r, int g, int b): red(r), blue(b), green(g) {};
};

class ColorPicker{
	static int current;
	public:
	static Color getColor() {

		switch(current) {
			case 0:
				current++;
				return Color(255,0,0);
				break;
			case 1: 
				current++;
				return Color(0,255,.0);
				break;
			case 2: 
				current++;
				return Color(0,0,255);
				break;
			default: return Color (rand()%255, rand()%255, rand()%255);
		}
	}
};

int ColorPicker::current = 0;

#endif
