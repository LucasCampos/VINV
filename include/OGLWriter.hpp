#ifndef OGLWRITER_HPP
#define OGLWRITER_HPP

#include "FrameWriter.hpp"
#include "VideoRecorder.hpp"
#include <GL/gl.h>

struct OGLWriter: public FrameWriter {

	private:

		VideoRecorder vr;

		void ChangeColor(int r, int g, int b) {
			if (!IsCurr(r,g,b))
				glColor3i(r,g,b);
		}

		bool IsCurr(int r, int g, int b) const {
			bool t = r==cRed;
			t &= g==cGreen;
			t &= g==cBlue;
			return t;
		}

	public:
		int cRed;
		int cGreen;
		int cBlue;

		OGLWriter(std::string name, std::string codec, int framerate, int resolutionX, int resolutionY, double right_, double left_, double up_, double down_) {

			vr.open(name, codec, framerate, resolutionX, resolutionY);

		}

		double NormalizingFactorX() { return 1;}
		double CenterX() { return (right-left)/2;}
		double NormalizingFactorY() { return 1;}
		double CenterY() { return (up-down)/2;}

		void StartFrame() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void EndFrame() {
			vr.GetFrame();
		}

		void line(double xfrom, double yfrom, double xto, double yto, double width, int red, int green, int blue) {
			glLineWidth(width);
			glChangeColour(red,green,blue);

			glBegin(GL_LINES);
			glVertex2d(xfrom, yfrom);
			glVertex2d(xto, yto);
			glEnd();
		}

		void multiline (const std::vector<double>& x, const std::vector<double>& y, double width, int red, int green, int blue) {

			glLineWidth(width);
			glColor3i(red,green,blue);

			glBegin(GL_LINE_STRIP);
			const int N = std::min(x.size(),y.size());
			for (int i=0; i<N; i++) {
				glVertex2d(x[i],y[i]);
			}
			glEnd();
		}

		void triangle(double x1, double y1, double x2, double y2, double x3, double y3, int red, int green, int blue) {

			glPolygonMode(GL_FRONT, GL_LINE);
			glPolygonMode(GL_BACK, GL_LINE);

			filledTriangle(x1,y1,x2,y2,x3,y3,red,green,blue);

			glPolygonMode(GL_FRONT, GL_FILL);
			glPolygonMode(GL_BACK, GL_FILL);
		}

		void filledTriangle(double x1, double y1, double x2, double y2, double x3, double y3, int red, int green, int blue) {
			glBegin(GL_TRIANGLES);
			glVertex2d(x1,y1);
			glVertex2d(x2,y2);
			glVertex2d(x3,y3);
			glEnd();
		}

		void square(double xfrom, double yfrom, double xto, double yto, int red, int green, int blue) {
			glPolygonMode(GL_FRONT, GL_LINE);
			glPolygonMode(GL_BACK, GL_LINE);

			filledSquare(xfrom,yfrom,xto,yto,red,green,blue);

			glPolygonMode(GL_FRONT, GL_FILL);
			glPolygonMode(GL_BACK, GL_FILL);

		}

		void filledSquare(double xfrom, double yfrom, double xto, double yto, int red, int green, int blue) {

			glBegin(GL_QUADS);
			glVertex2d(xfrom,yfrom);
			glVertex2d(xto  ,yfrom);
			glVertex2d(xto  ,yto);
			glVertex2d(xfrom,to);
			glEnd();

		}

		void circle(double xcentre, double ycentre, double radius, int red, int green, int blue) {
			glPolygonMode(GL_FRONT, GL_LINE);
			glPolygonMode(GL_BACK, GL_LINE);

			filledCircle(xcentre, ycentre, red,green,blue);

			glPolygonMode(GL_FRONT, GL_FILL);
			glPolygonMode(GL_BACK, GL_FILL);

		}
		void filledCircle(double xcentre, double ycentre, double radius, int red, int green, int blue) {

			glBegin( GL_TRIANGLE_FAN );
			glVertex2f(xcentre, ycentre);
			double const factor = 2*M_PI/segments;
			for( int n = 0; n <= segments; n++ ) {
				const double angle = n*factor;
				glVertex2f(xcentre + sin(angle)*radius, ycentre + cos(angle)*radius);
			}
			glEnd();

		}

		void writeText(double x, double y, std::string name, double fontSize, int red, int green, int blue){}

		void printname(){std::cout << "OpenGL FrameWriter" << endl;}
		void close(){}
};

#endif
