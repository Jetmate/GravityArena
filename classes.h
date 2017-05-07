#ifndef GRAVITYARENA_CLASSES_H
#define GRAVITYARENA_CLASSES_H

#include <SFML/Graphics.hpp>

typedef std::vector<sf::Sprite> SpriteVector;

class Thing {
public:
    Thing(sf::Vector2f coordinates = sf::Vector2f(), float rotation = float());
protected:
    sf::Vector2f coordinates;
    float rotation;
};

class Hitbox : virtual public Thing {
public:
    virtual bool contains(sf::Vector2f point) const = 0;
};

class RectHitBox : public Hitbox {
public:
    RectHitBox(sf::Vector2u dimensions);
    bool collided(const Hitbox& thing) const;
    virtual bool contains(sf::Vector2f point) const;
    void calculate_dimensions();
protected:
    sf::Vector2f dimensions;
    sf::Vector2u default_dimensions;
};

class CircleHitBox : public Hitbox {
public:
    CircleHitBox(int radius);
    bool collided(const CircleHitBox& thing) const;
    bool collided(const RectHitBox& thing) const;
    virtual bool contains(sf::Vector2f point) const;
    float distance_to_center(sf::Vector2f point) const;
protected:
    int radius;
};

class SpriteManager : virtual public Thing {
public:
    virtual void display(sf::RenderWindow& window);
    virtual void update_transform();
protected:
    virtual sf::Sprite& current_sprite() = 0;
};

class SingleSpriteManager : public SpriteManager {
public:
    SingleSpriteManager(sf::Sprite sprite);
protected:
    sf::Sprite sprite;
    virtual sf::Sprite& current_sprite();
};

class MultiSpriteManager : public SpriteManager {
public:
    bool update_sprites(bool reset, int speed = 4);
protected:
    int sprite_index = int();
    int sprite_count = int();
    virtual sf::Sprite& current_sprite();
    virtual SpriteVector& current_sprites() = 0;
};

class SimpleMultiSpriteManager : public MultiSpriteManager {
public:
    SimpleMultiSpriteManager(SpriteVector sprites);
protected:
    SpriteVector sprites;
    virtual SpriteVector& current_sprites();
};

class ComplexMultiSpriteManager : public MultiSpriteManager {
public:
    ComplexMultiSpriteManager(std::map<int, SpriteVector> sprites);
protected:
    std::map<int, SpriteVector> sprites;
    int sprite_type = int();
    virtual SpriteVector& current_sprites();
};

//class SimpleThing : public virtual Hitbox {
//protected:
//    sf::Sprite sprite;
//
//public:
//    SimpleThing(sf::Sprite& sprite);
//    void update_transform();
//    void update_box();
//    void display(sf::RenderWindow& window) const;
//};
//
//class ComplexThing : public virtual Hitbox {
//protected:
//    std::map<int, SpriteVector> sprites;
//    int sprite_index = int();
//    int sprite_count = int();
//    int default_sprite_type;
//    int sprite_type;
//
//public:
//    ComplexThing(std::map<int, SpriteVector>& sprites, int default_sprite_type);
//    void update_transform();
//    void update_box();
//    void display(sf::RenderWindow& window);
//    SpriteVector& current_sprites();
//    sf::Sprite& current_sprite();
//    bool update_sprites(bool reset, int speed=4);
//};

class MassThing : public virtual Thing {
public:
    MassThing(int mass);
    float find_force(sf::Vector2f coordinates, int mass) const;
    float find_angle(sf::Vector2f coordinates) const;
protected:
    int mass;
};

class MovingThing : public virtual Thing {
public:
    MovingThing(sf::Vector2f velocity = sf::Vector2f());
    void update_coordinates();
protected:
    sf::Vector2f velocity;
};

class RotatingThing : public virtual Thing {
public:
    RotatingThing(float rotation_velocity = 0);
    void update_rotation();
protected:
    float rotation_velocity;
};

class Bullet : public SingleSpriteManager, public RectHitBox, public MovingThing {
public:
    Bullet(sf::Vector2f coordinates,
           float rotation,
           sf::Vector2u dimensions,
           sf::Sprite sprite,
           sf::Vector2f velocity);
};

class Planet : public SingleSpriteManager, public CircleHitBox, public MassThing {
public:
    Planet(sf::Vector2f coordinates,
           int radius,
           sf::Sprite sprite,
           int mass);
};

class Player : public ComplexMultiSpriteManager, public RectHitBox, public MassThing, public MovingThing, public RotatingThing {
public:
    Player(sf::Vector2f coordinates,
           float rotation,
           sf::Vector2u dimensions,
           std::map<int, SpriteVector> sprites,
           int mass,
           sf::Vector2f velocity,
           float movement_speed,
           float rotation_speed,
           float bullet_speed,
           sf::Sprite trail_sprite,
           sf::Sprite bullet_sprite,
           sf::Vector2u bullet_dimensions,
           std::map<int, int> controls,
           int health,
           int bullet_damage,
           sf::Vector2u health_bar_sprite_dimensions,
           sf::Color health_bar_color,
           int side,
           sf::Vector2u health_bar_margins,
           sf::Vector2f health_bar_offset,
           sf::Sprite health_bar_sprite
    );
    void process_gravity(const MassThing& thing);
    void update_gravity(const std::vector<MassThing>& things);
    void update_trail(const std::vector<Planet>& planets);
    void display_trail(sf::RenderWindow& window);
    void update_bullets(RectHitBox display_hitbox);
    void display_bullets(sf::RenderWindow& window);
    void planet_collision();

    void accelerate(bool action);
    void turn(bool action, int direction);
    void shoot(bool action);

    void accelerate();
    void turn();
    void shoot();
    bool is_alive() const;

    bool in_controls(int control) const;
    int action_type(int control) const;

    void hurt(int damage);
    void die();
    bool is_moving();

    bool operator==(const Player& player) const;
    bool operator!=(const Player& player) const;

    void bullet_collision(std::vector<Player>& players, std::vector<Planet>& planets);
    void display_health(sf::RenderWindow& window) const;
    void display_health_box(sf::RenderWindow& window) const;
    void end();
    bool is_active() const;

private:
    float movement_speed;
    float rotation_speed;
    float bullet_speed;
    std::vector<sf::Vector2f> trail;
    sf::Sprite trail_sprite;
    sf::Sprite bullet_sprite;
    sf::Vector2u bullet_dimensions;
    int turning_direction;
    std::map<int, int> controls;
    int original_health;
    int health;
    std::vector<Bullet> bullets;
    int bullet_damage;

    bool accelerating = false;
    bool turning = false;
    bool shooting = false;
    bool moving = true;
    bool active = true;

    sf::Vector2u health_bar_dimensions;
    sf::Color health_bar_color;
    int side;
    sf::Vector2f health_bar_coordinates;
    sf::Sprite health_bar_sprite;
};

#endif
