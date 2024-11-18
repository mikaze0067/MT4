#include <Novice.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <numbers>
#include <Vector3.h>

const char kWindowTitle[] = "LC1C_16_ササノ_ミカゼ";

struct Quaternion {
	float x, y, z, w;
};

Quaternion IdentityQuaternion() {

	Quaternion identity;
	identity.w = 1.0f; // 実部を 1 に設定
	identity.x = 0.0f; // 虚部を 0 に設定
	identity.y = 0.0f;
	identity.z = 0.0f;

	return identity;
}

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs) {

	Quaternion result;

	result.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
	result.x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
	result.y = lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x;
	result.z = lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w;

	return result;
}

Quaternion Conjugate(const Quaternion& quaternion) {

	Quaternion conjugate;
	conjugate.w = quaternion.w;
	conjugate.x = -quaternion.x; // x の符号を反転
	conjugate.y = -quaternion.y; // y の符号を反転
	conjugate.z = -quaternion.z; // z の符号を反転

	return conjugate;
}

float Norm(const Quaternion& quaternion) {

	return std::sqrt(
		quaternion.w * quaternion.w +
		quaternion.x * quaternion.x +
		quaternion.y * quaternion.y +
		quaternion.z * quaternion.z
	);
}

Quaternion Normalize(const Quaternion& quaternion) {

	float norm = Norm(quaternion);

	//ゼロ除算を避けるためにノルムがゼロかどうかをチェック
	if (norm == 0.0f) {
		return { 0.0f, 0.0f, 0.0f, 0.0f };
	}

	//クォータニオンの各成分を正規化
	return {
		quaternion.x / norm,
		quaternion.y / norm,
		quaternion.z / norm,
		quaternion.w / norm
	};
}

Quaternion Inverse(const Quaternion& quaternion) {
	float normSquared = Norm(quaternion) * Norm(quaternion);
	if (normSquared == 0.0f) {
		return { 0.0f, 0.0f, 0.0f, 0.0f };
	}
	Quaternion conjugate = Conjugate(quaternion);
	return {
		conjugate.x / normSquared,
		conjugate.y / normSquared,
		conjugate.z / normSquared,
		conjugate.w / normSquared
	};
}

static const int kRowHeight = 20;
static const int kColumnWidth = 60;

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

		Quaternion q1 = { 2.0f, 3.0f, 4.0f, 1.0f };
		Quaternion q2 = { 1.0f, 3.0f, 5.0f, 2.0f };
		Quaternion identity = IdentityQuaternion();
		Quaternion conj = Conjugate(q1);
		Quaternion inv = Inverse(q1);
		Quaternion normal = Normalize(q1);
		Quaternion mul1 = Multiply(q1, q2);
		Quaternion mul2 = Multiply(q2, q1);
		float norm = Norm(q1);

		QuaternionScreenPrintf(0, kRowHeight * 0, identity, "   : Identity");
		QuaternionScreenPrintf(0, kRowHeight * 1, conj, "   : Conjugate");
		QuaternionScreenPrintf(0, kRowHeight * 2, inv, "   : Inverse");
		QuaternionScreenPrintf(0, kRowHeight * 3, normal, "   : Normalize");
		QuaternionScreenPrintf(0, kRowHeight * 4, mul1, "   : Multiply(q1, q2)");
		QuaternionScreenPrintf(0, kRowHeight * 5, mul2, "   : Multiply(q2, q1)");
		Novice::ScreenPrintf(0, kRowHeight * 6, "%.02f                          : Norm", norm);

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