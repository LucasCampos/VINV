/*
 *     This file is part of POPS.
 *
 *     POPS is free software; you can redistribute it and/or modify
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
#ifndef POLYREADER_HPP
#define POLYREADER_HPP
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "imageWriter.hpp"
#include "polygon.hpp"

/*
 *
 * This defines a class to handle the handling of data
 *
 */

typedef std::shared_ptr<std::ifstream>  IFStreamPtrType;
typedef std::shared_ptr<DrawablePolygon>  DrawablePolygonPtrType;

struct PolyReader {
	int _pointsPerPoly;
	IFStreamPtrType _in;
	bool _jump;
	std::vector<DrawablePolygonPtrType> _poly;

	PolyReader(int points, char* filename, bool jump, std::vector<DrawablePolygonPtrType> poly): 
		_pointsPerPoly(points), _in(new std::ifstream(filename)), _poly(poly) {
	};

	//Read the next block of data. Commonly, it will be called only once
	void updatePoly() {
		int size = _poly.size();
		for (int i=0; i<size; i++) {
			
			double x,y;
			(*_in) >> x >> y;

			//std::cout << i << "   " <<  charLeitor << "\n";
			
			bool notEnd = ((x != 0) && (y != 0)) &&  !(_in->eof());
			//notEnd = true;
			if (notEnd) {
				_poly[i]->_pos = Vector2D(x,y);
				_poly[i]->_vertex.clear();
			}
			for (int j =0; j<_pointsPerPoly; j++) {
				*_in >> x >> y;
				if(notEnd) 
					_poly[i]->_vertex.push_back(Vector2D(x,y));
			}
		}
	};

	//Call the drawing method of each polygon
	void draw(ImageWriter *writer) {
		int size = _poly.size();
		//cout << size << endl;
		for (int i=0; i<size; i++) 
			_poly[i]->draw(writer);
	};

	//Rescale each polygon by a constant factor
	void rescale (const double factor) {
		int size = _poly.size();
		//cout << size << endl;
		for (int i=0; i<size; i++) 
			_poly[i]->rescale(factor);
	};

	void translate (const Vector2D T) {
		int size = _poly.size();
		//cout << size << endl;
		for (int i=0; i<size; i++) 
			_poly[i]->translate(T);
	};

	//Draw lines between all particles that are within a certain distance
	void drawLines(ImageWriter *writer, std::vector<DrawablePolygonPtrType> p, const double distance, const double width) {

		int nHere = _poly.size();
		int nOther = p.size();

		for (int i=0; i<nHere; i++) {
			for(int j=0; j<nOther; j++) {

				Vector2D r =_poly[i]->_pos-(p[j]->_pos);
				//cout << r.norm() << " " << distance << std::endl;
				double norm = r.norm();
				if ((norm < distance) && (norm > 1e-5))
					writer->line(_poly[i]->_pos.getX(), _poly[i]->_pos.getY(), p[j]->_pos.getX(), p[j]->_pos.getY(), width, 0,0,0);



			}
		}
	};
};


#endif
