#pragma once
#include "stdafx.hpp"

namespace math {
	template<typename T>
	struct vector2 {
		T x;
		T y;

		vector2<T>() {}

		vector2<T>(T in_x, T in_y)
			: x(in_x), y(in_y)
		{}

		vector2<T> operator*(float value) {
			return vector2<T>(x * value, y * value);
		}

		vector2<T> operator*=(float value) {
			return vector2<T>(x * value, y * value);
		}

		vector2<T> operator*(vector2<T>& value) {
			return vector2<T>(x * value.x, y * value.y);
		}

		vector2<T> operator-(float value) {
			return vector2<T>(x - value, y - value);
		}

		vector2<T> operator-=(float value) {
			return vector2<T>(x - value, y - value);
		}

		vector2<T> operator-(vector2<T>& value) {
			return vector2<T>(x - value.x, y - value.y);
		}

		vector2<T> operator+(float value) {
			return vector2<T>(x + value, y + value);
		}

		vector2<T> operator+=(float value) {
			return vector2<T>(x + value, y + value);
		}

		vector2<T> operator+(vector2<T>& value) {
			return vector2<T>(x + value.x, y + value.y);
		}

		vector2<T> operator/(float value) {
			if (value == 0.f || x == 0 || y == 0) return vector2<T>(0, 0);
			return vector2<T>(x / value, y / value);
		}

		vector2<T> operator/=(float value) {
			if (value == 0.f || x == 0 || y == 0) return vector2<T>(0, 0);
			return vector2<T>(x / value, y / value);
		}

		vector2<T> operator/(vector2<T>& value) {
			return vector2<T>(x / value.x, y / value.y);
		}

		bool operator==(vector2<T>& value) {
			return x == value.x && y == value.y;
		}

		float getMathmaticalDistance(vector2<T>& value) {
			float a = value.x - x;
			float b = value.y - y;
			return (float)sqrt((a * a) + (b * b));
		}
	};

	template<typename T>
	struct vector3_ {
		T x;
		T y;
		T z;

		vector3_<T>() {}

		vector3_<T>(T in_x, T in_y, T in_z)
			: x(in_x), y(in_y), z(in_z)
		{}

		vector3_<T> operator*(float value) {
			return vector3_<T>(x * value, y * value, z * value);
		}

		vector3_<T> operator*=(float value) {
			return vector3_<T>(x * value, y * value, z * value);
		}

		vector3_<T> operator*(vector3_<T>& value) {
			return vector3_<T>(x * value.x, y * value.y, z * value.z);
		}

		vector3_<T> operator-(float value) {
			return vector3_<T>(x - value, y - value, z - value);
		}

		vector3_<T> operator-=(float value) {
			return vector3_<T>(x - value, y - value, z - value);
		}

		vector3_<T> operator-(vector3_<T>& value) {
			return vector3_<T>(x - value.x, y - value.y, z - value.z);
		}

		vector3_<T> operator+(float value) {
			return vector3_<T>(x + value, y + value, z + value);
		}

		vector3_<T> operator+=(float value) {
			return vector3_<T>(x + value, y + value, z + value);
		}

		vector3_<T> operator+(vector3_<T>& value) {
			return vector3_<T>(x + value.x, y + value.y, z + value.z);
		}

		vector3_<T> operator/(float value) {
			if (value == 0.f || x == 0 || y == 0 || z == 0) return vector3_<T>(0, 0);
			return vector3_<T>(x / value, y / value, z / value);
		}

		vector3_<T> operator/=(float value) {
			if (value == 0.f || x == 0 || y == 0) return vector3_<T>(0, 0);
			return vector3_<T>(x / value, y / value, z / value);
		}

		vector3_<T> operator/(vector3_<T>& value) {
			return vector3_<T>(x / value.x, y / value.y, z / value.z);
		}

		bool operator==(vector3_<T>& value) {
			return x == value.x && y == value.y && z == value.z;
		}

		float getMathmaticalDistance(vector3_<T>& value) {
			float a = value.x - x;
			float b = value.y - y;
			float c = value.z - z;
			return (float)sqrt((a * a) + (b * b) + (c * c));
		}
	};

	template<typename T>
	struct vector3 {
		T x;
		DWORD x_padding;
		T y;
		DWORD y_padding;
		T z;
		DWORD z_padding;

		vector3<T>() {}

		vector3<T>(T in_x, T in_y, T in_z)
			: x(in_x), y(in_y), z(in_z)
		{}

		vector3<T> operator*(float value) {
			return vector3<T>(x * value, y * value, z * value);
		}

		vector3<T> operator*=(float value) {
			return vector3<T>(x * value, y * value, z * value);
		}

		vector3<T> operator*(vector3<T>& value) {
			return vector3<T>(x * value.x, y * value.y, z * value.z);
		}

		vector3<T> operator-(float value) {
			return vector3<T>(x - value, y - value, z - value);
		}

		vector3<T> operator-=(float value) {
			return vector3<T>(x - value, y - value, z - value);
		}

		vector3<T> operator-(vector3<T>& value) {
			return vector3<T>(x - value.x, y - value.y, z - value.z);
		}

		vector3<T> operator+(float value) {
			return vector3<T>(x + value, y + value, z + value);
		}

		vector3<T> operator+=(float value) {
			return vector3<T>(x + value, y + value, z + value);
		}

		vector3<T> operator+=(vector3<T>& value) {
			return vector3<T>(x + value.x, y + value.y, z + value.z);
		}

		vector3<T> operator+(vector3<T>& value) {
			return vector3<T>(x + value.x, y + value.y, z + value.z);
		}

		vector3<T> operator/(float value) {
			if (value == 0.f || x == 0 || y == 0 || z == 0) return vector3<T>(0, 0);
			return vector3<T>(x / value, y / value, z / value);
		}

		vector3<T> operator/=(float value) {
			if (value == 0.f || x == 0 || y == 0) return vector3<T>(0, 0);
			return vector3<T>(x / value, y / value, z / value);
		}

		vector3<T> operator/(vector3<T>& value) {
			return vector3<T>(x / value.x, y / value.y, z / value.z);
		}

		bool operator==(vector3<T>& value) {
			return x == value.x && y == value.y && z == value.z;
		}

		float getMathmaticalDistance(vector3<T>& value) {
			float a = value.x - x;
			float b = value.y - y;
			float c = value.z - z;
			return (float)sqrt((a * a) + (b * b) + (c * c));
		}
		
		bool isZero() {
			return (x == .0f && y == .0f && z == .0f);
		}
	};

	template<typename T>
	struct vector4 {
		T x;
		T y;
		T z;
		T w;
	};

	template<typename T>
	bool within(T val, T min, T max) {
		return val <= max && val >= min;
	}

	template<typename T>
	T clamp(T val, T min, T max) {
		return val < min ? min : val > max ? max : val;
	}

	template<class T> T __ROL__(T value, int count) {
		const uint nbits = sizeof(T) * 8;

		if (count > 0) {
			count %= nbits;
			T high = value >> (nbits - count);
			if (T(-1) < 0)
				high &= ~((T(-1) << count));
			value <<= count;
			value |= high;
		} else {
			count = -count % nbits;
			T low = value << (nbits - count);
			value >>= count;
			value |= low;
		}
		return value;
	}

	inline uint64_t rol8(uint64_t value, int count) { return __ROL__((uint64_t)value, count); }
	inline uint64_t ror8(uint64_t value, int count) { return __ROL__((uint64_t)value, -count); }
}