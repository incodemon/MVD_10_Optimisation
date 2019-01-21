//
//  Copyright � 2018 Alun Evans. All rights reserved.
//
#pragma once
#include "includes.h"
#include "Shader.h"
#include "Components.h"
#include <unordered_map>

struct AABB {
	lm::vec3 center;
	lm::vec3 half_width;
};

struct Geometry {
    GLuint vao;
    GLuint num_tris;
	AABB aabb;
    Geometry() { vao = 0; num_tris = 0;}
    Geometry(int a_vao, int a_tris) : vao(a_vao), num_tris(a_tris) {}
};

struct Material {
    std::string name;
    int index = -1;
	int shader_id;
	lm::vec3 ambient;
    lm::vec3 diffuse;
    lm::vec3 specular;
    float specular_gloss;
    
    int diffuse_map;
    
    Material() {
        name = "";
		ambient = lm::vec3(0.1f, 0.1f, 0.1f);
        diffuse = lm::vec3(1.0f, 1.0f, 1.0f);
        specular = lm::vec3(1.0f, 1.0f, 1.0f);
        diffuse_map = -1;
        specular_gloss = 80.0f;
    }
};

class GraphicsSystem {
public:
	~GraphicsSystem();
    void init(int window_width, int window_height);
    void lateInit();
    void update(float dt);
    
	//viewport
	void updateMainViewport(int window_width, int window_height);

    //shader loader
	Shader* loadShader(std::string vs_path, std::string fs_path, bool compile_direct = false);

	//materials
    int createMaterial();
	Material& getMaterial(int mat_id) { return materials_.at(mat_id); }
    
    //geometry
    int createPlaneGeometry();
    int createGeometryFromFile(std::string filename);
    
private:
    //resources
	std::unordered_map<GLint, Shader*> shaders_; //compiled id, pointer
    std::vector<Geometry> geometries_;
    std::vector<Material> materials_;

	//shader stuff
	Shader* shader_ = nullptr; //current shader
	void useShader(Shader* s);
	void useShader(GLuint p);

	//materials stuff
    GLint current_material_ = -1;
    void setMaterialUniforms();

	//sorting and checking
	void sortMeshes_();
	void checkShaderAndMaterial(Mesh& mesh);
    
    //rendering
    void renderMeshComponent_(Mesh& comp);
    
	//AABB
	void setGeometryAABB_(Geometry& geom, std::vector<GLfloat>& vertices);
	bool BBInFrustum_(const AABB& aabb, const lm::mat4& model_view_projection);

    //create geometry buffers
    GLuint generateBuffers_(std::vector<float>& vertices,
                            std::vector<float>& uvs,
                            std::vector<float>& normals,
                            std::vector<unsigned int>& indices);
};
