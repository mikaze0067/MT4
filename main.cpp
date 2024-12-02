#include <Novice.h>
#include <cmath>
#include <cassert>

const char kWindowTitle[] = "MT4_01_04_Basic";

struct Vector3 {
	float x, y, z;
};

struct Quaternion {
	float x, y, z, w;
};

struct Matrix4x4 {
	float m[4][4];
};

float Dot(const Vector3& v1, const Vector3& v2) {
	float ans;
	ans = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	return ans;
}

float Length(const Vector3& v) {
	float ans;
	ans = sqrtf(Dot(v, v));
	return ans;
}
//正規化
Vector3 Normalize(const Vector3& v) {
	float len = Length(v);
	if (len == 0) {
		// 長さが0の場合、ゼロベクトルを返すか、エラー処理を行う
		return Vector3(0, 0, 0);
	}
	return Vector3(v.x / len, v.y / len, v.z / len);
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 ans;
	ans.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	ans.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	ans.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);
	ans.x /= w;
	ans.y /= w;
	ans.z /= w;

	return ans;
}

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
	// 回転軸を正規化
	Vector3 normalizedAxis = Normalize(axis);

	// 回転角度の半分を計算
	float halfAngle = angle / 2.0f;

	// コサインとサインを計算
	float sinHalfAngle = std::sin(halfAngle);
	float cosHalfAngle = std::cos(halfAngle);

	// クォータニオンの計算
	Quaternion q;
	q.x = normalizedAxis.x * sinHalfAngle; // x成分
	q.y = normalizedAxis.y * sinHalfAngle; // y成分
	q.z = normalizedAxis.z * sinHalfAngle; // z成分
	q.w = cosHalfAngle;                    // スカラー部分

	return q;
}


Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion) {
	// 1. ベクトルをクォータニオン形式に変換（スカラー部分は0）
	Quaternion r = { vector.x, vector.y, vector.z, 0.0f };

	// 2. クォータニオンの共役を計算
	Quaternion qConjugate = {
		-quaternion.x,
		-quaternion.y,
		-quaternion.z,
		quaternion.w
	};

	// 3. 計算: r' = q * r * q^*
	Quaternion temp; // q * r の結果
	temp.x = quaternion.w * r.x + quaternion.x * r.w + quaternion.y * r.z - quaternion.z * r.y;
	temp.y = quaternion.w * r.y - quaternion.x * r.z + quaternion.y * r.w + quaternion.z * r.x;
	temp.z = quaternion.w * r.z + quaternion.x * r.y - quaternion.y * r.x + quaternion.z * r.w;
	temp.w = quaternion.w * r.w - quaternion.x * r.x - quaternion.y * r.y - quaternion.z * r.z;

	Quaternion result; // temp * q^* の結果
	result.x = temp.w * qConjugate.x + temp.x * qConjugate.w + temp.y * qConjugate.z - temp.z * qConjugate.y;
	result.y = temp.w * qConjugate.y - temp.x * qConjugate.z + temp.y * qConjugate.w + temp.z * qConjugate.x;
	result.z = temp.w * qConjugate.z + temp.x * qConjugate.y - temp.y * qConjugate.x + temp.z * qConjugate.w;
	result.w = temp.w * qConjugate.w - temp.x * qConjugate.x - temp.y * qConjugate.y - temp.z * qConjugate.z;

	// 4. 回転後のベクトル部分 (x, y, z) を返す
	return { result.x, result.y, result.z };
}

Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion) {
	Matrix4x4 matrix;

	// クォータニオンの各成分
	float x = quaternion.x;
	float y = quaternion.y;
	float z = quaternion.z;
	float w = quaternion.w;

	// 成分の2乗や積を事前計算
	float xx = x * x;
	float yy = y * y;
	float zz = z * z;
	float ww = w * w;

	float xy = x * y;
	float xz = x * z;
	float yz = y * z;

	float wx = w * x;
	float wy = w * y;
	float wz = w * z;

	// 回転行列の各要素を計算
	matrix.m[0][0] = ww + xx - yy - zz;
	matrix.m[0][1] = 2.0f * (xy + wz);//
	matrix.m[0][2] = 2.0f * (xz - wy);//
	matrix.m[0][3] = 0.0f;

	matrix.m[1][0] = 2.0f * (xy - wz);//
	matrix.m[1][1] = ww - xx + yy - zz;
	matrix.m[1][2] = 2.0f * (yz + wx);//
	matrix.m[1][3] = 0.0f;

	matrix.m[2][0] = 2.0f * (xz + wy);//
	matrix.m[2][1] = 2.0f * (yz - wx);//
	matrix.m[2][2] = ww - xx - yy + zz;
	matrix.m[2][3] = 0.0f;

	matrix.m[3][0] = 0.0f;
	matrix.m[3][1] = 0.0f;
	matrix.m[3][2] = 0.0f;
	matrix.m[3][3] = 1.0f;

	return matrix;
}

static const int kRowHeight = 20;
static const int kColumnWidth = 60;
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix, const char* label) {
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(
				x + column * kColumnWidth, y + (row + 1) * kRowHeight, "%6.03f",
				matrix.m[row][column]);
		}
	}
}

void VectorScreenPrintf(int x, int y, const Vector3& vector, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

void QuaternionScreenPrintf(int x, int y, Quaternion quaternion, const char* label) {
	Novice::ScreenPrintf(x, y, "%.02f", quaternion.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", quaternion.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", quaternion.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%.02f", quaternion.w);
	Novice::ScreenPrintf(x + kColumnWidth * 4, y, label);
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	const int kWindowWidth = 1280;
	const int kWindowHeight = 720;
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 v1{ 1.0f, 3.0f, -5.0f };
	Vector3 v2{ 4.0f, -1.0f, 2.0f };
	//float k = { 4.0f };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Quaternion rotation = MakeRotateAxisAngleQuaternion(Normalize(Vector3{ 1.0f, 0.4f, -0.2f }), 0.45f);
		Vector3 pointY = { 2.1f, -0.9f, 1.3f };
		Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation);
		Vector3 rotateByQuaternion = RotateVector(pointY, rotation);
		Vector3 rotateByMatrix = Transform(pointY, rotateMatrix);

		QuaternionScreenPrintf(0, kRowHeight * 0, rotation, "   : rotation");
		MatrixScreenPrintf(0, kRowHeight * 1, rotateMatrix, "rotateMatrix");
		VectorScreenPrintf(0, kRowHeight * 6, rotateByQuaternion, "   : rotateByQuaternion");
		VectorScreenPrintf(0, kRowHeight * 7, rotateByMatrix, "   : rotateByMatrix");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}