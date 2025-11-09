#include <Novice.h>
#include <cstdint>
#include <assert.h>
#include <imgui.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>
#include "../../../../../../KamataEngine/DirectXGame/math/Vector4.h"
#include "numbers"

const char kWindowTitle[] = "LE2C_09_コバヤシ_トキヤ_タイトル";

struct Vector3
{
	float x, y, z;
};

struct Matrix4x4
{
	float m[4][4];
};

//クロス
Vector3 Cross(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;

	result.x = v1.y * v2.z - v1.z * v2.y;
	result.y = v1.z * v2.x - v1.x * v2.z;
	result.z = v1.x * v2.y - v1.y * v2.x;

	return result;
};

// 平行移動
Matrix4x4 MakeTranslateMatrix(const Vector3& translate)
{
	Matrix4x4 result = {};
	result.m[0][0] = 1.0f;
	result.m[1][1] = 1.0f;
	result.m[2][2] = 1.0f;
	result.m[3][3] = 1.0f;

	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;
	result.m[3][3] = 1.0f;

	return result;
}

// 拡大縮小
Matrix4x4 MakeScaleMatrix(const Vector3& scale)
{
	Matrix4x4 result = {};
	result.m[0][0] = scale.x;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = scale.y;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = scale.z;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	Vector3 result = {};
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];

	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
};

#pragma region 回転
Matrix4x4 MakeRotateXMatrix(float radian)
{
	Matrix4x4 result = {};
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = cosf(radian);
	result.m[1][2] = sinf(radian);
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = -sinf(radian);
	result.m[2][2] = cosf(radian);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakeRotateYMatrix(float radian)
{
	Matrix4x4 result = {};
	result.m[0][0] = cosf(radian);
	result.m[0][1] = 0.0f;
	result.m[0][2] = -sinf(radian);
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = sinf(radian);
	result.m[2][1] = 0.0f;
	result.m[2][2] = cosf(radian);
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 MakeRotateZMatrix(float radian)
{
	Matrix4x4 result = {};
	result.m[0][0] = cosf(radian);
	result.m[0][1] = sinf(radian);
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = -sinf(radian);
	result.m[1][1] = cosf(radian);
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}
#pragma endregion

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result = {};
	result.m[0][0] = m1.m[0][0] + m2.m[0][0];
	result.m[0][1] = m1.m[0][1] + m2.m[0][1];
	result.m[0][2] = m1.m[0][2] + m2.m[0][2];
	result.m[0][3] = m1.m[0][3] + m2.m[0][3];

	result.m[1][0] = m1.m[1][0] + m2.m[1][0];
	result.m[1][1] = m1.m[1][1] + m2.m[1][1];
	result.m[1][2] = m1.m[1][2] + m2.m[1][2];
	result.m[1][3] = m1.m[1][3] + m2.m[1][3];

	result.m[2][0] = m1.m[2][0] + m2.m[2][0];
	result.m[2][1] = m1.m[2][1] + m2.m[2][1];
	result.m[2][2] = m1.m[2][2] + m2.m[2][2];
	result.m[2][3] = m1.m[2][3] + m2.m[2][3];

	result.m[3][0] = m1.m[3][0] + m2.m[3][0];
	result.m[3][1] = m1.m[3][1] + m2.m[3][1];
	result.m[3][2] = m1.m[3][2] + m2.m[3][2];
	result.m[3][3] = m1.m[3][3] + m2.m[3][3];
	return result;
};

Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result = {};
	result.m[0][0] = m1.m[0][0] - m2.m[0][0];
	result.m[0][1] = m1.m[0][1] - m2.m[0][1];
	result.m[0][2] = m1.m[0][2] - m2.m[0][2];
	result.m[0][3] = m1.m[0][3] - m2.m[0][3];

	result.m[1][0] = m1.m[1][0] - m2.m[1][0];
	result.m[1][1] = m1.m[1][1] - m2.m[1][1];
	result.m[1][2] = m1.m[1][2] - m2.m[1][2];
	result.m[1][3] = m1.m[1][3] - m2.m[1][3];

	result.m[2][0] = m1.m[2][0] - m2.m[2][0];
	result.m[2][1] = m1.m[2][1] - m2.m[2][1];
	result.m[2][2] = m1.m[2][2] - m2.m[2][2];
	result.m[2][3] = m1.m[2][3] - m2.m[2][3];

	result.m[3][0] = m1.m[3][0] - m2.m[3][0];
	result.m[3][1] = m1.m[3][1] - m2.m[3][1];
	result.m[3][2] = m1.m[3][2] - m2.m[3][2];
	result.m[3][3] = m1.m[3][3] - m2.m[3][3];
	return result;
};

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result = {};
	result.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] * m2.m[2][0] + m1.m[0][3] * m2.m[3][0];
	result.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1] * m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1];
	result.m[0][2] = m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] * m2.m[3][2];
	result.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2] * m2.m[2][3] + m1.m[0][3] * m2.m[3][3];

	result.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0] + m1.m[1][3] * m2.m[3][0];
	result.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] * m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1];
	result.m[1][2] = m1.m[1][0] * m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
	result.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3] + m1.m[1][3] * m2.m[3][3];

	result.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0] + m1.m[2][3] * m2.m[3][0];
	result.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] * m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1];
	result.m[2][2] = m1.m[2][0] * m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
	result.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3] + m1.m[2][3] * m2.m[3][3];

	result.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0] + m1.m[3][3] * m2.m[3][0];
	result.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] * m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1];
	result.m[3][2] = m1.m[3][0] * m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
	result.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3] + m1.m[3][3] * m2.m[3][3];
	return result;
};

Matrix4x4 MakeAffinMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);

	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

	Matrix4x4 worldMatrix = Multiply(Multiply(scaleMatrix, rotateMatrix), translateMatrix);

	return worldMatrix;
}

Matrix4x4 Inverse(const Matrix4x4& m)
{
	float determinant =
	{
		m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] +
		m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2] -

		(m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1]) -
		(m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3]) -
		(m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]) -

		(m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3]) -
		(m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1]) -
		(m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]) +

		m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] +
		m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2] +

		m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] +
		m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2] -

		(m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1]) -
		(m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3]) -
		(m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]) -

		(m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0]) -
		(m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0]) -
		(m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]) +

		m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] +
		m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] +
		m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]
	};

	float determinantRecp = 1.0f / determinant;

	Matrix4x4 result = {};
	result.m[0][0] = determinantRecp * (
		m.m[1][1] * m.m[2][2] * m.m[3][3] +
		m.m[1][2] * m.m[2][3] * m.m[3][1] +
		m.m[1][3] * m.m[2][1] * m.m[3][2] -
		(m.m[1][3] * m.m[2][2] * m.m[3][1]) -
		(m.m[1][2] * m.m[2][1] * m.m[3][3]) -
		(m.m[1][1] * m.m[2][3] * m.m[3][2])
		);
	result.m[0][1] = determinantRecp * (
		-(m.m[0][1] * m.m[2][2] * m.m[3][3]) -
		(m.m[0][2] * m.m[2][3] * m.m[3][1]) -
		(m.m[0][3] * m.m[2][1] * m.m[3][2]) +
		m.m[0][3] * m.m[2][2] * m.m[3][1] +
		m.m[0][2] * m.m[2][1] * m.m[3][3] +
		m.m[0][1] * m.m[2][3] * m.m[3][2]
		);
	result.m[0][2] = determinantRecp * (
		m.m[0][1] * m.m[1][2] * m.m[3][3] +
		m.m[0][2] * m.m[1][3] * m.m[3][1] +
		m.m[0][3] * m.m[1][1] * m.m[3][2] -
		(m.m[0][3] * m.m[1][2] * m.m[3][1]) -
		(m.m[0][2] * m.m[1][1] * m.m[3][3]) -
		(m.m[0][1] * m.m[1][3] * m.m[3][2])
		);
	result.m[0][3] = determinantRecp * (
		-(m.m[0][1] * m.m[1][2] * m.m[2][3]) -
		(m.m[0][2] * m.m[1][3] * m.m[2][1]) -
		(m.m[0][3] * m.m[1][1] * m.m[2][2]) +
		m.m[0][3] * m.m[1][2] * m.m[2][1] +
		m.m[0][2] * m.m[1][1] * m.m[2][3] +
		m.m[0][1] * m.m[1][3] * m.m[2][2]
		);

	///////////////////
	result.m[1][0] = determinantRecp * (
		-(m.m[1][0] * m.m[2][2] * m.m[3][3]) -
		(m.m[1][2] * m.m[2][3] * m.m[3][0]) -
		(m.m[1][3] * m.m[2][0] * m.m[3][2]) +
		m.m[1][3] * m.m[2][2] * m.m[3][0] +
		m.m[1][2] * m.m[2][0] * m.m[3][3] +
		m.m[1][0] * m.m[2][3] * m.m[3][2]
		);


	result.m[1][1] = determinantRecp * (
		m.m[0][0] * m.m[2][2] * m.m[3][3] +
		m.m[0][2] * m.m[2][3] * m.m[3][0] +
		m.m[0][3] * m.m[2][0] * m.m[3][2] -
		(m.m[0][3] * m.m[2][2] * m.m[3][0]) -
		(m.m[0][2] * m.m[2][0] * m.m[3][3]) -
		(m.m[0][0] * m.m[2][3] * m.m[3][2])
		);

	result.m[1][2] = determinantRecp * (
		-(m.m[0][0] * m.m[1][2] * m.m[3][3]) -
		(m.m[0][2] * m.m[1][3] * m.m[3][0]) -
		(m.m[0][3] * m.m[1][0] * m.m[3][2]) +
		m.m[0][3] * m.m[1][2] * m.m[3][0] +
		m.m[0][2] * m.m[1][0] * m.m[3][3] +
		m.m[0][0] * m.m[1][3] * m.m[3][2]
		);
	result.m[1][3] = determinantRecp * (
		m.m[0][0] * m.m[1][2] * m.m[2][3] +
		m.m[0][2] * m.m[1][3] * m.m[2][0] +
		m.m[0][3] * m.m[1][0] * m.m[2][2] -
		(m.m[0][3] * m.m[1][2] * m.m[2][0]) -
		(m.m[0][2] * m.m[1][0] * m.m[2][3]) -
		(m.m[0][0] * m.m[1][3] * m.m[2][2])
		);
	///////////////////////////////////////////////////
	result.m[2][0] = determinantRecp * (
		m.m[1][0] * m.m[2][1] * m.m[3][3] +
		m.m[1][1] * m.m[2][3] * m.m[3][0] +
		m.m[1][3] * m.m[2][0] * m.m[3][1] -
		(m.m[1][3] * m.m[2][1] * m.m[3][0]) -
		(m.m[1][1] * m.m[2][0] * m.m[3][3]) -
		(m.m[1][0] * m.m[2][3] * m.m[3][1])
		);

	result.m[2][1] = determinantRecp * (
		-(m.m[0][0] * m.m[2][1] * m.m[3][3]) -
		(m.m[0][1] * m.m[2][3] * m.m[3][0]) -
		(m.m[0][3] * m.m[2][0] * m.m[3][1]) +
		m.m[0][3] * m.m[2][1] * m.m[3][0] +
		m.m[0][1] * m.m[2][0] * m.m[3][3] +
		m.m[0][0] * m.m[2][3] * m.m[3][1]
		);

	result.m[2][2] = determinantRecp * (
		m.m[0][0] * m.m[1][1] * m.m[3][3] +
		m.m[0][1] * m.m[1][3] * m.m[3][0] +
		m.m[0][3] * m.m[1][0] * m.m[3][1] -
		(m.m[0][3] * m.m[1][1] * m.m[3][0]) -
		(m.m[0][1] * m.m[1][0] * m.m[3][3]) -
		(m.m[0][0] * m.m[1][3] * m.m[3][1])
		);

	result.m[2][3] = determinantRecp * (
		-(m.m[0][0] * m.m[1][1] * m.m[2][3]) -
		(m.m[0][1] * m.m[1][3] * m.m[2][0]) -
		(m.m[0][3] * m.m[1][0] * m.m[2][1]) +
		m.m[0][3] * m.m[1][1] * m.m[2][0] +
		m.m[0][1] * m.m[1][0] * m.m[2][3] +
		m.m[0][0] * m.m[1][3] * m.m[2][1]
		);
	/////////////////////////////////////
	result.m[3][0] = determinantRecp * (
		-(m.m[1][0] * m.m[2][1] * m.m[3][2]) -
		(m.m[1][1] * m.m[2][2] * m.m[3][0]) -
		(m.m[1][2] * m.m[2][0] * m.m[3][1]) +
		m.m[1][2] * m.m[2][1] * m.m[3][0] +
		m.m[1][1] * m.m[2][0] * m.m[3][2] +
		m.m[1][0] * m.m[2][2] * m.m[3][1]
		);

	result.m[3][1] = determinantRecp * (
		m.m[0][0] * m.m[2][1] * m.m[3][2] +
		m.m[0][1] * m.m[2][2] * m.m[3][0] +
		m.m[0][2] * m.m[2][0] * m.m[3][1] -
		(m.m[0][2] * m.m[2][1] * m.m[3][0]) -
		(m.m[0][1] * m.m[2][0] * m.m[3][2]) -
		(m.m[0][0] * m.m[2][2] * m.m[3][1])
		);

	result.m[3][2] = determinantRecp * (
		-(m.m[0][0] * m.m[1][1] * m.m[3][2]) -
		(m.m[0][1] * m.m[1][2] * m.m[3][0]) -
		(m.m[0][2] * m.m[1][0] * m.m[3][1]) +
		m.m[0][2] * m.m[1][1] * m.m[3][0] +
		m.m[0][1] * m.m[1][0] * m.m[3][2] +
		m.m[0][0] * m.m[1][2] * m.m[3][1]
		);
	result.m[3][3] = determinantRecp * (
		m.m[0][0] * m.m[1][1] * m.m[2][2] +
		m.m[0][1] * m.m[1][2] * m.m[2][0] +
		m.m[0][2] * m.m[1][0] * m.m[2][1] -
		(m.m[0][2] * m.m[1][1] * m.m[2][0]) -
		(m.m[0][1] * m.m[1][0] * m.m[2][2]) -
		(m.m[0][0] * m.m[1][2] * m.m[2][1])
		);
	return result;
};

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{

	Matrix4x4 result = {};
	result.m[0][0] = (1 / aspectRatio) * (1 / tanf(fovY / 2));
	result.m[1][1] = (1 / tanf(fovY / 2));
	result.m[2][2] = (farClip / (farClip - nearClip));
	result.m[2][3] = 1.0f;
	result.m[3][2] = -(nearClip * farClip) / (farClip - nearClip);
	return result;

};

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farclip)
{
	Matrix4x4 result = {};
	result.m[0][0] = 2 / (right - left);
	result.m[1][1] = 2 / (top - bottom);
	result.m[2][2] = 1 / (farclip - nearClip);
	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farclip);
	result.m[3][3] = 1.0f;
	return result;
};

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	Matrix4x4 result = {};
	result.m[0][0] = width / 2;
	result.m[1][1] = -height / 2;
	result.m[2][2] = maxDepth - minDepth;
	result.m[3][0] = left + (width / 2);
	result.m[3][1] = top + (height / 2);
	result.m[3][2] = minDepth;
	result.m[3][3] = 1.0f;


	return result;
}

float Length(const Vector3& v)
{
	float result = {};
	result = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	return result;
};


static const int kColumnWidth = 60;
void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label)
{
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

Matrix4x4 MakeIdentity4x4()
{
	Matrix4x4 result{};
	for (int i = 0; i < 4; ++i)
		result.m[i][i] = 1.0f;
	return result;
}

//スカラー倍
Vector3 ScalarMultiply(float scalar, const Vector3& v)
{
	Vector3 result = {};
	result.x = v.x * scalar;
	result.y = v.y * scalar;
	result.z = v.z * scalar;
	return result;
};
//内積
float Dot(const Vector3& v1, const Vector3& v2)
{
	float result = {};
	result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
	return result;
};

//足し算
Vector3 Add(const Vector3& v1, const Vector3& v2)
{
	Vector3 result = {};
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;
	return result;
};

//引き算
Vector3 Subract(const Vector3& v1, const Vector3& v2)
{
	Vector3 result = {};
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;
	return result;
};

//正規化
Vector3 Normalize(const Vector3& v)
{
	Vector3 result = {};
	result.x = v.x / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	result.y = v.y / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
	result.z = v.z / sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

	return result;
};

//球
struct Sphere
{
	Vector3 center;
	float radius;
	int color;
};

//平面
struct Plane
{
	Vector3 normal;//法線ベクトル
	float distance;//原点からの距離
};

//線
struct Segment
{
	Vector3 origin;//始点
	Vector3 diff;  //終点-始点(方向ベクトル)
	int color;
};

//三角形
struct Triangle
{
	Vector3 vertices[3];// 頂点
	int color;
};

//AABB
struct AABB
{
	Vector3 min;//最小点
	Vector3 max;//最大点
	int color;
};

#pragma region 描画

#pragma region 地面
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
	const float kGridHalfWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);

	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex)
	{
		float x = -kGridHalfWidth + kGridEvery * xIndex;
		Vector3 start = { x,0.0f,-kGridHalfWidth };
		Vector3 end = { x,0.0f,kGridHalfWidth };

		Vector3 startScreen = Transform(start, viewProjectionMatrix);
		startScreen = Transform(startScreen, viewportMatrix);

		Vector3 endScreen = Transform(end, viewProjectionMatrix);
		endScreen = Transform(endScreen, viewportMatrix);

		uint32_t color = (x == 0.0f) ? 0x000000FF : 0xAAAAAAFF;

		Novice::DrawLine(
			int(startScreen.x), int(startScreen.y),
			int(endScreen.x), int(endScreen.y), color
		);
	}
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex)
	{
		float z = -kGridHalfWidth + kGridEvery * zIndex;
		Vector3 start = { -kGridHalfWidth,0.0f,z };
		Vector3 end = { kGridHalfWidth,0.0f,z };

		Vector3 startScreen = Transform(start, viewProjectionMatrix);
		startScreen = Transform(startScreen, viewportMatrix);

		Vector3 endScreen = Transform(end, viewProjectionMatrix);
		endScreen = Transform(endScreen, viewportMatrix);

		uint32_t color = (z == 0.0f) ? 0x000000FF : 0xAAAAAAFF;

		Novice::DrawLine(
			int(startScreen.x), int(startScreen.y),
			int(endScreen.x), int(endScreen.y), color
		);
	}
}
#pragma endregion

#pragma region 球体
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	const uint32_t kSubdivision = 16;
	const float kLonEvery = 2.0f * float(M_PI) / float(kSubdivision);
	const float kLatEvery = float(M_PI) / float(kSubdivision);;
	//緯度の方向に分割-π/2~π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; latIndex++)
	{
		float lan = -float(M_PI) / 2.0f + kLatEvery * latIndex;//現在の緯度
		float nextLat = lan + kLatEvery;
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; lonIndex++)
		{
			float lon = lonIndex * kLonEvery;
			float nextLon = lon + kLonEvery;

			Vector3 a, b, c;
			a = {
				sphere.center.x + sphere.radius * cosf(lan) * cosf(lon),
				sphere.center.y + sphere.radius * sinf(lan),
				sphere.center.z + sphere.radius * cosf(lan) * sinf(lon)
			};

			b = {
				sphere.center.x + sphere.radius * cosf(nextLat) * cosf(lon),
				sphere.center.y + sphere.radius * sinf(nextLat),
				sphere.center.z + sphere.radius * cosf(nextLat) * sinf(lon)
			};

			c = {
				sphere.center.x + sphere.radius * cosf(lan) * cosf(nextLon),
				sphere.center.y + sphere.radius * sinf(lan),
				sphere.center.z + sphere.radius * cosf(lan) * sinf(nextLon)
			};

			Vector3 screenA = Transform(Transform(a, viewProjectionMatrix), viewportMatrix);
			Vector3 screenB = Transform(Transform(b, viewProjectionMatrix), viewportMatrix);
			Vector3 screenC = Transform(Transform(c, viewProjectionMatrix), viewportMatrix);

			Novice::DrawLine(int(screenA.x), int(screenA.y), int(screenB.x),
				int(screenB.y), color);
			Novice::DrawLine(int(screenA.x), int(screenA.y), int(screenC.x),
				int(screenC.y), color);
		}
	}
}
#pragma endregion

#pragma region 無限遠平面
Vector3 Perpendicular(const Vector3& vector)
{
	if (vector.x != 0.0f || vector.y != 0.0f)
	{
		return{ -vector.y,vector.x,0.0f };
	}
	return { 0.0f,-vector.z,vector.y };
}
#pragma endregion

#pragma region 平面
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	// 平面上の1点（原点から法線方向にdistance進んだ位置）
	Vector3 center = ScalarMultiply(plane.distance, plane.normal);

	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z };
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };

	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index)
	{
		Vector3 extend = ScalarMultiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}

	Novice::DrawLine(static_cast<int>(points[3].x), static_cast<int>(points[3].y), static_cast<int>(points[1].x), static_cast<int>(points[1].y), color);
	Novice::DrawLine(static_cast<int>(points[1].x), static_cast<int>(points[1].y), static_cast<int>(points[2].x), static_cast<int>(points[2].y), color);
	Novice::DrawLine(static_cast<int>(points[2].x), static_cast<int>(points[2].y), static_cast<int>(points[0].x), static_cast<int>(points[0].y), color);
	Novice::DrawLine(static_cast<int>(points[0].x), static_cast<int>(points[0].y), static_cast<int>(points[3].x), static_cast<int>(points[3].y), color);
}
#pragma endregion

#pragma region 線
void DrawSegment(const Segment& segment, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 start = segment.origin;
	Vector3 end = Add(segment.origin, segment.diff);

	Vector3 clipStart = Transform(start, viewProjectionMatrix);
	Vector3 clipEnd = Transform(end, viewProjectionMatrix);

	Vector3 screenStart = Transform(clipStart, viewportMatrix);
	Vector3 screenEnd = Transform(clipEnd, viewportMatrix);

	Novice::DrawLine(
		static_cast<int>(screenStart.x), static_cast<int>(screenStart.y),
		static_cast<int>(screenEnd.x), static_cast<int>(screenEnd.y),
		color
	);
}
#pragma endregion

#pragma region 三角形
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	Vector3 screenPos[3];

	for (int i = 0; i < 3; ++i)
	{
		Vector3 v = Transform(triangle.vertices[i], viewProjectionMatrix);
		v = Transform(v, viewportMatrix);

		screenPos[i] = v;
	}

	// 三角形の辺を線で描画
	Novice::DrawTriangle(
		(int)screenPos[0].x, (int)screenPos[0].y,
		(int)screenPos[1].x, (int)screenPos[1].y,
		(int)screenPos[2].x, (int)screenPos[2].y,
		color, kFillModeWireFrame);
}
#pragma endregion

#pragma region AABB
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	// 8頂点を生成
	Vector3 vertices[8];
	vertices[0] = { aabb.min.x, aabb.min.y, aabb.min.z };
	vertices[1] = { aabb.max.x, aabb.min.y, aabb.min.z };
	vertices[2] = { aabb.min.x, aabb.max.y, aabb.min.z };
	vertices[3] = { aabb.max.x, aabb.max.y, aabb.min.z };
	vertices[4] = { aabb.min.x, aabb.min.y, aabb.max.z };
	vertices[5] = { aabb.max.x, aabb.min.y, aabb.max.z };
	vertices[6] = { aabb.min.x, aabb.max.y, aabb.max.z };
	vertices[7] = { aabb.max.x, aabb.max.y, aabb.max.z };

	// 各頂点を変換
	for (int i = 0; i < 8; i++)
	{
		vertices[i] = Transform(Transform(vertices[i], viewProjectionMatrix), viewportMatrix);
	}

	// 線分を描画（12本）
	// 前面
	Novice::DrawLine((int)vertices[0].x, (int)vertices[0].y, (int)vertices[1].x, (int)vertices[1].y, color);
	Novice::DrawLine((int)vertices[1].x, (int)vertices[1].y, (int)vertices[3].x, (int)vertices[3].y, color);
	Novice::DrawLine((int)vertices[3].x, (int)vertices[3].y, (int)vertices[2].x, (int)vertices[2].y, color);
	Novice::DrawLine((int)vertices[2].x, (int)vertices[2].y, (int)vertices[0].x, (int)vertices[0].y, color);

	// 背面
	Novice::DrawLine((int)vertices[4].x, (int)vertices[4].y, (int)vertices[5].x, (int)vertices[5].y, color);
	Novice::DrawLine((int)vertices[5].x, (int)vertices[5].y, (int)vertices[7].x, (int)vertices[7].y, color);
	Novice::DrawLine((int)vertices[7].x, (int)vertices[7].y, (int)vertices[6].x, (int)vertices[6].y, color);
	Novice::DrawLine((int)vertices[6].x, (int)vertices[6].y, (int)vertices[4].x, (int)vertices[4].y, color);

	// 側面
	Novice::DrawLine((int)vertices[0].x, (int)vertices[0].y, (int)vertices[4].x, (int)vertices[4].y, color);
	Novice::DrawLine((int)vertices[1].x, (int)vertices[1].y, (int)vertices[5].x, (int)vertices[5].y, color);
	Novice::DrawLine((int)vertices[2].x, (int)vertices[2].y, (int)vertices[6].x, (int)vertices[6].y, color);
	Novice::DrawLine((int)vertices[3].x, (int)vertices[3].y, (int)vertices[7].x, (int)vertices[7].y, color);
}
#pragma endregion

#pragma endregion

#pragma region 当たり判定

#pragma region 平面と球
bool IsCollision(const Sphere& sphere, const Plane& plane)
{
	Vector3 normalizedNormal = Normalize(plane.normal);

	float distance = Dot(sphere.center, normalizedNormal) - plane.distance;

	return fabs(distance) <= sphere.radius;
}
#pragma endregion

#pragma region 線と平面
bool IsCollision(const Segment& segment, const Plane& plane)
{
	//垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, segment.diff);

	//垂直＝平行であるので、衝突しているはずがない
	if (dot == 0.0f)
	{
		return false;
	}

	//tを求める
	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;

	//tの値と線の種類によって衝突しているかを判断する
	if (t >= 0.0f && t <= 1.0f)
	{
		return true;
	}
	return false;
}
#pragma endregion

#pragma region 三角形と線
bool IsCollsion(const Triangle& triangle, const Segment& segment)
{
	// 三角形の頂点を取得
	Vector3 v0 = triangle.vertices[0];
	Vector3 v1 = triangle.vertices[1];
	Vector3 v2 = triangle.vertices[2];

	// 三角形のエッジベクトルを計算
	Vector3 edge1 = Subract(v1, v0);
	Vector3 edge2 = Subract(v2, v0);

	// 三角形の法線を計算
	Vector3 normal = Normalize(Cross(edge1, edge2));

	// 線分の方向ベクトル
	Vector3 segDir = Subract(segment.diff, segment.origin);

	// 線分が平面と平行かどうかチェック
	float denom = Dot(normal, segDir);
	if (fabs(denom) == 0.0f)
	{
		return false; // 平行で交差しない
	}

	// 平面の方程式のdを計算
	float d = Dot(normal, v0);

	// 線分のパラメータtを計算（交差点の位置）
	float t = (d - Dot(normal, segment.origin)) / denom;

	// tが線分の範囲内（0～1）にあるか？
	if (t < 0.0f || t > 1.0f)
	{
		return false; // 線分の範囲外
	}

	// 交差点の位置を計算
	Vector3 intersection = {
		segment.origin.x + segDir.x * t,
		segment.origin.y + segDir.y * t,
		segment.origin.z + segDir.z * t
	};

	// 三角形の3辺ベクトル
	Vector3 v01 = Subract(v1, v0);
	Vector3 v12 = Subract(v2, v1);
	Vector3 v20 = Subract(v0, v2);

	// 各辺に対する交差点ベクトル
	Vector3 v0p = Subract(intersection, v0);
	Vector3 v1p = Subract(intersection, v1);
	Vector3 v2p = Subract(intersection, v2);

	// 外積で内外判定（すべての外積が同じ方向＝三角形の内側）
	Vector3 cross01 = Cross(v01, v0p);
	Vector3 cross12 = Cross(v12, v1p);
	Vector3 cross20 = Cross(v20, v2p);

	if (Dot(cross01, normal) >= 0.0f &&
		Dot(cross12, normal) >= 0.0f &&
		Dot(cross20, normal) >= 0.0f)
	{
		return true; // 衝突している
	}

	return false; // 衝突していない
}
#pragma endregion

#pragma region AABBとAABB
bool IsCollsion(const AABB& aabb1, const AABB& aabb2)
{
	return (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) &&//x軸
		(aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) &&//y軸
		(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z);//z軸
	//衝突]

}
#pragma endregion

#pragma region AABBと球
bool IsCollision(const AABB& aabb, const Sphere& sphere)
{
	//最近接点を求める
	Vector3 closestPoint{
		std::clamp(sphere.center.x,aabb.min.x,aabb.max.x),
		std::clamp(sphere.center.y,aabb.min.y,aabb.max.y),
		std::clamp(sphere.center.z,aabb.min.z,aabb.max.z)
	};

	//最近接点と球の中心との距離を求める
	float distance = Length(Subract(closestPoint, sphere.center));

	//距離が半径よりも小さければ衝突
	if (distance <= sphere.radius)
	{
		//衝突
		return true;
	}
	return false;
}
#pragma endregion

#pragma region AABBと線
bool IsCollision(const AABB& aabb, const Segment& segment)
{
	if (segment.diff.x == 0.0f)
	{
		if (segment.origin.x < aabb.min.x || segment.origin.x > aabb.max.x) return false;
	}
	if (segment.diff.y == 0.0f)
	{
		if (segment.origin.y < aabb.min.y || segment.origin.y > aabb.max.y) return false;
	}
	if (segment.diff.z == 0.0f)
	{
		if (segment.origin.z < aabb.min.z || segment.origin.z > aabb.max.z) return false;
	}

	float tx1 = (aabb.min.x - segment.origin.x) / segment.diff.x;
	float tx2 = (aabb.max.x - segment.origin.x) / segment.diff.x;
	float tNearX = min(tx1, tx2);
	float tFarX = max(tx1, tx2);

	float ty1 = (aabb.min.y - segment.origin.y) / segment.diff.y;
	float ty2 = (aabb.max.y - segment.origin.y) / segment.diff.y;
	float tNearY = min(ty1, ty2);
	float tFarY = max(ty1, ty2);

	float tz1 = (aabb.min.z - segment.origin.z) / segment.diff.z;
	float tz2 = (aabb.max.z - segment.origin.z) / segment.diff.z;
	float tNearZ = min(tz1, tz2);
	float tFarZ = max(tz1, tz2);

	float tmin = max(max(tNearX, tNearY), tNearZ);

	float tmax = min(min(tFarX, tFarY), tFarZ);

	if (tmin <= tmax)
	{
		return true;
	}
	return false;
}
#pragma endregion

#pragma endregion

#pragma region ベジェ曲線

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t)
{
	return
	{
		(1 - t) * v1.x + t * v2.x,
		(1 - t) * v1.y + t * v2.y,
		(1 - t) * v1.z + t * v2.z,
	};
};

Vector3 Bezier(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t)
{
	Vector3 p0p1 = Lerp(p0, p1, t);
	Vector3 p1p2 = Lerp(p1, p2, t);
	Vector3 p = Lerp(p0p1, p1p2, t);
	return p;
}

void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint2, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color)
{
	const int kSegmentCount = 32;
	for (int i = 0; i < kSegmentCount; ++i)
	{
		float t0 = float(i) / kSegmentCount;
		float t1 = float(i + 1) / kSegmentCount;

		Vector3 bezier0 = Bezier(controlPoint0, controlPoint1, controlPoint2, t0);
		Vector3 bezier1 = Bezier(controlPoint0, controlPoint1, controlPoint2, t1);

		Vector3 screen0 = Transform(Transform(bezier0, viewProjectionMatrix), viewportMatrix);
		Vector3 screen1 = Transform(Transform(bezier1, viewProjectionMatrix), viewportMatrix);

		Novice::DrawLine((int)screen0.x, (int)screen0.y, (int)screen1.x, (int)screen1.y, color);
	};

	int radius = 5;
	int pointColor = BLACK;

	Vector3 controlPoints[3] = {};
	controlPoints[0] = Transform(Transform(controlPoint0, viewProjectionMatrix), viewportMatrix);
	controlPoints[1] = Transform(Transform(controlPoint1, viewProjectionMatrix), viewportMatrix);
	controlPoints[2] = Transform(Transform(controlPoint2, viewProjectionMatrix), viewportMatrix);

	Novice::DrawEllipse((int)controlPoints[0].x, (int)controlPoints[0].y, radius, radius, 0.0f, pointColor, kFillModeSolid);
	Novice::DrawEllipse((int)controlPoints[1].x, (int)controlPoints[1].y, radius, radius, 0.0f, pointColor, kFillModeSolid);
	Novice::DrawEllipse((int)controlPoints[2].x, (int)controlPoints[2].y, radius, radius, 0.0f, pointColor, kFillModeSolid);
}
#pragma endregion

static const int kRowHeight = 20;
	void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label)
	{
		for (int row = 0; row < 4; ++row)
		{
			for (int column = 0; column < 4; ++column)
			{
				Novice::ScreenPrintf(
					x + column * kColumnWidth, y + row * kRowHeight, "%6.03f", matrix.m[row][column], label);
			}
		}

	}

#pragma region 演算子オーバーロード
//二項演算子
Vector3 operator+(const Vector3& v1, const Vector3& v2) { return Add(v1, v2); }
Vector3 operator-(const Vector3& v1, const Vector3& v2) { return Subract(v1, v2); }
Vector3 operator*(float s, const Vector3& v) { return ScalarMultiply(s, v); }
Vector3 operator*(const Vector3& v, float s) { return s * v; }
Vector3 operator/(const Vector3& v, float s) { return ScalarMultiply(1.0f / s, v); }
Matrix4x4 operator+(const Matrix4x4& m1, const Matrix4x4& m2) { return  Add(m1, m2); }
Matrix4x4 operator-(const Matrix4x4& m1, const Matrix4x4& m2) { return Subtract(m1, m2); }
Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) { return Multiply(m1, m2); }
//単項演算子
Vector3 operator-(const Vector3& v) { return { -v.x,-v.y,-v.z }; }
Vector3 operator+(const Vector3& v) { return v; }
//複合代入演算子
//Vector3& operator*=(float s) { x *= s; y *= s; z *= s; return *this; }
//Vector3& operator-=(const Vector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
//Vector3& operator+=(const Vector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
//Vector3& operator/=(float s) { x /= s; y /= s; z /= s; return *this; }
#pragma endregion

Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle)
{
	Vector3 n = Normalize(axis);
	float c = static_cast<float>(cos(angle));
	float s = static_cast<float>( sin(angle));
	float oneMinusC = 1.0f - c;

	Matrix4x4 R;
	R.m[0][0] = c + n.x * n.x * oneMinusC;
	R.m[0][1] = n.x * n.y * oneMinusC + n.z * s;
	R.m[0][2] = n.x * n.z * oneMinusC - n.y * s;
	R.m[0][3] = 0.0f;

	R.m[1][0] = n.y * n.x * oneMinusC - n.z * s;
	R.m[1][1] = c + n.y * n.y * oneMinusC;
	R.m[1][2] = n.y * n.z * oneMinusC + n.x * s;
	R.m[1][3] = 0.0f;

	R.m[2][0] = n.z * n.x * oneMinusC + n.y * s;
	R.m[2][1] = n.z * n.y * oneMinusC - n.x * s;
	R.m[2][2] = c + n.z * n.z * oneMinusC;
	R.m[2][3] = 0.0f;

	R.m[3][0] = 0.0f;
	R.m[3][1] = 0.0f;
	R.m[3][2] = 0.0f;
	R.m[3][3] = 1.0f;

	return R;
}

//MT4_01_01
Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to)
{
	// 0. from, toを正規化して向きを比較できるようにする
	Vector3 u = Normalize(from);
	Vector3 w = Normalize(to);

	// 1.u*vを求める。
	//fromとtoのクロス積の結果を入れたVector3の変数で作る
	Vector3 n = Cross(u, w);

	// 2.cosを求める。
	//floatの変数を作成して、uとvの内積を入れる
	float cosTheta = Dot(u, w);

	// 3. sinを求める。
	//floatの変数を作成して、uとvのクロス積の長さを入れる
	float sinTheta = Length(n);

	// 4. nを求める
		//資料のP6、P7を基にif文を作成。
	//反転行列かどうかを判断する。
	//反転行列の場合
		//(u.x=0||u.y=0)の場合と(u.x=0||u.z=0)の場合でif文を書く。
	//各if文の中身についてはP7を参照。
	//どちらにも当てはまらない場合は0ベクトルなのでassretではじく。
	//反転行列じゃない場合
	//1で求めたクロス積を正規化した内容がnになる
	if (sinTheta < 1e-6f)
	{
		if (cosTheta > 0.9999f)
		{
			return MakeIdentity4x4();
		}

		Vector3 axis;
		if (fabs(u.x) < fabs(u.y) && fabs(u.x) < fabs(u.z))
			axis = Vector3{ 1, 0, 0 };
		else if (fabs(u.y) < fabs(u.z))
			axis = Vector3{ 0, 1, 0 };
		else
			axis = Vector3{ 0, 0, 1 };

		axis = Normalize(Cross(u, axis));

		return MakeRotateAxisAngle(axis, std::numbers::pi_v<float>);
	}

	// 5.　求めた情報を基に任意軸回転を作成、returnする
	n = Normalize(n);
	float oneMinusCos = 1.0f - cosTheta;

	Matrix4x4 R;
	R.m[0][0] = cosTheta + n.x * n.x * oneMinusCos;
	R.m[0][1] = n.x * n.y * oneMinusCos + n.z * sinTheta;
	R.m[0][2] = n.x * n.z * oneMinusCos - n.y * sinTheta;
	R.m[0][3] = 0.0f;

	R.m[1][0] = n.y * n.x * oneMinusCos - n.z * sinTheta;
	R.m[1][1] = cosTheta + n.y * n.y * oneMinusCos;
	R.m[1][2] = n.y * n.z * oneMinusCos + n.x * sinTheta;
	R.m[1][3] = 0.0f;

	R.m[2][0] = n.z * n.x * oneMinusCos + n.y * sinTheta;
	R.m[2][1] = n.z * n.y * oneMinusCos - n.x * sinTheta;
	R.m[2][2] = cosTheta + n.z * n.z * oneMinusCos;
	R.m[2][3] = 0.0f;

	R.m[3][0] = 0.0f;
	R.m[3][1] = 0.0f;
	R.m[3][2] = 0.0f;
	R.m[3][3] = 1.0f;

	return R;
}

//MT4_01_03
struct Quaternion
{
	float x;
	float y;
	float z;
	float w;
};
//Quaternionの積
Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs)
{
	Quaternion qr = 
	{
	   lhs.w* rhs.x + rhs.w * lhs.x + lhs.y * rhs.z - lhs.z * rhs.y,//x
	   lhs.w* rhs.y + rhs.w * lhs.y + lhs.z * rhs.x - lhs.x * rhs.z,//y
	   lhs.w* rhs.z + rhs.w * lhs.z + lhs.x * rhs.y - lhs.y * rhs.x,//z
	   lhs.w* rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z //w
	};
	return qr;
}
//単位Quaternionを返す
Quaternion IdentityQuaternion()
{
	return { 0.0f, 0.0f, 0.0f, 1.0f };  // (x,y,z,w)
}
//共役Quaternionを返す
Quaternion Conjugate(const Quaternion& quaternion)
{
	return { -quaternion.x, -quaternion.y, -quaternion.z, quaternion.w };
}
//Quaternionのnormを返す
float Norm(const Quaternion& quaternion)
{
	return static_cast<float>(sqrt(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w));
}
//正規化したQuaternionを返す
Quaternion Normalize(const Quaternion& quaternion)
{
	float n = Norm(quaternion);
	if (n == 0.0f) return IdentityQuaternion();
	return { quaternion.x / n, quaternion.y / n, quaternion.z / n, quaternion.w / n };
}
//逆Quaternionを返す
Quaternion Inverse(const Quaternion& quaternion)
{
	float n2 = quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w;
	if (n2 == 0.0f) return IdentityQuaternion();
	Quaternion conj = Conjugate(quaternion);
	return { conj.x / n2, conj.y / n2, conj.z / n2, conj.w / n2 };
}


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	int kWindowWidth = 1280;
	int kWindowHeight = 720;

#pragma region 初期化
	//Vector3 rotate{};
	Vector3 sphere1Translate{};
	Vector3 sphere2Translate{};
	Vector3 cameraTranslate{ 0.0f,1.9f,-6.49f };
	Vector3 cameraRotate{ 0.26f,0.0f,0.0f };

	Vector3 kLocalVertices[3] = {};
	kLocalVertices[0] = { 0.0f,0.1f,0.0f };
	kLocalVertices[1] = { -0.1f,-0.1f,0.0f };
	kLocalVertices[2] = { 0.1f,-0.1f,0.0f };

	//球
	Sphere sphere = {};
	sphere.center = { -3.0f, 0.0f, 3.0f }; // center（x, y, z）
	sphere.radius = 1.0f;    // radius
	sphere.color = WHITE;

	//平面
	Plane plane = {};
	plane.normal = { 0.0f, 1.0f, 0.0f };
	plane.distance = 2.0f;


	Vector3 start = { -0.7f,0.3f,0.0f };
	Vector3 end = { 2.0f,-0.5f,0.0f };

	//線
	Segment segment = {};
	segment.origin = start;
	segment.diff = Subract(end, start);
	segment.color = WHITE;

	//三角形
	Triangle triangle = {};
	triangle.vertices[0] = { 1.0f,0.0f };
	triangle.vertices[1] = { 0.0f,1.0f };
	triangle.vertices[2] = { -1.0f,0.0f };
	triangle.color = WHITE;

	//AABB1
	AABB aabb1
	{
		aabb1.min = {-0.5f,-0.5f,-0.5f},
		aabb1.max = {0.5f,0.5f,0.5f}
	};
	aabb1.color = WHITE;

	//AABB2
	AABB aabb2
	{
		aabb2.min = {0.2f,0.2f,0.2f},
		aabb2.max = {1.0f,1.0f,1.0f}
	};
	aabb2.color = WHITE;

	//ベジェ曲線
	Vector3 controlPoints[3] =
	{
		{-0.8f,0.58f,1.0f},
		{1.76f,1.0f,-0.3f},
		{0.94f,-0.7f,2.3f},
	};

	////階層構造の初期化////
	Vector3 translates[3] =
	{
		{0.2f,1.0f,0.0f},
		{0.4f,0.0f,0.0f},
		{0.3f,0.0f,0.0f},
	};

	Vector3 rotates[3] =
	{
		{0.0f,0.0f,-6.8f},
		{0.0f,0.0f,-1.4f},
		{0.0f,0.0f,0.0f },
	};

	Vector3 scales[3] =
	{
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
		{1.0f,1.0f,1.0f},
	};
	
	//MT4
	//01_01
	Vector3 axis = Normalize(Vector3{ 1.0f,1.0f,1.0f });
	float angle =  0.44f ;
	Matrix4x4 rotateMatrix = MakeRotateAxisAngle(axis, angle);
	MatrixScreenPrintf(0, 0, rotateMatrix, "rotateMatrix");
	//01_02
	Vector3 from0 = Normalize(Vector3{ 1.0f,0.7f,0.5f });
	Vector3 to0 = -from0;
	Vector3 from1 = Normalize(Vector3{ -0.6f,0.9f,0.2f });
	Vector3 to1 = Normalize(Vector3{ 0.4f,0.7f,-0.5f });
	Matrix4x4 rotateMatrix0 = DirectionToDirection(Normalize(Vector3{ 1.0f,0.0f,0.0f }), Normalize(Vector3{ -1.0f,0.0f,0.0f }));
	Matrix4x4 rotateMatrix1 = DirectionToDirection(from0, to0);
	Matrix4x4 rotateMatrix2 = DirectionToDirection(from1, to1);
	//01_03
	Quaternion q1 = { 2.0f,3.0f,4.0f,1.0f };
	Quaternion q2 = { 1.0f,3.0f,5.0f,2.0f };
	Quaternion identity = IdentityQuaternion();
	Quaternion conj = Conjugate(q1);
	Quaternion inv = Inverse(q1);
	Quaternion normal = Normalize(q1);
	Quaternion mul1 = Multiply(q1,q2);
	Quaternion mul2 = Multiply(q2, q1);
	float norm = Norm(q1);
#pragma endregion

	//　ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0)
	{
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
#pragma region レンダリングパイプライン
		Matrix4x4 cameraMatrix = MakeAffinMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);

		Matrix4x4 viewMatrix = Inverse(cameraMatrix);

		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);

		Matrix4x4 viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
#pragma endregion

#pragma region 演算子オーバーロード
		Vector3 a{ 0.2f,1.0f,0.0f };
		Vector3 b{ 2.4f,3.1f,1.2f };
		Vector3 c = a + b;
		Vector3 d = Subract(a, b);
		Vector3 e = a * 2.4f;
		Vector3 rotate{ 0.4f,1.43f,-0.8f };
		Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
		Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
		Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	//	Matrix4x4 rotateMatrix = rotateXMatrix * rotateYMatrix * rotateZMatrix;
#pragma endregion

#pragma region ImGui
		ImGui::Begin("Window");

		//ImGui::Text("Sphere");
		//ImGui::DragFloat3("sphere.center", &sphere.center.x, 0.1f);
		//ImGui::DragFloat("sphere.radius", &sphere.radius, 0.1f);

		//ImGui::Text("Plane");
		//ImGui::DragFloat3("plane.normal", &plane.normal.x, 0.1f);
		//ImGui::DragFloat("plane.distance", &plane.distance, 0.1f);

		//ImGui::Text("Segment");
		//ImGui::DragFloat3("segment.origin", &segment.origin.x, 0.1f);
		//ImGui::DragFloat3("segment.diff", &segment.diff.x, 0.1f);

		//ImGui::Text("Triangle");
		//ImGui::DragFloat3("Triangle.v0 ", &triangle.vertices[0].x, 0.1f);
		//ImGui::DragFloat3("Triangle.v1 ", &triangle.vertices[1].x, 0.1f);
		//ImGui::DragFloat3("Triangle.v2 ", &triangle.vertices[2].x, 0.1f);

		//ImGui::Text("AABB1");
		//ImGui::DragFloat3("aabb1.min", &aabb1.min.x, 0.1f);
		//ImGui::DragFloat3("aabb1.max", &aabb1.max.x, 0.1f);

		//ImGui::Text("AABB2");
		//ImGui::DragFloat3("aabb2.min", &aabb2.min.x, 0.1f);
		//ImGui::DragFloat3("aabb2.max", &aabb2.max.x, 0.1f);

		//ImGui::Text("Bezier");
		//ImGui::DragFloat3("controlPoints[0]", &controlPoints[0].x, 0.01f);
		//ImGui::DragFloat3("controlPoints[1]", &controlPoints[1].x, 0.01f);
		//ImGui::DragFloat3("controlPoints[2]", &controlPoints[2].x, 0.01f);

		//ImGui::Text("CameraSetting");
		//ImGui::DragFloat3("cameraRotate", &cameraRotate.x, 0.01f);
		//ImGui::DragFloat3("cameraTranslate", &cameraTranslate.x, 0.01f);

		//ImGui::DragFloat3("translates[0]", &translates[0].x, 0.01f);
		//ImGui::DragFloat3("rotate[0]", &rotates[0].x, 0.01f);
		//ImGui::DragFloat3("scales[0]", &scales[0].x, 0.01f);
		//
		//ImGui::DragFloat3("translates[1]", &translates[1].x, 0.01f);
		//ImGui::DragFloat3("rotate[1]", &rotates[1].x, 0.01f);
		//ImGui::DragFloat3("scales[1]", &scales[1].x, 0.01f);
		//
		//ImGui::DragFloat3("translates[2]", &translates[2].x, 0.01f);
		//ImGui::DragFloat3("rotate[2]", &rotates[2].x, 0.01f);
		//ImGui::DragFloat3("scales[2]", &scales[2].x, 0.01f);

		//ImGui::Text("c:%f,%f,%f", c.x, c.y, c.z);
		//ImGui::Text("d:%f,%f,%f", d.x, d.y, d.z);
		//ImGui::Text("e:%f,%f,%f", e.x, e.y, e.z);
		//ImGui::Text(
		//	"matrix:\n%f,%f,%f,%f\n%f,%f,%f,%f,\n%f,%f,%f,%f,\n%f,%f,%f,%f,\n",
		//	rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2], rotateMatrix.m[0][3],
		//	rotateMatrix.m[1][0], rotateMatrix.m[1][1], rotateMatrix.m[1][2], rotateMatrix.m[1][3],
		//	rotateMatrix.m[2][0], rotateMatrix.m[2][1], rotateMatrix.m[2][2], rotateMatrix.m[2][3],
		//	rotateMatrix.m[3][0], rotateMatrix.m[3][1], rotateMatrix.m[3][2], rotateMatrix.m[3][3]);

		ImGui::End();
#pragma endregion

		plane.normal = Normalize(plane.normal);

#pragma region 当たったら赤にする
		//球と平面の当たった時の色変更
				/*
				bool isHit = IsCollision(sphere, plane);
				if (isHit)
				{
					sphere.color = RED;
				}
				else
				{
					sphere.color = WHITE;
				}
				*/

				//平面と線の当たった時の色変更
				/*
				bool isHit = IsCollision(segment, plane);
				if (isHit)
				{
					segment.color = RED;
				}
				else
				{
					segment.color = WHITE;
				}
				*/

				//線と三角形の当たった時の色変更
				/*
				bool isHit = IsCollsion(triangle, segment);
				if (isHit)
				{
					triangle.color = RED;
				}
				else
				{
					triangle.color = WHITE;
				}
				*/

				//AABB1とAABB2の当たった時に色変更
				//bool isHit = IsCollsion(aabb1, aabb2);
				//if (isHit)
				//{
				//	aabb1.color = RED;
				//	aabb2.color = RED;
				//}
				//else
				//{
				//	aabb1.color = WHITE;
				//	aabb2.color = WHITE;
				//}

				//AABB1と球の当たった時に色変更
				//bool isHit = IsCollision(aabb1, sphere);
				//if (isHit)
				//{
				//	aabb1.color = RED;
				//	sphere.color = RED;
				//}
				//else
				//{
				//	aabb1.color = WHITE;
				//	sphere.color = WHITE;
				//}

				//AABB1と線の当たった時に色変更
				//bool isHit = IsCollision(aabb1, sphere);
				//if (isHit)
				//{
				//	aabb1.color = RED;
				//	segment.color = RED;
				//}
				//else
				//{
				//	aabb1.color = WHITE;
				//	segment.color = WHITE;
				//}
#pragma endregion

#pragma region 階層構造
		//Matrix4x4 localMatrix[3] = {};
		//for (int i = 0; i < 3; i++)
		//{
		//	localMatrix[i] = MakeAffinMatrix(translates[i], rotates[i], scales[i]);
		//}
		//
		//Matrix4x4 worldMatrix[3] = {};
		//worldMatrix[0] = localMatrix[0];
		//worldMatrix[1] = Multiply(worldMatrix[0], localMatrix[1]);
		//worldMatrix[2] = Multiply(worldMatrix[1], localMatrix[2]); Vector3 sPosition = {};
		//
		//sPosition.x = { worldMatrix[0].m[3][0] };
		//sPosition.y = { worldMatrix[0].m[3][1] };
		//sPosition.z = { worldMatrix[0].m[3][2] };
		//
		//Vector3 ePosition = {};
		//ePosition.x = { worldMatrix[1].m[3][0] };
		//ePosition.y = { worldMatrix[1].m[3][1] };
		//ePosition.z = { worldMatrix[1].m[3][2] };
		//
		//Vector3 hPosition = {};
		//hPosition.x = { worldMatrix[2].m[3][0] };
		//hPosition.y = { worldMatrix[2].m[3][1] };
		//hPosition.z = { worldMatrix[2].m[3][2] };
		//
		////肩(赤)
		//Sphere shoulder = {};
		//shoulder.center = { sPosition.x,sPosition.y,sPosition.z }; // center（x, y, z）
		//shoulder.radius = 0.1f;    // radius
		//shoulder.color = RED;
		//
		////肘(緑)
		//Sphere elbow = {};
		//elbow.center = { ePosition.x, ePosition.y, ePosition.z }; // center（x, y, z）
		//elbow.radius = 0.1f;    // radius
		//elbow.color = GREEN;
		//
		////手(青)
		//Sphere hand = {};
		//hand.center = { hPosition.x, hPosition.y, hPosition.z }; // center（x, y, z）
		//hand.radius = 0.1f;    // radius
		//hand.color = BLUE;
		//
		////肩(赤)から肘(緑)
		//Segment	upperArm = {};
		//upperArm.origin = { shoulder.center };
		//upperArm.diff = Subract(elbow.center, shoulder.center);
		//upperArm.color = WHITE;
		////肘(緑)から手(青)
		//Segment forearm = {};
		//forearm.origin = { elbow.center };
		//forearm.diff = Subract(hand.center, elbow.center);
		//forearm.color = WHITE;
#pragma endregion


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
//MatrixScreenPrintf(0, 0, rotateMatrix, "rotateMatrix");
//MatrixScreenPrintf(0, 0, rotateMatrix0, "rotateMatrix0");
//MatrixScreenPrintf(0, kRowHeight*5, rotateMatrix1, "rotateMatrix1");
//MatrixScreenPrintf(0, kRowHeight * 10, rotateMatrix2, "rotateMatrix2");

#pragma region 01_03_Quaternion
Novice::ScreenPrintf(0, 0,  " %0.2f  %0.2f  %0.2f   %0.2f", identity.x, identity.y, identity.z, identity.w);
Novice::ScreenPrintf(0, 20, "%0.2f %0.2f %0.2f   %0.2f", conj.x, conj.y, conj.z, conj.w);
Novice::ScreenPrintf(0, 40, "%0.2f %0.2f %0.2f   %0.2f", inv.x, inv.y, inv.z, inv.w);
Novice::ScreenPrintf(0, 60, " %0.2f  %0.2f  %0.2f   %0.2f", normal.x, normal.y, normal.z, normal.w);
Novice::ScreenPrintf(0, 80, " %0.2f  %0.2f %0.2f %0.2f", mul1.x, mul1.y, mul1.z, mul1.w);
Novice::ScreenPrintf(0, 100," %0.2f %0.2f %0.2f %0.2f", mul2.x, mul2.y, mul2.z, mul2.w);
Novice::ScreenPrintf(0, 120," %0.2f", norm);

Novice::ScreenPrintf(230, 0,  ":Identity");
Novice::ScreenPrintf(230, 20, ":Conjugate");
Novice::ScreenPrintf(230, 40, ":Inverse");
Novice::ScreenPrintf(230, 60, ":Normalize");
Novice::ScreenPrintf(230, 80, ":Multiply(q1,q2)");
Novice::ScreenPrintf(230, 100,":Multiply(q2,q1)");
Novice::ScreenPrintf(230, 120,":Norm");
#pragma endregion

#pragma region 色んな形
		//地面
		//DrawGrid(viewProjectionMatrix, viewportMatrix);
		//球
		//DrawSphere(sphere, viewProjectionMatrix, viewportMatrix, sphere.color);
		//平面
		//DrawPlane(plane, viewProjectionMatrix, viewportMatrix, RED);
		//線
		//DrawSegment(segment, viewProjectionMatrix, viewportMatrix, segment.color);
		//三角形
		//DrawTriangle(triangle, viewProjectionMatrix, viewportMatrix, triangle.color);
		//AABB1
		//DrawAABB(aabb1, viewProjectionMatrix, viewportMatrix, aabb1.color);
		//AABB2
		//DrawAABB(aabb2, viewProjectionMatrix, viewportMatrix, aabb2.color);
		//ベジェ曲線
		//DrawBezier(controlPoints[0], controlPoints[1], controlPoints[2], viewProjectionMatrix, viewportMatrix, BLUE);
#pragma endregion

#pragma region 階層構造の描画
		//肩
		//DrawSphere(shoulder, viewProjectionMatrix, viewportMatrix, shoulder.color);
		////肘
		//DrawSphere(elbow, viewProjectionMatrix, viewportMatrix, elbow.color);
		////手
		//DrawSphere(hand, viewProjectionMatrix, viewportMatrix, hand.color);
		////肩から肘
		//DrawSegment(upperArm, viewProjectionMatrix, viewportMatrix, upperArm.color);
		////肘から手
		//DrawSegment(forearm, viewProjectionMatrix, viewportMatrix, forearm.color);
#pragma endregion

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0)
		{
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}