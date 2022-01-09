# LifeGame

<p align="center">
<img src="https://user-images.githubusercontent.com/79734873/148637883-7786afeb-26da-4c44-b93e-e5abc1c36630.png" alt="icon">
</p>

A simple life game simulator made by [OpenSiv3D](https://github.com/Siv3D/OpenSiv3D).

## Feature

* Large simulation field of 100x70 (Can change as you like)
* Can be simulated with seven types of rules
* Can change freely simulate speed and generation density
* Simple and easily UI

## Change Logs

Latest and stable release is ``release-1.0``(Ver 1.0).

|   Version   | Release Date  | Download |  Source  |
|:-----------:|:-------------:|:--------:|:--------:|
| release-1.0 |  2021/01/08   | [Here]() | [Here]() |

## How to Use

### Download

* You can download this app from [here](). The size is about xxMB.
* Unzip the downloaded zip file wherever you like.
* Launch ``Life Game  Ver1.0/App/LifeGame.exe``, you can play it!

### About UI

<p align="center">
<img src="https://user-images.githubusercontent.com/79734873/148667244-5f2ab445-1fe6-4779-9dd0-6d012abfe00b.png" alt="Screen Shot1">
</p>

#### 1. Field
You can see the current state of the cells.  Green is alive, and black is dead.

#### 2. UI field
You can change the game settings by using them:

* **Set Random** : Place cells in the field randomly.
* **Clear** : Clear all cells in the field.
* **Density** : Change the generation density at the time of **Set Random**. The larger this value, the denser the cells will place.
* **Run / Pause** : Start the simulation. Push this button again, simulation will pause.
* **Speed** : Change the speed at running the simulation.
* **Go to next step** : When pausing the simulation, you can take it to next step.
* **Game rule option** : Select one of the [seven game rules](#about-game-rule).

#### 3. Mouse Cursor
Left click to place an alive cell. Right click to delete the cell.

#### 4. Steps
Show how many generations have passed since the simulation start.

### About Game Rule

Two variables ``B`` and ``S`` are defined like this:
* ``B`` = Number of surrounding cells required for birth
* ``S`` = Number of surrounding cells required for survival

So, "B3/S23" means that B = 3 and S = 2, 3.

**Game rules list**
* 1 : B3/S23 (Default)
* 2 : B36/S23 (HighLife)
* 3 : B34/S34 (3-4Life)
* 4 : B3678/S34678 (Day and Night)
* 5 : B1357/S1357 (Replicator)
* 6 : B1357/S02468 (Replicator2 / Fredkin)
* 7 : B36/S125 (2x2)

Let's see for yourself what kind of movement these will do!

### Let's Play

Show you the flow until the start of simulation:

* Launch the app.
* After set density value as you like, press **Set Random** button.
* Select the game rule.
* Press **Run** to start simulation!


### Custom Settings from JSON

You can custom the settings of this app. **Changing this setting may cause the app to behave erratically. Please be careful.**
If you try, open ``Life Game  Ver1.0/App/config/settings.json`` and edit the value.
 
* **WorldSize - width** : number of cells of width of the field (Default is 100)
* **WorldSize - height** : number of cells of height of the field (Default is 70)
* **DefalutDensity** : Density value set by default (Default is 0.35)
* **Defalut Speed** : Speed value set by default (Default is 0.5)

## Feedback

For problems about the download and playing this, or suggestions to this README, please [submit a Github issue](https://github.com/Yuulis/LifeGame/issues).

## Licence

This app is available under the MIT license. Please look at [LICENSE.md](/LICENSE.md) for details.  
You can see the licence of OpenSiv3D is in ``Licences.html`` in the downloaded file or by pressing F1 while opening this app.

