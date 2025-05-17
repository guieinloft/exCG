#ifndef __VECTOR_2_H__
#define __VECTOR_2_H__

#include <stdio.h>
#include <math.h>

class Vector2
{
public:
   float x, y;

   Vector2()
   {
      x = y = 0;
   }

   Vector2(float _x, float _y)
   {
       x = _x;
       y = _y;
   }

   void set(float _x, float _y)
   {
       x = _x;
       y = _y;
   }

   void normalize()
   {
       float norm = (float)sqrt(x*x + y*y);

       if(norm==0.0)
       {
          printf("\n\nNormalize::Divisao por zero");
          x = 1;
          y = 1;
          return;
       }
       x /= norm;
       y /= norm;
   }

   float norm() {
       return (float)sqrt(x*x + y*y);
   }

   float dot(Vector2 b) {
	   return this->x * b.x + this->y * b.y;
   }

   Vector2 perp() {
	   Vector2 perp;
	   perp.set(-this->y, this->x);
	   return perp;
   }

   Vector2 operator - (const Vector2& v)
   {
        Vector2 aux( x - v.x, y - v.y);
        return( aux );
   }

   Vector2 operator + (const Vector2& v)
   {
       Vector2 aux( x + v.x, y + v.y);
       return( aux );
   }

   //Adicionem os demais overloads de operadores aqui.
   Vector2 operator += (const Vector2& v)
   {
       x += v.x;
       y += v.y;
       return *this;
   }

   Vector2 operator -= (const Vector2& v)
   {
       x += v.x;
       y += v.y;
       return *this;
   }

   Vector2 operator * ( const float f ) {
       Vector2 aux ( x * f, y * f );
       return aux;
   }
   
   Vector2 operator *= ( const float f ) {
       x *= f;
       y *= f;
       return *this;
   }

   Vector2 operator = ( const Vector2& v ) {
       x = v.x;
       y = v.y;
       return *this;
   }

   bool operator == ( const Vector2& v ) {
       return x == v.x && y == v.y;
   }

   bool operator != ( const Vector2& v ) {
       return x != v.x || y != v.y;
   }


};

#endif
