# include <Siv3D.hpp> // OpenSiv3D v0.6.3
using namespace System;
using namespace SimpleGUI;

// 1 cell => 1bit
struct Cell
{
	bool previous : 1 = 0;
	bool current : 1 = 0;
};

void RandomInit(Grid<Cell>& world, double rate) {
	world.fill(Cell{});

	for (auto y : Range(1, world.height() - 2))
	{
		for (auto x : Range(1, world.width() - 2)) {
			world[y][x] = Cell{0, RandomBool(rate)};
		}
	}
}

void UpdateWorld(Grid<Cell>& world, int32 gameRule) {
	for (auto& cell : world) {
		cell.previous = cell.current;
	}

	for (auto y : Range(1, world.height() - 2)) {
		for (auto x : Range(1, world.width() - 2)) {
			const int32 pre = world[y][x].previous;

			int32 cnt = 0;
			cnt += world[y - 1][x - 1].previous;
			cnt += world[y - 1][x].previous;
			cnt += world[y - 1][x + 1].previous;
			cnt += world[y][x - 1].previous;
			cnt += world[y][x + 1].previous;
			cnt += world[y + 1][x - 1].previous;
			cnt += world[y + 1][x].previous;
			cnt += world[y + 1][x + 1].previous;

			/*  About the game rule of Life Game
			*
			* Two variables "B" and "S" are defined like this:
			* B = Number of surrounding cells required for birth
			* S = Number of surrounding cells required for survival
			*
			* So, "B3/S23" means that B = 3 and S = 2, 3.
			*
			* <Game rule>
			* 1 : B3/S23
			*/

			if (gameRule == 1) {
				if (pre == 0) {
					if (cnt == 3) world[y][x].current = 1;
					else world[y][x].current = 0;
				}
				else if (pre == 1) {
					if (cnt == 2 || cnt == 3) world[y][x].current = 1;
					else world[y][x].current = 0;
				}
			}

			else if (gameRule == 2) {
				if (pre == 0) {
					if (cnt == 3) world[y][x].current = 1;
					else world[y][x].current = 0;
				}
				else if (pre == 1) {
					if (cnt == 2 || cnt == 3) world[y][x].current = 1;
					else world[y][x].current = 0;
				}
			}

			else if (gameRule == 3) {
				if (pre == 0) {
					if (cnt == 3) world[y][x].current = 1;
					else world[y][x].current = 0;
				}
				else if (pre == 1) {
					if (cnt == 2 || cnt == 3) world[y][x].current = 1;
					else world[y][x].current = 0;
				}
			}

			else if (gameRule == 4) {
				if (pre == 0) {
					if (cnt == 3) world[y][x].current = 1;
					else world[y][x].current = 0;
				}
				else if (pre == 1) {
					if (cnt == 2 || cnt == 3) world[y][x].current = 1;
					else world[y][x].current = 0;
				}
			}
		}
	}
}

void CopyToImg(const Grid<Cell>& world, Image& img) {
	for (auto y : step(img.height())) {
		for (auto x : step(img.width())) {
			img[y][x] = world[y + 1][x + 1].current ? Color{ 0, 255, 0 } : Palette::Black;

		}
	}
}

void Main() {
	// JSON
	const JSON settings = JSON::Load(U"config/settings.json");
	if (not settings) {
		throw Error{U"Failed to load `settings.json`."};
	}

	// ===== JSON Input =====
	// World size
	const int32 width = settings[U"WorldSize"][U"width"].get<int32>();
	const int32 height = settings[U"WorldSize"][U"height"].get<int32>();

	// Update speed
	double speed = settings[U"DefaultSpeed"].get<double>();

	// Game rule
	const int32 gameRule = settings[U"GameRule"].get<int32>();

	// Cell density
	const double density = settings[U"Density"].get<double>();

	// ======================


	// World data
	Grid<Cell> world(width + 2, height + 2, Cell{ 0,0 });

	// For visualization
	Image img{(uint32)width, (uint32)height, Palette::Black};

	// Texture
	DynamicTexture texture{img};

	// Time
	Stopwatch stopwatch{StartImmediately::Yes};

	// Do auto play?
	bool autoPlay = false;

	// Update img?
	bool imgUpdate = false;

	SetTerminationTriggers(UserAction::CloseButtonClicked);
	while (Update())
	{
		// When Button "Set Random" was pushed
		if (ButtonAt(U"Set Random", Vec2{ 700, 40 }, 170, !autoPlay)) {
			RandomInit(world, density);
			imgUpdate = true;
		}

		// When Button "Clear" was pushed
		if (ButtonAt(U"Clear", Vec2{ 700, 80 }, 170, !autoPlay)) {
			world.fill({ 0, 0 });
			imgUpdate = true;
		}

		// When Button "Run / Pause" was pushed
		if (ButtonAt(autoPlay ? U"Pause" : U"Run", Vec2{700, 160}, 170)) {
			autoPlay = !autoPlay;
		}

		// Update speed Slider
		SliderAt(U"Speed", speed, 1.0, 0.1, Vec2{700, 200}, 70, 100);

		// When Button "Go to next step" was pushed
		// Checking update
		if (ButtonAt(U"Go to next step", Vec2{700, 240}, 170, !autoPlay)
			|| (autoPlay && stopwatch.sF() >= (speed * speed)))
		{
			UpdateWorld(world, gameRule);
			imgUpdate = true;
			stopwatch.restart();
		}

		// Edit cell on the field
		if (!autoPlay && Rect{ 0, 0, 599 }.mouseOver()) {
			const Point MousePos = Cursor::Pos() / 10 + Point{1, 1};

			if (MouseL.pressed()) {
				world[MousePos].current = true;
				imgUpdate = true;
			}
			else if (MouseR.pressed()) {
				world[MousePos].current = false;
				imgUpdate = true;
			}
		}

		// Update image
		if (imgUpdate) {
			CopyToImg(world, img);
			texture.fill(img);
			imgUpdate = false;
		}
		ScopedRenderStates2D sampler{ SamplerState::ClampNearest };
		texture.scaled(10).draw();

		// Draw grid
		for (auto i : step(61))
		{
			Rect{ 0, i * 10, 600, 1 }.draw(ColorF{ 0.4 });
			Rect{ i * 10, 0, 1, 600 }.draw(ColorF{ 0.4 });
		}

		if (!autoPlay && Rect{ 0, 0, 599 }.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hidden);
			Rect{ Cursor::Pos() / 10 * 10, 10 }.draw(Palette::Orange);
		}

	}
}
