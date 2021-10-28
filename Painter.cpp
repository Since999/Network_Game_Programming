#include "Painter.h"

void drawLand(Shader& s, Character& chr)
{
	float cube[] =
	{
		// 상
		-1.0,  1.0, -1.0,
		1.0,  1.0, -1.0,
		1.0,  1.0,  1.0,
		1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		-1.0,  1.0, -1.0,

		// 하
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0, -1.0,  1.0,
		1.0, -1.0,  1.0,
		-1.0, -1.0,  1.0,
		-1.0, -1.0, -1.0,

		// 앞
		-1.0, -1.0,  1.0,
		1.0, -1.0,  1.0,
		1.0,  1.0,  1.0,
		1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		-1.0, -1.0,  1.0,

		// 뒤
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0,  1.0, -1.0,
		1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0,
		-1.0, -1.0, -1.0,

		// 좌
		-1.0,  1.0,  1.0,
		-1.0,  1.0, -1.0,
		-1.0, -1.0, -1.0,
		-1.0, -1.0, -1.0,
		-1.0, -1.0,  1.0,
		-1.0,  1.0,  1.0,

		// 우
		1.0,  1.0,  1.0,
		1.0,  1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0, -1.0,  1.0,
		1.0,  1.0,  1.0
	};

	float color[] =
	{
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,

		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,

		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,

		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,

		0.1, 0.1, 0.1,
		0.1, 0.1, 0.1,
		0.1, 0.1, 0.1,
		0.1, 0.1, 0.1,
		0.1, 0.1, 0.1,
		0.1, 0.1, 0.1,

		0.1, 0.1, 0.1,
		0.1, 0.1, 0.1,
		0.1, 0.1, 0.1,
		0.1, 0.1, 0.1,
		0.1, 0.1, 0.1,
		0.1, 0.1, 0.1,
	};

	std::vector<glm::vec3> pos, rgb;
	for (int i = 0; i < sizeof(cube) / sizeof(cube[0]); i += 3)
	{
		glm::vec3 p = { cube[i], cube[i + 1], cube[i + 2] };
		pos.push_back(p);
	}
	for (int i = 0; i < sizeof(color) / sizeof(color[0]); i += 3)
	{
		glm::vec3 c = { color[i], color[i + 1], color[i + 2] };
		rgb.push_back(c);
	}

	glUseProgram(s.pid);

	glm::mat4 s0 = glm::scale(glm::mat4(1.0f), glm::vec3(1, 0.1, 50));
	glm::mat4 t0 = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, chr.getPos().z));
	GLuint model_matrix_location = glGetUniformLocation(s.pid, "model");
	glUniformMatrix4fv(model_matrix_location, 1, GL_FALSE, glm::value_ptr(t0 * s0));
	s.setBufferData(pos, rgb); glDrawArrays(GL_TRIANGLES, 0, 36);

	glUseProgram(0);
}

void drawMap(Shader& s, Camera& c, std::vector<Obstacles>& obs, std::vector<Item>& item, Character& chr)
{
	glViewport(0, 0, 800, 600);
	chr.setCameraViewMatrix(c, s.pid);

	drawLand(s, chr);
	chr.draw(s, c);
	for (auto& i : obs)
	{
		// 캐릭터와 멀리 떨어져 있는 것은 그리지않는다.
		float z;
		if (i.cube != nullptr) z = i.cube->pos.z;
		else if (i.hCube != nullptr) z = i.hCube->pos.z;
		else if (i.vCube != nullptr) z = i.vCube->pos.z;
		if (abs(chr.getPos().z - z) > 20)
			continue;

		i.draw(s);
	}
	for (auto& i : item)
	{
		if (i.heal != nullptr &&
			abs(chr.getPos().z - i.heal->pos.z) <= 20 )
			i.heal->draw(s);
	}
}

void drawMiniMab(Shader& s, Camera& c, std::vector<Obstacles>& obs, std::vector<Item>& item, Character& chr)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(800 - 160, 600 - 120, 160, 120);
	chr.setTopCameraViewMatrix(c, s.pid);

	drawLand(s, chr);
	chr.draw(s, c);
	for (auto& i : obs)
	{
		float z;
		if (i.cube != nullptr) z = i.cube->pos.z;
		else if (i.hCube != nullptr) z = i.hCube->pos.z;
		else if (i.vCube != nullptr) z = i.vCube->pos.z;
		if (abs(chr.getPos().z - z) > 20)
			continue;

		i.draw(s);
	}
	for (auto& i : item)
	{
		if (i.heal != nullptr &&
			abs(chr.getPos().z - i.heal->pos.z) <= 20)
			i.heal->draw(s);
	}
}

void drawMiniMabEdge(Shader& s)
{
	std::vector<glm::vec3> pos, color;
	float _pos[] =
	{
		0.6, 1.0, 1.0,
		1.0, 1.0, 1.0,

		1.0, 1.0, 1.0,
		1.0, 0.6, 1.0,

		0.6, 0.6, 1.0,
		1.0, 0.6, 1.0,

		0.6, 1.0, 1.0,
		0.6, 0.6, 1.0,
	};

	float _color[] =
	{
		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,

		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,

		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,

		1.0, 1.0, 1.0,
		1.0, 1.0, 1.0,
	};

	for (int i = 0; i <24; i += 3)
	{
		pos.push_back(glm::vec3(_pos[i], _pos[i + 1], _pos[i + 2]));
		color.push_back(glm::vec3(_color[i], _color[i + 1], _color[i + 2]));
	}

	glViewport(0, 0, 800, 600);

	glUseProgram(s.pid);

	s.setBufferData(pos, color);
	glDrawArrays(GL_LINES, 0, 8);

	glUseProgram(0);
}

void drawMiniMabBGR(Shader& s)
{
	glViewport(800 - 160, 600 - 120, 160, 120);

	// 배경 검은색
	std::vector<glm::vec3> pos, color;
	float _pos[] =
	{
		1, 1, 1,
		-1, 1, 1,
		1, -1, 1,

		-1, 1, 1,
		-1, -1, 1,
		1, -1, 1
	};

	float _color[] =
	{
		0, 0, 0,
		0, 0, 0,
		0, 0, 0,

		0, 0, 0,
		0, 0, 0,
		0, 0, 0,
	};

	for (int i = 0; i < 18; i += 3)
	{
		pos.push_back(glm::vec3(_pos[i], _pos[i + 1], _pos[i + 2]));
		color.push_back(glm::vec3(_color[i], _color[i + 1], _color[i + 2]));
		
	}


	glUseProgram(s.pid);
	s.setBufferData(pos, color);
	glDrawArrays(GL_TRIANGLES, 0,6);
	glUseProgram(0);
}

void drawHPBar(Shader& s, int hp)
{
	std::vector<glm::vec3> pos, color;

	float _pos[] =
	{
		 0.5, 0.90, 1.0,
		-0.5, 0.90, 1.0,
		 0.5, 0.85, 1.0,

		-0.5, 0.90, 1.0,
		-0.5, 0.85, 1.0,
		 0.5, 0.85, 1.0,
	};

	float __pos[] =
	{
		-0.5 + hp / 100.0, 0.90, 1.0,
		-0.5,			   0.90, 1.0,
		-0.5 + hp / 100.0, 0.85, 1.0,

		-0.5,			   0.90, 1.0,
		-0.5,			   0.85, 1.0,
		-0.5 + hp / 100.0, 0.85, 1.0,
	};

	float _color[] =
	{
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,

		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
		0.2, 0.2, 0.2,
	};

	float __color[] =
	{
		0.9, 0.1, 0.1,
		0.9, 0.1, 0.1,
		0.9, 0.1, 0.1,

		0.9, 0.1, 0.1,
		0.9, 0.1, 0.1,
		0.9, 0.1, 0.1,
	};

	glViewport(0, 0, 800, 600);

	// 바탕
	for (int i = 0; i < 18; i += 3)
	{
		pos.push_back(glm::vec3(_pos[i], _pos[i + 1], _pos[i + 2]));
		color.push_back(glm::vec3(_color[i], _color[i + 1], _color[i + 2]));
	}

	glUseProgram(s.pid);
	s.setBufferData(pos, color);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	pos.clear(); color.clear();

	// 체력바
	for (int i = 0; i < 18; i += 3)
	{
		pos.push_back(glm::vec3(__pos[i], __pos[i + 1], __pos[i + 2]));
		color.push_back(glm::vec3(__color[i], __color[i + 1], __color[i + 2]));
	}

	s.setBufferData(pos, color);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glUseProgram(0);
}

void drawHPBarEdge(Shader& s)
{
	glViewport(0, 0, 800, 600);

	std::vector<glm::vec3> pos, color;
	float _pos[] =
	{
		-0.5, 0.90, 1.0,
		 0.5, 0.90, 1.0,

		0.5, 0.90, 1.0,
		0.5, 0.85, 1.0,

		 0.5, 0.85, 1.0,
		-0.5, 0.85, 1.0,

		-0.5, 0.85, 1.0,
		-0.5, 0.90, 1.0
	};

	float _color[] =
	{
		1, 1, 1,
		1, 1, 1,

		1, 1, 1,
		1, 1, 1,

		1, 1, 1,
		1, 1, 1,

		1, 1, 1,
		1, 1, 1,
	};

	for (int i = 0; i < 24; i += 3)
	{
		pos.push_back(glm::vec3(_pos[i], _pos[i + 1], _pos[i + 2]));
		color.push_back(glm::vec3(_color[i], _color[i + 1], _color[i + 2]));
	}

	glUseProgram(s.pid);
	s.setBufferData(pos, color);
	glDrawArrays(GL_LINES, 0, 8);
	glUseProgram(0);
}