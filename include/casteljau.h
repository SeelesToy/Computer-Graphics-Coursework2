#include <list>
#include <vector>
#include <algorithm>

#include "point.h"

point evaluate(float t, std::list<point> P)
{
	std::list<point> Q = P;
	//std::vector<point> Q;
	//std::copy(P.begin(), P.end(), std::back_inserter(Q));
	std::list<point>::iterator p1;
	std::list<point>::iterator p2;
	
	while (Q.size() > 1) {
		std::list<point> R;
		R.clear();
		p1 = Q.begin();
		p2 = ++Q.begin();
		
		for (int i = 0; i < Q.size() - 1; i++) {
			//point p1 = Q[i];
			//point p2 = Q[i + 1];
			//std::cout << "p1" << " " << (*p1).x << " " << (*p1).y << " " << (*p1).z << std::endl;
			//std::cout << "p2" << " " << (*p2).x << " " << (*p2).y << " " << (*p2).z << std::endl;
			point p = ((1.f - t) * (*p1)) + (t * (*p2));
			
			//std::cout << "p " << p.x << " " << p.y << " " << p.z << std::endl;
			p1++;
			p2++;
			R.push_back(p);
		}
		Q.clear();
		Q = R;
		//std::copy(R.begin(), R.end(), std::back_inserter(Q));
		
	} 
	//std::cout << Q.front().x<<" "<< Q.front().y<<" "<< Q.front().z << std::endl;
	return Q.front();
}

std::vector<point> EvaluateBezierCurve(std::vector<point>ctrl_points, int num_evaluations)
{
	std::list<point> ps(ctrl_points.begin(), ctrl_points.end());
	std::vector<point> curve;
	float offset = (float)1.f / num_evaluations;
	//std::cout << "off " << offset << std::endl;
	curve.push_back(ctrl_points[0]);
	for (int e = 0; e <= num_evaluations - 1; e++) {
		point p = evaluate(offset * (e + 1), ps);
		curve.push_back(p);
	}
	return curve;
}

float* MakeFloatsFromVector(std::vector<point> curve, int &num_verts, int &num_floats, float r, float g, float b)
{
	num_verts = curve.size();
	if (num_verts == 0)
		return NULL;
	num_floats = num_verts * 6;
	float *vertices = new float[num_floats];
	int i = 0;
	for (auto p : curve) {
		vertices[i*6] = p.x;
		vertices[i * 6 + 1] = p.y;
		vertices[i * 6 + 2] = p.z;
		vertices[i * 6 + 3] = r;
		vertices[i * 6 + 4] = g;
		vertices[i * 6 + 5] = b;
		i++;
	}

	return vertices;
}

