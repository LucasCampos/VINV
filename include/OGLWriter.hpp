#ifndef OGLWRITER_HPP
#define OGLWRITER_HPP

#include "frameWriter.hpp"
#include "VideoRecorder.hpp"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include <iostream>
#include <cstdlib>

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
typedef Bool (*glXMakeContextCurrentARBProc)(Display*, GLXDrawable, GLXDrawable, GLXContext);
static glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
static glXMakeContextCurrentARBProc glXMakeContextCurrentARB = 0;

struct OGLWriter: public FrameWriter {

	private:

		VideoRecorder vr;

		void ChangeColor(int r, int g, int b) {
			if (!IsCurr(r,g,b)) {
				float rf = r/255.0;
				float gf = g/255.0;
				float bf = b/255.0;
				std::cout << rf << std::endl;
				glColor3f(rf,gf,bf);
				cRed = r;
				cGreen = g;
				cBlue = b;
			}
		}

		bool IsCurr(int r, int g, int b) const {
			bool t = r==cRed;
			t &= g==cGreen;
			t &= g==cBlue;
			return t;
		}

		void OpenGLContext(int resolutionX, int resolutionY) {

			static int visual_attribs[] = {
				None
			};
			int context_attribs[] = {
				GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
				GLX_CONTEXT_MINOR_VERSION_ARB, 0,
				None
			};

			Display* dpy = XOpenDisplay(0);
			int fbcount = 0;
			GLXFBConfig* fbc = NULL;
			GLXContext ctx;
			GLXPbuffer pbuf;

			/* open display */
			if ( ! (dpy = XOpenDisplay(0)) ){
				std::cerr << "Failed to open display" << std::endl;
				exit(1);
			}

			/* get framebuffer configs, any is usable (might want to add proper attribs) */
			if ( !(fbc = glXChooseFBConfig(dpy, DefaultScreen(dpy), visual_attribs, &fbcount) ) ){
				std::cerr << "Failed to get FBConfig" << std::endl;
				exit(1);
			}

			/* get the required extensions */
			glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB");
			glXMakeContextCurrentARB = (glXMakeContextCurrentARBProc)glXGetProcAddressARB( (const GLubyte *) "glXMakeContextCurrent");
			if ( !(glXCreateContextAttribsARB && glXMakeContextCurrentARB) ){
				std::cerr << "missing support for GLX_ARB_create_context" << std::endl;
				XFree(fbc);
				exit(1);
			}

			/* create a context using glXCreateContextAttribsARB */
			if ( !( ctx = glXCreateContextAttribsARB(dpy, fbc[0], 0, True, context_attribs)) ){
				std::cerr << "Failed to create opengl context" << std::endl;
				XFree(fbc);
				exit(1);
			}

			/* create temporary pbuffer */
			int pbuffer_attribs[] = {
				GLX_PBUFFER_WIDTH, resolutionX,
				GLX_PBUFFER_HEIGHT, resolutionY,
				None
			};
			pbuf = glXCreatePbuffer(dpy, fbc[0], pbuffer_attribs);

			XFree(fbc);
			XSync(dpy, False);

			/* try to make it the current context */
			if ( !glXMakeContextCurrent(dpy, pbuf, pbuf, ctx) ){
				/* some drivers does not support context without default framebuffer, so fallback on
				 * using the default window.
				 */
				if ( !glXMakeContextCurrent(dpy, DefaultRootWindow(dpy), DefaultRootWindow(dpy), ctx) ){
					std::cerr << "failed to make current" << std::endl;
					exit(1);
				}
			}

			/* try it out */
			std::cout << "vendor: " << (const char*)glGetString(GL_VENDOR) << std::endl;

		}

	public:
		double right;
		double left;
		double up;
		double down;

		int cRed;
		int cGreen;
		int cBlue;

		OGLWriter(std::string name, std::string codec, int framerate, int resolutionX, int resolutionY, double right_, double left_, double up_, double down_): right(right_), left(left_), up(up_), down(down_), cRed(0), cGreen(0), cBlue(0), vr(VideoRecorder(name, codec, framerate, resolutionX, resolutionY)){

			
			OpenGLContext(resolutionX, resolutionY);
			glMatrixMode(GL_PROJECTION);
			glOrtho (right_, down_, left_, up_, 0, 1);

		}

		double NormalizingFactorX() const { return 1;}
		double CenterX() const { return (right-left)/2;}
		double NormalizingFactorY() const { return 1;}
		double CenterY() const { return (up-down)/2;}

		void StartFrame() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

		void EndFrame() {
			vr.GetFrame();
		}

		void line(double xfrom, double yfrom, double xto, double yto, double width, int red, int green, int blue) {
			glLineWidth(width);
			ChangeColor(red,green,blue);

			glBegin(GL_LINES);
			glVertex2d(xfrom, yfrom);
			glVertex2d(xto, yto);
			glEnd();
		}

		void multiline (const std::vector<double>& x, const std::vector<double>& y, double width, int red, int green, int blue) {

			glLineWidth(width);
			ChangeColor(red,green,blue);

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
			ChangeColor(red,green,blue);
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

			ChangeColor(red,green,blue);
			glBegin(GL_QUADS);
			glVertex2d(xfrom,yfrom);
			glVertex2d(xto  ,yfrom);
			glVertex2d(xto  ,yto);
			glVertex2d(xfrom,yto);
			glEnd();

		}

		void circle(double xcentre, double ycentre, double radius, int red, int green, int blue) {
			glPolygonMode(GL_FRONT, GL_LINE);
			glPolygonMode(GL_BACK, GL_LINE);

			filledCircle(xcentre, ycentre, radius, red,green,blue);

			glPolygonMode(GL_FRONT, GL_FILL);
			glPolygonMode(GL_BACK, GL_FILL);

		}
		void filledCircle(double xcentre, double ycentre, double radius, int red, int green, int blue) {

			ChangeColor(red,green,blue);
			glBegin( GL_TRIANGLE_FAN );
			glVertex2f(xcentre, ycentre);
			const double segments=20;
			//std::cout << "radius: " << radius << std::endl;
			double const factor = 2*M_PI/segments;
			for( int n = 0; n <= segments; n++ ) {
				const double angle = n*factor;
				glVertex2f(xcentre + sin(angle)*radius, ycentre + cos(angle)*radius);
			}
			glEnd();

		}

		void writeText(double x, double y, std::string name, double fontSize, int red, int green, int blue){}

		void printname(){std::cout << "OpenGL FrameWriter" << std::endl;}
		void close(){}
};

#endif
