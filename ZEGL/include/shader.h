#ifndef SHADER_H
#define SHADER_H

#include "mymath.h"
#include <string>
#include <vector>
#include <map>

class Camera;

class Shader
{
public:
	Shader(const std::string& fileName = "basic_shader");
	virtual ~Shader();

	void Load(const std::string& fileName);

	void Bind() const;
	void UnBind() const;

	void SetUniformi(const std::string& uniformName, int value) const;
	void SetUniformf(const std::string& uniformName, float value) const;
	void SetUniformVector3f(const std::string& uniformName, const Vector3f& value) const;
	void SetUniformMatrix4f(const std::string& uniformName, const Matrix4f& value) const;

	void UpdateUniforms(Camera& camera) const;

protected:
private:
	Shader(const Shader& other) = delete;
	Shader& operator=(Shader const&) = delete;

	void AddProgram(const std::string& text, int type);

	void AddAllAttributes(const std::string& vertexShaderText);
	void AddAllUniforms(const std::string& shaderText);
	void AddUniform(const std::string& uniformName, const std::string& uniformType);
	
	void CompileShader() const;

	std::string							m_fileName;
	int									m_program;
	std::vector<int>					m_shaders;
	std::vector<std::string>			m_uniformNames;
	std::vector<std::string>            m_uniformTypes;
	std::map<std::string, unsigned int> m_uniformMap;
};

#endif
