#include "single_state.hpp"

#include "game.hpp"

SingleState::SingleState()
{
    std::cout << "Creating Single" << std::endl;
    level_manager = new LevelManager;
}

SingleState::~SingleState()
{
}

void SingleState::readMessage(const proto::Response &msg)
{
}

void SingleState::update(const sf::Event &event)
{
    level_manager->update(event);
    updateHeap();
}

void SingleState::load()
{
    std::cout << "Loading LevelManager" << std::endl;
    level_manager->load();
    std::cout << "LevelManager loaded" << std::endl;
}

void SingleState::updateHeap()
{
    clearHeap();
    for (auto &object : level_manager->getObjects())
    {
        heap_.push(object);
    }
}

ObjectsHeap &SingleState::getHeap()
{
    return heap_;
}

/*
void SingleState::initBG()
{
    bg_ = std::make_shared<BackGround>();
    bg_->setTexture(AssetManager::getInstance()->getTexture("green_world_temple"));
    bg_->setScale(2.0f, 2.0f);
}

void SingleState::initAssets()
{
    AssetManager::getInstance()->loadAssets("level_1");
}

void SingleState::initSound() 
{
    SoundManager::getInstance()->loadSoundAndMusicForLevel("level_1");
}

void SingleState::initCamera()
{
    sf::Vector2u window_size = GameEngine::getWindow().getSize();
    sf::Vector2f camera_size = sf::Vector2f(window_size.x, window_size.y);
    // sf::FloatRect camera_restriction_ = bg_->getGlobalBounds();
    sf::FloatRect camera_restriction_ = sf::FloatRect(0, 0, 1000, 1000);

    camera_ = new CameraTarget(camera_size, camera_restriction_);
    camera_->setFollowByCoordinates(player_->getCenter().x, player_->getCenter().y);
}

void SingleState::initPUI()
{
    player_user_interface_ = std::make_shared<PUI>(
        sf::Vector2f(GameEngine::getWindow().getSize().x,
                     GameEngine::getWindow().getSize().y));
    // player_user_interface_ = new PUI(
    //     sf::Vector2f(GameEngine::getWindow().getSize().x,
    //                  GameEngine::getWindow().getSize().y));
    player_user_interface_->updateMaxStatusBar(false, player_->getHPMax());
    player_user_interface_->updateBar(false, player_->getHP());
    player_user_interface_->updateMoney(player_->getCoinsCount());
    player_user_interface_->updateExpirienceCurrentPoints(player_->getExp());
}

void SingleState::initPlayer()
{
    player_ = std::make_shared<Player>(
        sf::Vector2f(BASE_SIZE, BASE_SIZE * 2),
        sf::Vector2f(100.f, 100.f));
}

void SingleState::initTiles()
{
    std::cout << "Started initing tiles" << std::endl;
    size_t tiles_count = 10;
    float x = 0;
    float y = 400;
    sf::Vector2f tile_size(BASE_SIZE, BASE_SIZE);

    int cntr = 0;
    for (size_t i{}; i <= GameEngine::getWindow().getSize().x; i += BASE_SIZE)
    {
        // std::cout << "4" << std::endl;
        x = i;
        sf::Vector2f tile_coords(x, y);
        tiles_.push_back(std::make_shared<Tile>(tile_size, tile_coords));
        std::cout << "Tile #" << ++cntr << " created" << std::endl;
    }
}

void SingleState::initEnemies()
{
    spawnEnemies();
}

void SingleState::initCollisionHandler()
{
    collision_handler_ = std::make_shared<CollisionHandler>();
}

void SingleState::updatePlayer(const sf::Event &event, const float delta_time)
{
    if (!player_->isDead())
    {
        player_->update(event, delta_time);
        player_->updateAttack(event, enemies_.front(), delta_time);

        if (!player_->isAttack() && !player_->isJumping() && !player_->isRunning())
            player_->setStayAnimation();
    }
    else
    {
        SoundManager::getInstance()->playSoundEffect(SoundType::PLAYER_DEATH);
        if (player_->checkDeathFreeze(delta_time)) {
            int player_new_pos_x = random_int(0, GameEngine::getWindow().getSize().x);
            player_->setPosition(sf::Vector2f(player_new_pos_x, 100.f));
            player_->spawn();
            player_->setStayAnimation();
        }
    }
    player_->updateAnimation(delta_time);
}

void SingleState::updateCamera()
{
    GameEngine::getWindow().setView(camera_->getView());
    camera_->setFollowByCoordinates(player_->getCenter().x, player_->getCenter().y);
}

void SingleState::updatePUI()
{
    player_user_interface_->updateBar(false, player_->getHP());
    player_user_interface_->updateExpirienceLevel(player_->getCurrLvl(), player_->getExpMax());
    player_user_interface_->updateMoney(player_->getCoinsCount());
    player_user_interface_->updateExpirienceCurrentPoints(player_->getExp());
}

void SingleState::updateTiles(const sf::Event &event)
{
    for (const auto &tile : tiles_)
    {
        tile->update(event, 0);
    }
}

void SingleState::updateCollision()
{
    std::vector<PlayerShPtr> players;
    players.push_back(player_);

    collision_handler_->run(players, tiles_, enemies_);

    players.clear();
}

void SingleState::updateEnemies(const sf::Event &event, const float delta_time)
{
    if (enemies_.empty())
    {
        spawnEnemies();
    }

    for (const auto &enemy : enemies_)
    {
        // if (enemy->isDead)
        // {
        //     delete enemy
        // }
        enemy->update(event, delta_time, player_);
        // enemy->updateAttack(event_, player_, delta_time);
    }
}

void SingleState::updateNonExistentObjects()
{
    // Check player

    // Check enemies
    for (auto it = enemies_.begin(); it != enemies_.end();)
    {
        if (!(*it)->doesExist())
        {
            it = enemies_.erase(it);
        }
        else
        {
            ++it;
        }
    }
    // if (enemies_.empty())
    // {
    //     std::cout << "Enemy disappeared" << std::endl;
    // }
}

void SingleState::drawBG()
{
    bg_->draw(GameEngine::getWindow(), sf::RenderStates());
}

void SingleState::drawPlayer()
{
    player_->draw(GameEngine::getWindow(), sf::RenderStates());
}

void SingleState::drawTiles()
{
    for (const auto &tile : tiles_)
    {
        tile->draw(GameEngine::getWindow(), sf::RenderStates());
    }
}

void SingleState::drawEnemies()
{
    for (const auto &enemy : enemies_)
    {
        enemy->draw(GameEngine::getWindow(), sf::RenderStates());
    }
}

void SingleState::spawnEnemies()
{
    int enemy_pos_x = random_int(0, GameEngine::getWindow().getSize().x);
    EnemyShPtr enemy = std::make_shared<Enemy>(sf::Vector2f(BASE_SIZE, BASE_SIZE * 2), sf::Vector2f(enemy_pos_x, 100.f));

    enemies_.push_back(enemy);
}
*/