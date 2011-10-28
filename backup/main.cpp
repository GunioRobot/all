class Application {
	size_t ticks;
	bool running;
public:
	Application() : ticks(0), running(false) {

	}

	void run() {
		running = true;

		Timer framet;
		Accumulator fps(1.0), input(1.0 / 30.0), render(1.0 / 60.0);

		while (running) {
			framet.update();
			double dt = framet.delta();
			fps.add(dt); input.add(dt); render.add(dt);

			if (fps.ready()) {
				printf("%i\n", (int) (1.f / dt));
				fps.reset();
			}
			if (render.ready()) {
				render.reset();
			}
			if (input.ready()) {
				input.reset();
			}
			++ticks;
		}
	}
private:
	void onError(int error, const char* message) { printf("err %i %s\n", error, message); }
	void onKey(int key, bool state) {
		printf("ok %i %i\n", key, state);

		if (key == KEY_H && state) {
			static bool hidecursor = false;
			hidecursor = !hidecursor;
			window.hideCursor(hidecursor);
		}
		if (key == KEY_ESCAPE && state) {
			running = false;
		}
	}
	void onMouseClick(int index, bool state) { printf("omc %i %i\n", index, state); }
	void onMouseMove(int x, int y) { printf("omm %i %i\n", x, y); }
	void onMouseScroll(int x, int y) { printf("oms %i %i\n", x, y); }
	void onWindowClose() {
		printf("owc\n");
		running = false;
	}
	void onWindowResize(int new_width, int new_height) {
		printf("owr %i %i\n", new_width, new_height);
		//ogldemo.resize(new_width, new_height);
	}
};

int main(/*int argc, const char* argv[]*/) {
	Application app;
	app.run();

	return 0;
}
