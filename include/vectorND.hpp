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
#ifndef VECTORND_H
#define VECTORND_H

#include <cmath>

/*
 *
 * Defines 2D and 3D algebric vectors and its 
 * operations.
 *
 */

class Vector2D
	{
	private:
		double x,y;
	public:
		Vector2D(void){x = y = 0.0;}
		Vector2D(double a,double b){x = a;y = b;}
		double norm (void) const {return(sqrt(x*x + y*y));}
		double norm2(void) const {return (x*x + y*y);}
		void scalar(double a){x*=a;y*=a;}
		double getX() const {return (x);}
		double getY() const {return (y);}
		void setX(double a){x = a;}
		void setY(double b){y = b;}
		void normalize() { double vecNorm = norm(); x/=vecNorm; y/=vecNorm;};

		void rotate (double angle);

		Vector2D operator+(const Vector2D & vecB) const;
		Vector2D operator-(const Vector2D & vecB) const;
		Vector2D & operator=(const Vector2D & vecB);
		Vector2D & operator+=(const Vector2D & vecB);
		Vector2D & operator-=(const Vector2D & vecB);
		
		friend double dotProd(Vector2D A,Vector2D B);
		friend Vector2D operator*(double a,const Vector2D & vecB);				
	};

class Vector3D
	{
	private: 
		double x,y,z;
	public:
		Vector3D(void){x=y=z=0.0;}
		Vector3D(double x1,double x2,double x3){x=x1;y=x2;z=x3;}
		double norm (void) const{return(sqrt(x*x+y*y+z*z));}
		double norm2 (void)  const{return (x*x+y*y+z*z);}
		void scalar(double a){x*=a;y*=a;z*=a;}
		double getX() const {return x;}
		double getY() const {return y;}
		double getZ() const {return z;}
		void setX(double a){x = a;}
		void setY(double b){y = b;}
		void setZ(double c){z = c;}
		Vector3D operator+(const Vector3D & vecB) const;
		Vector3D operator-(const Vector3D & vecB) const;
		Vector3D operator^(const Vector3D & vecB) const;
		Vector3D & operator=(const Vector3D & vecB);
		Vector3D & operator+=(const Vector3D & vecB);
		Vector3D & operator-=(const Vector3D & vecB);
		friend Vector3D operator*(double a,const Vector3D & vecB);
		friend double dotProd(Vector3D A,Vector3D B);
};

//=======================================================================
//Methods of the Vector2D class
//=======================================================================

void Vector2D :: rotate(double angle) {
	Vector2D R = *this;

	x = R.x*cos(angle) -R.y*sin(angle);
	y = R.y*cos(angle) +R.x*sin(angle);

}

Vector2D Vector2D ::  operator+(const Vector2D & VectorB) const
{
	Vector2D C;
	C.x = x + VectorB.x;
	C.y = y + VectorB.y;
	return (C);
}

Vector2D Vector2D :: operator-(const Vector2D & VectorB) const
{
	Vector2D C;
	C.x = x - VectorB.x;
	C.y = y - VectorB.y;
	return (C);
}

Vector2D  & Vector2D :: operator=(const Vector2D & VectorB)
{
	x = VectorB.x;
	y = VectorB.y;
	return (*this);
}

Vector2D & Vector2D :: operator+=(const Vector2D & vecB)
{
	x+=vecB.x;
	y+=vecB.y;
	return(*this);
}

Vector2D & Vector2D :: operator-=(const Vector2D & vecB)
{
	x+=vecB.x;
	y+=vecB.y;
	return(*this);
}


Vector2D operator*(double a,const Vector2D & vecB)//Method friend
{
	Vector2D T;
	T.x = a*vecB.x;
	T.y = a*vecB.y;
	return(T);
}

double dotProd (Vector2D A, Vector2D B)//Method friend
{
	double c = A.x*B.x;
	c+=(A.y*B.y);
	return (c);
}

//=============================================================
//Methods of the Vector3D CLASS
//============================================================
Vector3D Vector3D :: operator+(const Vector3D & vecB) const
{
 	Vector3D C;
	C.x = x + vecB.x;     
	C.y = y + vecB.y;
	C.z = z + vecB.z;
	return (C);
}

Vector3D Vector3D :: operator-(const Vector3D & vecB) const{
	
	Vector3D C;
	C.x = x - vecB.x;     
	C.y = y - vecB.y;
	C.z = z - vecB.z;
	return (C);
}

//Determina o produto Vetorial!!!!!!!!	
Vector3D Vector3D :: operator^(const Vector3D & vecB) const
{
	Vector3D C;
	C.x = ((y*vecB.z) - (z*vecB.y));
	C.y = ((z*vecB.x) - (x*vecB.z));
	C.z = ((x*vecB.y) - (y*vecB.x));
	return (C);
}

Vector3D  & Vector3D :: operator=(const Vector3D & VectorB)
{
	x = VectorB.x;
	y = VectorB.y;
	z = VectorB.z;
	return (*this);
}

Vector3D & Vector3D :: operator+=(const Vector3D & vecB)
{
	x+=vecB.x;
	y+=vecB.y;
	z+=vecB.z;
	return(*this);
}

Vector3D & Vector3D :: operator-=(const Vector3D & vecB)
{
	x+=vecB.x;
	y+=vecB.y;
	z+=vecB.z;
	return(*this);
}

Vector3D operator*(double a,const Vector3D & vecB)
{
	Vector3D T;
	T.x = a*vecB.x;
	T.y = a*vecB.y;
	T.z = a*vecB.z;
	return(T);
}

double dotProd (Vector3D A, Vector3D B)//Method friend
{
	double c = A.x*B.x;
	c+=(A.y*B.y);
	c+=(A.z*B.z);
	return (c);
}

#endif
