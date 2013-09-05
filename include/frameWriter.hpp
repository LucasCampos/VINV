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

#ifndef FRAMEWRITER_HPP
#define FRAMEWRITER_HPP

#include <vector>
#include <cmath>
#include <iostream>
#include <string>
#include "vectorND.hpp"

/*
 *
 * General interface. One cannot instantiate a FrameWriter
 *
 * */

struct FrameWriter {

	public: 
	virtual double NormalizingFactorX() const = 0;
	virtual double CenterX() const =0;
	virtual double NormalizingFactorY() const = 0;
	virtual double CenterY() const =0;
	
	virtual void StartFrame()=0;
	virtual void EndFrame()=0;

	virtual void line(double xfrom, double yfrom, double xto, double yto, double width, int red, int green, int blue)=0;
	virtual void multiline (const std::vector<double>& x, const std::vector<double>& y, double width, int red, int green, int blue)=0;

	virtual void triangle(double x1, double y1, double x2, double y2, double x3, double y3, int red, int green, int blue)=0;
	virtual void filledTriangle(double x1, double y1, double x2, double y2, double x3, double y3, int red, int green, int blue)=0;

	virtual void square(double xfrom, double yfrom, double xto, double yto, int red, int green, int blue)=0;
	virtual void filledSquare(double xfrom, double yfrom, double xto, double yto, int red, int green, int blue)=0;

	virtual void circle(double xcentre, double ycentre, double radius, int red, int green, int blue)=0;
	virtual void filledCircle(double xcentre, double ycentre, double radius, int red, int green, int blue)=0;

	virtual void writeText(double x, double y, std::string name, double fontSize, int red, int green, int blue) =0;

	virtual void printname()=0;
	virtual void close()=0;
};


#endif
