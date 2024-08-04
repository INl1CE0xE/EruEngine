#pragma once

#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


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

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); // ‘рагментный шейдер
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
		int uformlocation = glGetUniformLocation(ID, name.c_str());
		glUniform1f(uformlocation, value);
	}

private:
	void check_failure(GLuint shader, std::string str_shader) {
		int success; // лог дл€ проверки шейдеров на компил€цию
		char infolog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infolog);
			std::cout << "Error::Shader:"<< str_shader << ":compilation_failed\n" << infolog << std::endl;
		}
	}
};