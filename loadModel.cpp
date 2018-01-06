#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <GL/glut.h>

static bool brick_loaded = false;
static bool wheel_loaded = false;

struct vert {
	float x, y, z;
};

struct vn {
	float x, y, z;
};

struct triangle {
	int v1,n1;
	int v2,n2;
	int v3,n3;
};

std::vector<vert> vertex;
std::vector<vn> normals;
std::vector<triangle> triangles;

std::vector<vert> wheel_vertex;
std::vector<vn> wheel_normals;
std::vector<triangle> wheel_triangles;

void read_file2(char* file) {
	FILE* f = fopen(file, "r");
	if (f==NULL)
		return;

    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, f)) != -1) {
		if (line[0] == 'v') {
			if (line[1] == 'n') {
				vn normal;
				sscanf(line, "vn %f %f %f",&normal.x,&normal.y,&normal.z);
				normals.push_back(normal);
			} else {
				vert ver;
				sscanf(line, "v %f %f %f",&ver.x,&ver.y,&ver.z);
				vertex.push_back(ver);
			}
		}

		if (line[0]=='f') {
			triangle t;
			sscanf(line, "f %d//%d %d//%d %d//%d", &t.v1, &t.n1, &t.v2, &t.n2, &t.v3, &t.n3);
			triangles.push_back(t);
		}
    }

    if (line) {
		free(line);
	}
	

	fclose(f);
}


void read_file(char* file) {
	FILE* f = fopen(file, "r");
	if (f==NULL)
		return;

    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, f)) != -1) {
		if (line[0] == 'v') {
			if (line[1] == 'n') {
				vn normal;
				sscanf(line, "vn %f %f %f",&normal.x,&normal.y,&normal.z);
				wheel_normals.push_back(normal);
			} else {
				vert ver;
				sscanf(line, "v %f %f %f",&ver.x,&ver.y,&ver.z);
				wheel_vertex.push_back(ver);
			}
		}

		if (line[0]=='f') {
			triangle t;
			sscanf(line, "f %d//%d %d//%d %d//%d", &t.v1, &t.n1, &t.v2, &t.n2, &t.v3, &t.n3);
			wheel_triangles.push_back(t);
		}
    }

    if (line) {
		free(line);
	}

	fclose(f);
}

static GLuint legoList;
static GLuint wheelList;

void free_display_lists() {
	glDeleteLists(legoList, 1);
	glDeleteLists(wheelList, 1);
}

void render() {
		glBegin(GL_TRIANGLES);
			for (triangle i : triangles) {
				glNormal3f(normals[i.n1-1].x,normals[i.n1-1].y,normals[i.n1-1].z);
				glVertex3f(vertex[i.v1-1].x,vertex[i.v1-1].y,vertex[i.v1-1].z);
				glNormal3f(normals[i.n2-1].x,normals[i.n2-1].y,normals[i.n2-1].z);
				glVertex3f(vertex[i.v2-1].x,vertex[i.v2-1].y,vertex[i.v2-1].z);
				glNormal3f(normals[i.n3-1].x,normals[i.n3-1].y,normals[i.n3-1].z);
				glVertex3f(vertex[i.v3-1].x,vertex[i.v3-1].y,vertex[i.v3-1].z);
			}
		glEnd();
}

void renderModel() {
	if (!brick_loaded) {
		char file[] = "legoCylinder.obj";
		read_file2(file);
		brick_loaded = true;

		legoList = glGenLists(1);
		glNewList(legoList, GL_COMPILE);
			render();
		glEndList();
	}

	glCallList(legoList);
}

void renderWheel() {
	if (!wheel_loaded) {
		char file[] = "wheel.obj";
		read_file(file);
		wheel_loaded = true;


		wheelList = glGenLists(1);
		glNewList(wheelList, GL_COMPILE);
		GLfloat Ka[] = {0.059701, 0.059701, 0.059701,1};
		GLfloat Kd[] = {0.651223, 0.643198, 0.595625,1};
		GLfloat Ks[] = {0.500000, 0.500000, 0.500000,1};
		GLfloat Ke[] = {0.000000, 0.000000, 0.000000,1};
		glMaterialfv(GL_FRONT, GL_AMBIENT,Ka);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,Kd);
		glMaterialfv(GL_FRONT, GL_SPECULAR,Ks);
		glMaterialfv(GL_FRONT, GL_EMISSION,Ke);
		glMaterialf(GL_FRONT, GL_SHININESS,96.078431);
		glBegin(GL_TRIANGLES);
			int counter = 0;
			for (triangle i : wheel_triangles) {
				if (counter == 2885) {
					GLfloat Ka[] = {0.10, 0.10, 0.10,1};
					GLfloat Kd[] = {0.011954, 0.011954, 0.011954,1};
					GLfloat Ks[] = {0.278796, 0.278796, 0.278796,1};
					GLfloat Ke[] = {0.000000, 0.000000, 0.000000,1};
					glMaterialfv(GL_FRONT, GL_AMBIENT,Ka);
					glMaterialfv(GL_FRONT, GL_DIFFUSE,Kd);
					glMaterialfv(GL_FRONT, GL_SPECULAR,Ks);
					glMaterialfv(GL_FRONT, GL_EMISSION,Ke);
					glMaterialf(GL_FRONT, GL_SHININESS,66.66667);
				}
				glNormal3f(wheel_normals[i.n1-1].x,wheel_normals[i.n1-1].y,wheel_normals[i.n1-1].z);
				glVertex3f(wheel_vertex[i.v1-1].x,wheel_vertex[i.v1-1].y,wheel_vertex[i.v1-1].z);
				glNormal3f(wheel_normals[i.n2-1].x,wheel_normals[i.n2-1].y,wheel_normals[i.n2-1].z);
				glVertex3f(wheel_vertex[i.v2-1].x,wheel_vertex[i.v2-1].y,wheel_vertex[i.v2-1].z);
				glNormal3f(wheel_normals[i.n3-1].x,wheel_normals[i.n3-1].y,wheel_normals[i.n3-1].z);
				glVertex3f(wheel_vertex[i.v3-1].x,wheel_vertex[i.v3-1].y,wheel_vertex[i.v3-1].z);
				counter++;
			}
		glEnd();
		glEndList();
	}

	glCallList(wheelList);
}
