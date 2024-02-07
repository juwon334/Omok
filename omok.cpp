#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>

const int WIDTH = 800;
const int HEIGHT = 800;
const int GRID_SIZE = 19;
const float CELL_SIZE = 2.0f / (GRID_SIZE - 1);
std::vector<std::pair<std::pair<float, float>, bool>> stones; // 돌의 위치와 색상 저장 (OpenGL 좌표계, 흑돌 여부)
bool isBlackTurn = true; // 흑돌 차례인지 여부

void drawBoard() {
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f); // 검은색 선

	for (int i = 0; i < GRID_SIZE; ++i) {
		float pos = -1.0f + i * CELL_SIZE;
		glVertex2f(pos, -1.0f);
		glVertex2f(pos, 1.0f);
		glVertex2f(-1.0f, pos);
		glVertex2f(1.0f, pos);
	}

	glEnd();
}

void drawStones() {
	for (const auto& stone : stones) {
		glBegin(GL_POLYGON);
		glColor3f(stone.second ? 0.0f : 1.0f, stone.second ? 0.0f : 1.0f, stone.second ? 0.0f : 1.0f); // 흑돌이면 검은색, 아니면 흰색
		for (int i = 0; i < 360; i++) {
			float degInRad = i * M_PI / 180.0f;
			glVertex2f(cos(degInRad) * 0.03f + stone.first.first, sin(degInRad) * 0.03f + stone.first.second);
		}
		glEnd();
	}
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		// 화면 좌표계에서 OpenGL 좌표계로 변환
		float x = (xpos / WIDTH) * 2.0f - 1.0f;
		float y = (ypos / HEIGHT) * -2.0f + 1.0f;

		// OpenGL 좌표를 격자 좌표로 변환 (가장 가까운 격자 위치 찾기)
		x = round((x + 1.0f) / CELL_SIZE) * CELL_SIZE - 1.0f;
		y = round((y + 1.0f) / CELL_SIZE) * CELL_SIZE - 1.0f;

		stones.push_back({{x, y}, isBlackTurn}); // 돌의 위치와 색상 저장
		isBlackTurn = !isBlackTurn; // 차례 변경
	}
}

int main(void) {
	GLFWwindow* window;

	if (!glfwInit()) {
		std::cerr << "GLFW 초기화 실패" << std::endl;
		return -1;
	}

	window = glfwCreateWindow(WIDTH, HEIGHT, "오목 게임", NULL, NULL);
	if (!window) {
		glfwTerminate();
		std::cerr << "GLFW 윈도우 생성 실패" << std::endl;
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);

	// 배경색을 흰색으로 설정
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		drawBoard();
		drawStones();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

