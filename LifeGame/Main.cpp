# include <Siv3D.hpp> // OpenSiv3D v0.6.3
using namespace std;
using namespace System;
using namespace SimpleGUI;
using namespace Window;
using namespace Palette;


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
			for (int i = -1; i < 2; i++) {
				for (int j = -1; j < 2; j++) {
					if (i == 0 && j == 0) continue;

					cnt += world[y + i][x + j].previous;
				}
			}

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
			* 2 : B36/S23 (HighLife)
			* 3 : B34/S34
			* 4 : B3678/S34678 (Day and Night)
			* 5 : B1357/S1357 (Replicator)
			* 6 : B1357/S02468 (Replicator2 / Fredkin)
			* 7 : B36/S125 (2x2) 
			* 
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
					if (cnt == 3 || cnt == 6) world[y][x].current = 1;
					else world[y][x].current = 0;
				}
				else if (pre == 1) {
					if (cnt == 2 || cnt == 3) world[y][x].current = 1;
					else world[y][x].current = 0;
				}
			}

			else if (gameRule == 3) {
				if (pre == 0) {
					if (cnt == 3 || cnt == 4) world[y][x].current = 1;
					else world[y][x].current = 0;
				}
				else if (pre == 1) {
					if (cnt == 3 || cnt == 4) world[y][x].current = 1;
					else world[y][x].current = 0;
				}
			}

			else if (gameRule == 4) {
				if (pre == 0) {
					if (cnt == 3 || cnt == 6 || cnt == 7 || cnt == 8) world[y][x].current = 1;
					else world[y][x].current = 0;
				}
				else if (pre == 1) {
					if (cnt == 3 || cnt == 4 || cnt == 6 || cnt == 7 || cnt == 8) world[y][x].current = 1;
					else world[y][x].current = 0;
				}
			}

			else if (gameRule == 5) {
				if (pre == 0) {
					if (cnt == 1 || cnt == 3 || cnt == 5 || cnt == 7) world[y][x].current = 1;
					else world[y][x].current = 0;
				}
				else if (pre == 1) {
					if (cnt == 1 || cnt == 3 || cnt == 5 || cnt == 7) world[y][x].current = 1;
					else world[y][x].current = 0;
				}
			}

			else if (gameRule == 6) {
				if (pre == 0) {
					if (cnt == 1 || cnt == 3 || cnt == 5 || cnt == 7) world[y][x].current = 1;
					else world[y][x].current = 0;
				}
				else if (pre == 1) {
					if (cnt == 0 || cnt == 2 || cnt == 4 || cnt == 6 || cnt == 8) world[y][x].current = 1;
					else world[y][x].current = 0;
				}
			}

			else if (gameRule == 7) {
				if (pre == 0) {
					if (cnt == 3 || cnt == 6) world[y][x].current = 1;
					else world[y][x].current = 0;
				}
				else if (pre == 1) {
					if (cnt == 1 || cnt == 2 || cnt == 5) world[y][x].current = 1;
					else world[y][x].current = 0;
				}
			}
		}
	}
}

void CopyToImg(const Grid<Cell>& world, Image& img) {
	for (auto y : step(img.height())) {
		for (auto x : step(img.width())) {
			img[y][x] = world[y + 1][x + 1].current ? Color{ 0, 255, 0 } : Black;

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

	// Cell density
	double density = settings[U"DefaultDensity"].get<double>();

	// ======================

	// Game rule
	// const int32 gameRule = settings[U"GameRule"].get<int32>();
	size_t gameRuleIndex = 0;
	const Array<String> textOptions = { U"1 : B3/S23",
										U"2 : B36/S23",
										U"3 : B34/S34",
										U"4 : B3678/S34678",
										U"5 : B1357/S1357",
										U"6 : B1357/S02468",
										U"7 : B36/S125" };
	const Array<int32> gameRuleOptions = { 1, 2, 3, 4, 5, 6, 7 };
	int32 gameRule = gameRuleOptions[(int32)gameRuleIndex];


	// World data
	Grid<Cell> world(width + 2, height + 2, Cell{ 0,0 });

	// For visualization
	Image img{(uint32)width, (uint32)height, Black};

	// Texture
	DynamicTexture texture{img};

	// Time
	Stopwatch stopwatch{StartImmediately::Yes};

	// Do auto play?
	bool autoPlay = false;

	// Update img?
	bool imgUpdate = false;

	// Step counter
	int32 stepCounter = 0;

	SetTitle(U"Life Game");
	SetTerminationTriggers(UserAction::CloseButtonClicked);
	while (Update())
	{
		Resize(width * 10 + 280, (height >= 65 ? height * 10 : 650));

		// When Button "Set Random" was pushed
		if (ButtonAt(U"Set Random", Vec2{ width * 10 + 140, 40 }, 245, !autoPlay)) {
			RandomInit(world, density);
			stepCounter = 0;
			imgUpdate = true;
		}

		// When Button "Clear" was pushed
		if (ButtonAt(U"Clear", Vec2{ width * 10 + 140, 80 }, 245, !autoPlay)) {
			world.fill({ 0, 0 });
			stepCounter = 0;
			imgUpdate = true;
		}

		// Density Slider
		SliderAt(U"Density {:.1f}"_fmt((density - 0.1) / 0.8 * 100), density, 0.1, 0.9, Vec2{ width * 10 + 140, 120 }, 120, 125, !autoPlay);

		// When Button "Run / Pause" was pushed
		if (ButtonAt(autoPlay ? U"Pause" : U"Run", Vec2{ width * 10 + 140, 180 }, 245)) {
			autoPlay = !autoPlay;
		}

		// Speed Slider
		SliderAt(U"Speed", speed, 1.0, 0.05, Vec2{ width * 10 + 140, 220 }, 70, 175);

		// When Button "Go to next step" was pushed
		// Checking update
		if (ButtonAt(U"Go to next step", Vec2{ width * 10 + 140, 260 }, 245, !autoPlay)
			|| (autoPlay && stopwatch.sF() >= (speed * speed)))
		{
			UpdateWorld(world, gameRule);
			stepCounter++;
			imgUpdate = true;
			stopwatch.restart();
		}

		// When Radio button "Game Rule" was changed
		Headline(U"Game rule option", Vec2{ width * 10 + 17.5, 300 }, 245, !autoPlay);
		if (RadioButtons(gameRuleIndex, textOptions, Vec2{ width * 10 + 17.5, 340 }, 245, !autoPlay))
		{
			gameRule = gameRuleOptions[gameRuleIndex];
		}

		// Edit cell on the field
		if (!autoPlay && Rect{ 0, 0, width * 10 - 1, height * 10 - 1 }.mouseOver()) {
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

		// Show step counter
		Headline(U"Steps : {}"_fmt(stepCounter), Vec2{ width * 10 + 17.5, 640 }, 245);

		// Update image
		if (imgUpdate) {
			CopyToImg(world, img);
			texture.fill(img);
			imgUpdate = false;
		}
		ScopedRenderStates2D sampler{ SamplerState::ClampNearest };
		texture.scaled(10).draw();

		// Draw grid
		for (auto i : step(height + 1))
		{
			Rect{ 0, i * 10, width * 10, 1 }.draw(ColorF{ 0.4 });
			
		}
		for (auto i : step(width + 1)) {
			Rect{ i * 10, 0, 1, height * 10 }.draw(ColorF{ 0.4 });
		}

		if (!autoPlay && Rect{ 0, 0, width * 10 - 1, height * 10 - 1 }.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hidden);
			Rect{ Cursor::Pos() / 10 * 10, 10 }.draw(Orange);
		}
	}
}
