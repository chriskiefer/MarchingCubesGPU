/****Classe Vector, Framework Nemo, Copyright Cyril Crassin 2004****/

#ifndef Vector_h
#define Vector_h

#include <math.h>
#include <string>

namespace NemoGraphics {

inline double log2(double N){
	return (log10(N)/log10(2.0));
}

template<class T>
class Vector2 {

public:

	T x, y;

	Vector2(void){}
	Vector2(T xx, T yy): x(xx), y(yy){}
	~Vector2(void){}

	Vector2<T> operator-(const Vector2 &v2) const {
		Vector2 res;
		res.x=this->x-v2.x;
		res.y=this->y-v2.y;

		return res;
	}

	Vector2<T> operator+(const Vector2 &v2)const {
		Vector2 res;
		res.x=this->x+v2.x;
		res.y=this->y+v2.y;

		return res;
	}

	Vector2<T> operator/(const Vector2 &v2)const {
		Vector2 res;
		res.x=this->x/v2.x;
		res.y=this->y/v2.y;

		return res;
	}

	Vector2<T> operator*(const Vector2 &v2) const {
		Vector2 res;
		res.x=this->x*v2.x;
		res.y=this->y*v2.y;

		return res;
	}

	Vector2<T> operator%(const Vector2 &v2) const {
		Vector2 res;
		res.x=this->x%v2.x;
		res.y=this->y%v2.y;

		return res;
	}

	Vector2<T> operator/(T nb) const {
		Vector2 res;
		res.x=this->x/nb;
		res.y=this->y/nb;

		return res;
	}

	Vector2<T> operator%(T nb) const {
		Vector2 res;
		res.x=this->x%nb;
		res.y=this->y%nb;

		return res;
	}

	Vector2<T> operator+(T nb) const {
		Vector2 res;
		res.x=this->x+nb;
		res.y=this->y+nb;

		return res;
	}
	Vector2<T> operator-(T nb) const {
		Vector2 res;
		res.x=this->x-nb;
		res.y=this->y-nb;

		return res;
	}

	Vector2<T> operator*(T nb) const {
		Vector2 res;
		res.x=this->x*nb;
		res.y=this->y*nb;

		return res;
	}

	friend Vector2<T> vceil(const Vector2<T> &v) {
		Vector2<T> res;
		res.x=ceil((double)v.x);
		res.y=ceil((double)v.y);
		return res;
	}

	friend Vector2<T> abs(const Vector2<T> &v) {
		Vector2<T> res;

		res.x=sqrt(pow(v.x, 2)); //abs( v.x);
		res.y=sqrt(pow(v.y, 2)); //abs( v.y);
		return res;
	}

	T squareDistance(const Vector2<T> &v) const {
		return ( pow(v.x-x, 2) + pow(v.y-y, 2) );
	}

	T distance(const Vector2<T> &v) const {
		return sqrt( pow(v.x-x, 2) + pow(v.y-y, 2) );
	}
};

template<class T>
class Vector3 {

public:

	T x, y, z;

	Vector3(void){}
	Vector3(T xx, T yy, T zz): x(xx), y(yy), z(zz){}
	Vector3(const float *v) : x(v[0]), y(v[1]), z(v[2]) { }
	Vector3(float *v) : x(v[0]), y(v[1]), z(v[2]) { }
	~Vector3(void){}


	std::string toString(){
		char buff[256];
		sprintf_s(buff, "Vector3(%d,%d,%d)", x, y, z);
		return std::string(buff);
	}

	Vector3<T> operator-(const Vector3<T> &v2) const {
		Vector3 res;
		res.x=this->x-v2.x;
		res.y=this->y-v2.y;
		res.z=this->z-v2.z;

		return res;
	}

	Vector3<T> operator/(int nb) const {
		Vector3 res;
		res.x=this->x/nb;
		res.y=this->y/nb;
		res.z=this->z/nb;

		return res;
	}

	Vector3<float> operator/(float nb) const {
		Vector3<float> res;
		res.x=this->x/nb;
		res.y=this->y/nb;
		res.z=this->z/nb;

		return res;
	}

	Vector3<T> operator+(const Vector3<T> &v2) const {
		Vector3 res;
		res.x=this->x+v2.x;
		res.y=this->y+v2.y;
		res.z=this->z+v2.z;

		return res;
	}

	Vector3<T> operator/(Vector3 &v2) const {
		Vector3 res;
		res.x=this->x/v2.x;
		res.y=this->y/v2.y;
		res.z=this->z/v2.z;

		return res;
	}

	Vector3<T> operator*(Vector3 &v2) const {
		Vector3 res;
		res.x=this->x*v2.x;
		res.y=this->y*v2.y;
		res.z=this->z*v2.z;

		return res;
	}


	Vector3<T> operator%(const Vector3 &v2) const {
		Vector3 res;
		res.x=this->x%v2.x;
		res.y=this->y%v2.y;
		res.z=this->z%v2.z;

		return res;
	}

	Vector3<T> operator+(T nb) const{
		Vector3 res;
		res.x=this->x+nb;
		res.y=this->y+nb;
		res.z=this->z+nb;

		return res;
	}

	Vector3<T> operator-(T nb) const{
		Vector3 res;
		res.x=this->x-nb;
		res.y=this->y-nb;
		res.z=this->z-nb;

		return res;
	}


	Vector3<T> operator*(T nb) const {
		Vector3 res;
		res.x=this->x*nb;
		res.y=this->y*nb;
		res.z=this->z*nb;

		return res;
	}

	T length(){
		return sqrt((float)x*x+y*y+z*z);
	}

	void clamp(const Vector3<T> &v0, const Vector3<T> &v1){
		if(x<v0.x)
			x=v0.x;
		if(y<v0.y)
			y=v0.y;
		if(z<v0.z)
			z=v0.z;


		if(x>v1.x)
			x=v1.x;
		if(y>v1.y)
			y=v1.y;
		if(z>v1.z)
			z=v1.z;

	}

	void normalize(){
		T l=this->length();
		x/=l;
		y/=l;
		z/=l;
	}

	friend Vector3<T> vceil(Vector3<T> &v){
		Vector3<T> res;
		res.x=ceilf((float)v.x);
		res.y=ceilf((float)v.y);
		res.z=ceilf((float)v.z);

		return res;
	}

	Vector3<T> abs() const{
		Vector3<T> res;
		res.x=x>0 ? x : 0-x;
		res.y=y>0 ? y : 0-y;
		res.z=z>0 ? z : 0-z;

		return res;
	}

	Vector3<T> floor() const{
		Vector3<T> res;
		res.x=floorf((float)x);
		res.y=floorf((float)y);
		res.z=floorf((float)z);

		return res;
	}
	Vector3<T> ceil() const{
		Vector3<T> res;
		res.x=ceilf((float)x);
		res.y=ceilf((float)y);
		res.z=ceilf((float)z);

		return res;
	}

	T squareDistance(Vector3<T> &v){
		return ( pow(v.x-x, 2) + pow(v.y-y, 2) + pow(v.z-z, 2));
	}

	T distance(Vector3<T> &v){
		return sqrt( pow(v.x-x, 2) + pow(v.y-y, 2) + pow(v.z-z, 2));
	}

	//Produit vectoriel
	Vector3<T> cross(Vector3<T> vVector2) {
		Vector3<T> vCross;								// The vector to hold the cross product
													// Get the X value
		vCross.x = ((this->y * vVector2.z) - (this->z * vVector2.y));
													// Get the Y value
		vCross.y = ((this->z * vVector2.x) - (this->x * vVector2.z));
													// Get the Z value
		vCross.z = ((this->x * vVector2.y) - (this->y * vVector2.x));

		return vCross;								// Return the cross product
	}

	inline T &operator[](int i) { return ((T*)&x)[i]; }
	inline const T operator[](int i) const { return ((T*)&x)[i]; }

	inline void cross(const Vector3<T> &v1,const Vector3<T> &v2) {
		x = v1.y * v2.z - v1.z * v2.y;
		y = v1.z * v2.x - v1.x * v2.z;
		z = v1.x * v2.y - v1.y * v2.x;
	}

	inline const Vector3<T> operator-() const { return Vector3<T>(-x,-y,-z); }
	inline Vector3<T> operator-() { return Vector3<T>(-x,-y,-z); }

	inline bool operator<(const Vector3<T> &v2) const{
		return x<v2.x && y<v2.y && z<v2.z;
	}
	inline bool operator==(const Vector3<T> &v2) const{
		return x==v2.x && y==v2.y && z==v2.z;
	}
	inline bool operator!=(const Vector3<T> &v2) const{
		return !(*this==v2);
	}


	inline T maxComp() const{
		if(x>y)
			if(x>z)
				return x;
			else
				return z;
		else
			if(y>z)
				return y;
			else
				return z;
	}

	/*inline Vector3<T> &operator=(const Vector4<T> &v2) {
		x=v2.x; y=v2.y; z=v2.z;
		return *this;
	}*/


	inline void rotate(const Vector3<T> &v, double ang){
	   // First we calculate [w,x,y,z], the rotation quaternion
	   double w,x,y,z;
	   Vector3<T> V=v;
	   V.normalize();
	   w=cos(-ang/2);  // The formula rotates counterclockwise, and I
					   // prefer clockwise, so I change 'ang' sign
	   double s=sin(-ang/2);
	   x=V.x*s;
	   y=V.y*s;
	   z=V.z*s;
	   // now we calculate [w^2, x^2, y^2, z^2]; we need it
	   double w2=w*w;
	   double x2=x*x;
	   double y2=y*y;
	   double z2=z*z;
	   
	   // And apply the formula
	   Vector3<T> res=Vector3<T>((*this).x*(w2+x2-y2-z2) + (*this).y*2*(x*y+w*z)   + (*this).z*2*(x*z-w*y),
			   (*this).x*2*(x*y-w*z)   + (*this).y*(w2-x2+y2-z2) + (*this).z*2*(y*z+w*x),
			   (*this).x*2*(x*z+w*y)   + (*this).y*2*(y*z-w*x)   + (*this).z*(w2-x2-y2+z2));

	   (*this)=res;
	   
	 }

	inline void translate(const Vector3<T> &v){
		this->x+=v.x;
		this->y+=v.y;
		this->z+=v.z;
	}


	Vector3<T> lerp(const Vector3<T> &v2, float a) const{
		return (*this)*a + v2*(1.0f-a);
	}

	///////Casts
	Vector3<float> toFloat() const {
		return Vector3<float>(x, y, z);
	}

	Vector3<int> toInt() const {
		return Vector3<int>((int)x, (int)y, (int)z);
	}

};

template<class T>
class Vector4 {

public:

	T x;
	T y;
	T z;
	T w;

	Vector4(void){}
	Vector4(T xx, T yy, T zz, T ww): x(xx), y(yy), z(zz), w(ww){}
	Vector4(T xx): x(xx), y(xx), z(xx), w(xx){}
	~Vector4(void){}

	Vector4(const Vector3<T> &v) : x(v.x), y(v.y), z(v.z), w(0){
	}


	Vector4<T> operator-(Vector4<T> &v2) const {
		Vector4 res;
		res.x=this->x-v2.x;
		res.y=this->y-v2.y;
		res.z=this->z-v2.z;

		res.w=this->w-v2.w;

		return res;
	}

	Vector4<T> operator/(T nb) const{
		Vector4 res;
		res.x=this->x/nb;
		res.y=this->y/nb;
		res.z=this->z/nb;

		res.w=this->w/nb;
		return res;
	}

	Vector4<T> operator+(Vector4<T> &v2) const{
		Vector4 res;
		res.x=this->x+v2.x;
		res.y=this->y+v2.y;
		res.z=this->z+v2.z;

		res.w=this->w+v2.w;
		return res;
	}

	Vector4<T> operator/(Vector4 &v2) const{
		Vector4 res;
		res.x=this->x/v2.x;
		res.y=this->y/v2.y;
		res.z=this->z/v2.z;

		res.w=this->w/v2.w;
		return res;
	}


	Vector4<T> operator+(T nb) const{
		Vector4 res;
		res.x=this->x+nb;
		res.y=this->y+nb;
		res.z=this->z+nb;

		res.w=this->w+nb;
		return res;
	}

	Vector4<T> operator-(T nb) const{
		Vector4 res;
		res.x=this->x-nb;
		res.y=this->y-nb;
		res.z=this->z-nb;

		res.w=this->w-nb;
		return res;
	}


	Vector4<T> operator*(T nb) const{
		Vector4<T> res;
		res.x=this->x*nb;
		res.y=this->y*nb;
		res.z=this->z*nb;

		res.w=this->w*nb;
		return res;
	}

	inline const Vector4<T> operator-() const { return Vector4<T>(-x,-y,-z, -w); }

	T length() const{
		return sqrt(x*x+y*y+z*z + w*w);
	}

	void normalize(){
		T l=this->length();
		x/=l;
		y/=l;
		z/=l;
		w/=l;
	}

	friend Vector4<T> vceil(Vector4<T> v){
		Vector4<T> res;
		res.x=ceil((double)v.x);
		res.y=ceil((double)v.y);
		res.z=ceil((double)v.z);

		res.w=ceil((double)v.w);
		return res;
	}

	T squareDistance(Vector4<T> v){
		return ( pow(v.x-x, 2) + pow(v.y-y, 2) + pow(v.z-z, 2)  + pow(v.w-w, 2));
	}

	T distance(Vector4<T> v){
		return sqrt( pow(v.x-x, 2) + pow(v.y-y, 2) + pow(v.z-z, 2)  + pow(v.w-w, 2));
	}

	//Produit vectoriel
	Vector4<T> cross(Vector4<T> vVector2) {
		Vector4<T> vCross;								// The vector to hold the cross product
													// Get the X value
		vCross.x = ((this->y * vVector2.z) - (this->z * vVector2.y));
													// Get the Y value
		vCross.y = ((this->z * vVector2.w) - (this->x * vVector2.w));
													// Get the Z value
		vCross.z = ((this->x * vVector2.y) - (this->y * vVector2.x));

		vCross.z = ((this->w * vVector2.x) - (this->w * vVector2.z));

		return vCross;								// Return the cross product
	}

	inline T &operator[](int i) { return ((T*)&x)[i]; }
	inline const T operator[](int i) const { return ((T*)&x)[i]; }

	Vector3<T> &xyz() const{
		return *(Vector3<T>*)((this));
		//return Vector3<T>(x, y, z);
	}

	Vector4<T> lerp(const Vector4<T> &v2, float a) const{
		return (*this)*(1-a) + v2*(a);
	}
};

typedef Vector2<int> Vector2i;
typedef Vector2<float> Vector2f;

typedef Vector3<int> Vector3i;
typedef Vector3<float> Vector3f;
typedef Vector4<float> Vector4f;


typedef Vector3<float> CVector3;



/////////////////
typedef Vector3<float> vec3;
typedef Vector4<float> vec4;


#define EPSILON 1e-6f
#ifndef PI
#define PI 3.14159265358979323846f
#endif

#define DEG2RAD (PI / 180.0f)
#define RAD2DEG (180.0f / PI)

struct Mat4 {
	
	Mat4() {
		mat[0] = 1.0; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
		mat[1] = 0.0; mat[5] = 1.0; mat[9] = 0.0; mat[13] = 0.0;
		mat[2] = 0.0; mat[6] = 0.0; mat[10] = 1.0; mat[14] = 0.0;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}
	Mat4(const vec3 &v) {
		translate(v);
	}
	Mat4(float x,float y,float z) {
		translate(x,y,z);
	}
	Mat4(const vec3 &axis,float angle) {
		rotate(axis,angle);
	}
	Mat4(float x,float y,float z,float angle) {
		rotate(x,y,z,angle);
	}
/*	Mat4(const mat3 &m) {
		mat[0] = m[0]; mat[4] = m[3]; mat[8] = m[6]; mat[12] = 0.0;
		mat[1] = m[1]; mat[5] = m[4]; mat[9] = m[7]; mat[13] = 0.0;
		mat[2] = m[2]; mat[6] = m[5]; mat[10] = m[8]; mat[14] = 0.0;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}*/
	Mat4(const float *m) {
		mat[0] = m[0]; mat[4] = m[4]; mat[8] = m[8]; mat[12] = m[12];
		mat[1] = m[1]; mat[5] = m[5]; mat[9] = m[9]; mat[13] = m[13];
		mat[2] = m[2]; mat[6] = m[6]; mat[10] = m[10]; mat[14] = m[14];
		mat[3] = m[3]; mat[7] = m[7]; mat[11] = m[11]; mat[15] = m[15];
	}
	Mat4(const Mat4 &m) {
		mat[0] = m[0]; mat[4] = m[4]; mat[8] = m[8]; mat[12] = m[12];
		mat[1] = m[1]; mat[5] = m[5]; mat[9] = m[9]; mat[13] = m[13];
		mat[2] = m[2]; mat[6] = m[6]; mat[10] = m[10]; mat[14] = m[14];
		mat[3] = m[3]; mat[7] = m[7]; mat[11] = m[11]; mat[15] = m[15];
	}
	
	vec3 operator*(const vec3 &v) const {
		vec3 ret;
		ret[0] = mat[0] * v[0] + mat[4] * v[1] + mat[8] * v[2] + mat[12];
		ret[1] = mat[1] * v[0] + mat[5] * v[1] + mat[9] * v[2] + mat[13];
		ret[2] = mat[2] * v[0] + mat[6] * v[1] + mat[10] * v[2] + mat[14];
		return ret;
	}
	vec4 operator*(const vec4 &v) const {
		vec4 ret;
		ret[0] = mat[0] * v[0] + mat[4] * v[1] + mat[8] * v[2] + mat[12] * v[3];
		ret[1] = mat[1] * v[0] + mat[5] * v[1] + mat[9] * v[2] + mat[13] * v[3];
		ret[2] = mat[2] * v[0] + mat[6] * v[1] + mat[10] * v[2] + mat[14] * v[3];
		ret[3] = mat[3] * v[0] + mat[7] * v[1] + mat[11] * v[2] + mat[15] * v[3];
		return ret;
	}
	Mat4 operator*(float f) const {
		Mat4 ret;
		ret[0] = mat[0] * f; ret[4] = mat[4] * f; ret[8] = mat[8] * f; ret[12] = mat[12] * f;
		ret[1] = mat[1] * f; ret[5] = mat[5] * f; ret[9] = mat[9] * f; ret[13] = mat[13] * f;
		ret[2] = mat[2] * f; ret[6] = mat[6] * f; ret[10] = mat[10] * f; ret[14] = mat[14] * f;
		ret[3] = mat[3] * f; ret[7] = mat[7] * f; ret[11] = mat[11] * f; ret[15] = mat[15] * f;
		return ret;
	}
	Mat4 operator*(const Mat4 &m) const {
		Mat4 ret;
		ret[0] = mat[0] * m[0] + mat[4] * m[1] + mat[8] * m[2] + mat[12] * m[3];
		ret[1] = mat[1] * m[0] + mat[5] * m[1] + mat[9] * m[2] + mat[13] * m[3];
		ret[2] = mat[2] * m[0] + mat[6] * m[1] + mat[10] * m[2] + mat[14] * m[3];
		ret[3] = mat[3] * m[0] + mat[7] * m[1] + mat[11] * m[2] + mat[15] * m[3];
		ret[4] = mat[0] * m[4] + mat[4] * m[5] + mat[8] * m[6] + mat[12] * m[7];
		ret[5] = mat[1] * m[4] + mat[5] * m[5] + mat[9] * m[6] + mat[13] * m[7];
		ret[6] = mat[2] * m[4] + mat[6] * m[5] + mat[10] * m[6] + mat[14] * m[7];
		ret[7] = mat[3] * m[4] + mat[7] * m[5] + mat[11] * m[6] + mat[15] * m[7];
		ret[8] = mat[0] * m[8] + mat[4] * m[9] + mat[8] * m[10] + mat[12] * m[11];
		ret[9] = mat[1] * m[8] + mat[5] * m[9] + mat[9] * m[10] + mat[13] * m[11];
		ret[10] = mat[2] * m[8] + mat[6] * m[9] + mat[10] * m[10] + mat[14] * m[11];
		ret[11] = mat[3] * m[8] + mat[7] * m[9] + mat[11] * m[10] + mat[15] * m[11];
		ret[12] = mat[0] * m[12] + mat[4] * m[13] + mat[8] * m[14] + mat[12] * m[15];
		ret[13] = mat[1] * m[12] + mat[5] * m[13] + mat[9] * m[14] + mat[13] * m[15];
		ret[14] = mat[2] * m[12] + mat[6] * m[13] + mat[10] * m[14] + mat[14] * m[15];
		ret[15] = mat[3] * m[12] + mat[7] * m[13] + mat[11] * m[14] + mat[15] * m[15];
		return ret;
	}
	Mat4 operator+(const Mat4 &m) const {
		Mat4 ret;
		ret[0] = mat[0] + m[0]; ret[4] = mat[4] + m[4]; ret[8] = mat[8] + m[8]; ret[12] = mat[12] + m[12];
		ret[1] = mat[1] + m[1]; ret[5] = mat[5] + m[5]; ret[9] = mat[9] + m[9]; ret[13] = mat[13] + m[13];
		ret[2] = mat[2] + m[2]; ret[6] = mat[6] + m[6]; ret[10] = mat[10] + m[10]; ret[14] = mat[14] + m[14];
		ret[3] = mat[3] + m[3]; ret[7] = mat[7] + m[7]; ret[11] = mat[11] + m[11]; ret[15] = mat[15] + m[15];
		return ret;
	}
	Mat4 operator-(const Mat4 &m) const {
		Mat4 ret;
		ret[0] = mat[0] - m[0]; ret[4] = mat[4] - m[4]; ret[8] = mat[8] - m[8]; ret[12] = mat[12] - m[12];
		ret[1] = mat[1] - m[1]; ret[5] = mat[5] - m[5]; ret[9] = mat[9] - m[9]; ret[13] = mat[13] - m[13];
		ret[2] = mat[2] - m[2]; ret[6] = mat[6] - m[6]; ret[10] = mat[10] - m[10]; ret[14] = mat[14] - m[14];
		ret[3] = mat[3] - m[3]; ret[7] = mat[7] - m[7]; ret[11] = mat[11] - m[11]; ret[15] = mat[15] - m[15];
		return ret;
	}
	
	Mat4 &operator*=(float f) { return *this = *this * f; }
	Mat4 &operator*=(const Mat4 &m) { return *this = *this * m; }
	Mat4 &operator+=(const Mat4 &m) { return *this = *this + m; }
	Mat4 &operator-=(const Mat4 &m) { return *this = *this - m; }
	
	operator float*() { return mat; }
	operator const float*() const { return mat; }
	
	float &operator[](int i) { return mat[i]; }
	const float operator[](int i) const { return mat[i]; }
	
	Mat4 rotation() const {
		Mat4 ret;
		ret[0] = mat[0]; ret[4] = mat[4]; ret[8] = mat[8]; ret[12] = 0;
		ret[1] = mat[1]; ret[5] = mat[5]; ret[9] = mat[9]; ret[13] = 0;
		ret[2] = mat[2]; ret[6] = mat[6]; ret[10] = mat[10]; ret[14] = 0;
		ret[3] = 0; ret[7] = 0; ret[11] = 0; ret[15] = 1;
		return ret;
	}
	Mat4 transpose() const {
		Mat4 ret;
		ret[0] = mat[0]; ret[4] = mat[1]; ret[8] = mat[2]; ret[12] = mat[3];
		ret[1] = mat[4]; ret[5] = mat[5]; ret[9] = mat[6]; ret[13] = mat[7];
		ret[2] = mat[8]; ret[6] = mat[9]; ret[10] = mat[10]; ret[14] = mat[11];
		ret[3] = mat[12]; ret[7] = mat[13]; ret[11] = mat[14]; ret[15] = mat[15];
		return ret;
	}
	Mat4 transpose_rotation() const {
		Mat4 ret;
		ret[0] = mat[0]; ret[4] = mat[1]; ret[8] = mat[2]; ret[12] = mat[12];
		ret[1] = mat[4]; ret[5] = mat[5]; ret[9] = mat[6]; ret[13] = mat[13];
		ret[2] = mat[8]; ret[6] = mat[9]; ret[10] = mat[10]; ret[14] = mat[14];
		ret[3] = mat[3]; ret[7] = mat[7]; ret[14] = mat[14]; ret[15] = mat[15];
		return ret;
	}
	
	float det() const {
		float det;
		det = mat[0] * mat[5] * mat[10];
		det += mat[4] * mat[9] * mat[2];
		det += mat[8] * mat[1] * mat[6];
		det -= mat[8] * mat[5] * mat[2];
		det -= mat[4] * mat[1] * mat[10];
		det -= mat[0] * mat[9] * mat[6];
		return det;
	}
	
	Mat4 inverse() const {
		Mat4 ret;
		float idet = 1.0f / det();
		ret[0] =  (mat[5] * mat[10] - mat[9] * mat[6]) * idet;
		ret[1] = -(mat[1] * mat[10] - mat[9] * mat[2]) * idet;
		ret[2] =  (mat[1] * mat[6] - mat[5] * mat[2]) * idet;
		ret[3] = 0.0;
		ret[4] = -(mat[4] * mat[10] - mat[8] * mat[6]) * idet;
		ret[5] =  (mat[0] * mat[10] - mat[8] * mat[2]) * idet;
		ret[6] = -(mat[0] * mat[6] - mat[4] * mat[2]) * idet;
		ret[7] = 0.0;
		ret[8] =  (mat[4] * mat[9] - mat[8] * mat[5]) * idet;
		ret[9] = -(mat[0] * mat[9] - mat[8] * mat[1]) * idet;
		ret[10] =  (mat[0] * mat[5] - mat[4] * mat[1]) * idet;
		ret[11] = 0.0;
		ret[12] = -(mat[12] * ret[0] + mat[13] * ret[4] + mat[14] * ret[8]);
		ret[13] = -(mat[12] * ret[1] + mat[13] * ret[5] + mat[14] * ret[9]);
		ret[14] = -(mat[12] * ret[2] + mat[13] * ret[6] + mat[14] * ret[10]);
		ret[15] = 1.0;
		return ret;
	}
	
	void zero() {
		mat[0] = 0.0; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
		mat[1] = 0.0; mat[5] = 0.0; mat[9] = 0.0; mat[13] = 0.0;
		mat[2] = 0.0; mat[6] = 0.0; mat[10] = 0.0; mat[14] = 0.0;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 0.0;
	}
	void identity() {
		mat[0] = 1.0; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
		mat[1] = 0.0; mat[5] = 1.0; mat[9] = 0.0; mat[13] = 0.0;
		mat[2] = 0.0; mat[6] = 0.0; mat[10] = 1.0; mat[14] = 0.0;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}
	void rotate(const vec3 &axis,float angle) {
		float rad = angle * DEG2RAD;
		float c = cos(rad);
		float s = sin(rad);
		vec3 v = axis;
		v.normalize();
		float xx = v.x * v.x;
		float yy = v.y * v.y;
		float zz = v.z * v.z;
		float xy = v.x * v.y;
		float yz = v.y * v.z;
		float zx = v.z * v.x;
		float xs = v.x * s;
		float ys = v.y * s;
		float zs = v.z * s;
		mat[0] = (1.0f - c) * xx + c; mat[4] = (1.0f - c) * xy - zs; mat[8] = (1.0f - c) * zx + ys; mat[12] = 0.0;
		mat[1] = (1.0f - c) * xy + zs; mat[5] = (1.0f - c) * yy + c; mat[9] = (1.0f - c) * yz - xs; mat[13] = 0.0;
		mat[2] = (1.0f - c) * zx - ys; mat[6] = (1.0f - c) * yz + xs; mat[10] = (1.0f - c) * zz + c; mat[14] = 0.0;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}
	void rotate(float x,float y,float z,float angle) {
		rotate(vec3(x,y,z),angle);
	}
	void rotate_x(float angle) {
		float rad = angle * DEG2RAD;
		float c = cos(rad);
		float s = sin(rad);
		mat[0] = 1.0; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
		mat[1] = 0.0; mat[5] = c; mat[9] = -s; mat[13] = 0.0;
		mat[2] = 0.0; mat[6] = s; mat[10] = c; mat[14] = 0.0;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}
	void rotate_y(float angle) {
		float rad = angle * DEG2RAD;
		float c = cos(rad);
		float s = sin(rad);
		mat[0] = c; mat[4] = 0.0; mat[8] = s; mat[12] = 0.0;
		mat[1] = 0.0; mat[5] = 1.0; mat[9] = 0.0; mat[13] = 0.0;
		mat[2] = -s; mat[6] = 0.0; mat[10] = c; mat[14] = 0.0;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}
	void rotate_z(float angle) {
		float rad = angle * DEG2RAD;
		float c = cos(rad);
		float s = sin(rad);
		mat[0] = c; mat[4] = -s; mat[8] = 0.0; mat[12] = 0.0;
		mat[1] = s; mat[5] = c; mat[9] = 0.0; mat[13] = 0.0;
		mat[2] = 0.0; mat[6] = 0.0; mat[10] = 1.0; mat[14] = 0.0;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}
	void scale(const vec3 &v) {
		mat[0] = v.x; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
		mat[1] = 0.0; mat[5] = v.y; mat[9] = 0.0; mat[13] = 0.0;
		mat[2] = 0.0; mat[6] = 0.0; mat[10] = v.z; mat[14] = 0.0;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}
	void scale(float x,float y,float z) {
		scale(vec3(x,y,z));
	}
	void translate(const vec3 &v) {
		mat[0] = 1.0; mat[4] = 0.0; mat[8] = 0.0; mat[12] = v.x;
		mat[1] = 0.0; mat[5] = 1.0; mat[9] = 0.0; mat[13] = v.y;
		mat[2] = 0.0; mat[6] = 0.0; mat[10] = 1.0; mat[14] = v.z;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}
	void translate(float x,float y,float z) {
		translate(vec3(x,y,z));
	}
	void reflect(const vec4 &plane) {
		float x = plane.x;
		float y = plane.y;
		float z = plane.z;
		float x2 = x * 2.0f;
		float y2 = y * 2.0f;
		float z2 = z * 2.0f;
		mat[0] = 1.0f - x * x2; mat[4] = -y * x2; mat[8] = -z * x2; mat[12] = -plane.w * x2;
		mat[1] = -x * y2; mat[5] = 1.0f - y * y2; mat[9] = -z * y2; mat[13] = -plane.w * y2;
		mat[2] = -x * z2; mat[6] = -y * z2; mat[10] = 1.0f - z * z2; mat[14] = -plane.w * z2;
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = 0.0; mat[15] = 1.0;
	}
	void reflect(float x,float y,float z,float w) {
		reflect(vec4(x,y,z,w));
	}
	
	void perspective(float fov,float aspect,float znear,float zfar) {
		if(fabs(fov - 90.0f) < EPSILON) fov = 89.9f;
		float y = tan(fov * PI / 360.0f);
		float x = y * aspect;
		mat[0] = 1.0f / x; mat[4] = 0.0; mat[8] = 0.0; mat[12] = 0.0;
		mat[1] = 0.0; mat[5] = 1.0f / y; mat[9] = 0.0; mat[13] = 0.0;
		mat[2] = 0.0; mat[6] = 0.0; mat[10] = -(zfar + znear) / (zfar - znear); mat[14] = -(2.0f * zfar * znear) / (zfar - znear);
		mat[3] = 0.0; mat[7] = 0.0; mat[11] = -1.0; mat[15] = 0.0;
	}
	void look_at(const vec3 &eye, const vec3 &dir,const vec3 &up) {
		vec3 x,y,z;
		Mat4 m0,m1;
		z = eye - dir;
		z.normalize();
		x.cross(up,z);
		x.normalize();
		y.cross(z,x);
		y.normalize();
		m0[0] = x.x; m0[4] = x.y; m0[8] = x.z; m0[12] = 0.0;
		m0[1] = y.x; m0[5] = y.y; m0[9] = y.z; m0[13] = 0.0;
		m0[2] = z.x; m0[6] = z.y; m0[10] = z.z; m0[14] = 0.0;
		m0[3] = 0.0; m0[7] = 0.0; m0[11] = 0.0; m0[15] = 1.0;
		m1.translate(-eye);
		*this = m0 * m1;
	}
	void look_at(const float *eye,const float *dir,const float *up) {
		look_at(vec3(eye),vec3(dir),vec3(up));
	}
	
	float mat[16];
};

};
#endif
