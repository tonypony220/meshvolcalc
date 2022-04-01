#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
//#include "utils.hpp"
#include <array>
#include <vector>
#include <map>
#include <cmath>
#include <cassert>

struct point {
	float x;
	float y;
	float z;

	point() : x(0), y(0), z(0) {}
	point(float xp, float yp, float zp) : x(xp), y(yp), z(zp) {}
};

bool operator==(const point & p1, const point & p2) {
	return p1.x	== p2.x && p1.y == p2.y && p1.z == p2.z;
}
bool operator<(const point & p1, const point & p2) {
	return p1.x	!= p2.x || p1.y != p2.y || p1.z != p2.z;
}
struct triangle {
	point normal;
	point v1;
	point v2;
	point v3;
	float area;
	triangle(point normalp, point v1p, point v2p, point v3p) :
			normal(normalp), v1(v1p), v2(v2p), v3(v3p) {}

	void calc_area() {
//		point p1(normal.x - v1.x, normal.y - v1.y, normal.z - v1.z);
//		point p2(normal.x - v2.x, normal.y - v2.y, normal.z - v2.z);
//		point p3(normal.x - v3.x, normal.y - v3.y, normal.z - v3.z);
		float v321 = v3.x * v2.y * v1.z;
		float v231 = v2.x * v3.y * v1.z;
		float v312 = v3.x * v1.y * v2.z;
		float v132 = v1.x * v3.y * v2.z;
		float v213 = v2.x * v1.y * v3.z;
		float v123 = v1.x * v2.y * v3.z;
		area = ((-v321 + v231 + v312 - v132 - v213 + v123) / 6.0f);
	}
};

struct stl_data {
	std::string name;
	std::vector<triangle> triangles;

	stl_data(std::string namep) : name(namep) {}
};

//	stl_data parse_stl(const std::string& stl_path);

std::ostream& operator<<(std::ostream& out, const point p) {
	out << "(" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;
	return out;
}

std::ostream& operator<<(std::ostream& out, const triangle& t) {
	out << "---- TRIANGLE ----" << std::endl;
	out << t.normal << std::endl;
	out << t.v1 << std::endl;
	out << t.v2 << std::endl;
	out << t.v3 << std::endl;
	return out;
}

float parse_float(std::ifstream& s) {
	char f_buf[sizeof(float)];
	s.read(f_buf, 4);
	float* fptr = (float*) f_buf;
	return *fptr;
}

point parse_point(std::ifstream & s) {
	float x = parse_float(s);
	float y = parse_float(s);
	float z = parse_float(s);
	return point(x, y, z);
}

stl_data parse_stl(const std::string& stl_path) {
	std::ifstream stl_file(stl_path.c_str(), std::ios::in | std::ios::binary);
	if (!stl_file) {
		std::cout << "ERROR: COULD NOT READ FILE" << std::endl;
		assert(false);
	}
	char header_info[80] = "";
	char n_triangles[4];
	stl_file.read(header_info, 80);
	stl_file.read(n_triangles, 4);
	std::string h(header_info);
	stl_data info(h);
	unsigned int* r = (unsigned int*) n_triangles;
	unsigned int num_triangles = *r;
	for (unsigned int i = 0; i < num_triangles; i++) {
		auto normal = parse_point(stl_file);
		auto v1 = parse_point(stl_file);
		auto v2 = parse_point(stl_file);
		auto v3 = parse_point(stl_file);
		info.triangles.push_back(triangle(normal, v1, v2, v3));
		char dummy[2];
		stl_file.read(dummy, 2);
	}
	return info;
}

//bool validate_obj_encloced(std::vector<triangle> & triangles) {
//	std::map<point, int> entries;
//	for (auto t : triangles) {
//		auto r = entries.find(t.v1);
//		if (r != entries.end())
//			r->second++;
//		else
//			entries.insert(std::make_pair(t.v1, 0));
//
//		r = entries.find(t.v2);
//		if (r != entries.end())
//			r->second++;
//		else
//			entries.insert(std::make_pair(t.v2, 0));
//
//		r = entries.find(t.v3);
//		if (r != entries.end())
//			r->second++;
//		else
//			entries.insert(std::make_pair(t.v3, 0));
//	}
//	for (auto en : entries) {
//		if (en.second != 3)
//			return false;
//	}
////		std::cout << en.first << "  " << en.second << std::endl;
//	return true;
//}

int exiting(int err) {
	std::cerr << "error: " << err << std::endl;
	exit(1);
	return 1;
}

int main  (int ac, char **argv)
{
	(void)argv;

	ac == 1 || exiting( 1 );
	auto info = parse_stl("input.stl");

	std::vector<triangle> triangles = info.triangles;
	std::cout << "STL HEADER = " << info.name << std::endl;
	std::cout << "# triangles = " << triangles.size() << std::endl;
	float sum = 0;
	for (auto t : info.triangles) {
		t.calc_area();
		sum += t.area;
	}
	std::cout << "Volume: " << fabs(sum) << std::endl;
	return 0;
}
