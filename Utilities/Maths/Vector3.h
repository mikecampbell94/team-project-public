#pragma once
/*
Class:Vector3
Implements:
Author:Rich Davison
Description:VERY simple Vector3 class. Students are encouraged to modify this as necessary!

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*/
#include <cmath>
#include <iostream>
#include "../../Resource Management/XMLParser.h"
#include "Vector2.h"
#include "VectorBuilder.h"
#include <iterator>
#include <sstream>

class NCLVector3	{
public:
	NCLVector3(void) {
		toZero();
	}

	NCLVector3(const float x, const float y, const float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	NCLVector3(NCLVector2 v)
	{
		this->x = v.x;
		this->y = v.y;
		this->z = 1;
	}

	~NCLVector3(void){}

	float x;
	float y;
	float z;

	static NCLVector3 interpolate(const NCLVector3& a, const NCLVector3& b, float factor)
	{
		return a + ((b - a) * factor);
	}

	const NCLVector3&	normalise() {
		float currentLength = length();

		if(currentLength != 0.0f)	{
			currentLength = 1.0f / currentLength;
			x = x * currentLength;
			y = y * currentLength;
			z = z * currentLength;
		}

		return *this;
	}

	float sqrLength() const
	{
		return (x*x) + (y*y) + (z*z);
	}

	void		toZero() {
		x = y = z = 0.0f;
	}

	float			length() const {
		return sqrt((x*x)+(y*y)+(z*z));	
	}

	float lengthSquared() const {
		return (x*x + y*y + z*z);
	}

	void			invert() {
		x = -x;
		y = -y;	
		z = -z;	
	}

	NCLVector3			inverse() const{
		return NCLVector3(-x,-y,-z);
	}

	static float	dot(const NCLVector3 &a, const NCLVector3 &b) {
		return (a.x*b.x)+(a.y*b.y)+(a.z*b.z);
	}

	static NCLVector3	cross(const NCLVector3 &a, const NCLVector3 &b) {
		return NCLVector3((a.y*b.z) - (a.z*b.y) , (a.z*b.x) - (a.x*b.z) , (a.x*b.y) - (a.y*b.x));	
	}

	inline friend std::ostream& operator<<(std::ostream& o, const NCLVector3& v) {
		o << "Vector3(" << v.x << "," << v.y << "," << v.z <<")" << std::endl;
		return o;
	}

	inline NCLVector3  operator+(const NCLVector3  &a) const{
		return NCLVector3(x + a.x,y + a.y, z + a.z);
	}

	inline NCLVector3  operator-(const NCLVector3  &a) const{
		return NCLVector3(x - a.x,y - a.y, z - a.z);
	}

	inline NCLVector3  operator-() const{
		return NCLVector3(-x,-y,-z);
	}

	inline void operator+=(const NCLVector3  &a){
		x += a.x;
		y += a.y;
		z += a.z;
	}

	inline void operator-=(const NCLVector3  &a){
		x -= a.x;
		y -= a.y;
		z -= a.z;
	}

	inline NCLVector3  operator*(const float a) const{
		return NCLVector3(x * a,y * a, z * a);
	}

	inline NCLVector3  operator*(const NCLVector3  &a) const{
		return NCLVector3(x * a.x,y * a.y, z * a.z);
	}

	inline NCLVector3  operator/(const NCLVector3  &a) const{
		return NCLVector3(x / a.x,y / a.y, z / a.z);
	};

	inline NCLVector3  operator/(const float v) const{
		return NCLVector3(x / v,y / v, z / v);
	};

	inline bool	operator==(const NCLVector3 &A)const {return (A.x == x && A.y == y && A.z == z) ? true : false;};
	inline bool	operator!=(const NCLVector3 &A)const {return (A.x == x && A.y == y && A.z == z) ? false : true;};

	static NCLVector3 builder(Node* node)
	{
		const float x = VectorBuilder::getVectorComponentFromNode(node->children[0]);
		const float y = VectorBuilder::getVectorComponentFromNode(node->children[1]);
		const float z = VectorBuilder::getVectorComponentFromNode(node->children[2]);
		return NCLVector3(x, y, z);
	}

	static NCLVector3 builder(std::string text)
	{
		std::stringstream ss(text);
		std::string item;
		std::vector<std::string> tokens;

		while (std::getline(ss, item, ','))
		{
			tokens.push_back(item);
		}

		return NCLVector3(stof(tokens[0]), stof(tokens[1]), stof(tokens[2]));
	}
};

