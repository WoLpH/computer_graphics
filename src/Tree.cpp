#include "Tree.hpp"
#include <cstdio>

AABB::AABB() : sub(0), radius(-1) { }

AABB::AABB(Vec3Df& pos, float radius) : pos(pos), radius(radius), sub(0) {
}

AABB::~AABB() {
	// can safeley delete the null pointer
	delete[] sub;
}

int AABB::follow(Vec3Df& v) {
	int axis = 0;
	for (int a = 0; a < 3; a++) {
		if (v.p[a] > pos.p[a] + radius)
			axis |= 1 << a;
	}

	return axis;
}

void AABB::split() {
	// if already subdivided, return
	if (sub)
		return;

	// make leaves
	sub = new AABB*[8];
	for (int x = 0; x < 2; x++)
	for (int y = 0; y < 2; y++)
	for (int z = 0; z < 2; z++) {
		Vec3Df subpos(pos + Vec3Df(x * radius, y * radius, z * radius));
		float subradius = radius * 0.5f;
		
		sub[z * 4 + y * 2 + x] = new AABB(subpos, subradius);
	}
}

Triangle* AABB::collide(Ray& ray) {
	// check x plane
	float x = pos.p[0];
	float a = (x - ray.orig.p[0]) / ray.dir.p[0];

	float y = a * ray.dir.p[1] + ray.orig.p[1];
	float z = a * ray.dir.p[2] + ray.orig.p[2];

	if (pos.p[1] < y && y < pos.p[1] + 2 * radius)
		if (pos.p[2] < z && y < pos.p[2] + 2 * radius)
			printf("erin\n");

	printf("(%f, %f)\n", y, z);

	return 0;
}

void Tree::calcSize(Mesh& mesh) {
	Vec3Df p1; // min
	Vec3Df p2; // max

	for (unsigned int i = 0; i < mesh.triangles.size(); i++) { // triangle
		for (int v = 0; v < 3; v++) { // vertex
			for (int d = 0; d < 3; d++) { // dimension
				const float dim = mesh.triangles[i].vertices[v].p.p[d];
				if (dim < p1.p[d])
					p1.p[d] = dim;
				if (dim > p2.p[d])
					p2.p[d] = dim;
			}
		}
	}
	printf("min: (%.1f,%.1f,%.1f)\n", p1.p[0], p1.p[1], p1.p[2]);
	printf("max: (%.1f,%.1f,%.1f)\n", p2.p[0], p2.p[1], p2.p[2]);

	Vec3Df dim = p2 - p1;
	float sdim = dim.p[0];
	sdim = max(sdim, dim.p[1]);
	sdim = max(sdim, dim.p[2]);

	root = new AABB(p1, sdim * 0.5f);
}

void Tree::build(Mesh& mesh) {
	printf("Building tree!\n");
	calcSize(mesh);

	for (unsigned int i = 0; i < mesh.triangles.size(); i++)
		add(mesh.triangles[i]);

	//Ray ray = Ray(Vec3Df(0, 0, 0), Vec3Df(-2, -5, 0), Vec3Df(-1, 0, 0));
	//Triangle* tr = root->collide(ray);
}

void Tree::add(Triangle& tr) {
	bool same = true;
	AABB* current = root;
	int a0, a1, a2;

	int depth = 0;
	while (depth < MAX_DEPTH) {
		a0 = current->follow(tr.vertices[0].p);
		a1 = current->follow(tr.vertices[1].p);
		a2 = current->follow(tr.vertices[2].p);

		printf("follow: %d, %d, %d\n", a0, a1, a2);

		if (a0 != a1 || a1 != a2)
			break;

		current->split();
		current = current->sub[a0];
		depth++;
	}

	printf("Depth: %d\n\n", depth);

	current->leaves.push_back(&tr);
}