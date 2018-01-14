#include "../headers/model.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

Model::Model(const char* file_name)
{
	this->file_name = strdup(file_name);
	this->materials_name = NULL;
	use_materials = false;
	material_file_read = false;
	file_read = false;
	loaded = false;
}

Model::Model(const char* file_name, const char* material_file_name)
{
	this->materials_name = strdup(material_file_name);
	this->file_name = strdup(file_name);
	use_materials = true;
	material_file_read = false;
	file_read = false;
	loaded = false;
}

Model::~Model()
{
	if (file_name)
		free(file_name);

	if (materials_name)
		free(materials_name);

	for (Material m : materials) {
		if (m.name)
			free(m.name);
	}

	glDeleteLists(display_list, 1);
}

void Model::read_materials()
{
	if (!use_materials || material_file_read) return;

	FILE* f = fopen(materials_name, "r");
	if (f == NULL)
		return;

    char * line = NULL;
    size_t len = 0;
    ssize_t read;
	Material m;

	int counter = 0;

    while ((read = getline(&line, &len, f)) != -1) {

		char* word_1 = (char*)malloc(len); 
		char* word_2 = (char*)malloc(len); 

		sscanf(line, "%s %s", word_1, word_2);
		if (strcmp(word_1, "newmtl")==0) {
			m.name = strdup(word_2);
			counter++;
		}

		if (word_1)
			free(word_1);
		if (word_2)
			free(word_2);


		if (len > 1 && line[0] == 'N' && line[1] == 's') {
			sscanf(line, "Ns %f", &m.shininess);
			counter++;
		}

		if (len > 1 && line[0] == 'K' && line[1] == 'a') {
			sscanf(line, "Ka %f %f %f", &m.ambient.x, &m.ambient.y, &m.ambient.z);
			counter++;
		}

		if (len > 1 && line[0] == 'K' && line[1] == 'd') {
			sscanf(line, "Kd %f %f %f", &m.diffuse.x, &m.diffuse.y, &m.diffuse.z);
			counter++;
		}

		if (len > 1 && line[0] == 'K' && line[1] == 's') {
			sscanf(line, "Ks %f %f %f", &m.specular.x, &m.specular.y, &m.specular.z);
			counter++;
		}

		if (len > 1 && line[0] == 'K' && line[1] == 'e') {
			sscanf(line, "Ke %f %f %f", &m.emission.x, &m.emission.y, &m.emission.z);
			counter++;
		}

		if (counter == 6) {
			materials.push_back(m);
			counter = 0;
		}

    }

    if (line) {
		free(line);
	}
	
	fclose(f);

	material_file_read = true;
}

void Model::read_file()
{
	if (file_read)
		return;

	if (use_materials && !material_file_read)
		read_materials();

	FILE* f = fopen(file_name, "r");
	if (f == NULL)
		return;

    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, f)) != -1) {
		if (line[0] == 'v') {
			if (line[1] == 'n') {
				Normal normal;
				sscanf(line, "vn %f %f %f",&normal.x,&normal.y,&normal.z);
				normals.push_back(normal);
			} else {
				Vertex ver;
				sscanf(line, "v %f %f %f",&ver.x,&ver.y,&ver.z);
				vertices.push_back(ver);
			}
		}
		else if (line[0]=='f') {
			Triangle t;
			sscanf(line, "f %d//%d %d//%d %d//%d", &t.vertex_index[0], &t.normal_index[0], &t.vertex_index[1], &t.normal_index[1], &t.vertex_index[2], &t.normal_index[2]);
			triangles.push_back(t);
		}
		else if (use_materials) {
			char* word_1 = (char*)malloc(len); 
			char* word_2 = (char*)malloc(len); 

			sscanf(line, "%s %s", word_1, word_2);
			if (strcmp(word_1, "usemtl")==0) {
				for (int i = 0; i < (int)materials.size(); i++) {
					if (strcmp(word_2, materials[i].name) == 0)
						materials_index.push_back(std::make_pair(triangles.size(), i));
				}
			}

			if (word_1)
				free(word_1);
			if (word_2)
				free(word_2);
		}
    }

    if (line) {
		free(line);
	}
	
	fclose(f);

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

