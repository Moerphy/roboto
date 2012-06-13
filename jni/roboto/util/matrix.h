#ifndef ROBOTO_MATRIX_H
#define ROBOTO_MATRIX_H

namespace roboto{
  
  class Matrix3D{
    private:
      float elements[16];

    public:
      Matrix3D();
      void scale(float sx, float sy, float sz);
      void translate(float tx, float ty, float tz);
      void rotate(float angle, float x, float y, float z);
      void multiply(Matrix3D* right);
      void set( Matrix3D* m );
      void loadIdentity();
      
      float* get();
  };
}

#endif
