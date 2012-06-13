#include "matrix.h"

#include <math.h>

#define PI 3.141592653589793238462
//
// A simple 4x4 Matrix utility class
//
namespace roboto{
  
  Matrix3D::Matrix3D() {
    this->loadIdentity();
  }


  void Matrix3D::scale(float sx, float sy, float sz) {
    this->elements[0*4+0] *= sx;
    this->elements[0*4+1] *= sx;
    this->elements[0*4+2] *= sx;
    this->elements[0*4+3] *= sx;

    this->elements[1*4+0] *= sy;
    this->elements[1*4+1] *= sy;
    this->elements[1*4+2] *= sy;
    this->elements[1*4+3] *= sy;

    this->elements[2*4+0] *= sz;
    this->elements[2*4+1] *= sz;
    this->elements[2*4+2] *= sz;
    this->elements[2*4+3] *= sz;
  }

  void Matrix3D::translate(float tx, float ty, float tz) {
    this->elements[3*4+0] += this->elements[0*4+0] * tx + this->elements[1*4+0] * ty + this->elements[2*4+0] * tz;
    this->elements[3*4+1] += this->elements[0*4+1] * tx + this->elements[1*4+1] * ty + this->elements[2*4+1] * tz;
    this->elements[3*4+2] += this->elements[0*4+2] * tx + this->elements[1*4+2] * ty + this->elements[2*4+2] * tz;
    this->elements[3*4+3] += this->elements[0*4+3] * tx + this->elements[1*4+3] * ty + this->elements[2*4+3] * tz;
  }

  void Matrix3D::rotate(float angle, float x, float y, float z) {
    float mag = sqrt(x*x + y*y + z*z);
    float sinAngle = sin(angle); //sin(angle * PI / 180.0);
    float cosAngle = cos(angle); //cos(angle * PI / 180.0);

    if (mag > 0) {
      float xx, yy, zz, xy, yz, zx, xs, ys, zs;
      float oneMinusCos;
      Matrix3D* rotMat;

      x /= mag;
      y /= mag;
      z /= mag;

      xx = x * x;
      yy = y * y;
      zz = z * z;
      xy = x * y;
      yz = y * z;
      zx = z * x;
      xs = x * sinAngle;
      ys = y * sinAngle;
      zs = z * sinAngle;
      oneMinusCos = 1.0 - cosAngle;

      rotMat = new Matrix3D();

      rotMat->elements[0*4+0] = (oneMinusCos * xx) + cosAngle;
      rotMat->elements[0*4+1] = (oneMinusCos * xy) - zs;
      rotMat->elements[0*4+2] = (oneMinusCos * zx) + ys;
      rotMat->elements[0*4+3] = 0.0;

      rotMat->elements[1*4+0] = (oneMinusCos * xy) + zs;
      rotMat->elements[1*4+1] = (oneMinusCos * yy) + cosAngle;
      rotMat->elements[1*4+2] = (oneMinusCos * yz) - xs;
      rotMat->elements[1*4+3] = 0.0;

      rotMat->elements[2*4+0] = (oneMinusCos * zx) - ys;
      rotMat->elements[2*4+1] = (oneMinusCos * yz) + xs;
      rotMat->elements[2*4+2] = (oneMinusCos * zz) + cosAngle;
      rotMat->elements[2*4+3] = 0.0;

      rotMat->elements[3*4+0] = 0.0;
      rotMat->elements[3*4+1] = 0.0;
      rotMat->elements[3*4+2] = 0.0;
      rotMat->elements[3*4+3] = 1.0;

      rotMat->multiply(this);
      this->set(rotMat);
      delete rotMat;
    }
  }

  void Matrix3D::multiply(Matrix3D* right) {
    Matrix3D* tmp = new Matrix3D();

    for (int i = 0; i < 4; i++) {
      tmp->elements[i*4+0] = 
        (this->elements[i*4+0] * right->elements[0*4+0]) +
        (this->elements[i*4+1] * right->elements[1*4+0]) +
        (this->elements[i*4+2] * right->elements[2*4+0]) +
        (this->elements[i*4+3] * right->elements[3*4+0]) ;

      tmp->elements[i*4+1] =
          (this->elements[i*4+0] * right->elements[0*4+1]) +
          (this->elements[i*4+1] * right->elements[1*4+1]) +
          (this->elements[i*4+2] * right->elements[2*4+1]) +
          (this->elements[i*4+3] * right->elements[3*4+1]) ;

      tmp->elements[i*4+2] =
        (this->elements[i*4+0] * right->elements[0*4+2]) +
        (this->elements[i*4+1] * right->elements[1*4+2]) +
        (this->elements[i*4+2] * right->elements[2*4+2]) +
        (this->elements[i*4+3] * right->elements[3*4+2]) ;

      tmp->elements[i*4+3] =
        (this->elements[i*4+0] * right->elements[0*4+3]) +
        (this->elements[i*4+1] * right->elements[1*4+3]) +
        (this->elements[i*4+2] * right->elements[2*4+3]) +
        (this->elements[i*4+3] * right->elements[3*4+3]) ;
    }

    this->set(tmp);
    delete tmp;
  }

  void Matrix3D::set( Matrix3D* m ) {
    for (int i = 0; i < 16; i++) {
      this->elements[i] = m->elements[i];
    }
  }

  void Matrix3D::loadIdentity() {
    for (int i = 0; i < 16; i++){
      this->elements[i] = 0;
    }
    this->elements[0*4+0] = 1.0;
    this->elements[1*4+1] = 1.0;
    this->elements[2*4+2] = 1.0;
    this->elements[3*4+3] = 1.0;
  }

  float* Matrix3D::get(){
    return this->elements;
  }

}
