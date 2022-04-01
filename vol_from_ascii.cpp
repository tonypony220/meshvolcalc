#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
//#include "utils.hpp"
#include <array>
#include <vector>
#include <cmath>
#include <cerrno>

enum states {
	VERTICES = 1,
	TETRA ,
	END
};

enum errors
{
	missing = 1,//= 1,
	badline  ,  //= 2,
	badtetra ,  //= 3,
	badvert  ,  //= 4,
	badfile	 ,
	badargs  ,
	baddouble  ,
	badtetchar,
	badtetdig,
	badtetlen,
	toomanydoubles,
};

struct point {
	float x;
	float y;
	float z;

	point() : x(0), y(0), z(0) {}
	point(float xp, float yp, float zp) : x(xp), y(yp), z(zp) {}
};

std::ostream& operator<<(std::ostream& out, const point p) {
	out << "(" << p.x << ", " << p.y << ", " << p.z << ")" << std::endl;
	return out;
}

struct Tetrahedron {
	point v1;
	point v2;
	point v3;
	point v4;
	float area;
	Tetrahedron(point v1p, point v2p, point v3p, point v4p) :
			v1(v1p), v2(v2p), v3(v3p), v4(v4p) {}

	float get_area() {
		point p1(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
		point p2(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);
		point p3(v4.x - v1.x, v4.y - v1.y, v4.z - v1.z);

		float v321 = p3.x*p2.y*p1.z;
		float v231 = p2.x*p3.y*p1.z;
		float v312 = p3.x*p1.y*p2.z;
		float v132 = p1.x*p3.y*p2.z;
		float v213 = p2.x*p1.y*p3.z;
		float v123 = p1.x*p2.y*p3.z;
		area = ((-v321 + v231 + v312 - v132 - v213 + v123) / 6.0f);
//		p(area);
		return area;
	}
};

template <typename T>
class FileMemory {
private:
	size_t number;
	std::string filename;
	std::fstream file;
	bool ok;
public:
	FileMemory(const std::string & name) : number(0), filename(name) {
		file.open(filename,
			std::ios::binary | std::ios::out | std::ios::in | std::ios::trunc);
		if (!file.is_open()) {
			std::cerr << "Can't open file "<<  '\n'; //filename << ": " << std::strerror(errno) <<
			ok = false;
		}
		ok = true;
	}
	~FileMemory() { file.close(); }
	bool is_ok() { return ok && file.good(); }
	size_t size() { return number; }
	bool add(const T & data) {
		size_t offset = sizeof(T) * number;
		file.seekp(offset, std::ios::beg);
		file.write(reinterpret_cast<const char*>(&data), sizeof(data));
		number++;
		return true;
	}
	T get(size_t num) {
		T data;
		size_t offset = sizeof(data) * num;
		file.seekg(offset, std::ios::beg);
		file.read(reinterpret_cast<char*>(&data), sizeof(T));
		return data;
	}
};


int exiting(int err) {
	std::cerr << "error: " << err << std::endl;
	exit(1);
	return 1;
}

int validate_vert(const std::array<double, 3> & vert) {
	for (auto n : vert)	{
		if (n != n
			||  n < -std::numeric_limits<double>::max()
			||  n > std::numeric_limits<double>::max())
			return baddouble;
	}
	return 0;
}

int parse(std::ifstream & in,
		  FileMemory<point> & points,
		  FileMemory<std::array<int, 4> > & tetras ) {
	int state = 0;
	int ret = 0;
	std::stringstream ss;
	std::string s;
	while ( std::getline(in, s) && state < END) {
		if ( s == "VERTICES" || s == "TETRA" || s == "END" ) {
			state++;
			continue;
		}
		ss << s;
		if (state == VERTICES) {
			point p;
			ss >> p.x >> p.y >> p.z;
			ret = validate_vert(std::array<double, 3>{p.x, p.y, p.z});
			if (ret)
				return ret;
			if (!ss.eof())
				return toomanydoubles;
			points.add(p);
		}
		if (state == TETRA) {
			std::array<int, 4> t;
			ss >> t[0] >> t[1] >> t[2] >> t[3];
			tetras.add(t);
		}
		if (!ss || !state)
			return badline;
		ss.clear();
		ss.str("");
	}
	if (points.size() == 0 || tetras.size() == 0)
		return missing;
	return 0;
}

int main  (int ac, char **argv)
{
	FileMemory<point>				points("points"); // better to check opened
	FileMemory<std::array<int, 4> > tetras("tetras"); // better to check opened
	std::array<int, 4> 				numbers;

	(void)argv;
	ac == 1 || exiting( badargs );
	std::ifstream in( "input.mes" );
	in || exiting( badfile );
	int ret = parse(in, points, tetras);
	in.close();
	if (ret)
		exiting(ret);
	double sum = 0;
	for (size_t i = 0; i < tetras.size(); i++) {
		numbers = tetras.get(i);
//		p("tetra: ");
//		p(numbers);
		Tetrahedron t(points.get(numbers[0] - 1),
					  points.get(numbers[1] - 1),
					  points.get(numbers[2] - 1),
					  points.get(numbers[3] - 1)
		);
		sum += t.get_area();
	}
	std::cout << sum << std::endl;
//	p(sum);
	return 0;
}
