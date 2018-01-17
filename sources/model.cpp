#include "../headers/model.h"
#include <fstream>
#include <sstream>

Model::Model(std::string file_name) :
	file_name(file_name), materials_name(""),
	file_read(false), material_file_read(false),
	use_materials(false), loaded(false) {}

Model::Model(std::string file_name, std::string material_file_name) :
	file_name(file_name), materials_name(material_file_name),
	file_read(false), material_file_read(false),
	use_materials(true), loaded(false) {}

Model::~Model()
{
	glDeleteLists(display_list, 1);
}

void Model::read_materials()
{
	if (!use_materials || material_file_read) return;

	std::ifstream f(materials_name);

	if(!f)
		return;

	Material m;

	int counter = 0;
	std::string line;

    while (std::getline(f, line)) {

		if (line.length() == 0)
			continue;

		std::istringstream line_stream(line);

		std::string type;
		line_stream >> type;

		if (type == "newmtl") {
			line_stream >> m.name;
			counter++;
		}
		else if (type == "Ns") {
			line_stream >> m.shininess;
			counter++;
		}
		else if (type == "Ka") {
			line_stream >> m.ambient.x >> m.ambient.y >> m.ambient.z;
			counter++;
		}
		else if (type == "Kd") {
			line_stream >> m.diffuse.x >> m.diffuse.y >> m.diffuse.z;
			counter++;
		}
		else if (type == "Ks") {
			line_stream >> m.specular.x >> m.specular.y >> m.specular.z;
			counter++;
		}
		else if (type == "Ke") {
			line_stream >> m.emission.x >> m.emission.y >> m.emission.z;
			counter++;
		}

		if (counter == 6) {
			materials.push_back(m);
			counter = 0;
		}

    }

	f.close();

	material_file_read = true;
}

void Model::read_file()
{
	if (file_read)
		return;

	if (use_materials && !material_file_read)
		read_materials();

	std::ifstream f(file_name);

	if (!f)
		return;

	std::string line;

    while (std::getline(f, line)) {
		if (line.length() == 0)
			continue;
		std::istringstream line_stream(line);
		std::string type;
		line_stream >> type;

		if (type == "vn") {
			Normal normal;
			line_stream >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (type == "v") {
			Vertex ver;
			line_stream >> ver.x >> ver.y >> ver.z;
			vertices.push_back(ver);
		}
		else if (type == "f") {
			Triangle t;
			char slash;
			for (int i = 0; i < 3; i++) {
				line_stream >> t.vertex_index[i] >> slash >> slash >> t.normal_index[i];
			}
			triangles.push_back(t);
		}
		else if (use_materials && type == "usemtl") {
			std::string mat_name;
			line_stream >> mat_name;
			for (int i = 0; i < (int)materials.size(); i++) {
				if (mat_name == materials[i].name)
					materials_index.push_back(std::make_pair(triangles.size(), i));
			}
		}
	}

	f.close();
	
	file_read = true;
}

bool Model::make_display_list()
{
	display_list = glGenLists(1);

	if (!display_list)
		return false;

	glNewList(display_list, GL_COMPILE);

	glBegin(GL_TRIANGLES);
	int counter = 0;
	int current_material = 0;

	for (Triangle t : triangles) {

		/* set material */
		if (use_materials && counter == materials_index[current_material].first) {
			Material m =materials[materials_index[current_material].second];
			GLfloat Ka[] = {m.ambient.x, m.ambient.y, m.ambient.z,1};
			GLfloat Kd[] = {m.diffuse.x, m.diffuse.y, m.diffuse.z,1};
			GLfloat Ks[] = {m.specular.x, m.specular.y, m.specular.z,1};
			GLfloat Ke[] = {m.emission.x, m.emission.y, m.emission.z,1};
			glMaterialfv(GL_FRONT, GL_AMBIENT, Ka);
			glMaterialfv(GL_FRONT, GL_DIFFUSE,Kd);
			glMaterialfv(GL_FRONT, GL_SPECULAR,Ks);
			glMaterialfv(GL_FRONT, GL_EMISSION,Ke);
			glMaterialf(GL_FRONT, GL_SHININESS,m.shininess);
			current_material++;
		}

		for (int i = 0; i < 3; i++) {
			glNormal3f(normals[t.normal_index[i]-1].x,normals[t.normal_index[i]-1].y,normals[t.normal_index[i]-1].z);
			glVertex3f(vertices[t.vertex_index[i]-1].x,vertices[t.vertex_index[i]-1].y,vertices[t.vertex_index[i]-1].z);
		}

		counter++;
	}
	glEnd();

	glEndList();

	return true;
}

void Model::load()
{
	if (!loaded) {
		read_materials();
		read_file();
		loaded = make_display_list();
	}
}

void Model::render() const
{
	if (loaded)
		glCallList(display_list);
}
