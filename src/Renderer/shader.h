#pragma once

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "external/glm/glm.hpp" //glm GL Math
#include "external/glm/gtc/matrix_transform.hpp"
#include "external/glm/gtc/type_ptr.hpp"

struct Shader {
	
	unsigned int Shader::ID ;

	struct Shader_files {
		std::ifstream inshader;
		std::string shader;
	};
	
	Shader(const char* vertexPath, const char* fragmentPath) {
		std::string line;
		Shader_files Vshader, Fshader;

		Vshader.inshader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		Fshader.inshader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try {
			Vshader.inshader.open(vertexPath);
			Fshader.inshader.open(fragmentPath);

			std::stringstream vShaderStream, fShaderStream;
			
			vShaderStream << Vshader.inshader.rdbuf();
			fShaderStream << Fshader.inshader.rdbuf();

			Vshader.inshader.close();
			Fshader.inshader.close();

			Vshader.shader = vShaderStream.str();
			Fshader.shader = fShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << " " << e.what() << std::endl;
		}

		const char* vertex_shader_code = Vshader.shader.c_str();
		const char* fragment_shader_code = Fshader.shader.c_str();
	
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertex_shader_code, NULL);
		glCompileShader(vertexShader);
		check_failure(vertexShader, "vertex");

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // ����������� ������
		glShaderSource(fragmentShader, 1, &fragment_shader_code, NULL);
		glCompileShader(fragmentShader);
		check_failure(fragmentShader, "fragment");

		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}
	
	void use() {
		glUseProgram(ID);
	};

	void setfloat(const std::string &name, float value) {
		unsigned int uformlocation = glGetUniformLocation(ID, name.c_str());
		glUniform1f(uformlocation, value);
	}

	void setmat4(const std::string& name, glm::mat4 trans) {
		unsigned int transformLoc = glGetUniformLocation(ID, name.c_str());
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
	}

private:
	void check_failure(GLuint shader, std::string str_shader) {
		int success; // ��� ��� �������� �������� �� ����������
		char infolog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infolog);
			std::cout << "Error::Shader:"<< str_shader << ":compilation_failed\n" << infolog << std::endl;
		}
	}
};