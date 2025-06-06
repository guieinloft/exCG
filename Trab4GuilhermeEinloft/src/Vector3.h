#ifndef __VECTOR_3_H__
#define __VECTOR_3_H__


class Vector3
{
public:
   float x, y, z;

   Vector3()
   {
      x = y = z = 0;
   }

   Vector3(float _x, float _y, float _z)
   {
       x = _x;
       y = _y;
       z = _z;
   }

   void set(float _x, float _y, float _z)
   {
       x = _x;
       y = _y;
       z = _z;
   }

   void normalize()
   {
       float norm = (float)sqrt(x*x + y*y + z*z);

       if(norm==0.0)
       {
          printf("\n\nNormalize::Divisao por zero");
          x = 1;
          y = 1;
          z = 1;
          return;
       }
       x /= norm;
       y /= norm;
   }

   float norm() {
       return (float)sqrt(x*x + y*y + z*z);
   }

   Vector3 operator - (const Vector3& v)
   {
        Vector3 aux( x - v.x, y - v.y, z - v.z);
        return( aux );
   }

   Vector3 operator + (const Vector3& v)
   {
       Vector3 aux( x + v.x, y + v.y, z + v.z);
       return( aux );
   }

   //Adicionem os demais overloads de operadores aqui.
   Vector3 operator += (const Vector3& v)
   {
       x += v.x;
       y += v.y;
       z += v.z;
       return *this;
   }

   Vector3 operator -= (const Vector3& v)
   {
       x -= v.x;
       y -= v.y;
       z -= v.z;
       return *this;
   }

   Vector3 operator * ( const float f ) {
       Vector3 aux ( x * f, y * f, z * f );
       return aux;
   }
   
   Vector3 operator *= ( const float f ) {
       x *= f;
       y *= f;
       z *= f;
       return *this;
   }

   Vector3 operator = ( const Vector3& v ) {
       x = v.x;
       y = v.y;
       z = v.z;
       return *this;
   }

   bool operator == ( const Vector3& v ) {
       return x == v.x && y == v.y && z == v.z;
   }

   bool operator != ( const Vector3& v ) {
       return x != v.x || y != v.y || z != v.z;
   }


};

#endif
