#include "Game.h"
#include <iostream>
#include <chrono>
#include <thread>

//private functions 

void Game::initVariables()
{
	this->window = nullptr;

	//Game logic 
	int points;

	float enemySpawnTimer;
	float enemySpawnTimerMax;
	int maxEnemiesCnt;

	float projectileSpawnTimer;
	float projectileSpawnTimerMax;
	int maxProjectilesCnt;
	float sleep_time;

	this->points = 0;
	this->enemySpawnTimerMax = 100.f;
	this->enemySpawnTimer = this->enemySpawnTimerMax;
	this->maxEnemiesCnt = 6;

	this->projectileSpawnTimerMax = 1.f;
	this->projectileSpawnTimer = this->projectileSpawnTimerMax;
	this->maxProjectilesCnt = 10; //надеюсь компилятор не лопнет подкрутим короче OUT OF RANGE AAAAAAAAAA
	this->sleep_time = 500; //podcrutit'

}

void Game::initWindow()
{
	/*
	Sets the basic parameters of the window 
	-height 
	-width 
	draws the game window itself
	sets the framerate (it helps not to get some visual bugs)

	*/
	this->videoMode.height = 600;
	this->videoMode.width = 800;

	this->window = new sf::RenderWindow(sf::VideoMode(800, 600), "Game 1", sf::Style::Titlebar | sf::Style::Close);

	this->window->setFramerateLimit(60);
}

void Game::initPlayer()
{
	/*
	player class 
	-geometry poition and size and everything
	*/
	this->player.setSize(sf::Vector2f(100.f, 100.f));
	this->player.setFillColor(sf::Color::Red);
	this->player.setOutlineColor(sf::Color::Magenta);
	this->player.setOutlineThickness(5.f);
	this->player.setPosition(100.f,
		(this->window->getSize().y * 0.5) - (this->player.getSize().y * 0.5));

}

void Game::initEnemies()
{
	/*
	enemy class 
	-geometry and position data
	*/
	this->enemy.setPosition(10.f,10.f);
	this->enemy.setSize(sf::Vector2f(100.f, 100.f));
	this->enemy.setScale(sf::Vector2f(0.5f, 0.5f));
	this->enemy.setFillColor(sf::Color::Cyan);
	this->enemy.setOutlineColor(sf::Color::Green);
	this->enemy.setOutlineThickness(1.f);

}

void Game::initProjectiles()
{
	/*
	projectile class 
	-geometry and position data
	-position doesnt matter i just need the function to get memory for it anyway
	*/
	this->projectile.setPosition(10.f, 10.f);
	this->projectile.setSize(sf::Vector2f(10.f, 10.f));
	this->projectile.setFillColor(sf::Color::Yellow);
	this->projectile.setOutlineColor(sf::Color::Red);
	this->projectile.setOutlineThickness(1.f);
}


//Constructors / destructors

Game::Game()
{
	/*
	calls the game objects: game screen (window) and the enemy
	*/
	this->initVariables(); //the order is important (because it doesnt have a pointer - another mistery of the code)
	this->initWindow();
	this->initPlayer();
	this->initProjectiles();
	this->initEnemies();

}

Game::~Game()
{
	delete this->window; //to prevent memory leak (idk how it works) but this is very important
}




//Accessors

const bool Game::running() const
{
	//this one returns the info if the game is still open
	return this->window->isOpen();
}



//functions

void Game::pollEvents()
{
	//Event polling
	// this one is made to be able to close the game
	while (this->window->pollEvent(this->ev))
	{
		switch (ev.type)
		{
		case sf::Event::Closed:  //закртие окна на крестик
			this->window->close();
		case sf::Event::KeyPressed:
			if (ev.key.code == sf::Keyboard::Escape) //закрытие кона на эскейп
				this->window->close();
			break;
		}
	}
}


void Game::spawnPlayer() //delete this later (dont need that anymore) 
{
	/*
	sets the player on the screen 
	*/
	this->player.setPosition(
		100.f,
		(this->window->getSize().y * 0.5) - (this->player.getSize().y*0.5)
	); 
}

void Game::updatePlayer()
{
	/*
	updates the poisition of the player 
	moves the player
	*/

	//Move player //add something that prevents player from leaving the screen
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		this->player.setPosition(this->player.getPosition().x, this->player.getPosition().y - 3.f);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		this->player.setPosition(this->player.getPosition().x, this->player.getPosition().y + 3.f);
	}
	
}

void Game::renderPlayer()
{
	//renders the player on the screen 
	this->window->draw(this->player);
}

void Game::spawnEnemy()
{
	/*
		return void 

		Spawn enemies and sets their color and position 
		-sets a random position 
		-sets a random color 
		-adds enemy to the vector
	*/

	//set position of the enemy
	this->enemy.setPosition(
		this->window->getSize().x,
		static_cast<float>(rand() % static_cast<int>(this->window->getSize().y - this->enemy.getSize().y))
	);

	//set color
	this->enemy.setFillColor(sf::Color(rand()%255,rand()%255,rand()%255,255));

	//spawn enemy
	this->enemies.push_back(this->enemy);

}

void Game::updateEnemies()
{
	/*
	*return void 
	updates the enemy spawn timer and spawn the enemies 
	when the total amoubt of enemies is smaller then the max

	moves the enemies downwards
	*/

	//updating the timer for enemy spawning 
	if (this->enemies.size() < this->maxEnemiesCnt) {


		if (this->enemySpawnTimer >= this->enemySpawnTimerMax)
		{
			//spawns the enemy and resets the timer
			this->spawnEnemy();
			this->enemySpawnTimer = 0.f;
		}
		else
		{
			this->enemySpawnTimer += 1.f;
		}
	}

	//Moving and killing enemies
	for (int i = 0; i < this->enemies.size(); i++)
	{
		//bool deleted = false; // make sure one enemy dies at a time

		this->enemies[i].move(-1.f, 0.f);

		//if the enemy goes further the left side of the window
		if (this->enemies[i].getPosition().x <0.f)
		{
			this->enemies.erase(this->enemies.begin() + i);
		}

		for (rsize_t j = 0; j < this->projectiles.size(); j++)
		{
			bool deleted = false;
			if (this->enemies[i].getGlobalBounds().contains(this->projectiles[j].getPosition()))
			{

				//this->enemies.erase(this->enemies.begin() + i);
				this->projectiles.erase(this->projectiles.begin() + j);
				deleted = true;

			}
		}

	}
	

}

void Game::renderEnemies()
{
	//Render all the enemies
	for (auto& e : this->enemies)
	{
		this->window->draw(e);
	}
}

void Game::spawnProjectiles()
{
	/*
		return void

		Spawn projectiles in the player (sets their color position)
		-sets a position related to the player place
		-adds projectile to the vector
	*/
	
	//set position of the projectile (in the centre of the player)
	this->projectile.setPosition(
		(this->player.getPosition().x+ this->player.getSize().x*0.5),
		(this->player.getPosition().y+ this->player.getSize().y*0.5)
	);


	//spawn projectile
	this->projectiles.push_back(this->projectile);
}


void Game::updateProjectiles()
{
	/*
	*return void
	- spawns projectile every n seconds; (need to change the whole thing i think) 
	-set the max size of vector (somehow)!!!!!! dont mind that it works
	-moves the projectile to the right 
	-interacts with the enemy, deletes the enemy and deletes itself
	*/

	sf::Time t = clock.getElapsedTime();
	if (t.asMilliseconds() > sleep_time)
	{
		if (this->projectiles.size() < this->maxProjectilesCnt)
		{
			this->spawnProjectiles();
			clock.restart();
		}
		
	}


	//Moving projectiles
	for (int i = 0; i < this->projectiles.size(); i++)
	{
		bool deleted = false; // make sure one enemy dies at a time

		this->projectiles[i].move(4.f, 0.f);

		//if the projectile goes further the right side of the window
		if (this->projectiles[i].getPosition().x > this->window->getSize().x)
		{
			this->projectiles.erase(this->projectiles.begin() + i);
			deleted = true;
		}

	}

	//check if projectile hits the enemy //PROBLES HERE ALARM OUT OF RANGE ALARM
	
	/*for (rsize_t i = 0; i < this->enemies.size(); i++) {
		for (rsize_t j = 0; j < this->projectiles.size(); j++)
		{
			bool deleted = false;
			if (this->enemies[i].getGlobalBounds().contains(this->projectiles[j].getPosition()))
			{
				
				//this->enemies.erase(this->enemies.begin() + i);
				this->projectiles.erase(this->projectiles.begin() + j);
				deleted = true;

			}
		}

	}*/


}

void Game::renderProjectiles()
{
	//Render all the projectiles
	for (auto& a : this->projectiles)
	{
		this->window->draw(a);
	}
}

void Game::update() 
{
	/*
	game goes through tis every frame 
	-keeps the game open and gives the ability to cloe it 
	-get u know where is the mouse
	*/
	this->pollEvents(); //call it here because we use it in every update
	

	this->updatePlayer();

	this->updateProjectiles();

	this->updateEnemies();

}

void Game::render() 
{
	/*
	draws the window and everything on it that neesd to be there
	 -clear old frame 
	 -render objects 
	 -display frame in window 
	*/
	this->window->clear();

	//Draw game here
	this->renderPlayer();
	this->renderProjectiles();
	this->renderEnemies();

	this->window->display();

}