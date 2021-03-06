/*
 * Name:	VINV - Plotter of VINV Screens
 * Author:	Lucas Costa Campos
 * Email: 	Rmk236@gmail.com
 * Version:	0.02
 * License:	GNU General Public License
 * 		Copyright: 2013 Lucas Costa Campos
 * Website: 	https://github.com/LucasCampos/VINV
 */

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
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "include/frameWriter.hpp"
#include "include/polygon.hpp"
#include "include/colorPicker.hpp"
#include "include/polyReader.hpp"
#include "include/OGLWriter.hpp"

using namespace std;

void Normalize(FrameWriter* writer, std::vector<PolyReader>& poly, bool extendedBox);
void TakeParameters(int argc, char* argv[], FrameWriter** writer, std::vector<PolyReader>& poly, std::vector<std::string>& toWrite, double& lineWidth, double& fontSize,  bool& drawDistance, double& minDist, bool& extendedBox);
void Draw(FrameWriter* writer, std::vector<PolyReader>& poly, std::vector<std::string>& toWrite, bool extendedBox, bool drawDistance, double minDist, double lineWidth, double fontSize);
void connectPoly(FrameWriter *writer, std::vector<PolyReader>& poly, double minDist, double lineWidth);
void SimpleHelp();

int main(int argc, char* argv[])
{
	std::vector<PolyReader> poly;
	std::vector<std::string> toWrite;
	bool extendedBox = true;
	bool drawDistance = false;
	double minDist = 1e9;
	double lineWidth = 0.1;
	double fontSize = 1.0;
	FrameWriter* writer;
	TakeParameters(argc, argv, &writer, poly, toWrite, lineWidth, fontSize, drawDistance, minDist, extendedBox);
	writer->printname();
	Draw(writer, poly, toWrite, extendedBox, drawDistance, minDist, lineWidth, fontSize);
	delete writer;
}

void Normalize(FrameWriter* writer, std::vector<PolyReader>& poly, bool extendedBox) {
	double factor = writer->NormalizingFactorX();
	double center = writer->CenterX();

	Vector2D T(center,center);
	for (unsigned i=0; i<poly.size(); i++){
		
		poly[i].rescale(factor);
		//if (extendedBox)
		//	poly[i].translate(T);
	}
};

void TakeParameters (int argc, char* argv[], FrameWriter** writer, std::vector<PolyReader>& poly, std::vector<std::string>& toWrite, double& lineWidth, double& fontSize, bool& drawDistance, double& minDist, bool& extendedBox){

	bool hasOne = false;
	std::string nameOutput;
	double box=10;
	double resolution=640;
	int framerate = 30;

	if (argc == 1) {
		SimpleHelp();
		exit(0);
	}

	for (int i=0;i<argc; i++){
		if (string(argv[i]) == "-s"){
			char* file = argv[i+1];
			int N = atoi(argv[i+2]);

			vector<DrawablePolygonPtrType> p;
			Color c = ColorPicker::getColor();

			for (int j=0; j<N; j++) {
				DrawablePolygonPtrType polygon(new Square(c.red, c.green,c.blue));
				p.push_back(polygon);
			}

			poly.push_back(PolyReader(4, file, true,p));

			hasOne = true;

		} else if (string(argv[i]) == "-t"){
			char* file = argv[i+1];
			int N = atoi(argv[i+2]);

			vector<DrawablePolygonPtrType> p;
			Color c = ColorPicker::getColor();

			for (int j=0; j<N; j++) {
				DrawablePolygonPtrType polygon(new Triangle(c.red, c.green, c.blue));
				p.push_back(polygon);
			}

			poly.push_back(PolyReader(3, file, true,p));

			hasOne = true;

		} else if (string(argv[i]) == "-c"){
			char* file = argv[i+1];
			int N = atoi(argv[i+2]);
			double radius = atof(argv[i+3]);

			vector<DrawablePolygonPtrType> p;
			Color c = ColorPicker::getColor();

			for (int j=0; j<N; j++) {
				DrawablePolygonPtrType polygon(new Circle(radius, c.red,c.green,c.blue));
				p.push_back(polygon);
			}

			poly.push_back(PolyReader(0, file, true,p));
	
			hasOne = true;

		} else if (string(argv[i]) == "-o") {

			if (argc <= i+1) {
				cout << "Incomplete parameters for -o \n";
				exit(1);
			}
			nameOutput= std::string(argv[i+1]);	
		} else if (string(argv[i]) == "-f") {

			if (argc <= i+1) {
				cout << "Incomplete parameters for -f \n";
				exit(1);
			}
			resolution = atoi(argv[i+1]);	
		} else if (string(argv[i]) == "-b") {

			if (argc <= i+1) {
				cout << "Incomplete parameters for -b \n";
				exit(1);
			}
			box = atof(argv[i+1]);	
		} else if (string(argv[i]) == "-r") {

			if (argc <= i+1) {
				cout << "Incomplete parameters for -r \n";
				exit(1);
			}

			drawDistance = true;
			minDist = atof(argv[i+1]);
		} else if (string(argv[i]) == "-w") {

			if (argc <= i+1) {
				cout << "Incomplete parameters for -r \n";
				exit(1);
			}

			lineWidth = atof(argv[i+1]);
		} else if (string(argv[i]) == "-h") {
			SimpleHelp();
			exit(0);
		} else if (string(argv[i]) == "-e"){
			extendedBox = false;
		} else if (string(argv[i]) == "-l") {
			if (argc <= i+1) {
				cout << "Incomplete parameters for -l\n";
				exit(1);
			}
			std::string label(argv[i+1]);
			toWrite.push_back(label);
		} else if (string(argv[i]) == "-fs") {
			if (argc <= i+1) {
				cout << "Incomplete parameters for -fs\n";
				exit(1);
			}
			fontSize = atof(argv[i+1]);
		} else if (string(argv[i]) == "-fr") {
			if (argc <= i+1) {
				cout << "Incomplete parameters for -fr\n";
				exit(1);
			}
			framerate = atoi(argv[i+1]);
		}
	}
	if (!hasOne) {
		SimpleHelp();
		exit(1);
	}

	std::string codec;
	if (nameOutput.substr(nameOutput.length()-4) == ".avi")
		codec = "MJPG";
	else{
		codec = "PIM1";
		if (!(nameOutput.substr(nameOutput.length()-4) == ".mkv"))
			nameOutput+=".mkv";
	}

	if (extendedBox)
		*writer = new OGLWriter(nameOutput, codec, framerate, resolution, resolution, -box, box, -box, box);
	else
		*writer = new OGLWriter(nameOutput, codec, framerate, resolution, resolution, 0, box, 0, box);


}

void Draw(FrameWriter* writer, std::vector<PolyReader>& poly, std::vector<std::string>& toWrite, bool extendedBox, bool drawDistance, double minDist, double lineWidth, double fontSize)
{
	bool end=true;
	do {
		writer->StartFrame();
		glColor3f(1.0,0.0,0.0);
		end=true;

		for (unsigned i=0; i<poly.size(); i++) 
			end &= !poly[i].updatePoly();

		Normalize(writer, poly, extendedBox);
		for (unsigned i=0; i<poly.size(); i++) 
			poly[i].draw(writer);

		if (drawDistance) 
			connectPoly(writer, poly, minDist, lineWidth);
		//glColor3f(1.0,0,0);
		//writer->filledSquare(-100, -100, 100, 100, 255,0,0);
		/*
		for (unsigned i=0; i<toWrite.size(); i++) {
			writer->writeText(.95*writer->left, (0.97-.05*fontSize - .1*fontSize*i)*writer->up, toWrite[i], fontSize, 0, 0, 0);
		}
		*/
		writer->EndFrame();
	} while (!end);
}

void connectPoly(FrameWriter *writer, std::vector<PolyReader>& poly, double minDist, double lineWidth){

	cout << "Connecting points\n";

	int N = poly.size();
	for (int i=0; i<N;i++){
		for (int j=0; j<N; j++) {
			poly[i].drawLines(writer, poly[j]._poly, minDist, lineWidth);
		}
	}
}


void SimpleHelp() {
	
	cout << "If you find any bug, please write to lqcc@df.ufpe.br\n\n";
	cout << "VINV: 0.02\n\n";	
	std::cout << "Commands: \n\n";
	std::cout << " -h\n\tShow this help dialog\n\n"; 
	std::cout << " -s filename Quantidade\n\tAdd a new kind of square, with Quantity squares. To each square, there must be five lines on the file.\n\n";
	std::cout << " -t filename Quantidade\n\tAdd a new kind of triangle, with Quantity triangles. To each triangle, there must be four lines on the file.\n\n";
	std::cout << " -c filename Quantidade Radius\n\tAdd a new kind of circle, with Quantity circles and a given radius. To each circle, there must be one line on the file.\n\n";
	std::cout << " -o Name\n\tOutput filename.\n\n";
	std::cout << " -f Frame\n\tChooses the resolution of the output file. If this flag is not set, the resolution will be 640x640.\n\n";
	std::cout << " -b Box\n\tBox size. If unset, the value will be 10.\n\n";
	std::cout << " -r Distance\n\tDesenha draw a line between particle centers' if they are close than Distance.\n\n";
	std::cout << " -e\nThe standard is having the box centred on (0,0). If you wish its left-down corner to be on (0,0), set this flag.\t\n\n";
	std::cout << " -w width\n\tChooses linewidth. The standard is 0.1.\n\n";
	std::cout << " -l name\n\tAdds a new label on the left-up side.\n\n";
	std::cout << " -fs size\n\tSelects the new font size.\n\n";
	std::cout << " -fr framerate\n\tSelects the framerate. The default is 30fps\n\n";
}
