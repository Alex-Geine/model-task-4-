#pragma once

namespace math3d
{

	/////////////////////////////////////////////////
	// Матрица преобразования в 3-мерном пространстве
	/////////////////////////////////////////////////
	class mat4
	{
		friend class vec4;

		// Ранг матрицы
		static const int _rank = 4;

		// Элементы матрицы
		float _matrix[_rank][_rank];

	public:

		// Конструктор по умолчанию
		mat4() { reset(); }

		// Конструктор
		mat4(float m11, float m12, float m13,
			float m21, float m22, float m23,
			float m31, float m32, float m33,
			float dx = 0, float dy = 0, float dz = 0);

		// Операция присваивания
		mat4& operator = (const mat4& right);

		// Операция умножения на матрицу
		mat4 operator * (const mat4& right);

		// Сброс матрицы в единичную
		void reset();

		// Поворот относительно оси X. Угол в градусах!
		mat4& rotateX(float angle);

		// Поворот относительно оси X. Угол в градусах!
		void rotateX(float angle, mat4& res);

		// Поворот относительно оси Y. Угол в градусах!
		mat4& rotateY(float angle);

		// Поворот относительно оси Y. Угол в градусах!
		void rotateY(float angle, mat4& res);

		// Поворот относительно оси Z. Угол в градусах!
		mat4& rotateZ(float angle);

		// Поворот относительно оси Z. Угол в градусах!
		void rotateZ(float angle, mat4& res);

		// Перенос (сдвиг)
		mat4& translate(float x, float y, float z);

		// Перенос (сдвиг)
		void translate(float x, float y, float z, mat4& res);

		// Масштабирование
		mat4& scale(float x, float y, float z);

		// Масштабирование
		void scale(float x, float y, float z, mat4& res);

		// Перспективное преобразование вдоль оси X
		mat4& perspective(float center);

		// Перспективное преобразование вдоль оси X
		void perspective(float center, mat4& res);
	};

	// ------------------------------------------------------------

	///////////////////////////////////////////////////////////////
	// Вектор - однородные координаты точки в 3-мерном пространстве
	///////////////////////////////////////////////////////////////
	class vec4
	{
		// Размерность вектора
		static const int _size = 4;

		// Элементы вектора
		float _vector[_size];

	public:

		// Конструктор
		vec4(float x = 0, float y = 0, float z = 0);

		// Операция присваивания
		vec4& operator = (const vec4& right);

		// Операция умножения на матрицу
		vec4 operator * (const mat4& right);

		// Получить ссылку на координату X
		float& x() const { return (float&)_vector[0]; }

		// Получить ссылку на координату Y
		float& y() const { return (float&)_vector[1]; }

		// Получить ссылку на координату Z
		float& z() const { return (float&)_vector[2]; }

		void Setz(float z) {
			_vector[2] = z;
		}
		void Setx(float x) {
			_vector[0] = x;
		}
		void Sety(float y) {
			_vector[1] = y;
		}
	};
}

