#pragma once
#include <d3d10_1.h>
#include <d3d10.h>
#include <DirectXMath.h>
#include "include\D3DX10math.h"
#ifdef NDEBUG
#define Assert( _exp ) ((void)0)
#else
#define Assert(x)
#endif

#include <sstream>

#define CHECK_VALID( _v ) 0

#define FastSqrt(x)	(sqrt)(x)

#define M_PI		3.14159265358979323846	// matches value in gcc v2 math.h

#define M_PI_2      (M_PI * 2.f)

#define M_PI_F		((float)(M_PI))	// Shouldn't collide with anything.

#define M_PHI		1.61803398874989484820 // golden ratio

// NJS: Inlined to prevent floats from being autopromoted to doubles, as with the old system.
#ifndef RAD2DEG
#define RAD2DEG(x)  ((float)(x) * (float)(180.f / M_PI_F))
#endif

#ifndef DEG2RAD
#define DEG2RAD(x)  ((float)(x) * (float)(M_PI_F / 180.f))
#endif

// MOVEMENT INFO
enum
{
	PITCH = 0,	// up / down
	YAW,		// left / right
	ROLL		// fall over
};

// decls for aligning data


#define ALIGN16 __declspec(align(16))
#define VALVE_RAND_MAX 0x7fff
#define Vector3Expand(v) (v).x, (v).y, (v).z
typedef struct D3DXMATRIXX
{
	struct {
		FLOAT _11; FLOAT _21;  FLOAT  _31; FLOAT _41;
		FLOAT _12; FLOAT _22;  FLOAT _32;  FLOAT _42;
		FLOAT _13; FLOAT _23;  FLOAT _33;  FLOAT _43;
		FLOAT _14; FLOAT _24;  FLOAT _34;  FLOAT _44;
	};
} D3DXMATRIXX, * LPD3DXMATRIXX;
typedef float vec_t;
class Vector4
{
public:
	float x;
	float y;
	float z;
	float w;
};
class Vector3
{
public:
	float x, y, z;
	Vector3(void);
	Vector3(float X, float Y, float Z);
	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);
	float operator[](int i) const;
	float& operator[](int i);
	inline void Zero();
	bool operator==(const Vector3& v) const;
	bool operator!=(const Vector3& v) const;
	inline Vector3& operator+=(const Vector3& v);
	inline Vector3& operator-=(const Vector3& v);
	inline Vector3& operator*=(const Vector3& v);
	inline Vector3& operator*=(float s);
	inline Vector3& operator/=(const Vector3& v);
	inline Vector3& operator/=(float s);
	inline Vector3& operator+=(float fl);
	inline Vector3& operator-=(float fl);
	inline float	Length() const;
	inline float LengthSqr(void) const
	{
		CHECK_VALID(*this);
		return (x * x + y * y + z * z);
	}
	float magnitude()
	{
		return sqrt(x * x + y * y + z * z);
	}
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance&&
			y > -tolerance && y < tolerance&&
			z > -tolerance && z < tolerance);
	}
	inline Vector3 ToFRotator()
	{
		float RADPI = (float)(180 / M_PI);
		float Yaw = (float)atan2f(y, x) * RADPI;
		float Pitch = (float)atan2f(z, sqrtf(powf(x, 2.0) + powf(y, 2.0))) * RADPI;
		float Roll = 0;
		return  Vector3(Pitch, Yaw, Roll);
	}
	Vector3 to_angle() const
	{
		Vector3 angles;

		if (y == 0.0f && x == 0.0f)
		{
			angles.x = (z > 0.0f) ? 270.0f : 90.0f;
			angles.y = 0.0f;
		}
		else
		{
			angles.x = std::atan2(-z, Length()) * -180 / (float)M_PI;
			angles.y = std::atan2(y, x) * 180 / (float)M_PI;

			if (angles.y > 90)
				angles.y -= 180;
			else if (angles.y < 90)
				angles.y += 180;
			else if (angles.y == 90)
				angles.y = 0;
		}

		angles.z = 0.0f;

		return angles;
	}

	Vector3	Normalize();
	float	NormalizeInPlace();
	inline float	DistTo(const Vector3& vOther) const;
	inline float	DistToSqr(const Vector3& vOther) const;
	float	Dot(const Vector3& vOther) const;
	float	Length2D(void) const;
	float	Length2DSqr(void) const;
	void	MulAdd(const Vector3& a, const Vector3& b, float scalar);
	Vector3& operator=(const Vector3& vOther);
	Vector3	operator-(void) const;
	Vector3	operator+(const Vector3& v) const;
	Vector3	operator-(const Vector3& v) const;
	Vector3	operator*(const Vector3& v) const;
	Vector3	operator/(const Vector3& v) const;
	Vector3	operator*(float fl) const;
	Vector3	operator/(float fl) const;
	static float Dot(Vector3 lhs, Vector3 rhs)
	{
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
	}
	// Base address...
	float* Base();
	float const* Base() const;
};

//===============================================
inline void Vector3::Init(float ix, float iy, float iz)
{
	x = ix; y = iy; z = iz;
	CHECK_VALID(*this);
}
//===============================================
inline Vector3::Vector3(float X, float Y, float Z)
{
	x = X; y = Y; z = Z;
	CHECK_VALID(*this);
}
//===============================================
inline Vector3::Vector3(void) { }
//===============================================
inline void Vector3::Zero()
{
	x = y = z = 0.0f;
}
//===============================================
inline void Vector3Clear(Vector3& a)
{
	a.x = a.y = a.z = 0.0f;
}
//===============================================
inline Vector3& Vector3::operator=(const Vector3& vOther)
{
	CHECK_VALID(vOther);
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}
//===============================================
inline float& Vector3::operator[](int i)
{
	Assert((i >= 0) && (i < 3));
	return ((float*)this)[i];
}
//===============================================
inline float Vector3::operator[](int i) const
{
	Assert((i >= 0) && (i < 3));
	return ((float*)this)[i];
}
//===============================================
inline bool Vector3::operator==(const Vector3& src) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (src.x == x) && (src.y == y) && (src.z == z);
}
//===============================================
inline bool Vector3::operator!=(const Vector3& src) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (src.x != x) || (src.y != y) || (src.z != z);
}
//===============================================
inline void Vector3Copy(const Vector3& src, Vector3& dst)
{
	CHECK_VALID(src);
	dst.x = src.x;
	dst.y = src.y;
	dst.z = src.z;
}
//===============================================
inline  Vector3& Vector3::operator+=(const Vector3& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x += v.x; y += v.y; z += v.z;
	return *this;
}
//===============================================
inline  Vector3& Vector3::operator-=(const Vector3& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}
//===============================================
inline  Vector3& Vector3::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;
	CHECK_VALID(*this);
	return *this;
}
//===============================================
inline  Vector3& Vector3::operator*=(const Vector3& v)
{
	CHECK_VALID(v);
	x *= v.x;
	y *= v.y;
	z *= v.z;
	CHECK_VALID(*this);
	return *this;
}
//===============================================
inline Vector3& Vector3::operator+=(float fl)
{
	x += fl;
	y += fl;
	z += fl;
	CHECK_VALID(*this);
	return *this;
}
//===============================================
inline Vector3& Vector3::operator-=(float fl)
{
	x -= fl;
	y -= fl;
	z -= fl;
	CHECK_VALID(*this);
	return *this;
}
//===============================================
inline  Vector3& Vector3::operator/=(float fl)
{
	Assert(fl != 0.0f);
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	CHECK_VALID(*this);
	return *this;
}
//===============================================
inline  Vector3& Vector3::operator/=(const Vector3& v)
{
	CHECK_VALID(v);
	Assert(v.x != 0.0f && v.y != 0.0f && v.z != 0.0f);
	x /= v.x;
	y /= v.y;
	z /= v.z;
	CHECK_VALID(*this);
	return *this;
}
//===============================================
inline float Vector3::Length(void) const
{
	CHECK_VALID(*this);

	float root = 0.0f;

	float sqsr = x * x + y * y + z * z;

	root = sqrt(sqsr);

	return root;
}
//===============================================
inline float Vector3::Length2D(void) const
{
	CHECK_VALID(*this);

	float root = 0.0f;

	float sqst = x * x + y * y;

	root = sqrt(sqst);

	return root;
}
//===============================================
inline float Vector3::Length2DSqr(void) const
{
	return (x * x + y * y);
}
//===============================================
inline Vector3 CrossProduct(const Vector3& a, const Vector3& b)
{
	return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}
//===============================================
float Vector3::DistTo(const Vector3& vOther) const
{
	Vector3 delta;

	delta.x = x - vOther.x;
	delta.y = y - vOther.y;
	delta.z = z - vOther.z;

	return delta.Length();
}
float Vector3::DistToSqr(const Vector3& vOther) const
{
	Vector3 delta;

	delta.x = x - vOther.x;
	delta.y = y - vOther.y;
	delta.z = z - vOther.z;

	return delta.LengthSqr();
}
//===============================================
inline Vector3 Vector3::Normalize()
{
	Vector3 Vector3;
	float length = this->Length();

	if (length != 0)
	{
		Vector3.x = x / length;
		Vector3.y = y / length;
		Vector3.z = z / length;
	}
	else
	{
		Vector3.x = Vector3.y = 0.0f; Vector3.z = 1.0f;
	}

	return Vector3;
}
//===============================================
// changed that to fit awall, paste from xaE
inline float Vector3::NormalizeInPlace()
{
	float radius = FastSqrt(x * x + y * y + z * z);

	// FLT_EPSILON is added to the radius to eliminate the possibility of divide by zero.
	float iradius = 1.f / (radius + FLT_EPSILON);

	x *= iradius;
	y *= iradius;
	z *= iradius;

	return radius;
}
//===============================================
inline void Vector3::MulAdd(const Vector3& a, const Vector3& b, float scalar)
{
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
	z = a.z + b.z * scalar;
}
//===============================================
inline float Vector3Normalize(Vector3& v)
{
	Assert(v.IsValid());
	float l = v.Length();
	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		// FIXME:
		// Just copying the existing implemenation; shouldn't res.z == 0?
		v.x = v.y = 0.0f; v.z = 1.0f;
	}
	return l;
}
//===============================================
inline float Vector3Normalize(float* v)
{
	return Vector3Normalize(*(reinterpret_cast<Vector3*>(v)));
}
//===============================================
inline Vector3 Vector3::operator+(const Vector3& v) const
{
	Vector3 res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	return res;
}

//===============================================
inline Vector3 Vector3::operator-(const Vector3& v) const
{
	Vector3 res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	return res;
}
//===============================================
inline Vector3 Vector3::operator*(float fl) const
{
	Vector3 res;
	res.x = x * fl;
	res.y = y * fl;
	res.z = z * fl;
	return res;
}
//===============================================
inline Vector3 Vector3::operator*(const Vector3& v) const
{
	Vector3 res;
	res.x = x * v.x;
	res.y = y * v.y;
	res.z = z * v.z;
	return res;
}
//===============================================
inline Vector3 Vector3::operator/(float fl) const
{
	Vector3 res;
	res.x = x / fl;
	res.y = y / fl;
	res.z = z / fl;
	return res;
}
//===============================================
inline Vector3 Vector3::operator/(const Vector3& v) const
{
	Vector3 res;
	res.x = x / v.x;
	res.y = y / v.y;
	res.z = z / v.z;
	return res;
}
inline float Vector3::Dot(const Vector3& vOther) const
{
	const Vector3& a = *this;

	return(a.x * vOther.x + a.y * vOther.y + a.z * vOther.z);
}

//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------

inline float Vector3Length(const Vector3& v)
{
	CHECK_VALID(v);
	return (float)FastSqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

//Vector3 SUBTRAC
inline void Vector3Subtract(const Vector3& a, const Vector3& b, Vector3& c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

//Vector3ADD
inline void Vector3Add(const Vector3& a, const Vector3& b, Vector3& c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}

//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------
inline float* Vector3::Base()
{
	return (float*)this;
}

inline float const* Vector3::Base() const
{
	return (float const*)this;
}

inline void Vector3MAInline(const float* start, float scale, const float* direction, float* dest)
{
	dest[0] = start[0] + direction[0] * scale;
	dest[1] = start[1] + direction[1] * scale;
	dest[2] = start[2] + direction[2] * scale;
}

inline void Vector3MAInline(const Vector3& start, float scale, const Vector3& direction, Vector3& dest)
{
	dest.x = start.x + direction.x * scale;
	dest.y = start.y + direction.y * scale;
	dest.z = start.z + direction.z * scale;
}

inline void Vector3MA(const Vector3& start, float scale, const Vector3& direction, Vector3& dest)
{
	Vector3MAInline(start, scale, direction, dest);
}

inline void Vector3MA(const float* start, float scale, const float* direction, float* dest)
{
	Vector3MAInline(start, scale, direction, dest);
}


class ALIGN16 Vector3Aligned : public Vector3
{
public:
	inline Vector3Aligned(void) {};
	inline Vector3Aligned(float X, float Y, float Z)
	{
		Init(X, Y, Z);
	}

#ifdef Vector3_NO_SLOW_OPERATIONS

private:
	// No copy constructors allowed if we're in optimal mode
	Vector3Aligned(const Vector3Aligned& vOther);
	Vector3Aligned(const Vector3& vOther);

#else
public:
	explicit Vector3Aligned(const Vector3& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
	}

	Vector3Aligned& operator=(const Vector3& vOther)
	{
		Init(vOther.x, vOther.y, vOther.z);
		return *this;
	}

#endif
	float w;	// this space is used anyway
};



inline unsigned long& FloatBits(float& f)
{
	return *reinterpret_cast<unsigned long*>(&f);
}

inline bool IsFinite(float f)
{
	return ((FloatBits(f) & 0x7F800000) != 0x7F800000);
}

//=========================================================
// 2D Vector2
//=========================================================

class Vector2
{
public:
	// Members
	float x, y;

	// Construction/destruction
	Vector2(void);
	Vector2(float X, float Y);
	Vector2(const float* pFloat);
	static Vector2 Scale(Vector2 a, Vector2 b)
	{
		return Vector2(a.x * b.x, a.y * b.y);
	}
	// Initialization
	void Init(float ix = 0.0f, float iy = 0.0f);

	// Got any nasty NAN's?
	bool IsValid() const;

	// array access...
	float  operator[](int i) const;
	float& operator[](int i);

	// Base address...
	float* Base();
	float const* Base() const;

	// Initialization methods
	void Random(float minVal, float maxVal);

	// equality
	bool operator==(const Vector2& v) const;
	bool operator!=(const Vector2& v) const;

	// arithmetic operations
	Vector2& operator+=(const Vector2& v);
	Vector2& operator-=(const Vector2& v);
	Vector2& operator*=(const Vector2& v);
	Vector2& operator*=(float           s);
	Vector2& operator/=(const Vector2& v);
	Vector2& operator/=(float           s);

	// negate the Vector2 components
	void Negate();

	// Get the Vector2's magnitude.
	float Length() const;

	// Get the Vector2's magnitude squared.
	float LengthSqr(void) const;

	// return true if this Vector3 is (0,0) within tolerance
	bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance&&
			y > -tolerance && y < tolerance);
	}

	float Normalize();

	// Normalize in place and return the old length.
	float NormalizeInPlace();

	// Compare length.
	bool IsLengthGreaterThan(float val) const;
	bool IsLengthLessThan(float    val) const;

	// Get the distance from this Vector2 to the other one.
	float DistTo(const Vector2& vOther) const;

	// Get the distance from this Vector2 to the other one squared.
	float DistToSqr(const Vector2& vOther) const;

	// Copy
	void CopyToArray(float* rgfl) const;

	// Multiply, add, and assign to this (ie: *this = a + b * scalar). This
	// is about 12% faster than the actual Vector2 equation (because it's done per-component
	// rather than per-Vector2).
	void MulAdd(const Vector2& a, const Vector2& b, float scalar);

	// Dot product.
	float Dot(const Vector2& vOther) const;

	// assignment
	Vector2& operator=(const Vector2& vOther);

#ifndef Vector3_NO_SLOW_OPERATIONS
	// copy constructors
	Vector2(const Vector2& vOther);

	// arithmetic operations
	Vector2 operator-(void) const;

	Vector2 operator+(const Vector2& v) const;
	Vector2 operator-(const Vector2& v) const;
	Vector2 operator*(const Vector2& v) const;
	Vector2 operator/(const Vector2& v) const;
	Vector2 operator+(const int       i1) const;
	Vector2 operator+(const float     fl) const;
	Vector2 operator*(const float     fl) const;
	Vector2 operator/(const float     fl) const;

	// Cross product between two Vector3s.
	Vector2 Cross(const Vector2& vOther) const;

	// Returns a Vector2 with the min or max in X, Y, and Z.
	Vector2 Min(const Vector2& vOther) const;
	Vector2 Max(const Vector2& vOther) const;

#else

private:
	// No copy constructors allowed if we're in optimal mode
	Vector2(const Vector2& vOther);
#endif
};

//-----------------------------------------------------------------------------

const Vector2 vec2_origin(0, 0);
//const Vector2 vec2_invalid(3.40282347E+38F, 3.40282347E+38F);

//-----------------------------------------------------------------------------
// Vector2 related operations
//-----------------------------------------------------------------------------

// Vector2 clear
void Vector2Clear(Vector2& a);

// Copy
void Vector2Copy(const Vector2& src, Vector2& dst);

// Vector2 arithmetic
void Vector2Add(const Vector2& a, const Vector2& b, Vector2& result);
void Vector2Subtract(const Vector2& a, const Vector2& b, Vector2& result);
void Vector2Multiply(const Vector2& a, float           b, Vector2& result);
void Vector2Multiply(const Vector2& a, const Vector2& b, Vector2& result);
void Vector2Divide(const Vector2& a, float           b, Vector2& result);
void Vector2Divide(const Vector2& a, const Vector2& b, Vector2& result);
void Vector2MA(const Vector2& start, float       s, const Vector2& dir, Vector2& result);

// Store the min or max of each of x, y, and z into the result.
void Vector2Min(const Vector2& a, const Vector2& b, Vector2& result);
void Vector2Max(const Vector2& a, const Vector2& b, Vector2& result);

#define Vector2Expand( v ) (v).x, (v).y

// Normalization
float Vector2Normalize(Vector2& v);

// Length
float Vector2Length(const Vector2& v);

// Dot Product
float DotProduct2D(const Vector2& a, const Vector2& b);

// Linearly interpolate between two Vector3s
void Vector2Lerp(const Vector2& src1, const Vector2& src2, float t, Vector2& dest);


//-----------------------------------------------------------------------------
//
// Inlined Vector2 methods
//
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// constructors
//-----------------------------------------------------------------------------

inline Vector2::Vector2(void)
{
#ifdef _DEBUG
	// Initialize to NAN to catch errors
	//x = y = float_NAN;
#endif
}

inline Vector2::Vector2(float X, float Y)
{
	x = X; y = Y;
	Assert(IsValid());
}

inline Vector2::Vector2(const float* pFloat)
{
	Assert(pFloat);
	x = pFloat[0]; y = pFloat[1];
	Assert(IsValid());
}


//-----------------------------------------------------------------------------
// copy constructor
//-----------------------------------------------------------------------------

inline Vector2::Vector2(const Vector2& vOther)
{
	Assert(vOther.IsValid());
	x = vOther.x; y = vOther.y;
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------

inline void Vector2::Init(float ix, float iy)
{
	x = ix; y = iy;
	Assert(IsValid());
}

inline void Vector2::Random(float minVal, float maxVal)
{
	x = minVal + ((float)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
	y = minVal + ((float)rand() / VALVE_RAND_MAX) * (maxVal - minVal);
}

inline void Vector2Clear(Vector2& a)
{
	a.x = a.y = 0.0f;
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------

inline Vector2& Vector2::operator=(const Vector2& vOther)
{
	Assert(vOther.IsValid());
	x = vOther.x; y = vOther.y;
	return *this;
}

//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------

inline float& Vector2::operator[](int i)
{
	Assert((i >= 0) && (i < 2));
	return ((float*)this)[i];
}

inline float Vector2::operator[](int i) const
{
	Assert((i >= 0) && (i < 2));
	return ((float*)this)[i];
}

//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------

inline float* Vector2::Base()
{
	return (float*)this;
}

inline float const* Vector2::Base() const
{
	return (float const*)this;
}

//-----------------------------------------------------------------------------
// IsValid?
//-----------------------------------------------------------------------------

inline bool Vector2::IsValid() const
{
	return IsFinite(x) && IsFinite(y);
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------

inline bool Vector2::operator==(const Vector2& src) const
{
	Assert(src.IsValid() && IsValid());
	return (src.x == x) && (src.y == y);
}

inline bool Vector2::operator!=(const Vector2& src) const
{
	Assert(src.IsValid() && IsValid());
	return (src.x != x) || (src.y != y);
}


//-----------------------------------------------------------------------------
// Copy
//-----------------------------------------------------------------------------

inline void Vector2Copy(const Vector2& src, Vector2& dst)
{
	Assert(src.IsValid());
	dst.x = src.x;
	dst.y = src.y;
}

inline void	Vector2::CopyToArray(float* rgfl) const
{
	Assert(IsValid());
	Assert(rgfl);
	rgfl[0] = x; rgfl[1] = y;
}

//-----------------------------------------------------------------------------
// standard math operations
//-----------------------------------------------------------------------------

inline void Vector2::Negate()
{
	Assert(IsValid());
	x = -x; y = -y;
}

inline Vector2& Vector2::operator+=(const Vector2& v)
{
	Assert(IsValid() && v.IsValid());
	x += v.x; y += v.y;
	return *this;
}

inline Vector2& Vector2::operator-=(const Vector2& v)
{
	Assert(IsValid() && v.IsValid());
	x -= v.x; y -= v.y;
	return *this;
}

inline Vector2& Vector2::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	Assert(IsValid());
	return *this;
}

inline Vector2& Vector2::operator*=(const Vector2& v)
{
	x *= v.x;
	y *= v.y;
	Assert(IsValid());
	return *this;
}

inline Vector2& Vector2::operator/=(float fl)
{
	Assert(fl != 0.0f);
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	Assert(IsValid());
	return *this;
}

inline Vector2& Vector2::operator/=(const Vector2& v)
{
	Assert(v.x != 0.0f && v.y != 0.0f);
	x /= v.x;
	y /= v.y;
	Assert(IsValid());
	return *this;
}

inline void Vector2Add(const Vector2& a, const Vector2& b, Vector2& c)
{
	Assert(a.IsValid() && b.IsValid());
	c.x = a.x + b.x;
	c.y = a.y + b.y;
}

inline void Vector2Add(const Vector2& a, const int b, Vector2& c)
{
	Assert(a.IsValid());
	c.x = a.x + b;
	c.y = a.y + b;
}

inline void Vector2Add(const Vector2& a, const float b, Vector2& c)
{
	Assert(a.IsValid());
	c.x = a.x + b;
	c.y = a.y + b;
}

inline void Vector2Subtract(const Vector2& a, const Vector2& b, Vector2& c)
{
	Assert(a.IsValid() && b.IsValid());
	c.x = a.x - b.x;
	c.y = a.y - b.y;
}

inline void Vector2Multiply(const Vector2& a, const float b, Vector2& c)
{
	Assert(a.IsValid() && IsFinite(b));
	c.x = a.x * b;
	c.y = a.y * b;
}

inline void Vector2Multiply(const Vector2& a, const Vector2& b, Vector2& c)
{
	Assert(a.IsValid() && b.IsValid());
	c.x = a.x * b.x;
	c.y = a.y * b.y;
}


inline void Vector2Divide(const Vector2& a, const float b, Vector2& c)
{
	Assert(a.IsValid());
	Assert(b != 0.0f);
	float oob = 1.0f / b;
	c.x = a.x * oob;
	c.y = a.y * oob;
}

inline void Vector2Divide(const Vector2& a, const Vector2& b, Vector2& c)
{
	Assert(a.IsValid());
	Assert((b.x != 0.0f) && (b.y != 0.0f));
	c.x = a.x / b.x;
	c.y = a.y / b.y;
}

inline void Vector2MA(const Vector2& start, float s, const Vector2& dir, Vector2& result)
{
	Assert(start.IsValid() && IsFinite(s) && dir.IsValid());
	result.x = start.x + s * dir.x;
	result.y = start.y + s * dir.y;
}

// FIXME: Remove
// For backwards compatability
inline void	Vector2::MulAdd(const Vector2& a, const Vector2& b, float scalar)
{
	x = a.x + b.x * scalar;
	y = a.y + b.y * scalar;
}

inline void Vector2Lerp(const Vector2& src1, const Vector2& src2, float t, Vector2& dest)
{
	dest[0] = src1[0] + (src2[0] - src1[0]) * t;
	dest[1] = src1[1] + (src2[1] - src1[1]) * t;
}

//-----------------------------------------------------------------------------
// dot, cross
//-----------------------------------------------------------------------------
inline float DotProduct2D(const Vector2& a, const Vector2& b)
{
	Assert(a.IsValid() && b.IsValid());
	return(a.x * b.x + a.y * b.y);
}

// for backwards compatability
inline float Vector2::Dot(const Vector2& vOther) const
{
	return DotProduct2D(*this, vOther);
}


//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------
inline float Vector2Length(const Vector2& v)
{
	Assert(v.IsValid());
	return (float)FastSqrt(v.x * v.x + v.y * v.y);
}

inline float Vector2::LengthSqr(void) const
{
	Assert(IsValid());
	return (x * x + y * y);
}

inline float Vector2::NormalizeInPlace()
{
	return Vector2Normalize(*this);
}

inline bool Vector2::IsLengthGreaterThan(float val) const
{
	return LengthSqr() > val * val;
}

inline bool Vector2::IsLengthLessThan(float val) const
{
	return LengthSqr() < val * val;
}

inline float Vector2::Length(void) const
{
	return Vector2Length(*this);
}


inline void Vector2Min(const Vector2& a, const Vector2& b, Vector2& result)
{
	result.x = (a.x < b.x) ? a.x : b.x;
	result.y = (a.y < b.y) ? a.y : b.y;
}


inline void Vector2Max(const Vector2& a, const Vector2& b, Vector2& result)
{
	result.x = (a.x > b.x) ? a.x : b.x;
	result.y = (a.y > b.y) ? a.y : b.y;
}


//-----------------------------------------------------------------------------
// Normalization
//-----------------------------------------------------------------------------
inline float Vector2Normalize(Vector2& v)
{
	Assert(v.IsValid());
	float l = v.Length();
	if (l != 0.0f)
	{
		v /= l;
	}
	else
	{
		v.x = v.y = 0.0f;
	}
	return l;
}


//-----------------------------------------------------------------------------
// Get the distance from this Vector2 to the other one
//-----------------------------------------------------------------------------
inline float Vector2::DistTo(const Vector2& vOther) const
{
	Vector2 delta;
	Vector2Subtract(*this, vOther, delta);
	return delta.Length();
}

inline float Vector2::DistToSqr(const Vector2& vOther) const
{
	Vector2 delta;
	Vector2Subtract(*this, vOther, delta);
	return delta.LengthSqr();
}


//-----------------------------------------------------------------------------
// Computes the closest point to vecTarget no farther than flMaxDist from vecStart
//-----------------------------------------------------------------------------
inline void ComputeClosestPoint2D(const Vector2& vecStart, float flMaxDist, const Vector2& vecTarget, Vector2* pResult)
{
	Vector2 vecDelta;
	Vector2Subtract(vecTarget, vecStart, vecDelta);
	float flDistSqr = vecDelta.LengthSqr();
	if (flDistSqr <= flMaxDist * flMaxDist)
	{
		*pResult = vecTarget;
	}
	else
	{
		vecDelta /= FastSqrt(flDistSqr);
		Vector2MA(vecStart, flMaxDist, vecDelta, *pResult);
	}
}



//-----------------------------------------------------------------------------
//
// Slow methods
//
//-----------------------------------------------------------------------------

#ifndef Vector3_NO_SLOW_OPERATIONS
#endif
//-----------------------------------------------------------------------------
// Returns a Vector2 with the min or max in X, Y, and Z.
//-----------------------------------------------------------------------------

inline Vector2 Vector2::Min(const Vector2& vOther) const
{
	return Vector2(x < vOther.x ? x : vOther.x,
		y < vOther.y ? y : vOther.y);
}

inline Vector2 Vector2::Max(const Vector2& vOther) const
{
	return Vector2(x > vOther.x ? x : vOther.x,
		y > vOther.y ? y : vOther.y);
}


//-----------------------------------------------------------------------------
// arithmetic operations
//-----------------------------------------------------------------------------

inline Vector2 Vector2::operator-(void) const
{
	return Vector2(-x, -y);
}

inline Vector2 Vector2::operator+(const Vector2& v) const
{
	Vector2 res;
	Vector2Add(*this, v, res);
	return res;
}

inline Vector2 Vector2::operator-(const Vector2& v) const
{
	Vector2 res;
	Vector2Subtract(*this, v, res);
	return res;
}

inline Vector2 Vector2::operator+(const int i1) const
{
	Vector2 res;
	Vector2Add(*this, i1, res);
	return res;
}

inline Vector2 Vector2::operator+(const float fl) const
{
	Vector2 res;
	Vector2Add(*this, fl, res);
	return res;
}

inline Vector2 Vector2::operator*(const float fl) const
{
	Vector2 res;
	Vector2Multiply(*this, fl, res);
	return res;
}

inline Vector2 Vector2::operator*(const Vector2& v) const
{
	Vector2 res;
	Vector2Multiply(*this, v, res);
	return res;
}

inline Vector2 Vector2::operator/(const float fl) const
{
	Vector2 res;
	Vector2Divide(*this, fl, res);
	return res;
}

inline Vector2 Vector2::operator/(const Vector2& v) const
{
	Vector2 res;
	Vector2Divide(*this, v, res);
	return res;
}

inline Vector2 operator*(const float fl, const Vector2& v)
{
	return v * fl;
}

class QAngleByValue;
class QAngle
{
public:
	// Members
	float x, y, z;

	// Construction/destruction
	QAngle(void);
	QAngle(float X, float Y, float Z);
	//      QAngle(RadianEuler const &angles);      // evil auto type promotion!!!

	// Allow pass-by-value
	operator QAngleByValue& () { return *((QAngleByValue*)(this)); }
	operator const QAngleByValue& () const { return *((const QAngleByValue*)(this)); }

	// Initialization
	void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f);
	void Random(float minVal, float maxVal);

	// Got any nasty NAN's?
	bool IsValid() const;
	void Invalidate();

	// array access...
	float operator[](int i) const;
	float& operator[](int i);

	// Base address...
	float* Base();
	float const* Base() const;

	// equality
	bool operator==(const QAngle& v) const;
	bool operator!=(const QAngle& v) const;

	// arithmetic operations
	QAngle& operator+=(const QAngle& v);
	QAngle& operator-=(const QAngle& v);
	QAngle& operator*=(float s);
	QAngle& operator/=(float s);

	// Get the Vector3's magnitude.
	float   Length() const;
	float   LengthSqr() const;

	// negate the QAngle components
	//void  Negate();
	__forceinline float Size() const {
		return sqrt(x * x + y * y + z * z);
	}
	// No assignment operators either...
	QAngle& operator=(const QAngle& src);

#ifndef Vector3_NO_SLOW_OPERATIONS
	// copy constructors

	// arithmetic operations
	QAngle  operator-(void) const;

	QAngle  operator+(const QAngle& v) const;
	QAngle  operator-(const QAngle& v) const;
	QAngle  operator*(float fl) const;
	QAngle  operator*(const QAngle& v) const;
	QAngle  operator/(float fl) const;
#else

private:
	// No copy constructors allowed if we're in optimal mode
	QAngle(const QAngle& vOther);

#endif
};

//-----------------------------------------------------------------------------
// constructors
//-----------------------------------------------------------------------------
inline QAngle::QAngle(void)
{
#ifdef _DEBUG
#ifdef Vector3_PARANOIA
	// Initialize to NAN to catch errors
	x = y = z = VEC_T_NAN;
#endif
#endif
}

inline QAngle::QAngle(float X, float Y, float Z)
{
	x = X; y = Y; z = Z;
	CHECK_VALID(*this);
}

//-----------------------------------------------------------------------------
// initialization
//-----------------------------------------------------------------------------
inline void QAngle::Init(float ix, float iy, float iz)
{
	x = ix; y = iy; z = iz;
	CHECK_VALID(*this);
}

inline void QAngle::Random(float minVal, float maxVal)
{
	x = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	y = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	z = minVal + ((float)rand() / RAND_MAX) * (maxVal - minVal);
	CHECK_VALID(*this);
}

//-----------------------------------------------------------------------------
// assignment
//-----------------------------------------------------------------------------
inline QAngle& QAngle::operator=(const QAngle& vOther)
{
	CHECK_VALID(vOther);
	x = vOther.x; y = vOther.y; z = vOther.z;
	return *this;
}

//-----------------------------------------------------------------------------
// comparison
//-----------------------------------------------------------------------------
inline bool QAngle::operator==(const QAngle& src) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (src.x == x) && (src.y == y) && (src.z == z);
}

inline bool QAngle::operator!=(const QAngle& src) const
{
	CHECK_VALID(src);
	CHECK_VALID(*this);
	return (src.x != x) || (src.y != y) || (src.z != z);
}

//-----------------------------------------------------------------------------
// standard math operations
//-----------------------------------------------------------------------------
inline QAngle& QAngle::operator+=(const QAngle& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x += v.x; y += v.y; z += v.z;
	return *this;
}

inline QAngle& QAngle::operator-=(const QAngle& v)
{
	CHECK_VALID(*this);
	CHECK_VALID(v);
	x -= v.x; y -= v.y; z -= v.z;
	return *this;
}

inline QAngle& QAngle::operator*=(float fl)
{
	x *= fl;
	y *= fl;
	z *= fl;
	CHECK_VALID(*this);
	return *this;
}

inline QAngle& QAngle::operator/=(float fl)
{
	Assert(fl != 0.0f);
	float oofl = 1.0f / fl;
	x *= oofl;
	y *= oofl;
	z *= oofl;
	CHECK_VALID(*this);
	return *this;
}

//-----------------------------------------------------------------------------
// Base address...
//-----------------------------------------------------------------------------
inline float* QAngle::Base()
{
	return (float*)this;
}

inline float const* QAngle::Base() const
{
	return (float const*)this;
}

//-----------------------------------------------------------------------------
// Array access
//-----------------------------------------------------------------------------
inline float& QAngle::operator[](int i)
{
	Assert((i >= 0) && (i < 3));
	return ((float*)this)[i];
}

inline float QAngle::operator[](int i) const
{
	Assert((i >= 0) && (i < 3));
	return ((float*)this)[i];
}

//-----------------------------------------------------------------------------
// length
//-----------------------------------------------------------------------------
inline float QAngle::Length() const
{
	CHECK_VALID(*this);
	return (float)FastSqrt(LengthSqr());
}



inline float QAngle::LengthSqr() const
{
	CHECK_VALID(*this);
	return x * x + y * y + z * z;
}


//-----------------------------------------------------------------------------
// arithmetic operations (SLOW!!)
//-----------------------------------------------------------------------------
#ifndef Vector3_NO_SLOW_OPERATIONS

inline QAngle QAngle::operator-(void) const
{
	return QAngle(-x, -y, -z);
}

inline QAngle QAngle::operator+(const QAngle& v) const
{
	QAngle res;
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;
	return res;
}

inline QAngle QAngle::operator-(const QAngle& v) const
{
	QAngle res;
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;
	return res;
}

inline QAngle QAngle::operator*(float fl) const
{
	QAngle res;
	res.x = x * fl;
	res.y = y * fl;
	res.z = z * fl;
	return res;
}

inline QAngle QAngle::operator*(const QAngle& v) const
{
	QAngle res;
	res.x = x * v.x;
	res.y = y * v.y;
	res.z = z * v.z;
	return res;
}

inline QAngle QAngle::operator/(float fl) const
{
	QAngle res;
	res.x = x / fl;
	res.y = y / fl;
	res.z = z / fl;
	return res;
}

inline QAngle operator*(float fl, const QAngle& v)
{
	return v * fl;
}

#endif // Vector3_NO_SLOW_OPERATIONS


//QANGLE SUBTRAC
inline void QAngleSubtract(const QAngle& a, const QAngle& b, QAngle& c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
}

//QANGLEADD
inline void QAngleAdd(const QAngle& a, const QAngle& b, QAngle& c)
{
	CHECK_VALID(a);
	CHECK_VALID(b);
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
}


class FRotator
{
public:
	static FRotator CalcAngle(Vector3 LocalHeadPosition, Vector3 AimPosition)
	{
		Vector3 vecDelta = Vector3((LocalHeadPosition.x - AimPosition.x), (LocalHeadPosition.y - AimPosition.y), (LocalHeadPosition.z - AimPosition.z));
		float hyp = (float)sqrt(vecDelta.x * vecDelta.x + vecDelta.y * vecDelta.y);

		FRotator ViewAngles = {};
		ViewAngles.Pitch = -(float)atan(vecDelta.z / hyp) * (float)(180.f / M_PI);
		ViewAngles.Yaw = (float)atan(vecDelta.y / vecDelta.x) * (float)(180.f / M_PI);
		ViewAngles.Roll = (float)0.f;

		if (vecDelta.x >= 0.f)
			ViewAngles.Yaw += 180.0f;

		return ViewAngles;
	}
	FRotator() : Pitch(0.f), Yaw(0.f), Roll(0.f) {}

	FRotator(float _pitch, float _yaw, float _roll) : Pitch(_pitch), Yaw(_yaw), Roll(_roll) {}

	~FRotator() {}

	float Pitch;
	float Yaw;
	float Roll;

	inline double Length()
	{
		return sqrtf(powf(Pitch, 2.0) + powf(Yaw, 2.0) + powf(Roll, 2.0));
	};
	FRotator ClampAngles()
	{
		FRotator angles = FRotator(Pitch, Yaw, Roll);

		while (angles.Yaw < -180.0f)
			angles.Yaw += 360.0f;
		while (angles.Yaw > 180.0f)
			angles.Yaw -= 360.0f;

		if (angles.Pitch < -74.f) //The actual value for max pitch seems to be 74.9999 something, but 74 will work fine.
			angles.Pitch = -74.f;
		if (angles.Pitch > 74.f)
			angles.Pitch = 74.f;

		return angles;
	}
	void Clamp()
	{
		if (Pitch > 180)
			Pitch -= 360;

		else if (Pitch < -180)
			Pitch += 360;

		if (Yaw > 180)
			Yaw -= 360;

		else if (Yaw < -180)
			Yaw += 360;

		if (Pitch < -89)
			Pitch = -89;

		if (Pitch > 89)
			Pitch = 89;

		while (Yaw < -180.0f)
			Yaw += 360.0f;

		while (Yaw > 180.0f)
			Yaw -= 360.0f;

		Roll = 0;
		return;
	}
	__forceinline float Size() const {
		return sqrt(Pitch * Pitch + Yaw * Yaw + Roll * Roll);
	}

	Vector3  ToVector3()
	{
		float radPitch = (float)(Pitch * M_PI / 180.f);
		float radYaw = (float)(Yaw * M_PI / 180.f);

		float SP = (float)sinf(radPitch);
		float CP = (float)cosf(radPitch);
		float SY = (float)sinf(radYaw);
		float CY = (float)cosf(radYaw);

		return  Vector3(CP * CY, CP * SY, SP);
	}

	D3DXMATRIX SMatrix(Vector3 origin = Vector3(0.0f, 0.0f, 0.0f))
	{
		float radPitch = (Pitch * float(M_PI) / 180.f);
		float radYaw = (Yaw * float(M_PI) / 180.f);
		float radRoll = (Roll * float(M_PI) / 180.f);

		float SP = sinf(radPitch);
		float CP = cosf(radPitch);
		float SY = sinf(radYaw);
		float CY = cosf(radYaw);
		float SR = sinf(radRoll);
		float CR = cosf(radRoll);

		D3DMATRIX matrix;
		matrix.m[0][0] = CP * CY;
		matrix.m[0][1] = CP * SY;
		matrix.m[0][2] = SP;
		matrix.m[0][3] = 0.0f;

		matrix.m[1][0] = SR * SP * CY - CR * SY;
		matrix.m[1][1] = SR * SP * SY + CR * CY;
		matrix.m[1][2] = -SR * CP;
		matrix.m[1][3] = 0.0f;

		matrix.m[2][0] = -(CR * SP * CY + SR * SY);
		matrix.m[2][1] = CY * SR - CR * SP * SY;
		matrix.m[2][2] = CR * CP;
		matrix.m[2][3] = 0.0f;

		matrix.m[3][0] = origin.x;
		matrix.m[3][1] = origin.y;
		matrix.m[3][2] = origin.z;
		matrix.m[3][3] = 1.0f;

		return matrix;
	}

	D3DMATRIX GetAxesS()
	{
		auto tempMatrix = SMatrix();
		return tempMatrix;
	}

	D3DMATRIX GetAxes(float Pitch, float Yaw, float Roll)
	{
		auto tempMatrix = Matrix(Pitch, Yaw, Roll);
		return tempMatrix;
	}

	void GetAxese(float Pitch, float Yaw, float Roll, Vector3& X, Vector3& Y, Vector3& Z)
	{
		D3DXMATRIX M;
		M = Matrix(Pitch, Yaw, Roll);
		X.x = M._11;
		X.y = M._12;
		X.z = M._13;

		Y.x = M._21;
		Y.y = M._22;
		Y.z = M._23;

		Z.x = M._31;
		Z.y = M._32;
		Z.z = M._33;
	}

	D3DXMATRIX Matrix(float Pitcha, float Yawy, float Rolla)
	{
		float radPitch = (Pitcha * float(M_PI) / 180.f);
		float radYaw = (Yawy * float(M_PI) / 180.f);
		float radRoll = (Rolla * float(M_PI) / 180.f);

		float SP = sinf(radPitch);
		float CP = cosf(radPitch);
		float SY = sinf(radYaw);
		float CY = cosf(radYaw);
		float SR = sinf(radRoll);
		float CR = cosf(radRoll);

		D3DMATRIX matrix;
		matrix.m[0][0] = CP * CY;
		matrix.m[0][1] = CP * SY;
		matrix.m[0][2] = SP;
		matrix.m[0][3] = 0.0f;

		matrix.m[1][0] = SR * SP * CY - CR * SY;
		matrix.m[1][1] = SR * SP * SY + CR * CY;
		matrix.m[1][2] = -SR * CP;
		matrix.m[1][3] = 0.0f;

		matrix.m[2][0] = -(CR * SP * CY + SR * SY);
		matrix.m[2][1] = CY * SR - CR * SP * SY;
		matrix.m[2][2] = CR * CP;
		matrix.m[2][3] = 0.0f;

		matrix.m[3][0] = 0.0f;
		matrix.m[3][1] = 0.0f;
		matrix.m[3][2] = 0.0f;
		matrix.m[3][3] = 1.0f;

		return matrix;
	}

	FRotator operator+(FRotator v)
	{
		return FRotator(Pitch + v.Pitch, Yaw + v.Yaw, Roll + v.Roll);
	}

	FRotator operator-(FRotator v)
	{
		return FRotator(Pitch - v.Pitch, Yaw - v.Yaw, Roll - v.Roll);
	}

	FRotator operator*(FRotator v)
	{
		return FRotator(Pitch * v.Pitch, Yaw * v.Yaw, Roll * v.Roll);
	}

	FRotator operator/(FRotator v)
	{
		return FRotator(Pitch / v.Pitch, Yaw / v.Yaw, Roll / v.Roll);
	}
};

template<class T>
struct TArray
{
	friend struct FString;

public:
	inline TArray()
	{
		Data = nullptr;
		Count = Max = 0;
	};

	inline int Num() const
	{
		return Count;
	};

	inline T& operator[](int i)
	{
		return Data[i];
	};

	inline const T& operator[](int i) const
	{
		return Data[i];
	};

	inline bool IsValidIndex(int i) const
	{
		return i < Num();
	}

public:
	T* Data;
	int32_t Count;
	int32_t Max;

};

struct FNameEntryId
{
	uint32_t Value;

	FNameEntryId() : Value(0) {}
	FNameEntryId(uint32_t value) : Value(value) {}

	bool operator==(FNameEntryId Rhs) const { return Value == Rhs.Value; }
};
struct UENameInfo
{
	FNameEntryId Index;
	const char* Name;
	uint32_t Len;
};

struct FString : public TArray<wchar_t>
{
	inline FString()
	{
	};

	FString(const wchar_t* other)
	{
		Max = Count = *other ? (int)std::wcslen(other) + 1 : 0;

		if (Count)
		{
			Data = const_cast<wchar_t*>(other);
		}
	};

	inline bool IsValid() const
	{
		return Data != nullptr;
	}

	inline const wchar_t* c_str() const
	{
		return Data;
	}

	std::string ToString() const
	{
		auto length = std::wcslen(Data);

		std::string str(length, '\0');

		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

		return str;
	}
};
struct FTslPlayerStatistics
{
	int NumKills;
	int NumAssists;
};
typedef struct _D3DMATRIX2
{
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float m[4][4];
	};
} D3DMATRIX2;
struct FQuat
{
	float x;
	float y;
	float z;
	float w;
};

struct FTransform
{
	FQuat rot;
	Vector3 translation;
	char pad[4];
	Vector3 scale;
	char pad1[4];
	D3DMATRIX2 ToMatrixWithScale()
	{
		D3DMATRIX2 m;
		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		float x2 = rot.x + rot.x;
		float y2 = rot.y + rot.y;
		float z2 = rot.z + rot.z;

		float xx2 = rot.x * x2;
		float yy2 = rot.y * y2;
		float zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.z;

		float yz2 = rot.y * z2;
		float wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.z;
		m._23 = (yz2 + wx2) * scale.y;

		float xy2 = rot.x * y2;
		float wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.y;
		m._12 = (xy2 + wz2) * scale.x;

		float xz2 = rot.x * z2;
		float wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.z;
		m._13 = (xz2 - wy2) * scale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};
struct BoneFTransform
{
	FQuat rot;
	Vector3 translation;
	char pad[4];
	Vector3 scale;
	char pad1[4];
	D3DMATRIX2 ToMatrixWithScale()
	{
		D3DMATRIX2 m;
		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		float x2 = rot.x + rot.x;
		float y2 = rot.y + rot.y;
		float z2 = rot.z + rot.z;

		float xx2 = rot.x * x2;
		float yy2 = rot.y * y2;
		float zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.z;

		float yz2 = rot.y * z2;
		float wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.z;
		m._23 = (yz2 + wx2) * scale.y;

		float xy2 = rot.x * y2;
		float wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.y;
		m._12 = (xy2 + wz2) * scale.x;

		float xz2 = rot.x * z2;
		float wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.z;
		m._13 = (xz2 - wy2) * scale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};
struct ItFVector2D
{
	float                                              X;                                                        // 0x0000(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
	float                                              Y;                                                        // 0x0004(0x0004) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)

	inline ItFVector2D() : X(0), Y(0) {}

	inline ItFVector2D(float x, float y) : X(x), Y(y) {}

	__forceinline ItFVector2D operator-(const ItFVector2D& V) {
		return ItFVector2D(X - V.X, Y - V.Y);
	}

	__forceinline ItFVector2D operator+(const ItFVector2D& V) {
		return ItFVector2D(X + V.X, Y + V.Y);
	}

	__forceinline ItFVector2D operator*(float Scale) const {
		return ItFVector2D(X * Scale, Y * Scale);
	}

	__forceinline ItFVector2D operator/(float Scale) const {
		const float RScale = 1.f / Scale;
		return ItFVector2D(X * RScale, Y * RScale);
	}

	__forceinline ItFVector2D operator+(float A) const {
		return ItFVector2D(X + A, Y + A);
	}

	__forceinline ItFVector2D operator-(float A) const {
		return ItFVector2D(X - A, Y - A);
	}

	__forceinline ItFVector2D operator*(const ItFVector2D& V) const {
		return ItFVector2D(X * V.X, Y * V.Y);
	}

	__forceinline ItFVector2D operator/(const ItFVector2D& V) const {
		return ItFVector2D(X / V.X, Y / V.Y);
	}

	__forceinline float operator|(const ItFVector2D& V) const {
		return X * V.X + Y * V.Y;
	}

	__forceinline float operator^(const ItFVector2D& V) const {
		return X * V.Y - Y * V.X;
	}

	__forceinline ItFVector2D& operator+=(const ItFVector2D& v) {
		X += v.X;
		Y += v.Y;
		return *this;
	}

	__forceinline ItFVector2D& operator-=(const ItFVector2D& v) {
		X -= v.X;
		Y -= v.Y;
		return *this;
	}

	__forceinline ItFVector2D& operator*=(const ItFVector2D& v) {
		X *= v.X;
		Y *= v.Y;
		return *this;
	}

	__forceinline ItFVector2D& operator/=(const ItFVector2D& v) {
		X /= v.X;
		Y /= v.Y;
		return *this;
	}

	__forceinline bool operator==(const ItFVector2D& src) const {
		return (src.X == X) && (src.Y == Y);
	}

	__forceinline bool operator!=(const ItFVector2D& src) const {
		return (src.X != X) || (src.Y != Y);
	}
};


struct FWeaponTrajectoryConfig
{
	float                                              InitialSpeed;                                             // 0x0000(0x0004) (Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData)
	DWORD64                                            BallisticCurve;                                           // 0x0010(0x0008) (Edit, ZeroConstructor, DisableEditOnInstance, IsPlainOldData)
	float                                              SimulationSubstepTime;                                    // 0x0028(0x0004) (Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnInstance, IsPlainOldData)
	float                                              VDragCoefficient;                                         // 0x002C(0x0004) (Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnInstance, IsPlainOldData)
	float                                              BDS;                                                      // 0x0030(0x0004) (Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnInstance, IsPlainOldData)
	bool                                               bUseAdvancedBallistics;                                   // 0x0034(0x0001) (Edit, BlueprintVisible, BlueprintReadOnly, ZeroConstructor, DisableEditOnInstance, IsPlainOldData)
	float                                              TrajectoryGravityZ;

};
static D3DMATRIX2 MatrixMultiplication(D3DMATRIX2 pM1, D3DMATRIX2 pM2)
{
	D3DMATRIX2 pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;
	return pOut;
}

static D3DMATRIX2 Matrix2(Vector3 rot, Vector3 origin = Vector3(0, 0, 0)) {

	float radPitch = (rot.x * float(M_PI) / 180.f);
	float radYaw = (rot.y * float(M_PI) / 180.f);
	float radRoll = (rot.z * float(M_PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX2 matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;

	return matrix;
}
double distance(Vector3 p1, Vector3 p2)
{
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));
}
double distance(DirectX::XMFLOAT3 p1, DirectX::XMFLOAT3 p2)
{
	return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z));
}
struct FMinimalViewInfo
{
	Vector3 Location; //0x0000
	Vector3 Rotation; //0x000C
	float FOV; //0x0018
	float DesiredFOV; //0x001C
	float OrthoWidth; //0x0020
	float OrthoNearClipPlane; //0x0024
	float OrthoFarClipPlane; //0x0028
	float AspectRatio; //0x002C
};

struct FCameraCacheEntry
{
	float                                              Timestamp;                                                // 0x0000(0x0004) (CPF_ZeroConstructor, CPF_IsPlainOldData, CPF_NoDestructor, CPF_HasGetValueTypeHash, CPF_NativeAccessSpecifierPublic)
	unsigned char                                      UnknownData00[0xC];                                       // 0x0004(0x000C) MISSED OFFSET
	struct FMinimalViewInfo                            POV;                                                      // 0x0010(0x0580) (CPF_NativeAccessSpecifierPublic)
};
static D3DXMATRIX Matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0))
{
	float radPitch = (rot.x * float(M_PI) / 180.f);
	float radYaw = (rot.y * float(M_PI) / 180.f);
	float radRoll = (rot.z * float(M_PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;

	return matrix;
}

const Vector3 Vector3_zero = { 0.f, 0.f, 0.f };
const Vector3 Vector3_one = { 1.f, 1.f, 1.f };
const Vector3 Vector3_up = { 0.f, 1.f, 0.f };
const Vector3 Vector3_down = { 0.f, -1.f, 0.f };
const Vector3 Vector3_left = { -1.f, 0.f, 0.f };
const Vector3 Vector3_right = { 1.f, 0.f, 0.f };
const Vector3 Vector3_forward = { 0.f, 0.f, 1.f };
const Vector3 Vector3_back = { 0.f, 0.f, -1.f };

