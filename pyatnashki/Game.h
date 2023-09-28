#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <vector>
#include <ctime>

#include <SFML/System/Clock.hpp> //delete if it doesnt work 


/*
* Class that acts as the game engine
* wrapp class.
*/

class Game
{
private:
	//variables
	//window 
	sf::RenderWindow* window;
	sf::VideoMode videoMode;
	sf::Event ev;


	//Game logic
	unsigned points; //leave this 

	float enemySpawnTimer; //leave this 
	float enemySpawnTimerMax; //leave this
	int maxEnemiesCnt; //leave this

	float projectileSpawnTimer; //delete if time works
	float projectileSpawnTimerMax; //delete if time works
	int maxProjectilesCnt; // aded this one to be able to prevent the infinite creating of projectiles- this shouldnt aplly to the game
	
	int sleep_time;
	sf::Clock clock;

	
	//Game objects
	std::vector<sf::RectangleShape> enemies;
	sf::RectangleShape enemy;

	sf::RectangleShape player;

	std::vector<sf::RectangleShape> projectiles; //im nor sure i want to make it with vector but need to try
	sf::RectangleShape projectile;

	//private functions

	void initVariables();
	void initWindow();
	void initEnemies();
	void initPlayer();
	void initProjectiles();

public:
	//Concstuctors / destructors
	Game();
	virtual ~Game();
	//Accessors
	const bool running() const;

	//FUNCTIONS 

	//connected to the game build
	void pollEvents();
	void update();
	void render();

	//connected to the player behaviour 
	void spawnPlayer();
	void updatePlayer(); //delete later 
	void renderPlayer();

	//projectiles
	void spawnProjectiles();
	void updateProjectiles();
	void renderProjectiles();

	//connected to the enemy behaviour
	void spawnEnemy();
	void updateEnemies();
	void renderEnemies();

};

