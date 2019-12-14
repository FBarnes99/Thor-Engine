#include "R_Shader.h"

#include "Globals.h"

#include "Glew/include/glew.h"

R_Shader::R_Shader() : Resource(Resource::SHADER)
{
	memset(shaderObjects, 0, (int)Object_Type::Unknown - 1);
}

R_Shader::~R_Shader()
{

}

int R_Shader::Save(char** buffer)
{
	int size = 0;

	return size;
}

bool R_Shader::LoadFromText(const char* buffer)
{
	//TODO: this seems like a rather slow process, measure it and (maybe) optimize
	bool ret = false;
	std::string file(buffer);
	for (uint i = 0; i < (int)Object_Type::Unknown; ++i)
	{
		//Searching for each shader object macro type. If found, compile it
		std::string macroStr = GetShaderMacroStr((Object_Type)i);
		if (file.find(macroStr) != std::string::npos)
		{
			if (int GLmacro = GetShaderMacro((Object_Type)i))
			{
				//Compile the shader, type and define are ready
				std::string define = "#version 330 core\n"; //TODO: temporal to make it work
				define += std::string("#define ") + macroStr + "\n";

				std::string shaderObjectFile = define + file;

				shaderObjects[i] = glCreateShader(GLmacro);
				const char* str = shaderObjectFile.c_str();
				glShaderSource(shaderObjects[i], 1, &str, nullptr);
				glCompileShader(shaderObjects[i]);

				//Check for compiling errors, display message
				int success;
				glGetShaderiv(shaderObjects[i], GL_COMPILE_STATUS, &success);
				if (success == 0)
				{
					char str[512];
					glGetShaderInfoLog(shaderObjects[i], 512, nullptr, str);
					LOG("Shader Compilation Error: %s", str);
					DeleteShaderObject(i);
				}
				else ret = true;
			}
		}
	}
	return ret;
}

bool R_Shader::LoadFromBinary(const char* buffer)
{
	bool ret = true;

	return ret;
}

bool R_Shader::Link()
{
	int ret = 0;
	
	//TODO: delete existing program if any
	shaderProgram = glCreateProgram();

	for (uint i = 0; i < (int)Object_Type::Unknown; ++i)
		if (shaderObjects[i] != 0) glAttachShader(shaderProgram, shaderObjects[i]);

	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &ret);
	if (ret == 0)
	{
		char str[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, str);
		
		LOG("Shader Linking Error %s", str);
	}

	return ret == 1;
}

void R_Shader::DeleteShaderObject(int index)
{
	if (index < (int)Object_Type::Unknown)
	{
		glDeleteShader(shaderObjects[index]);
		shaderObjects[index] = 0;
	}
}

std::string R_Shader::GetShaderMacroStr(Object_Type type)
{
	switch (type)
	{
		case(Object_Type::Vertex): return "__COMPILE_VERTEX__";
		case(Object_Type::Fragment): return "__COMPILE_FRAGMENT__";
	}
	return "";
}

int R_Shader::GetShaderMacro(Object_Type type)
{
	switch (type)
	{
	case(Object_Type::Vertex): return GL_VERTEX_SHADER;
	case(Object_Type::Fragment): return GL_FRAGMENT_SHADER;
	}
	return 0;
}