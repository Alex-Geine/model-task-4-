#include "pch.h"
#include "math3d.h"
#include <iostream>
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>

using namespace math3d;
using namespace std;

// Конструктор
mat4::mat4(float m11, float m12, float m13,
	float m21, float m22, float m23,
	float m31, float m32, float m33,
	float dx, float dy, float dz) : _matrix{ {m11, m12,m13, 0}, {m21, m22, m23, 0}, {m31, m32, m33, 0}, {dx, dy, dz, 1} } {

}

// Операция присваивания
mat4& mat4::operator = (const mat4& right) {
	for (int i = 0; i < _rank; i++)
		copy(begin(right._matrix[i]), end(right._matrix[i]), begin(this->_matrix[i]));
	return *this;
}

// Операция умножения на матрицу
mat4 mat4::operator * (const mat4& right) {
	mat4 buf = *this;
	mat4 res;
	for (int i = 0; i < 4; i++) {		
		for(int j = 0; j < 4; j ++){
			float num = 0;
			for (int k = 0; k < 4; k++) {
				num += buf._matrix[i][k] * right._matrix[k][j];
			}
			res._matrix[i][j] = num;
		}			 
	}
	return res;
}

// Сброс матрицы в единичную(все нули, на диоганали единицы)
void mat4::reset() {		
	for (int i = 0; i < 4; i++) {
		memset(_matrix[i], 0, sizeof(_matrix[i]));
		_matrix[i][i] = 1;
	}	
 }

// Поворот относительно оси X. Угол в градусах!
mat4& mat4::rotateX(float angle) {
	float Cos = cos(angle * M_PI / 180);
	float Sin = sin(angle * M_PI / 180);

	mat4 mat(1, 0, 0, 0, Cos, Sin, 0, -Sin, -Cos);
	*this = (*this) * mat;

	return *this;
}

// Поворот относительно оси Y. Угол в градусах!
void mat4::rotateX(float angle, mat4& res) {
	float Cos = cos(angle * M_PI / 180);
	float Sin = sin(angle * M_PI / 180);

	mat4 mat(1, 0, 0, 0, Cos, Sin, 0, -Sin, -Cos);
	res = (*this) * mat;

	//return res;
}

// Поворот относительно оси Y. Угол в градусах!
mat4& mat4::rotateY(float angle) {
	float Cos = cos(angle * M_PI / 180);
	float Sin = sin(angle * M_PI / 180);

	mat4 mat(Cos, 0, -Sin, 0, 1, 0, Sin, 0, Cos);
	*this = (*this) * mat;

	return *this;
}
// Поворот относительно оси Y. Угол в градусах!
void mat4::rotateY(float angle, mat4& res) {
	float Cos = cos(angle * M_PI / 180);
	float Sin = sin(angle * M_PI / 180);

	mat4 mat(Cos, 0, -Sin, 0, 1, 0, Sin, 0, Cos);
	res = (*this) * mat;

	//return res;
}

// Поворот относительно оси Z. Угол в градусах!
mat4& mat4::rotateZ(float angle) {
	float Cos = cos(angle * M_PI / 180);
	float Sin = sin(angle * M_PI / 180);

	mat4 mat(Cos, Sin, 0, -Sin, Cos, 0, 0, 0, 1);
	*this = (*this) * mat;

	return *this;
}

// Поворот относительно оси Y. Угол в градусах!
void mat4::rotateZ(float angle, mat4& res) {
	float Cos = cos(angle * M_PI / 180);
	float Sin = sin(angle * M_PI / 180);

	mat4 mat(Cos, Sin, 0, -Sin, Cos, 0, 0, 0, 1);
	res = (*this) * mat;

	//return res;
}

// Перенос (сдвиг)
mat4& mat4::translate(float x, float y, float z) {
	mat4 mat(1,0,0,0,1,0,0,0,1,x,y,z);

	*this = (*this) * mat;
	return *this;	
}

// Перенос (сдвиг)
void mat4::translate(float x, float y, float z, mat4& res) {
	mat4 mat(1, 0, 0, 0, 1, 0, 0, 0, 1, x, y, z);
	res = (*this) * mat;
}

// Масштабирование
mat4& mat4::scale(float x, float y, float z) {
	mat4 mat(x,0,0, 0,y,0,0,0,z);

	(*this) = (*this) * mat;
	return (*this);
}

// Масштабирование
void mat4::scale(float x, float y, float z, mat4& res) {
	mat4 mat(x, 0, 0, 0, y, 0, 0, 0, z);

	res = (*this) * mat;
}

// Перспективное преобразование вдоль оси X
mat4& mat4::perspective(float center) {
	mat4 mat1, mat2;
	mat1._matrix[0][3] = -1/center;
	mat2._matrix[0][0] = 0;

	(*this) = (*this) * (mat1 * mat2);
	return (*this);
}	

// Перспективное преобразование вдоль оси X
void mat4::perspective(float center, mat4& res) {
	mat4 mat1, mat2;
	mat1._matrix[0][3] = -1 / center;
	mat2._matrix[0][0] = 0;

	res = (*this) * (mat1 * mat2);
}

// Конструктор
vec4::vec4(float x, float y, float z ) : _vector{ x,y,z } {

}

// Операция присваивания
vec4& vec4::operator = (const vec4& right) {
	for (int i = 0; i < _size; i++)
		this->_vector[i] = right._vector[i];
	return *this;
}

// Операция умножения на матрицу
vec4 vec4::operator * (const mat4& right) {
	vec4 res;
	vec4 buf = *this;
	
		for (int j = 0; j < 3; j++) {
			float num = 0;
			for (int k = 0; k < 3; k++) {
				num += buf._vector[k] * right._matrix[k][j];
			}
			res._vector[j] = num;
		}
		return res;
}

