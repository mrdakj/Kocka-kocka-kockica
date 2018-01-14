#ifndef _MODEL_H_
#define _MODEL_H_

#include <GL/glut.h>
#include <vector>

struct Float_3 {
	float x, y, z;
};

typedef Float_3 Vertex;
typedef Float_3 Normal;

struct Triangle {
	int vertex_index[3];
	int normal_index[3];
};

struct Material {
	float shininess;
	Float_3 ambient;
	Float_3 diffuse;
	Float_3 specular;
	Float_3 emission;
	char* name;
};

class Model {

public:
	Model(const char* file_name, const char* material_file_name);
	Model(const char* file_name);
	~Model();

	void render() const;
	void load();
	
private:
	char* file_name;
	char* materials_name;

	bool loaded;
	bool file_read;
	bool material_file_read;
	bool use_materials;

	std::vector<Vertex> vertices;
	std::vector<Normal> normals;
	std::vector<Triangle> triangles;
	std::vector<Material> materials;
	std::vector<std::pair<int, int>> materials_index;

	GLuint display_list;

	void read_file();
	void read_materials();
	bool make_display_list();
};

#endif
