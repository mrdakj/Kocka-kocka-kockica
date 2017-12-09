#include <stdio.h>
#include <vector>
#include <cstdlib>
#include <GL/glut.h>

static bool loaded = false;

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

    if (line)
        free(line);

	fclose(f);
}

void renderModel() {
	if (!loaded) {
		char file[] = "lego3x2x1.obj";
		read_file2(file);
		loaded = true;
	}
	
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
