#pragma once

#include<cmath>
#include<iostream>
#include<limits>
#include<random>
using std::sqrt;
using std::shared_ptr;
using std::make_shared;

class vec3 {
public:
	//构造函数
	vec3() :e{ 0,0,0 } { }
	vec3(double e0, double e1, double e2) :e{ e0, e1, e2 } { }
	//坐标
	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }
	//运算符
	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }
	vec3& operator+=(const vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}
	vec3& operator*=(const double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}
	vec3& operator/=(const double t) {
		return *this *= 1 / t;
	}
	//长度相关
	double length() const {
		return sqrt(length_squared());
	}
	double length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	vec3& operator+(double i) ;

	bool near_zero() const {
		// Return true if the vector is close to zero in all dimensions.
		const auto s = 1e-8;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}
public:
	double e[3];
};

inline vec3& vec3::operator+(double i)
{
	e[0] += i;
	e[1] += i;
	e[2] += i;
	return *this;
}

using point3 = vec3;	//3D point
using color = vec3;		//RGB color
// vec3 Utility Functions
inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}
inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}
inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}
inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}
inline vec3 operator*(double t, const vec3& v) {
	return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
inline vec3 operator*(const vec3& v, double t) {
	return t * v;
}
inline vec3 operator/(const vec3& v, double t) {
	return (1 / t) * v;
}
//数量积
inline double dot(const vec3& u, const vec3& v) {
	return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}
//叉乘
inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
		u.e[2] * v.e[0] - u.e[0] * v.e[2],
		u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}
inline vec3 normalize(const vec3& v) {
	return v / v.length();
}

static vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}

static vec3 refract(const vec3& v, const vec3& n, double etai_over_etat)
{
	auto cos_theta = dot(-v, n);
	vec3 r_out_perp = etai_over_etat * (v + cos_theta * n);
	vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}

constexpr double PI = 3.141592657;
constexpr double Infinity = std::numeric_limits<double>::infinity();

inline double degree2radian(const double degree)
{
	return degree * PI / 180.0;
}

//返回一个随机double数
inline double random_double()
{
	return rand() / (RAND_MAX + 1.0);
}
inline double random_double(double min, double max)
{
	return min + (max - min) * random_double();
}

//返回一个随机的vec3
static vec3 random()
{
	return {random_double(), random_double(), random_double()};
}

static vec3 random(double min, double max)
{
	return {random_double(min, max), random_double(min, max), random_double(min, max)};
}

//在单位球里面选一个随机vec3点
inline vec3 random_in_unit_sphere()
{
	auto a = random_double(0, 2 * PI);
	auto z = random_double(-1, 1);
	auto r = sqrt(1 - z * z);
	return {r * cos(a), r * sin(a), z};
}

inline int random_int(int min = 0, int max = 100)
{
	return static_cast<int>(random_double(min, max+1));
}

//在单位圆盘里随机选一个点
inline vec3 random_in_unit_disk()
{
	while (true) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() >= 1)
			continue;
		return p;
	}
}

inline double clamp(double max, double min, double x)
{
	if (x < min) x = min;
	if (x > max) x = max;
	return x;
}

inline double schlick(double cosine, double ref_idx)
{
	double R0 = (1 - ref_idx) / (1 + ref_idx);
	R0 *= R0;
	return R0 + (1 - R0) * pow((1 - cosine), 5);
}
