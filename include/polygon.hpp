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
#ifndef POLYGON_H
#define POLYGON_H

#include "vectorND.hpp"
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include "imageWriter.hpp"

/*
 *
 * Creates a general polygon, a drawable polygon, 
 * and some kinds, such as circles and
 * squares.
 *
 */

class Polygon {

	public:
		Vector2D _pos;
		std::vector<Vector2D> _vertex;

		Polygon(Vector2D pos): _pos(pos){};
		Polygon(){};
		void addVertex(Vector2D vertex){
			_vertex.push_back(vertex);
		}

		void translate(const Vector2D& T) {
			_pos += T;
		}


		void rotate(double angle){
			const int nVertex =_vertex.size(); 
			for(int i=0; i<nVertex; i++) 
				_vertex[i].rotate(angle);
		}

		virtual void rescale(double factor) {
			_pos = factor * _pos;

			const int nVertex =_vertex.size(); 
			for(int i=0; i<nVertex; i++) 
				_vertex[i] = factor* _vertex[i];
		}


};

/*
 *
 * A drawable polygon. This cannot be intantiate unless the
 * draw method is overloaded.
 *
 * */

class DrawablePolygon: public Polygon {

	public:

		double _red, _green, _blue;

		DrawablePolygon(double red, double green, double blue): _red(red), _green(green), _blue(blue) {};
		DrawablePolygon(): _red(1.0), _green(1.0), _blue(1.0) {};

		virtual void draw(ImageWriter *writer)=0;
		
};

class Square: public DrawablePolygon {

	public:

		Square(double red, double green, double blue): DrawablePolygon (red,green,blue){

			addVertex(Vector2D(-.5,-.5));
			addVertex(Vector2D(-.5, .5));
			addVertex(Vector2D( .5, .5));
			addVertex(Vector2D( .5,-.5));
		}

		Square(double size,double red, double green, double blue): DrawablePolygon (red,green,blue) {
			addVertex(Vector2D(-size,-size));
			addVertex(Vector2D(-size, size));
			addVertex(Vector2D( size, size));
			addVertex(Vector2D( size,-size));
		}

		void draw(ImageWriter *writer) {
			
			int size = _vertex.size();
			
			Vector2D p1 = _vertex[0]+_pos;
			Vector2D p2 = _vertex[2]+_pos;

			writer -> filledSquare(p1.getX(), p1.getY(), p2.getX(), p2.getY(), _red, _green, _blue);
		};


};

class Triangle: public DrawablePolygon{

	public:
		Triangle(double red, double green, double blue): DrawablePolygon (red,green,blue) {
			Vector2D T(0,sqrt(3.0)/3.0);
			addVertex(T);
			T.rotate(2*M_PI/3.0);
			addVertex(T);
			T.rotate(2*M_PI/3.0);
			addVertex(T);
		}

		Triangle(double size,double red, double green, double blue): DrawablePolygon (red,green,blue) {
			Vector2D T(0,sqrt(3.0)/3.0*size);
			addVertex(T);
			T.rotate(2*M_PI/3.0);
			addVertex(T);
			T.rotate(2*M_PI/3.0);
			addVertex(T);
		}
		
		void draw(ImageWriter *writer) {

			Vector2D p1 = _vertex[0]+_pos;
			Vector2D p2 = _vertex[1]+_pos;
			Vector2D p3 = _vertex[2]+_pos;
			
			writer->filledTriangle(p1.getX(), p1.getY(), p2.getX(), p2.getY(),  p3.getX(), p2.getY(), _red, _green, _blue);
			
		};

};

class Circle: public DrawablePolygon {

	public:
		double _radius;

		Circle(double radius,double red, double green, double blue): DrawablePolygon (red,green,blue), _radius(radius) {};
		void draw(ImageWriter *writer){

			writer->filledCircle(_pos.getX(), _pos.getY(), _radius, _red, _green, _blue);


		};
		void rescale(const double factor) {

			_pos = factor*_pos;

			_radius *= factor;

		}
};

#endif
